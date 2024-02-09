#include "Game.h"
#include "Direct3D.h"
#include "Input.h"

// static�ϐ��̎��̐錾
GameObject* Game::mHalOsaka1;
GameObject* Game::mHalOsaka2;
GameObject* Game::mHalOsaka[];


void Game::Initialize()
{
	// �Q�[���I�u�W�F�N�g�̃C���X�^���X���쐬
	mHalOsaka1 = new GameObject();
	mHalOsaka1->x = 1;
	mHalOsaka2 = new GameObject();
	mHalOsaka2->x = -1;

	for (int i = 0; i < mMaxGameObject; i++)
	{
		mHalOsaka[i] = new GameObject();
		mHalOsaka[i]->z = 10;
		mHalOsaka[i]->y = 2 - i * 1.2f;
	}
}

void Game::Update()
{
	Input::Update();

	// �Q�[���I�u�W�F�N�g���X�V
	//mHalOsaka1->Update();
}

void Game::Draw()
{
	// ��ʃN���A�i�w��F�œh��Ԃ��j
	// �h��Ԃ��F�̎w��i�����F0.0f�`1.0f�j
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha

	DIRECT3D* d3d = Direct3D_Get();

	d3d->context->ClearRenderTargetView(d3d->renderTarget, clearColor);

	// Z�o�b�t�@�A�X�e���V���o�b�t�@�N���A
	d3d->context->ClearDepthStencilView(
		d3d->depthStencilView,			// �f�v�X�X�e���V���r���[
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,			// �y�o�b�t�@��1.0�ŃN���A����(0.0f�`1.0f)
		0);				// �X�e���V���o�b�t�@��0�ŃN���A����

	// ���@���O�̕`�揈���������ɏ��� *******

	// �Q�[���I�u�W�F�N�g��`��
	mHalOsaka1->Draw();
	mHalOsaka2->Draw();

	for (int i = 0; i < mMaxGameObject; i++)
	{
		mHalOsaka[i]->Draw();
	}

	// ���@���O�̕`�揈���������ɏ��� *******

	// �_�u���E�o�b�t�@�̃f�B�X�v���C�̈�ւ̃R�s�[����
	d3d->swapChain->Present(0, 0);
}

void Game::Release()
{
	// �Q�[���I�u�W�F�N�g��S�ĉ��
	delete mHalOsaka1;
	delete mHalOsaka2;

	for (int i = 0; i < mMaxGameObject; i++)
	{
		delete mHalOsaka[i];
	}
}
