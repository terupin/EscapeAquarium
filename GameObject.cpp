#include "GameObject.h"

extern DWORD gDeltaTime;

using namespace DirectX;

GameObject::GameObject()
{
	mModel = nullptr;
	mSpeed = 0.0f;
	mHit = nullptr;
}

GameObject::~GameObject()
{
	delete mModel;
	delete mHit;
}

void GameObject::Draw()
{
	mModel->Draw();
}

void GameObject::Update()
{
	// �O�����x�N�g�����v�Z����
	XMFLOAT3 forwardVector;
	float radianY = XMConvertToRadians(mModel->mRotate.y-90.0f);
	forwardVector.x = sinf(radianY);
	forwardVector.z = cosf(radianY);
	forwardVector.y = 0.0f;
	// �O�����x�N�g���������o�[�ϐ��ɕۑ�
	mForwardVector = forwardVector;

	// �ړ�����
	mModel->mPos.x += forwardVector.x * mSpeed * gDeltaTime;
	mModel->mPos.z += forwardVector.z * mSpeed * gDeltaTime;
}

Model * GameObject::GetModel()
{
	return mModel;
}

DirectX::XMFLOAT3 GameObject::GetForwardVector()
{
	return mForwardVector;
}
