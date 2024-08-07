#pragma once
#include <memory.h>

class KDXInput
{
private:
    //bool* _pKeys = nullptr;
    bool m_keys[256];
public:
    static KDXInput& GetInstance()
    {
        static KDXInput i;
        return i;
    }

    KDXInput();
    ~KDXInput();
    void GetKeys();
    void InitKeys() {
        memset(m_keys, 0, sizeof(m_keys));
    }
    bool operator()(int ch)
    {
        return m_keys[ch];
    }
    void Update();
};

//#define _KVirtualInput KDXInput::GetInstance()
#define _KInput KDXInput::GetInstance()
