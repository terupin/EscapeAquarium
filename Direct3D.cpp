#include "Direct3d.h"
#include "WICTextureLoader.h"

// DirectX11�̃��C�u�����������N�w��
#pragma comment (lib, "d3d11.lib")

// �֐��̃v���g�^�C�v�錾

// �f�o�C�X���쐬����֐�
BOOL CreateDevice(HWND hWnd);

// �����_�[�^�[�Q�b�g���쐬����֐�
BOOL CreateRenderTarget();

// �V�F�[�_�[�I�u�W�F�N�g���쐬����֐�
BOOL CreateShader();

// �r���[�|�[�g��ݒ肷��֐�
void SetViewPort();


// �O���[�o���ϐ��̐錾

static DIRECT3D* gpD3D; // DIRECT3D�\���̂𓮓I�m�ۂ����A�h���X������
static UINT gScreenWidth, gScreenHeight; // ��ʂ̕��E����

#include "vs.h"  // �R���p�C���ςݒ��_�V�F�[�_�[
#include "ps.h"  // �R���p�C���ς݃s�N�Z���V�F�[�_�[


BOOL Direct3D_Init(HWND hWnd)
{
	// DIRECT3D�\���̂𓮓I�m�ۂ��ă[���N���A����
	// malloc�֐��@���@�����Ŏw�肵���o�C�g���̃��������m�ۂ���
	// �@�g����悤�ɂ���B
	gpD3D = (DIRECT3D*)malloc(sizeof(DIRECT3D));
	// ZeroMemory�֐��@���@�������Ŏw�肵���A�h���X����A
	// �@�������Ŏw�肵���T�C�Y���A�[���Ŗ��߂�B
	ZeroMemory(gpD3D, sizeof(DIRECT3D));

	// �f�o�C�X�쐬�֐����Ăяo��
	CreateDevice(hWnd);

	// �����_�[�^�[�Q�b�g�쐬�֐����Ăяo��
	CreateRenderTarget();

	// �V�F�[�_�[�I�u�W�F�N�g���쐬����֐����Ăяo��
	CreateShader();

	// �r���[�|�[�g��ݒ肷��֐����Ăяo��
	SetViewPort();

	return 0;
}

void Direct3D_Release()
{
	// DIRECT3D�\���̂̒��̕ϐ��ɂ���
	// ������s���B
	COM_SAFE_RELEASE(gpD3D->samplerPoint);
	COM_SAFE_RELEASE(gpD3D->blendAdd);
	COM_SAFE_RELEASE(gpD3D->blendSub);
	COM_SAFE_RELEASE(gpD3D->blendAlpha);
	COM_SAFE_RELEASE(gpD3D->inputLayout);
	COM_SAFE_RELEASE(gpD3D->pixelShader);
	COM_SAFE_RELEASE(gpD3D->vertexShader);

	COM_SAFE_RELEASE(gpD3D->renderTarget);
	COM_SAFE_RELEASE(gpD3D->swapChain);
	COM_SAFE_RELEASE(gpD3D->context);
	COM_SAFE_RELEASE(gpD3D->device);

	// malloc�֐��Ŋm�ۂ��������������
	if (gpD3D != NULL)
	{
		free(gpD3D); // malloc�Ŋm�ۂ������������������֐�
		gpD3D = NULL;
	}
}

DIRECT3D * Direct3D_Get()
{
	return gpD3D;
}

BOOL CreateDevice(HWND hWnd)
{
	UINT flags = 0;
	// Graphic Tool���g���ăf�o�b�O���s���ꍇ�́A���̍s�̃R�����g���O���ăf�o�b�O���[�h�Ńf�o�C�X���쐬����B
	//flags |= D3D11_CREATE_DEVICE_DEBUG;

	D3D_FEATURE_LEVEL pLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL level;

	RECT                rect;
	DXGI_SWAP_CHAIN_DESC scDesc;

	// �E�C���h�E�̃N���C�A���g�̈�i���ۂɕ`��ł���͈́j�̃T�C�Y���擾
	GetClientRect(hWnd, &rect);
	ZeroMemory(&scDesc, sizeof(scDesc));

	gScreenWidth = (UINT)(rect.right - rect.left);
	gScreenHeight = (UINT)(rect.bottom - rect.top);

	scDesc.BufferCount = 1;
	scDesc.BufferDesc.Width = gScreenWidth;
	scDesc.BufferDesc.Height = gScreenHeight;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.OutputWindow = hWnd;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;
	scDesc.Windowed = TRUE; // �E�C���h�E���[�h�ƃt���X�N���[�����[�h�؂�ւ�

	   // �@�\���x��
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,					// DirectX11.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_11_0,					// DirectX11�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_1,					// DirectX10.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_0,					// DirectX10�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_3,					// DirectX9.3�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_2,					// DirectX9.2�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_1					// Direct9.1�Ή�GPU���x��
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	HRESULT  hr;
	// DirectX11�f�o�C�X�A�R���e�L�X�g�A�X���b�v�`�F�C���̍쐬
	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		flags,
		pLevels,
		1,
		D3D11_SDK_VERSION,
		&scDesc,
		&gpD3D->swapChain,
		&gpD3D->device,
		&level,
		&gpD3D->context);

	return SUCCEEDED(hr);

}

BOOL CreateRenderTarget()
{
	// �o�b�N�o�b�t�@�擾
	ID3D11Texture2D* pBackBuffer = NULL;

	HRESULT hr;
	hr = gpD3D->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	if (SUCCEEDED(hr))
	{
		// �����_�[�^�[�Q�b�g�r���[���쐬
		hr = gpD3D->device->CreateRenderTargetView(pBackBuffer, NULL, &gpD3D->renderTarget);


		if (SUCCEEDED(hr))
		{
			//�[�x�X�e���V���o�b�t�@�쐬
			D3D11_TEXTURE2D_DESC txDesc;

			ZeroMemory(&txDesc, sizeof(txDesc));
			txDesc.Width = gScreenWidth;
			txDesc.Height = gScreenHeight;
			txDesc.MipLevels = 1;
			txDesc.ArraySize = 1;
			txDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			txDesc.SampleDesc.Count = 1;
			txDesc.SampleDesc.Quality = 0;
			txDesc.Usage = D3D11_USAGE_DEFAULT;
			txDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			txDesc.CPUAccessFlags = 0;
			txDesc.MiscFlags = 0;

			hr = gpD3D->device->CreateTexture2D(&txDesc, NULL, &gpD3D->depthStencilTexture);

			if (SUCCEEDED(hr)) {
				D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
				ZeroMemory(&dsDesc, sizeof(dsDesc));
				dsDesc.Format = txDesc.Format;
				dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				dsDesc.Texture2D.MipSlice = 0;
				hr = gpD3D->device->CreateDepthStencilView(gpD3D->depthStencilTexture, &dsDesc, &gpD3D->depthStencilView);
			}

			if (SUCCEEDED(hr))
			{
				// �����_�����O�^�[�Q�b�g��ݒ�@�i���j����2D�Ȃ̂�Z�o�b�t�@�����ɂ��Ă���
				gpD3D->context->OMSetRenderTargets(1, &gpD3D->renderTarget, gpD3D->depthStencilView);

				return TRUE;
			}
		}
	}

	COM_SAFE_RELEASE(pBackBuffer); // �o�b�N�o�b�t�@���

	return FALSE;
}

BOOL CreateShader()
{
	HRESULT hr;
	//���_�V�F�[�_�[����
	hr = gpD3D->device->CreateVertexShader(&g_vs_main, sizeof(g_vs_main), NULL, &gpD3D->vertexShader);

	if (FAILED(hr))
		return FALSE;

	//�s�N�Z���V�F�[�_�[����
	hr = gpD3D->device->CreatePixelShader(&g_ps_main, sizeof(g_ps_main), NULL, &gpD3D->pixelShader);

	if (FAILED(hr))
		return FALSE;

	

	//���_���C�A�E�g�쐬�@�����_�f�[�^��ύX������A���̔z����ύX����
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] = {
		//{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, // xyz
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },// xy

		

//		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
//		  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },// color

		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },// uv

		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },// normal

		

	};

	hr = gpD3D->device->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc),
		g_vs_main, sizeof(g_vs_main), &gpD3D->inputLayout);

	if (FAILED(hr))
		return FALSE;

	// ���_�V�F�[�_�[�̃p�����[�^�Z�b�g
	gpD3D->context->VSSetShader(gpD3D->vertexShader, NULL, 0);
	// �s�N�Z���V�F�[�_�[�̃p�����[�^�Z�b�g
	gpD3D->context->PSSetShader(gpD3D->pixelShader, NULL, 0);

	// ���_���C�A�E�g���Z�b�g
	gpD3D->context->IASetInputLayout(gpD3D->inputLayout);
	// �g�|���W�[�i�v���~�e�B�u�^�C�v�j���Z�b�g
	gpD3D->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // �O�p�`���X�g���w��

	// �u�����h�X�e�[�g�ݒ�
	D3D11_BLEND_DESC BlendDesc;

	// �e�N�X�`�����ߏ������s����悤�ɐݒ�
	ZeroMemory(&BlendDesc, sizeof(BlendDesc));
	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;
	BlendDesc.RenderTarget[0].BlendEnable = TRUE;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	hr = gpD3D->device->CreateBlendState(&BlendDesc, &gpD3D->blendAlpha);

	// ���Z�����p�u�����h�X�e�[�g���쐬
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	hr = gpD3D->device->CreateBlendState(&BlendDesc, &gpD3D->blendAdd);

	// ���Z����
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;

	hr = gpD3D->device->CreateBlendState(&BlendDesc, &gpD3D->blendSub);

	if (SUCCEEDED(hr)) {
		gpD3D->context->OMSetBlendState(gpD3D->blendAlpha, NULL, 0xffffffff);
	}

	// �T���v���[�X�e�[�g���쐬
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // �|�C���g�⊮
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;

	hr = gpD3D->device->CreateSamplerState(&samplerDesc, &gpD3D->samplerPoint);


	return TRUE;
}

void SetViewPort()
{
	//�r���[�|�[�g�ݒ�
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)gScreenWidth;
	viewport.Height = (FLOAT)gScreenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	gpD3D->context->RSSetViewports(1, &viewport);
}

void Direct3D_LoadTexture(const wchar_t * fileName, ID3D11ShaderResourceView ** outTexture)
{
	HRESULT hr;
	DIRECT3D* d3d = Direct3D_Get();
	// �e�N�X�`����ǂݍ���
	hr = DirectX::CreateWICTextureFromFile(d3d->device,
		fileName,
		NULL,
		outTexture);

	// �G���[����
	if (FAILED(hr)) // ���s�����ꍇ
	{
		MessageBoxA(NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�G���[����", MB_OK);
	}
}
