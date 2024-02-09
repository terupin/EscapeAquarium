#include "Gole_Scene.h"
#include "Game_Scene.h"
#include "Direct3d.h"
#include "input.h"
#include <DirectXMath.h>
#include "WICTextureLoader.h"
#include "ObjModelLoader.h"
#include "Model.h"
#include "BackCamera.h"
#include "NormalObject.h"
#include"Camera.h"
#include "Explosion.h"

#include <map> // �A�z�z��
#include <vector> // �ϔz��

extern ID3D11Buffer* gpConstBuffer;

extern std::map<std::string, ModelData> gModelManager;

//	Sprite�̕ϐ�(�C���X�g�̐錾) 
GameObject* gpGoal;
GameObject* gpG_Enter;
GameObject* gpGsae;

extern ModelData CreateSquarePolygon(float width, float height, float uvWidth, float uvHeight, const wchar_t* texFile);

using namespace DirectX;

extern int Scenenow;

Gole_Scene::Gole_Scene()
{// �萔�o�b�t�@�쐬
// �R���X�^���g�o�b�t�@�Ƃ��č쐬���邽�߂̏��ݒ�
	D3D11_BUFFER_DESC contstat_buffer_desc;
	contstat_buffer_desc.ByteWidth = 512;	// �o�b�t�@�̃T�C�Y�i4x4�s��x4�j
	contstat_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;		// �g�p���@
	contstat_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// �o�b�t�@�̎��(�R���X�^���g�o�b�t�@)
	contstat_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;			// CPU�A�N�Z�X�ݒ�
	contstat_buffer_desc.MiscFlags = 0;				// ���̑��̃I�v�V����
	contstat_buffer_desc.StructureByteStride = 0;			// �\���̃T�C�Y(�s����g���������0�ł����삷�邱�Ƃ����؂���)

	Direct3D_Get()->device->CreateBuffer(&contstat_buffer_desc,
		nullptr, &gpConstBuffer);

	// 2D�L�����p�|���S������
	gModelManager["goal"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.00f,
			L"assets/kuria.png");

	gModelManager["goal_enter"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.00f,
			L"assets/goal.png");

	gModelManager["sea"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/umi.jpg");

	//�e�N�X�`���ݒ�
	gpGoal = new NormalObject();
	Model* G_G_Model = gpGoal->GetModel();
	G_G_Model->SetModelData(gModelManager["goal"]);
	G_G_Model->Set2dRender(true);
	G_G_Model->SetDiffuse(XMFLOAT4(1, 1, 1, 1));
	G_G_Model->mPos = XMFLOAT3(0.0, 0.0, 0.0);
	G_G_Model->mScale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	//�e�N�X�`���ݒ�
	gpG_Enter = new NormalObject();
	Model* G_entModel = gpG_Enter->GetModel();
	G_entModel->SetModelData(gModelManager["goal_enter"]);
	G_entModel->Set2dRender(true);
	G_entModel->SetDiffuse(XMFLOAT4(1, 1, 1, 0.5f));
	G_entModel->mPos = XMFLOAT3(0.0, -0.6, 0.0);
	G_entModel->mScale = XMFLOAT3(1.0f, 0.7f, 1.0f);

	//�e�N�X�`���ݒ�
	gpGsae = new NormalObject();
	Model* G_saeModel = gpGsae->GetModel();
	G_saeModel->SetModelData(gModelManager["sea"]);
	G_saeModel->Set2dRender(true);
	G_saeModel->SetDiffuse(XMFLOAT4(1, 1, 1, 1));
	G_saeModel->mScale = XMFLOAT3(2.0f, 2.0f, 2.0f);

}


Gole_Scene::~Gole_Scene()
{	
	// ���f���}�l�[�W���[�����S���f�������
	for (auto i = gModelManager.begin();//�A�z�z��̐擪���
		i != gModelManager.end();//�A�z�z��̍Ō�����
		i++)
	{
		// first �c �Y����
		// second �c �v�f���̂���
		COM_SAFE_RELEASE(i->second.mSRV);//�e�N�X�`��
		COM_SAFE_RELEASE(i->second.mVertexBuffer);//���_�o�b�t�@
	}
	// �A�z�z��̑S�v�f���폜����i�v�f���O�ɂ���j
	gModelManager.clear();
}

void Gole_Scene::Update()
{
	if (Input_GetKeyTrigger(VK_RETURN))
		Scenenow = Title;
}

void Gole_Scene::Draw()
{
	// DIRECT3D�\���̂ɃA�N�Z�X����
	DIRECT3D* d3d = Direct3D_Get();
	// ��ʃN���A�i�w��F�œh��Ԃ��j
	// �h��Ԃ��F�̎w��i�����F0.0f�`1.0f�j
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha

	d3d->context->ClearRenderTargetView(d3d->renderTarget, clearColor);

	// Z�o�b�t�@�A�X�e���V���o�b�t�@�N���A
	d3d->context->ClearDepthStencilView(
		d3d->depthStencilView,			// �f�v�X�X�e���V���r���[
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,			// �y�o�b�t�@��1.0�ŃN���A����(0.0f�`1.0f)
		0);				// �X�e���V���o�b�t�@��0�ŃN���A����

		// �Q�[���I�u�W�F�N�g��`��
	d3d->context->OMSetBlendState(d3d->blendAlpha, NULL, 0xffffffff);// �A���t�@�u�����h

	gpGoal->Draw();
	gpG_Enter->Draw();
	gpGsae->Draw();

	// �_�u���o�b�t�@�̐؂�ւ�
	d3d->swapChain->Present(0, 0);
}
