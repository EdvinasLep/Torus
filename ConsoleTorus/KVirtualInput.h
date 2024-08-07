#pragma once
#ifdef D3D
#include "../KDXInput.h"
class KInput
{
public:
    enum VirtualKeys
    {
        VKEY_UP = VK_UP,
        VKEY_LEFT = VK_LEFT,
        VKEY_RIGHT = VK_RIGHT,
        VKEY_DOWN = VK_DOWN
    };
};
#else
#include "KInput.h"
#endif // D3D

