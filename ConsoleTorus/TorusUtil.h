#pragma once
#include <string>
#include <windows.h>
#include <deque>
#include "KVector2.h"
#include "TorusDefines.h"
#include "KQueue.h"

namespace TorusUtil
{
    void DrawText(int x, int y, const std::string& text);
    void DrawText(int x, int y, int numRows, const std::string& text);
    void DrawDeque(const KQueue& q, int queueIndex);
   /* void DrawDeque(const KQueue& q);*/
}
