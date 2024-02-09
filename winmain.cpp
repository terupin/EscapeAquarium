#undef UNICODE  // Unicode�ł͂Ȃ��A�}���`�o�C�g�������g��

#include <Windows.h>
#include "Direct3d.h"
#include "input.h"
#include <DirectXMath.h>
#include "WICTextureLoader.h"
#include "ObjModelLoader.h"
#include "Model.h"
#include "BackCamera.h"
#include "NormalObject.h"
#include "Explosion.h"
#include <map> // �A�z�z��
#include <vector> // �ϔz��
#include"Game_Scene.h"
#include "Title_Scene.h"
#include"nanido.h"
#include"Gole_Scene.h"

#pragma comment (lib, "winmm.lib") // timeGetTime�֐��̂���

// �}�N����`
#define CLASS_NAME    "DX21Smpl"// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME  "�X�P���g���v���O����"// �E�B���h�E�̖��O

#define SCREEN_WIDTH (1024)	// �E�C���h�E�̕�
#define SCREEN_HEIGHT (576+30)	// �E�C���h�E�̍���


// �֐��̃v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// ���_�o�b�t�@�p�̕ϐ�
ID3D11Buffer* gpVertexBuffer;

// ���_�������ϐ�
int gNumVertex;

// �e�N�X�`���p�̕ϐ�
ID3D11ShaderResourceView* gpTextureHalOsaka; // HAL OSAKA

//��ʑJ�ڗp�ϐ�
int Scenenow = Title;

extern ID3D11Buffer* gpConstBuffer;

using namespace DirectX;

bool Game_Scene_Start = false;

// �f���^�^�C���p�̕ϐ�
DWORD gDeltaTime;

// �r���{�[�h�p�̃|���S�����쐬���A�e�N�X�`�����ǂݍ���
ModelData CreateSquarePolygon(float width, float height, float uvWidth, float uvHeight, const wchar_t* texFile)
{
	// ���f�����_�f�[�^�쐬
	const float w = width / 2.0f;
	const float h = height / 2.0f;
	ModelVertex vx[] = {
		{ -w,  h, 0, 0.0f, 0.0f },
		{  w,  h, 0, uvWidth, 0.0f },
		{  w, -h, 0, uvWidth, uvHeight },

		{  w, -h, 0, uvWidth, uvHeight },
		{ -w, -h, 0, 0.0f, uvHeight },
		{ -w,  h, 0, 0.0f, 0.0f },
	};

	vx[0].u = 0.0f;
	vx[0].v = 0.0f;
	vx[1].u = uvWidth;
	vx[1].v = 0.0f;
	vx[2].u = uvWidth;
	vx[2].v = uvHeight;
	vx[3].u = uvWidth;
	vx[3].v = uvHeight;
	vx[4].u = 0.0f;
	vx[4].v = uvHeight;
	vx[5].u = 0.0f;
	vx[5].v = 0.0f;

	ModelData modelData;

	modelData.mNumVertex = sizeof(vx) / sizeof(ModelVertex); // ���_�����v�Z

	// ���f���p�̒��_�o�b�t�@�쐬
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(vx);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// �o�b�t�@�̏����l�w��
	D3D11_SUBRESOURCE_DATA initialVertexData;
	// ���_�o�b�t�@�̏����l
	initialVertexData.pSysMem = vx;
	// ���_�o�b�t�@�ł͎g�p���Ȃ�
	initialVertexData.SysMemPitch = 0;
	initialVertexData.SysMemSlicePitch = 0;

	HRESULT hr = Direct3D_Get()->device->CreateBuffer(&bufferDesc, &initialVertexData, &modelData.mVertexBuffer);

	if (FAILED(hr))
	{
		throw hr;
	}

	Direct3D_LoadTexture(texFile, &modelData.mSRV);

	return modelData;
}

// WinMain�֐������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc; // WND = Window

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc); // Windows�ɃE�C���h�E����o�^

	HWND hWnd; // H=Handle=�|�C���^ WND=Window
	hWnd = CreateWindowEx(0,// �g���E�B���h�E�X�^�C��
		CLASS_NAME,// �E�B���h�E�N���X�̖��O
		WINDOW_NAME,// �E�B���h�E�̖��O
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,// �E�B���h�E�X�^�C��
		CW_USEDEFAULT,// �E�B���h�E�̍���w���W
		CW_USEDEFAULT,// �E�B���h�E�̍���x���W 
		SCREEN_WIDTH,// �E�B���h�E�̕�
		SCREEN_HEIGHT,// �E�B���h�E�̍���
		NULL,// �e�E�B���h�E�̃n���h��
		NULL,// ���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,// �C���X�^���X�n���h��
		NULL);// �E�B���h�E�쐬�f�[�^

	// �w�肳�ꂽ�E�B���h�E�̕\����Ԃ�ݒ�(�E�B���h�E��\��)
	ShowWindow(hWnd, nCmdShow);
	// �E�B���h�E�̏�Ԃ𒼂��ɔ��f(�E�B���h�E�̃N���C�A���g�̈���X�V)
	UpdateWindow(hWnd);

	// Direct3D�̏������֐����Ăяo��
	Direct3D_Init(hWnd);

	Title_Scene*Title_S = new Title_Scene;
	Game_Scene*Game = new Game_Scene;
	Gole_Scene*GOAL = new Gole_Scene;
	nanido*STAGE = new nanido;

	MSG msg;
	// ���C�����[�v
	for (;;) {
		// �O��̃��[�v���烆�[�U�[���삪�����������ׂ�
		BOOL doesMessageExist = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

		if (doesMessageExist)
		{
			// �ԐړI�ɃE�C���h�E�v���V�[�W�����Ăяo��
			DispatchMessage(&msg);

			// �A�v���I�����߂�����
			if (msg.message == WM_QUIT) {
				break;
			}
		}
		else
		{
			// �f���^�^�C���̌v��
			static DWORD lastTime = timeGetTime(); // �O��v�����̎���
			timeBeginPeriod(1); // ���x���P�~���b�ɏグ��
			DWORD nowTime = timeGetTime(); // ���݂̎���
			timeEndPeriod(1); // ���x�����ɖ߂�
			gDeltaTime = nowTime - lastTime; // �������f���^�^�C��
			lastTime = nowTime; // �O��v�����ԂƂ��ĕۑ�

			// �Q�[�����[�v
			switch (Scenenow)
			{
			case Title:
				Title_S->Update();
				Title_S->Draw();
				break;
			
			case Stage:
				STAGE->Update();
				STAGE->Draw();
				break;

			case Game_:
				Game->Update();
				Game->Draw();
				break;

			case Goal:
				GOAL->Update();
				GOAL->Draw();

				break;
			}

			Input_Refresh(); // �L�[��Ԃ̍X�V
		}
	}

	delete Game;
	delete Title_S;
	delete GOAL;
	// Direct3D�̉���֐����Ăяo��
	Direct3D_Release();

	UnregisterClass(CLASS_NAME, hInstance);

	return (int)msg.wParam;
}


// �E�C���h�E�v���V�[�W���֐������
// ���֐�������̂̓O���[�o���̈�(=�ǂ̊֐��̒��ł��Ȃ��ꏊ)�����I
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// uMsg�i���̊֐��̑�2�����j���A���[�U�[�����ID���
	switch (uMsg)
	{
	case WM_DESTROY:// �E�B���h�E�j���̃��b�Z�[�W
		PostQuitMessage(0);// �gWM_QUIT�h���b�Z�[�W�𑗂�@���@�A�v���I��
		break;

	case WM_CLOSE:  // x�{�^���������ꂽ��
		DestroyWindow(hWnd);  // �gWM_DESTROY�h���b�Z�[�W�𑗂�
		break;

	case WM_LBUTTONDOWN: // ���N���b�N���ꂽ�Ƃ�
		Input_SetKeyDown(VK_LBUTTON);
		break;

	case WM_LBUTTONUP: // ���N���b�N���ꂽ�Ƃ�
		Input_SetKeyUp(VK_LBUTTON);
		break;

	case WM_RBUTTONDOWN: // ���N���b�N���ꂽ�Ƃ�
		Input_SetKeyDown(VK_RBUTTON);
		break;

	case WM_RBUTTONUP: // ���N���b�N���ꂽ�Ƃ�
		Input_SetKeyUp(VK_RBUTTON);
		break;

	case WM_MOUSEMOVE: // �}�E�X�J�[�\�����������Ƃ�
		break;

	case WM_KEYDOWN:
		// �L�[�������ꂽ���̃��A�N�V����������
		// ESC�������ꂽ�̂��ǂ����`�F�b�N
		if (LOWORD(wParam) == VK_ESCAPE)
		{
			// ���b�Z�[�W�{�b�N�X�ŏC���m�F
			int result;
			result = MessageBox(NULL, "�I�����Ă�낵���ł����H",
				"�I���m�F", MB_YESNO | MB_ICONQUESTION);
			if (result == IDYES) // �u�͂��v�{�^���������ꂽ��
			{
				// x�{�^���������ꂽ�̂Ɠ������ʂ𔭊�����
				PostMessage(hWnd, WM_CLOSE, wParam, lParam);
			}
		}
		Input_SetKeyDown(LOWORD(wParam));
		break;

	case WM_KEYUP: // �L�[�������ꂽ�C�x���g
		Input_SetKeyUp(LOWORD(wParam));
		break;

	default:
		// ���case�ȊO�̏ꍇ�̏��������s
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}




