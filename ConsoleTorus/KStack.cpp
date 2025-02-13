#include "KStack.h"
#include <assert.h>
#include "TorusUtil.h"
#include "../EmptyProject.h"

KStack::KStack() {
	_sp = 0;
	_maxSize = 4;
}

void KStack::SetSize(int size) {
	_maxSize = size;
}

void KStack::Push(TORUS t) {
	if (_sp >= _maxSize)
		return;
	_buffer[_sp] = t;
	_sp += 1;
}

void KStack::Pop() {
	if (_sp <= 0)
		return;
	_sp -= 1;
}

// @pre must not empty
TORUS KStack::Top() {
	return _buffer[_sp - 1];
}

bool KStack::Empty() {
	return _sp == 0;
}

bool KStack::IsFull()
{
	return _sp >= _maxSize;
}

void KStack::Draw(int x, int y, int stackLane) {
	int cx = x;
	int cy = y;
	int numTorusInStack = _sp;
	int numEmpty = _maxSize - numTorusInStack;
	int handleY = y;

	EmptyProject anim;
	for (int i = 0; i < numEmpty; ++i) {
		TorusUtil::DrawText(cx, handleY, " | ");
		//cy += 1;
		handleY += 1;
	}
	const int topIndex = _sp - 1;
	for (int i = 0; i <= topIndex; ++i) {
		const TORUS t = _buffer[i];
		assert(t >= 0 && t < 4);
		//TorusUtil::DrawText(cx, cy, g_torusText[t]);
		anim.Draw2dTorus(D3DXVECTOR3((int)cx + 21 + (stackLane * 60), (int)cy + 280, 0), t, 0);
		cy -= 15;
		handleY += 1;
	}
	TorusUtil::DrawText(cx - 1, handleY, "======");
}
