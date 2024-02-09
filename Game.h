#pragma once

#include "GameObject.h"

// �Q�[���S�̘̂g�ƂȂ�N���X�B
// ���̃N���X�ɃQ�[�����ŕK�v�ƂȂ�I�u�W�F�N�g����������B
// ���̃N���X��static�ȃN���X�Ƃ��Đ݌v���邽�߁A
// ���̃N���X��public�����o�[�ɂ̓v���O�����̂ǂ�����ł��A�N�Z�X�ł���B

class Game
{

public: // �N���X�̌��J�v�f

	// �����o�[�֐�
	static void Initialize();	// �Q�[���̏���������
	static void Update();		// �X�V����
	static void Draw();			// �`�揈��
	static void Release();		// �������

	// �萔
	static constexpr int mMaxGameObject = 5;

private: // �N���X�̔���J�v�f
	
	// ���̃Q�[���ɓo�ꂷ��Q�[���I�u�W�F�N�g
	static GameObject* mHalOsaka1;
	static GameObject* mHalOsaka2;
	static GameObject* mHalOsaka[mMaxGameObject];

};

