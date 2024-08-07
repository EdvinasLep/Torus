#include "KTorusAnimation.h"
#include "KTime.h"

void Draw2dTorus(D3DXVECTOR3 position, TORUS torus, int animSequence);
void DrawD3DTorus(float x, float y, TORUS t);

void KTorusAnimation::Init(D3DXVECTOR3 pos, TORUS t, double delay)
{
    _position = pos;
    _torus = t;
    _delay = delay; // millisecond
    _animSequence = 0;
    _timer = 0;
}

void KTorusAnimation::SetPosition(D3DXVECTOR3 pos)
{
    _position = pos;
}

void KTorusAnimation::Update()
{
    _timer += _KTime.deltaTime;
    // state machine
    if (_timer > _delay) {
        _animSequence += 1;
        _animSequence %= 4;
        _timer -= _delay;
    }
}

void KTorusAnimation::Render()
{
   // Draw2dTorus(, _torus, _animSequence);
    DrawD3DTorus(_position.x, _position.y, _torus);
}
