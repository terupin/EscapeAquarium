#pragma once

#include "ModelData.h"
#include "Camera.h"
#include <DirectXMath.h>

// 3D���f����\������
class Model
{

public:
	virtual void Draw(); // �\������

	// ���̃N���X�ŕ\������3D�f�[�^���Z�b�g����
	void SetModelData(ModelData model);

	int direction;

	// ���z���E�i���[���h�j��̈ʒu���w�肷����W
	DirectX::XMFLOAT3 mPos;
	// ��]�p�x
	DirectX::XMFLOAT3 mRotate;
	// �g��k����
	DirectX::XMFLOAT3 mScale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

	// �g��k������xyz�ɓ����l�Őݒ肷��
	void SetScale(float newScale);

	// ���̃��f���ɓK�p����J�����I�u�W�F�N�g
	Camera* mCamera;

	bool m2dRender = false;
	
	// 2D�`����s�����ǂ����؂�ւ���
	void Set2dRender(bool is2D)
	{
		m2dRender = is2D;
		mLightActive = !is2D;
	}

	// ���f���̃f�B�t���[�Y�}�e���A���F��ݒ�
	void SetDiffuse(DirectX::XMFLOAT4 diffuse)
	{
		mDiffuse = diffuse;
	}

	// UV�A�j���[�V�����p�I�t�Z�b�g
	DirectX::XMFLOAT2 mUvOffset = DirectX::XMFLOAT2(0,0);

protected:
	// 3D�f�[�^�����ϐ�
	ModelData mModelData;

	// ���C�g��L���ɂ��邩
	bool mLightActive = true;

	// �f�B�t���[�Y�}�e���A���F
	DirectX::XMFLOAT4 mDiffuse = DirectX::XMFLOAT4(1,1,1,1);

	// �萔�o�b�t�@�̓]���p
	struct ConstBufferData
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
		DirectX::XMMATRIX worldRotate;
		// UV�A�j���[�V�����p�I�t�Z�b�g�iz��w�͎g��Ȃ��j
		DirectX::XMFLOAT4 uvOffset;
		// �f�B�t���[�Y�E�}�e���A��
		DirectX::XMFLOAT4 diffuse;
		// ���s����
		DirectX::XMFLOAT3 lightVector;
		float lightPower;
		// �A���r�G���g
		DirectX::XMFLOAT4 ambient;
	};

	// ���[���h�E�r���[�E�v���W�F�N�V�����E��]�s����쐬����out�ɃZ�b�g����
	virtual void GetWVPRMatrix(ConstBufferData& out);

};

