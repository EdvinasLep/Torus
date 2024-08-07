#pragma once
#include "DXUT.h"
#include "resource.h"
#include "SDKmisc.h"
#include <thread>
#include <chrono>
#include "KScene.h"
#include "KVirtualInput.h"
#include "KTime.h"
//{{ sprite animation
#include "KTorusAnimation.h"

class EmptyProject
{
public:
	void Draw2dTorus(D3DXVECTOR3 position, TORUS torus, int animSequence);
};