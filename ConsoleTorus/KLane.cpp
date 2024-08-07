#include "KLane.h"
#include "TorusDefines.h"
#include "KTime.h"
#include "TorusUtil.h"

KLane::KLane()
{
	_height = 0;
	_torusState = ETorusState::BEGIN;
	_torus = TORUS::TORUS_RED;
}

KLane::~KLane()
{
}

void KLane::SetPos(int x, int y)
{
	_pos.x = x;
	_pos.y = y;
	_torusPos = _pos;
}

void KLane::SetTorusCallback(std::function<void(KLane*)> cb)
{
	_torusCallback = cb;
}

void KLane::SetHeight(int h)
{
	_height = h;
}

void KLane::InitTorus(KVector2 velocity, TORUS t)
{
	_torusPos = _pos;
	_torusPosBegin = _torusPos;
	_torusVel = velocity;
	_torus = t;
	_torusState = ETorusState::MOVING;
#ifdef D3D
	_torusAnim.Init(D3DXVECTOR3(_torusPos.x, _torusPos.y, 0), t, 0.1);
#endif
}

void KLane::Draw()
{
	int x = (int)_pos.x;
	int y = (int)_pos.y;
	for (int row = 0; row < _height; ++row) {
		const int ty = row + y;
		TorusUtil::DrawText(x, ty, "...");
	}

#ifdef D3D
	_torusAnim.Render();
#else
	TorusUtil::DrawText((int)_torusPos.x, (int)_torusPos.y, g_torusText[_torus]);
#endif
}

void KLane::Update()
{
	if (_torusState == ETorusState::BEGIN) {
		//
	}
	else if (_torusState == ETorusState::MOVING) {
		_torusPos = _torusPos - _torusVel * _KTime.deltaTime;
		const double offset = _torusPosBegin.y - _torusPos.y;
		if (offset >= _height) {
			_torusState = ETorusState::END;
		}
	}
	else if (_torusState == ETorusState::END) {
		// access queue, push torus into a queue
		if (_torusCallback != nullptr) {
			_torusCallback(this);
		}
	}
#ifdef D3D
	_torusAnim.SetPosition(D3DXVECTOR3(_torusPos.x*0.8f - 15.8, _torusPos.y + 14, 0));
	_torusAnim.Update();
#endif
}

TORUS KLane::GetTorus()
{
	return _torus;
}
