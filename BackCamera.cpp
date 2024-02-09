#include "BackCamera.h"

using namespace DirectX;

void BackCamera::Update()
{
	// �ǔ��J�����̏���
	// �@����L�����̑O�����x�N�g�����擾����
	XMFLOAT3 forwardVec = mTarget->GetForwardVector();
	// �A�O�����x�N�g���𔽓]�����Č������x�N�g�������
	XMFLOAT3 backVec;
	backVec.x = -forwardVec.x;
	backVec.y = -forwardVec.y;
	backVec.z = -forwardVec.z;
	// �B�������x�N�g����ɃJ������u��
	XMFLOAT3 camEye;
	Model* pTargetModel = mTarget->GetModel();
	camEye.x = pTargetModel->mPos.x + backVec.x * 2.0f;
	camEye.y = pTargetModel->mPos.y + backVec.y * 2.0f + 0.5f;
	camEye.z = pTargetModel->mPos.z + backVec.z * 2.0f;
	// �ɂ₩�J��������
	// 1�t���[���O�̃J�����ʒu�ۑ��ϐ�
	static XMFLOAT3 lastCamEye;
	float blendFactor = 0.997f; // ���ω��̏d��
	camEye.x = lastCamEye.x * blendFactor
		+ camEye.x * (1.0f - blendFactor);
	camEye.y = lastCamEye.y * blendFactor
		+ camEye.y * (1.0f - blendFactor);
	camEye.z = lastCamEye.z * blendFactor
		+ camEye.z * (1.0f - blendFactor);

	this->SetEye(camEye);
	lastCamEye = camEye; // ���̃��[�v�̂��ߕۑ�

	// �C�J���������_�ifocus�j���L�����̏����O�ɐݒ�
	XMFLOAT3 camFocus;
	camFocus.x = pTargetModel->mPos.x + forwardVec.x;
	camFocus.y = pTargetModel->mPos.y + forwardVec.y;
	camFocus.z = pTargetModel->mPos.z + forwardVec.z;
	this->SetFocus(camFocus);

	// �r���[�ϊ��s��v�Z
	Camera::Update();
}

void BackCamera::SetTarget(GameObject * pTarget)
{
	mTarget = pTarget;
}
