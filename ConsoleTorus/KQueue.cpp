#include "KQueue.h"

KQueue::KQueue()
{
}

KQueue::~KQueue()
{
}

void KQueue::SetPos( int x, int y )
{
    _pos.x = x;
    _pos.y = y;
}

int KQueue::GetTorusCount() const
{
    return (int)q0.size();
}

bool KQueue::IsFull() const
{
    return q0.size() >= (size_t)dqmax;
}

//void KQueue::DrawDeque()
//{
//    ::DrawDeque( q0, (int)_pos.x, (int)_pos.y, dqmax );
//}

void KQueue::Update()
{
    // queue update routine here
}

TORUS KQueue::GetAt(int i) const
{
    return q0[i];
}

bool KQueue::Erase(int i)
{
    if (i >= GetTorusCount())
        return false;

    q0.erase(q0.begin() + i);
    return true;
}
