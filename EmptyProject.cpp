//--------------------------------------------------------------------------------------
// File: EmptyProject.cpp
//
// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License (MIT).
//--------------------------------------------------------------------------------------
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
#include "EmptyProject.h"
//}} sprite animation

ID3DXFont* g_pFont = NULL;         // Font for drawing text
ID3DXSprite* g_pSprite = NULL;       // Sprite for batching draw text calls
LPD3DXMESH      g_pBox = NULL;
D3DXMATRIX      g_matBox;
D3DMATERIAL9    g_mtrlBox;
LPD3DXMESH      g_pSphere = NULL;
//{{ sprite
LPDIRECT3DTEXTURE9 texture;
LPD3DXSPRITE sprite;
KTorusAnimation g_torusAnim;
//}} sprite
using namespace std::chrono;
bool    g_isGameLoop = true;
KScene  g_scene;

//{{ sprite
LPDIRECT3DTEXTURE9 g_charTexture;
LPD3DXSPRITE g_charSprite;

void EmptyProject::Draw2dTorus(D3DXVECTOR3 position, TORUS torus, int animSequence)
{
	static int lookup[] = { 3,0,1,2 };
	int row = lookup[torus];
	RECT rect;
	{
		rect.left = animSequence * 29;
		rect.top = row * 29;
		rect.right = animSequence * 29 + 29;
		rect.bottom = row * 29 + 29;
	}
	D3DXVECTOR3 center;
	center = D3DXVECTOR3(15, 15, 0);
	D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);
	if (sprite && texture) {
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
		sprite->Draw(texture, &rect, &center, &position, color);
		sprite->End();
	}
}
void DrawD3DTorus(float x, float y, TORUS t)
{
	IDirect3DDevice9* pd3dDevice = DXUTGetD3D9Device();

	D3DMATERIAL9    mtrl;
	D3DXCOLOR colorLookup[TORUS::TORUS_MAX]{
		D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),
		D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),
		D3DXCOLOR(0.0f,0.0f,1.0f,1.0f),
		D3DXCOLOR(1.0f,0.0f,1.0f,1.0f)
	};

	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse = mtrl.Ambient = colorLookup[t];
	pd3dDevice->SetMaterial(&mtrl);

	D3DXMATRIX nonUniformScale;
	D3DXMATRIX trans;

	D3DXMATRIX rot;
	static float angle = 0;
	angle += _KTime.deltaTime;
	D3DXMatrixScaling(&nonUniformScale, 1, 0.25, 1);
	D3DXMatrixRotationX(&rot, angle);
	D3DXMatrixTranslation(&trans, x, y, 0);
	D3DXMATRIX matSphere = nonUniformScale * rot * trans;
	pd3dDevice->SetTransform(D3DTS_WORLD, &matSphere);
	g_pSphere->DrawSubset(0);
}
//}} sprite

void DrawSpriteText(D3DXVECTOR3 position, int charCode)
{
	if (charCode < 32 || charCode > 127)
		return;

	int row = (charCode - 32) / 15;
	int column = (charCode - 32) % 15;
	RECT rect;
	{
		rect.left = column * 20;
		rect.top = row * 20;
		rect.right = column * 20 + 20;
		rect.bottom = row * 20 + 20;
	}
	const float horiNormal = 512.0 / 300.0;
	const float vertNormal = 256.0 / 160.0;
	{
		rect.left *= horiNormal;
		rect.right *= horiNormal;
		rect.top *= vertNormal;
		rect.bottom *= vertNormal;
	}
	D3DXVECTOR3 center;
	center = D3DXVECTOR3(16, 17, 0);
	D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255);
	if (g_charSprite && g_charTexture) {
		g_charSprite->Begin(D3DXSPRITE_ALPHABLEND);
		g_charSprite->Draw(g_charTexture, &rect, &center, &position, color);
		g_charSprite->End();
	}
}

void DrawSpriteText(D3DXVECTOR3 position, char text[100])
{
	int len = strnlen_s(text, 100);
	for (int i = 0; i < len; i++)
	{
		const char char_ = text[i];
		position.x += 20;
		DrawSpriteText(position, char_);
	}
}

//--------------------------------------------------------------------------------------
// Rejects any D3D9 devices that aren't acceptable to the app by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK IsD3D9DeviceAcceptable(D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat,
	bool bWindowed, void* pUserContext)
{
	// Typically want to skip back buffer formats that don't support alpha blending
	IDirect3D9* pD3D = DXUTGetD3D9Object();
	if (FAILED(pD3D->CheckDeviceFormat(pCaps->AdapterOrdinal, pCaps->DeviceType,
		AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
		D3DRTYPE_TEXTURE, BackBufferFormat)))
		return false;

	return true;
}


//--------------------------------------------------------------------------------------
// Before a device is created, modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
{
	return true;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that will live through a device reset (D3DPOOL_MANAGED)
// and aren't tied to the back buffer size
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9CreateDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext)
{
	//HRESULT hr = 0L;
	D3DXCreateFont(pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET
		, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE
		, L"Arial", &g_pFont);
	D3DXCreateBox(pd3dDevice, 1, 1, 1, &g_pBox, NULL);

	KScene::KInitParam param;
	{
		param.laneHeight = 8;
		param.numLanes = 3;
		param.queueSize = 5;
		param.stackSize = 5;
		param.x = 1;
		param.y = 1;
	}
	g_scene.Initialize(param);
	//{{ sprite animation
	g_torusAnim.Init(D3DXVECTOR3(256, 256, 0), TORUS_GREEN, 0.25);
	//}} sprite animation

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Create any D3D9 resources that won't live through a device reset (D3DPOOL_DEFAULT) 
// or that are tied to the back buffer size 
//--------------------------------------------------------------------------------------
HRESULT CALLBACK OnD3D9ResetDevice(IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext)
{
	if (g_pFont)
		g_pFont->OnResetDevice();
	D3DXCreateSprite(pd3dDevice, &g_pSprite);

	//{{ sprite
	HRESULT hr;
	hr = D3DXCreateTextureFromFile(pd3dDevice, L"MsDos-TorusSprite_20240510.png", &texture);

	if (FAILED(hr)) {
		return S_FALSE;
	}
	D3DXCreateSprite(pd3dDevice, &sprite);
	//}} sprite

		//{{ sprite
	hr = D3DXCreateTextureFromFile(pd3dDevice, L"x05mo.png", &g_charTexture);
	if (FAILED(hr)) {
		return S_FALSE;
	}
	D3DXCreateSprite(pd3dDevice, &g_charSprite);
	//}} sprite
	D3DXCreateSphere(pd3dDevice, 0.5, 10, 10, &g_pSphere, NULL);
	return S_OK;
}


//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
	LPDIRECT3DDEVICE9 pd3dDevice = DXUTGetD3D9Device();

	D3DXVECTOR3 vEyePt(0.0f, 10.0f, -10.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 1.0f, 100.0f);
	D3DXMatrixOrthoLH(&matProj, 32, 24, 1.0f, 100.0f);
	pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.0f;
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	vecDir = D3DXVECTOR3(1, -1, 1);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);

	pd3dDevice->SetLight(0, &light);
	pd3dDevice->LightEnable(0, TRUE);

	// Finally, turn on some ambient light.
	pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00808080);
	pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	{
		D3DXMatrixIdentity(&g_matBox);
		ZeroMemory(&g_mtrlBox, sizeof(D3DMATERIAL9));
		g_mtrlBox.Diffuse.r = g_mtrlBox.Ambient.r = 1.0f;
		g_mtrlBox.Diffuse.g = g_mtrlBox.Ambient.g = 0.0f;
		g_mtrlBox.Diffuse.b = g_mtrlBox.Ambient.b = 1.0f;
		g_mtrlBox.Diffuse.a = g_mtrlBox.Ambient.a = 0.5f;

		D3DXMATRIX matScale;
		D3DXMatrixRotationY(&g_matBox, (float)fTime);
	}
	if (g_isGameLoop)
	{
		_KInput.Update();
		_KTime.Update();
		//std::this_thread::sleep_for(100ms);
		//{{ sprite animation
		g_torusAnim.Update();
		//}} sprite animation
		g_scene.Update();
	}
}


//--------------------------------------------------------------------------------------
// Render the scene using the D3D9 device
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9FrameRender(IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext)
{
	HRESULT hr;

	// Clear the render target and the zbuffer 
	V(pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0));

	// Render the scene
	if (SUCCEEDED(pd3dDevice->BeginScene()))
	{
		/*pd3dDevice->SetTransform(D3DTS_WORLD, &g_matBox);
		pd3dDevice->SetMaterial(&g_mtrlBox);
		g_pBox->DrawSubset(0);*/
		/*DrawD3DTorus(1, 0, TORUS::TORUS_BLUE);
		DrawD3DTorus(2, 0, TORUS::TORUS_RED);
		DrawD3DTorus(3, 0, TORUS::TORUS_GREEN);*/
		//{{ sprite
		for (int i = 0; i < 4; ++i) {
			//Draw2dTorus(D3DXVECTOR3(256, 32 + 29 * i, 0), TORUS_RED, i);
			//Draw2dTorus(D3DXVECTOR3(256 + 32, 32 + 29 * i, 0), TORUS_GREEN, i);
			//Draw2dTorus(D3DXVECTOR3(256 + 32 * 2, 32 + 29 * i, 0), TORUS_BLUE, i);
			//Draw2dTorus(D3DXVECTOR3(256 + 32 * 3, 32 + 29 * i, 0), TORUS_MAGENTA, i);
		}
		g_torusAnim.Render();
		//}} sprite

		if (DXUTIsKeyDown(VK_LEFT))
		{
			CDXUTTextHelper txtHelper(g_pFont, g_pSprite, 15);

			// Output statistics
			/*txtHelper.Begin();
			txtHelper.SetInsertionPos(16, 16);
			txtHelper.SetForegroundColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
			txtHelper.DrawTextLine(L"Hello World");
			txtHelper.DrawFormattedTextLine(L"fTime: %0.1f", fTime);
			txtHelper.End();*/
		}

		int score_ = g_scene.GetScore();
		char charArray[100];
		sprintf(charArray, "Score: %d", score_);
		DrawSpriteText(D3DXVECTOR3(0, 350, 0), charArray);
		g_scene.Draw();

		V(pd3dDevice->EndScene());
	}
}


//--------------------------------------------------------------------------------------
// Handle messages to the application 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
	bool* pbNoFurtherProcessing, void* pUserContext)
{
	return 0;
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9ResetDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9LostDevice(void* pUserContext)
{
	if (g_pFont)
		g_pFont->OnLostDevice();
	SAFE_RELEASE(g_pSprite);
	//{{ sprite
	SAFE_RELEASE(sprite);
	SAFE_RELEASE(texture);
	//}} sprite
	// 
	//{{ text sprite
	SAFE_RELEASE(g_charSprite);
	SAFE_RELEASE(g_charTexture);
	//}} text sprite
}


//--------------------------------------------------------------------------------------
// Release D3D9 resources created in the OnD3D9CreateDevice callback 
//--------------------------------------------------------------------------------------
void CALLBACK OnD3D9DestroyDevice(void* pUserContext)
{
	SAFE_RELEASE(g_pFont);
	SAFE_RELEASE(g_pBox);
	SAFE_RELEASE(g_pSphere);
}


//--------------------------------------------------------------------------------------
// Initialize everything and go into a render loop
//--------------------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	// Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// Set the callback functions
	DXUTSetCallbackD3D9DeviceAcceptable(IsD3D9DeviceAcceptable);
	DXUTSetCallbackD3D9DeviceCreated(OnD3D9CreateDevice);
	DXUTSetCallbackD3D9DeviceReset(OnD3D9ResetDevice);
	DXUTSetCallbackD3D9FrameRender(OnD3D9FrameRender);
	DXUTSetCallbackD3D9DeviceLost(OnD3D9LostDevice);
	DXUTSetCallbackD3D9DeviceDestroyed(OnD3D9DestroyDevice);
	DXUTSetCallbackDeviceChanging(ModifyDeviceSettings);
	DXUTSetCallbackMsgProc(MsgProc);
	DXUTSetCallbackFrameMove(OnFrameMove);

	// TODO: Perform any application-level initialization here

	// Initialize DXUT and create the desired Win32 window and Direct3D device for the application
	DXUTInit(true, true); // Parse the command line and show msgboxes
	DXUTSetHotkeyHandling(true, true, true);  // handle the default hotkeys
	DXUTSetCursorSettings(true, true); // Show the cursor and clip it when in full screen
	DXUTCreateWindow(L"D3D Torus");
	DXUTCreateDevice(true, 640, 480);

	// Start the render loop
	DXUTMainLoop();

	// TODO: Perform any application-level cleanup here

	return DXUTGetExitCode();
}


