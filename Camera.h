#pragma once

#include <DirectXMath.h>

// �J������\���N���X
class Camera
{

public:
	// �r���[�ϊ��s���Ԃ�
	DirectX::XMMATRIX GetViewMatrix();

	// �J�����ʒu��ݒ�
	void SetEye(DirectX::XMFLOAT3 newEye);
	// �J���������_
	void SetFocus(DirectX::XMFLOAT3 newFocus);
	// �J����������x�N�g��
	void SetUp(DirectX::XMFLOAT3 newUp);

	// �J�����ɕK�v�ȃp�����[�^�ϐ�
	DirectX::XMFLOAT3 mEye, mFocus, mUp;

	// �X�V�����i�����[�v�P��Ăяo���j
	virtual void Update();

	static Camera* mMainCamera;

private:
	// �v�Z�����r���[�ϊ��s���ۑ�����ϐ�
	DirectX::XMMATRIX mViewMatrix;

};

