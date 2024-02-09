#pragma once

#include <DirectXMath.h>

class HitSphere
{

public:

	// ���肷��
	// �����Ŏ󂯎�����~�Ǝ������g�i�~�j���Փ˂��Ă邩true/false�ŕԂ�
	bool IsHit(HitSphere* pOther);

	// �Q�ƌ^�����o�[�ϐ��ɏ����l�ݒ肷��ɂ́A�����t���R���X�g���N�^���g��
	HitSphere(DirectX::XMFLOAT3& center, float hankei);

	void SetHankei(float hankei)
	{
		mHankei = hankei;
	}

private:
	bool active = true; // �����蔻���ON/OFF�؂�ւ��ϐ�

	DirectX::XMFLOAT3& mCenter;
	float mHankei; // �~�̔��a

};

