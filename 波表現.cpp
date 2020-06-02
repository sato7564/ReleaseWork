
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

#define	SCRW		1280	// �E�B���h�E���iWidth
#define	SCRH		780		// �E�B���h�E�����iHeight

LPDIRECT3D9				lpD3D;		// Direct3D�C���^�[�t�F�C�X

LPDIRECT3DDEVICE9		lpD3DDevice;	// Direct3DDevice�C���^�[�t�F�C�X

D3DPRESENT_PARAMETERS d3dpp;


// ���_�t�H�[�}�b�g�̒�`
struct VERTEX
{
	D3DXVECTOR3 Pos;
	D3DCOLOR Color;
	D3DXVECTOR2 Tex;
};

//�v���g�^�C�v�錾

void LoadTexture(LPDIRECT3DTEXTURE9 *lpTex, const char fname[], int W, int H, D3DCOLOR Color);

////  �O���[�o���ϐ��錾

const int ropeNum=150;
const int waveNum = 30;
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

	D3DXCOLOR Color;
}rope[ropeNum][waveNum];

LPD3DXSPRITE lpSprite;	// �X�v���C�g
LPD3DXFONT lpFont;		// �t�H���g

bool gameFullScreen;	// �t���X�N���[���itrue,false)




float pie = 3.14159265359;

int dea = 5*2;//���a

int flame;//�t���[����

int buttontime;



#define	FVF_VERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// �X�V����

void Init(void)
{
	for (int i = 0; i < waveNum; i++)
	{
		rope[0][i].sx = 0;
		rope[0][i].sy = 0+30*i;
		for (int j = 0; j < ropeNum; j++)
		{
			rope[j][i].deg = 90;
		}
	}
	LoadTexture(&rope[1][0].Tex, "Boss1_1.png", 0, 0/*�摜�S���ǂݍ���*/, NULL);

	for (int i=2;i<ropeNum;i++)
	{
		for (int j = 0; j < waveNum; j++)
		{
			LoadTexture(&rope[i][j].Tex, "�R������.png", 0, 0/*�摜�S���ǂݍ���*/, NULL);
		}
	}

	for(int i=0;i<ropeNum;i++)
		for (int j = 0; j < waveNum; j++)
		{
			rope[i][j].Color = D3DCOLOR_ARGB(255, 0, 1 * (waveNum-j), (255/waveNum) * (waveNum - j));
		}
			
	
}




void Update(void)
{
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		for (int i=0;i<waveNum;i++)
		rope[0][i].sy+=5;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		for (int i = 0; i < waveNum; i++)
		rope[0][i].sy-=5;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		for (int i = 0; i < waveNum; i++)
		rope[0][i].sx+=5;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		for (int i = 0; i < waveNum; i++)
		rope[0][i].sx-=5;
	}


	for (int i = 0; i < ropeNum; i++)
	{
		for (int j = 0; j < waveNum; j++)
		{
			rope[i][j].rad = (pie / 180)*rope[i][j].deg;//�ύX�Ȃ�
			if (GetAsyncKeyState('W') & 0x8000)
			{
				rope[i][j].deg += 1;
			}
			if (GetAsyncKeyState('S') & 0x8000)
			{
				rope[i][j].deg -= 1;
			}

			if (/*flame % 2 == 0*/1)
			{
				rope[i][j].ex = rope[i][j].sx + dea * sin(rope[i][j].rad);
				rope[i][j].ey = rope[i][j].sy + dea * cos(rope[i][j].rad);
			}

			if (i != 0)
			{
				rope[i][j].sx = rope[i - 1][j].ex;
				rope[i][j].sy = rope[i - 1][j].ey;

				rope[i][j].vrad = atan2((rope[i][j].ex - rope[i - 1][j].ex), (rope[i][j].ey - rope[i - 1][j].ey));
			}





			D3DXMatrixRotationZ(&rope[i][j].RMat, pie * 2 - rope[i][j].vrad);
			D3DXMatrixTranslation(&rope[i][j].TMat, rope[i][j].sx, rope[i][j].sy, 0);
			rope[i][j].Mat = rope[i][j].RMat*rope[i][j].TMat;
		}

	}

}

void End(void)
{
	for (int i = 1; i < ropeNum; i++)
	{
		for (int j = 0; i < waveNum; i++)
		{
			rope[i][j].Tex->Release();
		}
	}

}

// 3D�`��
void Render3D(void)
{

}

// 2D�`��
void Render2D(void)
{
	//////////////////////////////////////////////////
	///	�X�v���C�g�̕`�揈��
	//////////////////////////////////////////////////
	// �`��J�n
	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);

	//               ����E��
	RECT rcrope = { 0,0,63,63 };	
	for (int i =ropeNum-1; i >= 2; i--)
	{
		for (int j = waveNum-1; j >= 0; j--)
		{
			lpSprite->SetTransform(&rope[i][j].Mat);
			lpSprite->Draw(rope[i][j].Tex, &rcrope, &D3DXVECTOR3(32, 32, 0), NULL, rope[i][j].Color);
		}
	}
	for (int j = 0; j < waveNum; j++)
	{
		RECT rcboss = { 0,0,128,128 };
		lpSprite->SetTransform(&rope[1][j].Mat);
		lpSprite->Draw(rope[1][j].Tex, &rcboss, &D3DXVECTOR3(64, 64, 0), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	}

	// �`��I��
	lpSprite->End();
}

void GameFrame(void)
{
	// �o�b�N�o�b�t�@�� Z �o�b�t�@���N���A
	lpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);
	//�F�ς��
	//lpD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(flame%27, flame%128, flame%54), 1.0f, 0);


	// �X�V����
	Update();


	// �`��J�n
	lpD3DDevice->BeginScene();

	D3DXMATRIX mView, mProj;

	// ���_�s��̐ݒ�
	D3DXMatrixLookAtLH(&mView,
		&D3DXVECTOR3(0.0f, 0.0f, -10.0f),	// �J�����̈ʒu
		&D3DXVECTOR3(0.0f, 0.0f, 0.0f),	// �J�����̎��_
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f)	// �J�����̓��̕���
	);

	// ���e�s��̐ݒ�
	D3DXMatrixPerspectiveFovLH(&mProj, D3DXToRadian(60), (float)SCRW / (float)SCRH, 1.0f, 1000.0f);

	//�s��ݒ�
	lpD3DDevice->SetTransform(D3DTS_VIEW, &mView);
	lpD3DDevice->SetTransform(D3DTS_PROJECTION, &mProj);



	// 3D�`��
	Render3D();

	// 2D�`��
	Render2D();



	// �`��I��
	lpD3DDevice->EndScene();

	// �o�b�N�o�b�t�@���v���C�}���o�b�t�@�ɃR�s�[
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

	gameFullScreen = false;	// �E�B���h�E���[�h

	if (gameFullScreen) {
		ShowCursor(FALSE);
	}
	else {
		RECT rc = { 0,0,SCRW,SCRH };
		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
		SetWindowPos(hwnd, NULL, 50, 50, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW | SWP_NOZORDER);
	}

	//---------------------DirectX Graphics�֘A-----------------------

	// Direct3D �I�u�W�F�N�g���쐬
	lpD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!lpD3D)
	{
		// �I�u�W�F�N�g�쐬���s
		MessageBox(NULL, "Direct3D �̍쐬�Ɏ��s���܂����B", "ERROR", MB_OK | MB_ICONSTOP);
		// �I������
		return 0;
	}
	int adapter;

	// �g�p����A�_�v�^�ԍ�
	adapter = D3DADAPTER_DEFAULT;

	// �E�C���h�E�̍쐬�����������̂ŁADirect3D ������������
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	// Direct3D �������p�����[�^�̐ݒ�
	if (gameFullScreen)
	{
		// �t���X�N���[�� : �قƂ�ǂ̃A�_�v�^�ŃT�|�[�g����Ă���t�H�[�}�b�g���g�p
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	else
	{
		// �E�C���h�E : ���݂̉�ʃ��[�h���g�p
		D3DDISPLAYMODE disp;
		// ���݂̉�ʃ��[�h���擾
		lpD3D->GetAdapterDisplayMode(adapter, &disp);
		d3dpp.BackBufferFormat = disp.Format;
	}
	// �\���̈�T�C�Y�̐ݒ�
	d3dpp.BackBufferWidth = SCRW;
	d3dpp.BackBufferHeight = SCRH;
	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;

	if (!gameFullScreen)
	{
		// �E�C���h�E���[�h
		d3dpp.Windowed = 1;
	}

	// Z �o�b�t�@�̎����쐬
	d3dpp.EnableAutoDepthStencil = 1;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	//�ޯ��ޯ̧��ۯ��\�ɂ���(GetDC���\�ɂȂ�)
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	// �f�o�C�X�̍쐬 - T&L HAL
	if (FAILED(lpD3D->CreateDevice(adapter, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &lpD3DDevice)))
	{
		// ���s�����̂� HAL �Ŏ��s
		if (FAILED(lpD3D->CreateDevice(adapter, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &lpD3DDevice)))
		{
			// ���s�����̂� REF �Ŏ��s
			if (FAILED(lpD3D->CreateDevice(adapter, D3DDEVTYPE_REF, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &lpD3DDevice)))
			{
				// ���ǎ��s����
				MessageBox(NULL, "DirectX9���������ł��܂���B\n���Ή��̃p�\�R���Ǝv���܂��B", "ERROR", MB_OK | MB_ICONSTOP);
				lpD3D->Release();
				// �I������
				return 0;
			}
		}
	}

	// �����_�����O�E�X�e�[�g��ݒ�
	// Z �o�b�t�@�L����->�O��֌W�̌v�Z�𐳊m�ɂ��Ă����
	lpD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	lpD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// �A���t�@�u�����f�B���O�L����
	lpD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// �A���t�@�u�����f�B���O���@��ݒ�
	lpD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	lpD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �����_�����O���̃A���t�@�l�̌v�Z���@�̐ݒ�
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	// �e�N�X�`���̐F���g�p
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	// ���_�̐F���g�p
	lpD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	// �����_�����O���̐F�̌v�Z���@�̐ݒ�
	lpD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	//���ʃJ�����O
	lpD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �t�B���^�ݒ�
	lpD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	lpD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	lpD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	// ���C�g
	lpD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	lpD3DDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	// ���_�t�H�[�}�b�g�̐ݒ�
	lpD3DDevice->SetFVF(FVF_VERTEX);

	timeBeginPeriod(1);

	// �Q�[���Ɋւ��鏉�������� ---------------------------

	Init();



	// �X�v���C�g�쐬
	D3DXCreateSprite(lpD3DDevice, &lpSprite);
	lpSprite->OnResetDevice();

	// �t�H���g�쐬
	D3DXCreateFont(lpD3DDevice, 30, 30, FW_REGULAR, NULL, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN, "�l�r �S�V�b�N", &lpFont);

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

	// �Q�[���Ɋւ���I������ ---------------------------
	End();


	lpSprite->Release();	// �X�v���C�g
	lpFont->Release();		// �t�H���g


	timeEndPeriod(1);

	// Direct3D �I�u�W�F�N�g�����
	lpD3DDevice->Release();
	lpD3D->Release();

	return (int)msg.wParam;
}