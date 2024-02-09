#pragma once

#include <Windows.h>

// Input�N���X

// ���J�����o�[�֐�

// �L�[�������ꂽ�C�x���g������������A���̏�Ԃ�ۑ�����֐�
void Input_SetKeyDown(int key);

// �L�[�������ꂽ�C�x���g������������A���̏�Ԃ�ۑ�����֐�
void Input_SetKeyUp(int key);

// �w�肵���L�[��������Ă��邩�A���̏�ԁi�v���X�j��Ԃ��֐�
BOOL Input_GetKeyDown(int key);

// �g���K�[�@�c�@�L�[�������ꂽ���̂P�t���[�����������ꂽ����
BOOL Input_GetKeyTrigger(int key);

// �Q�[�����[�v�̍Ō�ŃL�[��Ԃ��X�V����֐�
void Input_Refresh();


// C++��

class Input
{
private:
	bool mKeyState[256];

public:
	Input();
	void SetKeyDown(int key);
	bool GetKeyDown(int key);
};