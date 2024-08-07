#include "DXUT.h"
#include "KDXInput.h"

KDXInput::KDXInput()
{
}

KDXInput::~KDXInput()
{
}

void KDXInput::GetKeys()
{
    //memcpy(m_keys, DXUTGetKeys(), sizeof(m_keys));
    for (int ch = 0; ch < 255; ++ch)
    {
        if(DXUTWasKeyPressed((BYTE)ch))
            m_keys[ch] = 1;
    }
}

void KDXInput::Update()
{
    InitKeys();
    GetKeys();
}