#pragma once

#include <d3d11.h> // DirectX11�̃C���N���[�h�t�@�C��

// ����p�̃}�N��
#define COM_SAFE_RELEASE(o)  if(o!=NULL){o->Release();o=NULL;}

// Direct3D�֌W�̕ϐ����܂Ƃ߂�\����
struct DIRECT3D
{
	ID3D11Device* device; // �f�o�C�X
	ID3D11DeviceContext* context; // �R���e�L�X�g
	IDXGISwapChain* swapChain; // �X���b�v�`�F�C��
	ID3D11RenderTargetView* renderTarget; // �����_�[�^�[�Q�b�g
	ID3D11VertexShader* vertexShader; // ���_�V�F�[�_�[
	ID3D11PixelShader* pixelShader; // �s�N�Z���V�F�[�_�[
	ID3D11InputLayout* inputLayout; // �C���v�b�g���C�A�E�g
	ID3D11BlendState* blendAlpha;  // �A���t�@�u�����f�B���O�p�u�����h�X�e�[�g
	ID3D11BlendState* blendAdd; // ���Z�����p
	ID3D11BlendState* blendSub; // ���Z�����p
	ID3D11SamplerState* samplerPoint; // �|�C���g�⊮�p�T���v���[
	ID3D11Texture2D*        depthStencilTexture;
	ID3D11DepthStencilView* depthStencilView;
};

// �֐��̃v���g�^�C�v�錾

// Direct3D�̏��������s���֐�
BOOL Direct3D_Init(HWND hWnd);

// Direct3D�̉�����s���֐�
void Direct3D_Release();

// DIRECT3D�\���̂̎��̂̃A�h���X��Ԃ�
DIRECT3D* Direct3D_Get();

// �e�N�X�`����ǂݍ��ފ֐�
// �������F�e�N�X�`���̃t�@�C����
// �������F�ǂݍ��񂾃e�N�X�`�����i�[����ϐ��̃A�h���X
void Direct3D_LoadTexture(const wchar_t* fileName,
	ID3D11ShaderResourceView** outTexture);

