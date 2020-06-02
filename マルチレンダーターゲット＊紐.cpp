///////////////////
#include<windows.h>
#include<stdio.h>
#include<d3dx9.h>
#include<time.h>
#include<math.h>
#include <string>
#include <vector>
#include<iostream>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#pragma warning(disable:4996)

#define	SCRW		1280	// ウィンドウ幅（Width
#define	SCRH		780		// ウィンドウ高さ（Height

LPDIRECT3D9				lpD3D;		// Direct3Dインターフェイス

LPDIRECT3DDEVICE9		lpD3DDevice;	// Direct3DDeviceインターフェイス

D3DPRESENT_PARAMETERS d3dpp;

float pie = 3.141592;

int dea =1;//直径

float moveX = 100, moveY = 100;

int flame;//フレーム数

int buttontime;


// 頂点フォーマットの定義
struct VERTEX
{
	D3DXVECTOR3 Pos;
	D3DCOLOR Color;
	D3DXVECTOR2 Tex;
};

class C_stencil
{
public:
	void Init(void);
	void Move(void);
	void Draw(void);
	void Release(void);
private:
	LPDIRECT3DTEXTURE9 Tex;
	D3DXMATRIX ScaleMat;
	D3DXMATRIX PosMat;
	D3DXMATRIX Mat;
	float Size;
	int X;
	int Y;
}stencil;

LPDIRECT3DTEXTURE9 TmpTex;
LPDIRECT3DSURFACE9 lpTmpBuf;
LPDIRECT3DSURFACE9 lpTmpZBuf;
LPDIRECT3DSURFACE9 lpBackBuf;
LPDIRECT3DSURFACE9 lpZBuf;

//オリジナル用
D3DXMATRIX tmpMat;

//ワイプ用
D3DXMATRIX tmpPos_W;
D3DXMATRIX tmpScale_W;
D3DXMATRIX tmpMat_W;




//プロトタイプ宣言

void LoadTexture(LPDIRECT3DTEXTURE9 *lpTex, const char fname[], int W, int H, D3DCOLOR Color);


const int ropeNum=20;
class
{
public:

	float deg;
	float rad;

	float vrad;//visualrad


	float sx;
	float sy;
	float ex;
	float ey;

	LPDIRECT3DTEXTURE9 Tex;
	D3DXMATRIX TMat; 
	D3DXMATRIX RMat;
	D3DXMATRIX Mat;
}rope[ropeNum];

LPD3DXSPRITE lpSprite;	// スプライト
LPD3DXFONT lpFont;		// フォント

bool gameFullScreen;	// フルスクリーン（true,false)

const int waveNum = 150;
const int colorNum = 30;

class RAINBOW
{
public:

	float deg;
	float rad;

	float vrad;//visualrad

	float sx;
	float sy;
	float ex;
	float ey;

	LPDIRECT3DTEXTURE9 Tex;
	D3DXMATRIX TMat;
	D3DXMATRIX RMat;
	D3DXMATRIX Mat;

	D3DXCOLOR Color;

}rainbow[waveNum][colorNum];
int colorFlg;

D3DCOLOR red = D3DCOLOR_ARGB(125,255,0,0);
D3DCOLOR red1 = D3DCOLOR_ARGB(125, 125, 125, 0);
D3DCOLOR red2 = D3DCOLOR_ARGB(125, 0, 255, 0);
D3DCOLOR red3 = D3DCOLOR_ARGB(125, 0, 125, 125);
D3DCOLOR red4 = D3DCOLOR_ARGB(125, 0, 0, 255);
D3DCOLOR red5 = D3DCOLOR_ARGB(125, 125, 0, 125);
D3DCOLOR rain(int a)
{
	switch (a)
	{
	case 1:
		return red;
	case 2:
		return red1;
	case 0:
		return red2;
	case 3:
		return red3;
	case 4:
		return red4;
	case 5:
		return red5;
	}
}

void rInit();
void rUpdate();
void rDraw();
void rEnd();

void rInit(void)
{
	for (int i = 0; i < colorNum; i++)
	{
		rainbow[0][i].sx = 0;
		rainbow[0][i].sy = 0 + 12 * i;
		for (int j = 0; j < waveNum; j++)
		{
			rainbow[j][i].deg = 90;
		}
	}
	LoadTexture(&rainbow[1][0].Tex, NULL, 0, 0/*画像全部読み込み*/, NULL);

	for (int i = 2; i < waveNum; i++)
	{
		for (int j = 0; j < colorNum; j++)
		{
			LoadTexture(&rainbow[i][j].Tex, "紐あそび.png", 0, 0/*画像全部読み込み*/, NULL);
		}
	}

	for (int i = 0; i < waveNum; i++)
		for (int j = 0; j < colorNum; j++)
		{
			rainbow[i][j].Color = rain(j%6);
		}

}

void rUpdate(void)
{
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		for (int i = 0; i < colorNum; i++)
			rainbow[0][i].sy += 5;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		for (int i = 0; i < colorNum; i++)
			rainbow[0][i].sy -= 5;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		for (int i = 0; i < colorNum; i++)
			rainbow[0][i].sx += 5;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		for (int i = 0; i < colorNum; i++)
			rainbow[0][i].sx -= 5;
	}


	for (int i = 0; i < waveNum; i++)
	{
		for (int j = 0; j < colorNum; j++)
		{
			rainbow[i][j].rad = (pie / 180)*rainbow[i][j].deg;//変更なし
			if (GetAsyncKeyState('W') & 0x8000)
			{
				rainbow[i][j].deg += 1;
			}
			if (GetAsyncKeyState('S') & 0x8000)
			{
				rainbow[i][j].deg -= 1;
			}

			if (/*flame % 2 == 0*/1)
			{
				rainbow[i][j].ex = rainbow[i][j].sx + dea * sin(rainbow[i][j].rad);
				rainbow[i][j].ey = rainbow[i][j].sy + dea * cos(rainbow[i][j].rad);
			}

			if (i != 0)
			{
				rainbow[i][j].sx = rainbow[i - 1][j].ex;
				rainbow[i][j].sy = rainbow[i - 1][j].ey;

				rainbow[i][j].vrad = atan2((rainbow[i][j].ex - rainbow[i - 1][j].ex), (rainbow[i][j].ey - rainbow[i - 1][j].ey));
			}





			D3DXMatrixRotationZ(&rainbow[i][j].RMat, pie * 2 - rainbow[i][j].vrad);
			D3DXMatrixTranslation(&rainbow[i][j].TMat, rainbow[i][j].sx, rainbow[i][j].sy, 0);
			rainbow[i][j].Mat = rainbow[i][j].RMat*rainbow[i][j].TMat;
		}

	}
}
void rEnd(void)
{
	for (int i = 2; i < waveNum; i++)
	{
		for (int j = 0; j < colorNum; j++)
		{
			rainbow[i][j].Tex->Release();
		}
	}

}
void rDraw(void)
{
	RECT rcrainbow = { 0,0,16,16 };
	for (int i = waveNum - 1; i >= 2; i--)
	{
		for (int j = colorNum - 1; j >= 0; j--)
		{
			lpSprite->SetTransform(&rainbow[i][j].Mat);
			lpSprite->Draw(rainbow[i][j].Tex, &rcrainbow, &D3DXVECTOR3(32, 32, 0), NULL, rainbow[i][j].Color);
		}
	}
	for (int j = 0; j < colorNum; j++)
	{
		RECT rcboss = { 0,0,128,128 };
		lpSprite->SetTransform(&rainbow[1][j].Mat);
		lpSprite->Draw(rainbow[1][j].Tex, &rcboss, &D3DXVECTOR3(64, 64, 0), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	}

}

void C_stencil::Init()
{
	LoadTexture(&Tex, "stencil.png", 0, 0, NULL);
	Size = 1;
}

////  グローバル変数宣言
void C_stencil::Move()
{
	//if (GetAsyncKeyState('D') & 0x8000)
	//{
	//	X += 5;
	//}

	//if (GetAsyncKeyState('A') & 0x8000)
	//{
	//	X -= 5;
	//}

	//if (GetAsyncKeyState('W') & 0x8000)
	//{
	//	Y -= 5;
	//}

	//if (GetAsyncKeyState('S') & 0x8000)
	//{
	//	Y += 5;
	//}

	if (GetAsyncKeyState('Z') & 0x8000)
	{
		Size += 0.05f;
	}

	if (GetAsyncKeyState('X') & 0x8000)
	{
		Size -= 0.05f;
		if (Size < 0)
		{
			Size = 0;
		}
	}

	D3DXMatrixTranslation(&PosMat, X, Y, 0);
	D3DXMatrixScaling(&ScaleMat, Size, Size, 1);
	Mat = ScaleMat * PosMat;
}

void C_stencil::Draw()
{
	RECT rcSTC = { 0,0,512,512 };
	lpSprite->SetTransform(&Mat);
	lpSprite->Draw(Tex, &rcSTC, &D3DXVECTOR3(0, 0, 0), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
}

void C_stencil::Release()
{
	if (Tex == NULL)
	{
		return;
	}
	Tex->Release();
}

#define	FVF_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 更新処理

void Init(void)
{
	rope[0].sx = 64;
	rope[0].sy = 300;
	rope[0].deg = 0;

	LoadTexture(&rope[0].Tex, "Boss1_1.png", 0, 0/*画像全部読み込み*/, NULL);

	for (int i=1;i<ropeNum;i++)
	{
		 LoadTexture(&rope[i].Tex, NULL, 0, 0/*画像全部読み込み*/, NULL);
	}
	
}




void Update(void)
{
	if (GetAsyncKeyState('J') & 0x8000)
	{
		rope[0].sy+=5;
	}
	if (GetAsyncKeyState('U') & 0x8000)
	{
		rope[0].sy-=5;
	}
	if (GetAsyncKeyState('K') & 0x8000)
	{
		rope[0].sx+=5;
	}
	if (GetAsyncKeyState('H') & 0x8000)
	{
		rope[0].sx-=5;
	}




	for (int i = 0; i < ropeNum; i++)
	{
		rope[i].rad=(pie/180)*rope[i].deg;//変更なし
		if (GetAsyncKeyState('W') & 0x8000)
		{
			rope[i].deg += 10;
		}
		if (GetAsyncKeyState('S') & 0x8000)
		{
			rope[i].deg -= 10;
		}


		if (/*flame % 2 == 0*/1)
		{
			rope[i].ex = rope[i].sx + dea * sin(rope[i].rad);
			rope[i].ey = rope[i].sy + dea * cos(rope[i].rad);
		}

		if (i != 0)
		{
			rope[i].sx = rope[i - 1].ex;
			rope[i].sy = rope[i - 1].ey;

			rope[i].vrad = atan2((rope[i].ex - rope[i - 1].ex), (rope[i].ey - rope[i - 1].ey));
		}




		


		

		D3DXMatrixRotationZ(&rope[i].RMat, pie*2-rope[i].vrad);
		D3DXMatrixTranslation(&rope[i].TMat, rope[i].sx, rope[i].sy, 0);
		rope[i].Mat = rope[i].RMat*rope[i].TMat;
	}
	rUpdate();
	stencil.Move();
	D3DXMatrixTranslation(&tmpMat, 0, 0, 0);
}

void End(void)
{


}

// 3D描画
void Render3D(void)
{

}

// 動的テクスチャ作成
void DynamicRender2D(void)
{
	lpD3DDevice->SetRenderTarget(0, lpTmpBuf);
	lpD3DDevice->SetDepthStencilSurface(lpTmpZBuf);
	//////////////////////////////////////////////////
	///	スプライトの描画処理
	//////////////////////////////////////////////////
	// 描画開始
	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);


	// バックバッファと Z バッファをクリア
	lpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	rDraw();

	stencil.Draw();

	// 描画終了
	lpSprite->End();
}


// 2D描画
void Render2D(void)
{
	//////////////////////////////////////////////////
	///	スプライトの描画処理
	//////////////////////////////////////////////////
	// 描画開始

	lpD3DDevice->SetRenderTarget(0, lpBackBuf);
	lpD3DDevice->SetDepthStencilSurface(lpZBuf);
	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);
	//// アルファブレンディング方法を設定
	lpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	lpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//               左上右下
	RECT rcrope = { 0,0,64,16 };	
	for (int i = 1; i < ropeNum; i++)
	{
		lpSprite->SetTransform(&rope[i].Mat);
		lpSprite->Draw(rope[i].Tex, &rcrope, &D3DXVECTOR3(32, 32, 0), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	RECT rcboss = { 0,0,128,128 };
	lpSprite->SetTransform(&rope[0].Mat);
	lpSprite->Draw(rope[0].Tex, &rcboss, &D3DXVECTOR3(64, 64, 0), NULL, D3DCOLOR_ARGB(100, 255, 255, 255));
	lpSprite->Flush();
	//// アルファブレンディング方法を設定
	lpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ZERO);
	lpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR);

	RECT rc = { 0,0,500,500 };
	lpSprite->SetTransform(&tmpMat);
	lpSprite->Draw(TmpTex, &rc, &D3DXVECTOR3(0, 0, 0), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	lpSprite->Flush();

	// 描画終了
	lpSprite->End();
}

void GameFrame(void)
{
	// バックバッファと Z バッファをクリア
	lpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
	//色変わる
	//lpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(flame%27, flame%128, flame%54), 1.0f, 0);


	// 更新処理
	Update();


	// 描画開始
	lpD3DDevice->BeginScene();

	D3DXMATRIX mView, mProj;

	// 視点行列の設定
	D3DXMatrixLookAtLH(&mView,
		&D3DXVECTOR3(0.0f, 0.0f, -10.0f),	// カメラの位置
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// カメラの視点
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f)	// カメラの頭の方向
	);

	// 投影行列の設定
	D3DXMatrixPerspectiveFovLH(&mProj, D3DXToRadian(60), (float)SCRW / (float)SCRH, 1.0f, 1000.0f);

	//行列設定
	lpD3DDevice->SetTransform(D3DTS_VIEW, &mView);
	lpD3DDevice->SetTransform(D3DTS_PROJECTION, &mProj);



	// 3D描画
	Render3D();
	DynamicRender2D();
	// 2D描画
	Render2D();



	// 描画終了
	lpD3DDevice->EndScene();

	// バックバッファをプライマリバッファにコピー
	lpD3DDevice->Present(NULL, NULL, NULL, NULL);

	//flame count
	flame++;
}

LRESULT APIENTRY WndFunc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		}
		return 0;

	}
	return DefWindowProc(hwnd, msg, wParam, lParam);

}

void LoadTexture(LPDIRECT3DTEXTURE9 *lpTex, const char fname[], int W, int H, D3DCOLOR Color)
{
	if (W == 0)W = D3DX_DEFAULT;
	if (H == 0)H = D3DX_DEFAULT;
	D3DXCreateTextureFromFileEx(lpD3DDevice, fname, W, H, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_DEFAULT, Color, NULL, NULL, lpTex);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev,
	LPSTR lpszCmdParam, int nCmdshow)
{
	MSG msg;

	HWND hwnd;
	WNDCLASS wc;
	char szAppName[] = "Generic Game SDK Window";

	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WndFunc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = szAppName;

	RegisterClass(&wc);

	hwnd = CreateWindowEx(
		0,
		szAppName,
		"Direct X",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		SCRW, SCRH,
		NULL, NULL, hInst,
		NULL);

	if (!hwnd)return FALSE;

	ShowWindow(hwnd, nCmdshow);
	UpdateWindow(hwnd);
	SetFocus(hwnd);

	gameFullScreen = false;	// ウィンドウモード

	if (gameFullScreen) {
		ShowCursor(FALSE);
	}
	else {
		RECT rc = { 0,0,SCRW,SCRH };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		SetWindowPos(hwnd, NULL, 50, 50, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW | SWP_NOZORDER);
	}

	//---------------------DirectX Graphics関連-----------------------

	// Direct3D オブジェクトを作成
	lpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!lpD3D)
	{
		// オブジェクト作成失敗
		MessageBox(NULL, "Direct3D の作成に失敗しました。", "ERROR", MB_OK | MB_ICONSTOP);
		// 終了する
		return 0;
	}
	int adapter;

	// 使用するアダプタ番号
	adapter = D3DADAPTER_DEFAULT;

	// ウインドウの作成が完了したので、Direct3D を初期化する
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	// Direct3D 初期化パラメータの設定
	if (gameFullScreen)
	{
		// フルスクリーン : ほとんどのアダプタでサポートされているフォーマットを使用
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	else
	{
		// ウインドウ : 現在の画面モードを使用
		D3DDISPLAYMODE disp;
		// 現在の画面モードを取得
		lpD3D->GetAdapterDisplayMode(adapter, &disp);
		d3dpp.BackBufferFormat = disp.Format;
	}
	// 表示領域サイズの設定
	d3dpp.BackBufferWidth = SCRW;
	d3dpp.BackBufferHeight = SCRH;
	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;

	if (!gameFullScreen)
	{
		// ウインドウモード
		d3dpp.Windowed = 1;
	}

	// Z バッファの自動作成
	d3dpp.EnableAutoDepthStencil = 1;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	//ﾊﾞｯｸﾊﾞｯﾌｧをﾛｯｸ可能にする(GetDCも可能になる)
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	// デバイスの作成 - T&L HAL
	if (FAILED(lpD3D->CreateDevice(adapter, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &lpD3DDevice)))
	{
		// 失敗したので HAL で試行
		if (FAILED(lpD3D->CreateDevice(adapter, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &lpD3DDevice)))
		{
			// 失敗したので REF で試行
			if (FAILED(lpD3D->CreateDevice(adapter, D3DDEVTYPE_REF, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &lpD3DDevice)))
			{
				// 結局失敗した
				MessageBox(NULL, "DirectX9が初期化できません。\n未対応のパソコンと思われます。", "ERROR", MB_OK | MB_ICONSTOP);
				lpD3D->Release();
				// 終了する
				return 0;
			}
		}
	}

	// レンダリング・ステートを設定
	// Z バッファ有効化->前後関係の計算を正確にしてくれる
	lpD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	lpD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// アルファブレンディング有効化
	lpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// アルファブレンディング方法を設定
	lpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	lpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// レンダリング時のアルファ値の計算方法の設定
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	// テクスチャの色を使用
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	// 頂点の色を使用
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	// レンダリング時の色の計算方法の設定
	lpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	//裏面カリング
	lpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// フィルタ設定
	lpD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	lpD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	lpD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// ライト
	lpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	lpD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	// 頂点フォーマットの設定
	lpD3DDevice->SetFVF(FVF_VERTEX);

	timeBeginPeriod(1);

	// ゲームに関する初期化処理 ---------------------------

	Init();
	stencil.Init();
	rInit();

	//テクスチャー作成
	D3DXCreateTexture(lpD3DDevice,
		SCRW, SCRH,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT,
		&TmpTex);

	//テクスチャーのサーフェスを取得
	TmpTex->GetSurfaceLevel(0, &lpTmpBuf);

	//Zバッファー作成
	lpD3DDevice->GetDepthStencilSurface(&lpTmpZBuf);//切り替え先の初期化

	//バックアップ１
	lpD3DDevice->GetBackBuffer(0, 0,
		D3DBACKBUFFER_TYPE_MONO, &lpBackBuf);

	//バックアップ２
	lpD3DDevice->GetDepthStencilSurface(&lpZBuf);

	// スプライト作成
	D3DXCreateSprite(lpD3DDevice, &lpSprite);
	lpSprite->OnResetDevice();

	// フォント作成
	D3DXCreateFont(lpD3DDevice, 30, 30, FW_REGULAR, NULL, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "ＭＳ ゴシック", &lpFont);

	lpFont->OnResetDevice();

	while (1) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0))
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			GameFrame();
		}
	}

	// ゲームに関する終了処理 ---------------------------
	End();
	rEnd();


	lpSprite->Release();	// スプライト
	lpFont->Release();		// フォント


	timeEndPeriod(1);

	// Direct3D オブジェクトを解放
	lpD3DDevice->Release();
	lpD3D->Release();

	return (int)msg.wParam;
}