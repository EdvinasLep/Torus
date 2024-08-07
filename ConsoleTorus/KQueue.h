#pragma once
#include <deque>
#include "TorusDefines.h"

class KQueue
{
private:
    std::deque<TORUS> q0;
    int dqmax = 5;
    KVector2    _pos;

public:
    KQueue();
    KQueue(const KQueue& rhs)
    {
        if (this == &rhs)
            return;
        q0 = rhs.q0;
        dqmax = rhs.dqmax;
        _pos = rhs._pos;
    }
    KQueue& operator=(const KQueue& rhs)
    {
        if (this == &rhs)
            return *this;
        q0 = rhs.q0;
        dqmax = rhs.dqmax;
        _pos = rhs._pos;
        return *this;
    }
    ~KQueue();

    void SetPos( int x, int y );
    KVector2 GetPos() const { return _pos; }
    void SetSize(int size) {
        dqmax = size;
    }
    int GetSize() const { return dqmax; }
    int GetTorusCount() const;

    void PopFront() {
        q0.pop_front();
    }
    void PopBack() {
        q0.pop_back();
    }
    TORUS Back()
    {
        return q0.back();
    }
    TORUS Front() {
        return q0.front();
    }
    void PushFront(const TORUS& v) {
        q0.push_front(v);
    }
    void PushBack(const TORUS& v) {
        q0.push_back(v);
    }
    bool IsFull() const;
    //void DrawDeque();
    void Update();

    TORUS GetAt(int i) const;
    bool Erase(int i);
    std::deque<TORUS>::const_iterator Begin() const { return q0.begin(); }
    std::deque<TORUS>::const_iterator End() const { return q0.end(); }
};
