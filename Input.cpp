#include "input.h"

// �S�L�[��Ԃ�ۑ�����z��
BOOL gKeyState[256];
// �S�L�[��ԁi�P�t���[���O�j��ۑ�����z��
BOOL gOldKeyState[256];

void Input_SetKeyDown(int key)
{
	gKeyState[key] = TRUE;
}

void Input_SetKeyUp(int key)
{
	gKeyState[key] = FALSE;
}

BOOL Input_GetKeyDown(int key)
{
	return gKeyState[key];
}

BOOL Input_GetKeyTrigger(int key)
{
	// �P�t���[���O�͉�����Ă��炸�A���͉�����Ă���
	return gOldKeyState[key] == FALSE
		&& gKeyState[key] == TRUE;
}

void Input_Refresh()
{
	// �Q�[�����[�v�̍Ō�Ō��݂̃L�[��Ԃ�gOldKeyState�ɕۑ�
	memcpy_s(gOldKeyState, sizeof(gOldKeyState),
		gKeyState, sizeof(gKeyState));
}

Input::Input()
{
	ZeroMemory(mKeyState, sizeof(mKeyState));
}

void Input::SetKeyDown(int key)
{
	mKeyState[key] = true;
}

bool Input::GetKeyDown(int key)
{
	return mKeyState[key];
}
