#pragma once

#include "Camera.h"
#include "GameObject.h"

// �Ǐ]�J������\���N���X
// �E�Ǐ]�@�\
class BackCamera :
	public Camera
{

public:
	void Update() override;

	// �Ǐ]�^�[�Q�b�g���w�肷��֐�
	void SetTarget(GameObject* pTarget);

private:
	// �Ǐ]�^�[�Q�b�g���w�肷��ϐ�
	GameObject* mTarget;

};

