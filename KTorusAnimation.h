#pragma once
#include "DXUT.h"
#include "TorusDefines.h"

class KTorusAnimation
{
private:
    D3DXVECTOR3 _position;
    TORUS       _torus;
    double      _delay; // millisecond
    int         _animSequence;
    double      _timer;

public:
    void Init(D3DXVECTOR3 pos, TORUS t, double delay);
    void SetPosition(D3DXVECTOR3 pos);
    void Update();
    void Render();
};

