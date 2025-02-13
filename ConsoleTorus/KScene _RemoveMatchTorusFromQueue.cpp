#include "KScene.h"
#include "KInput.h"
#include <functional>
#include <cstdlib>

using namespace std::placeholders;

KScene::KScene()
{
    _stackStampOld = 0;
    _stackStampNew = 0;
    _sceneState = SCENE_STATE_NONE;
	//{{ day06
    _queueDataStamp = 0;
    _queueDataStampNew = 0;
	//}} day06
}

KScene::~KScene()
{
}

void KScene::Initialize(KInitParam initParam)
{
    _param = initParam;
    for (int i = 0; i < initParam.numLanes; ++i) {
        _lanes.push_back( KLane() );
        _queues.push_back(KQueue());
    }
    _pos = KVector2(initParam.x, initParam.y);
    int tx = _pos.x;
    int ty = _pos.y;
    for (int i = 0; i < _lanes.size(); ++i) {
        KLane& l = _lanes[i];
        l.SetPos( tx, ty );
        l.SetHeight(initParam.laneHeight);
        l.SetTorusCallback( std::bind( &KScene::TorusEndCallback, this, _1 ) );
        l.SetId(i);
        KQueue& q = _queues[i];
        q.SetPos( tx, ty + initParam.laneHeight );
        q.SetSize(initParam.queueSize);
        tx += 4;
    }
    _stack.SetSize(initParam.stackSize);
    int numSkipLane = (_param.numLanes - 1) / 2;
    tx = initParam.x + numSkipLane * 4;
    ty = initParam.y + _lanes[0].GetHeight() + _queues[0].GetSize();
    _stackPos.x = tx;
    _stackPos.y = ty;
	//{{ day06
    _stackLane = numSkipLane; // lane number start from 0
	//}} day06
    //_stack.SetPos( tx, ty );
}

void KScene::Update()
{
    for (int i = 0; i < _lanes.size(); ++i) {
        KLane& l = _lanes[i];
        l.Update();
    }
    for (int i = 0; i < _queues.size(); ++i) {
        KQueue& q = _queues[i];
        q.Update();
    }
    //_stack.Update();

    void( KScene::*stateCallback[4])() = {
        &KScene::_Update_SCENE_STATE_NONE,
        &KScene::_Update_SCENE_STATE_INITIALIZED,
        &KScene::_Update_SCENE_STATE_PLAYING,
        &KScene::_Update_SCENE_STATE_END
    };
    ( this->*stateCallback[_sceneState] )( );

    if (_KInput(KInput::VKEY_LEFT)) {
		//{{ day06
        if (_stackLane > 0) {
            _stackLane -= 1;
		//}} day06
            _stackStampNew += 1;
            _stackPosOld = _stackPos;
            _stackPos.x -= 4;
        }
    }
    else if (_KInput( KInput::VKEY_RIGHT )) {
		//{{ day06
        if (_stackLane < _param.numLanes - 1) {
            _stackLane += 1;
		//}} day06
            _stackStampNew += 1;
            _stackPosOld = _stackPos;
            _stackPos.x += 4;
        }
    }
	//{{ day06
    else if (_KInput(KInput::VKEY_DOWN)) {
        KQueue& q = _queues[_stackLane];
        if (q.GetTorusCount() > 0 && !_stack.IsFull()) {
            TORUS t = q.Front();
            q.PopFront();
            _stack.Push(t);
            _queueDataStampNew += 1;
        }
    }
    else if (_KInput(KInput::VKEY_UP)) {
        KQueue& q = _queues[_stackLane];
        if (!q.IsFull() > 0 && !_stack.Empty()) {
            TORUS t = _stack.Top();
            _stack.Pop();
            q.PushFront(t);
            _queueDataStampNew += 1;
        }
    }

    if (_queueDataStamp != _queueDataStampNew)
    {
        _queueDataStamp = _queueDataStampNew;
        _RemoveMatchTorusFromQueue();
    }
	//}} day06
}

void KScene::Draw()
{
    for (int i = 0; i < _lanes.size(); ++i) {
        KLane& l = _lanes[i];
        l.Draw();
    }
    for (int i = 0; i < _queues.size(); ++i) {
        KQueue& q = _queues[i];
        q.DrawDeque();
    }
    // observer design pattern
    if (_stackStampOld != _stackStampNew) {
        DrawText(_stackPosOld.x
            , _stackPosOld.y
            , _stack.GetSize() + 1, "   ");
        _stackStampOld = _stackStampNew;
    }
    _stack.Draw(_stackPos.x, _stackPos.y);
}

void KScene::TorusEndCallback( KLane* pLane )
{
	//{{ day06
    TORUS tOld = pLane->GetTorus();
    int laneId = pLane->GetId();
    KQueue& q = _queues[laneId];
    if (q.IsFull()) {
        extern bool g_isGameLoop;
        g_isGameLoop = false;
        return;
    }

    q.PushBack(tOld);
    _queueDataStampNew += 1;
	//}} day06
    TORUS t = (TORUS)( std::rand() % (int)TORUS_MAX );
    KVector2 vel = KVector2( 0, 1 + std::rand() % 2 );
    pLane->InitTorus( vel, t );
}

void KScene::_Update_SCENE_STATE_NONE()
{
    _sceneState = SCENE_STATE_INITIALIZED;
}

void KScene::_Update_SCENE_STATE_INITIALIZED()
{
    for (int i = 0; i < _param.numLanes; ++i) {
        TORUS t = (TORUS)( std::rand() % (int)TORUS_MAX );
        KVector2 vel = KVector2( 0, 1 + std::rand() % 2 );
        KLane& lane = _lanes[i];
        lane.InitTorus( vel, t );
    }
    _sceneState = SCENE_STATE_PLAYING;
}

void KScene::_Update_SCENE_STATE_PLAYING()
{
}

void KScene::_Update_SCENE_STATE_END()
{
}

//{{ day06
bool KScene::_EraseLineFromQueue(int lineFrom0)
{
    for (KQueue& q : _queues)
    {
        if (lineFrom0 > q.GetTorusCount())
            return false;
    }
    for (KQueue& q : _queues)
    {
        q.Erase(lineFrom0);
    }
    _queueDataStampNew += 1;
    return true;
}

void KScene::_RemoveMatchTorusFromQueue()
{
    _ValidateMatchingTorus();
}
//}} day06


bool KScene::_ValidateMatchingTorus()
{
    KQueue* smallestQueue = &_queues[1];
    for (KQueue& q : _queues)
    {
        if (q.GetTorusCount() < smallestQueue->GetTorusCount())
            smallestQueue = &q;
    }
    for (int i = 0; i < smallestQueue->GetTorusCount(); ++i)
    {
        TORUS firstTorus = _queues[0].GetAt(i);
        bool differentTorusPresentInLine = false;
        for (KQueue& q : _queues)
        {
            if (q.GetAt(i) != firstTorus)
                differentTorusPresentInLine = true;
        }
        if (!differentTorusPresentInLine)
        {
            _EraseLineFromQueue(i);
            return true;
        }
    }
    return false;
}