#ifdef D3D
#include "DXUT.h"
#include "../resource.h"
#include "SDKmisc.h"
#endif // D3D
#include "TorusUtil.h"
#include "TorusDefines.h"
#include <iostream>
#include "../EmptyProject.h"



#ifdef D3D
extern ID3DXFont* g_pFont;         // Font for drawing text
extern ID3DXSprite* g_pSprite;       // Sprite for batching draw text calls
#endif // D3D

void TorusUtil::DrawText(int x, int y, const std::string& text)
{
#ifdef D3D
	std::wstring wtext;
	wtext.assign(text.begin(), text.end());

	CDXUTTextHelper txtHelper(g_pFont, g_pSprite, 15);

	// Output statistics
	txtHelper.Begin();
	txtHelper.SetInsertionPos(x * 16, y * 16);
	txtHelper.SetForegroundColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	txtHelper.DrawTextLine(wtext.c_str());
	//txtHelper.DrawFormattedTextLine(L"fTime: %0.1f", fTime);
	txtHelper.End();
#else
	gotoxy(x, y);
	std::cout << text.c_str();
#endif // D3D
}

void TorusUtil::DrawText(int x, int y, int numRows, const std::string& text)
{
	int tx = x;
	int ty = y;
	for (int i = 0; i < numRows; ++i) {
		DrawText(tx, ty, text);
		ty += 1;
	}
}

void TorusUtil::DrawDeque(const KQueue& q, int queueIndex)
{
	const int maxQueue = q.GetSize();
	KVector2 pos = q.GetPos();
	std::deque<TORUS>::const_iterator it;
	const int numEmpty = maxQueue - q.GetTorusCount();
	const int numTorus = maxQueue - numEmpty;

	EmptyProject anim;
	for (int i = 0; i < numEmpty; i++)
	{
		DrawText((int)pos.x, (int)pos.y, "---");
		pos.y += 1;
	}
	pos.y += numTorus - 1;
	for (it = q.Begin(); it != q.End(); it++)
	{
		anim.Draw2dTorus(D3DXVECTOR3((int)pos.x + 21 + (queueIndex * 60), (int)pos.y + 205, 0), *it, 0);
		//DrawText((int)pos.x, (int)pos.y, g_torusText[*it]);
		pos.y -= 15;
	}
}
