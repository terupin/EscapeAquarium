#include "nanido.h"
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
#include "BackCamera.h"

#include <map> // �A�z�z��
#include <vector> // �ϔz��

extern ID3D11Buffer* gpConstBuffer;

using namespace DirectX;

extern std::map<std::string, ModelData> gModelManager;

extern ModelData CreateSquarePolygon(float width, float height, float uvWidth, float uvHeight, const wchar_t* texFile);

extern int Scenenow;

GameObject* nani;
GameObject* easy;
GameObject* normal;
GameObject* hard;
GameObject* arrow;
GameObject* n_sae;

extern int level_nonber;

int carsol = 0;

nanido::nanido()
{
	// �萔�o�b�t�@�쐬
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

	// ���f���ǂݍ���
// 2D�L�����p�|���S������
	gModelManager["nanido"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/level.png");

	gModelManager["easy"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/easy.png");

	gModelManager["normal"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/normal.png");

	gModelManager["hard"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/Hard.png");

	gModelManager["arrow"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/yazirusi.png");

	gModelManager["sea"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/umi.jpg");

	nani = new NormalObject;
	Model* n_n_Model = nani->GetModel();
	n_n_Model->SetModelData(gModelManager["nanido"]);
	n_n_Model->Set2dRender(true);
	n_n_Model->mPos = XMFLOAT3(0.0, 0.8, 0.0);
	n_n_Model->SetDiffuse(XMFLOAT4(1, 1, 1, 0.5f));
	n_n_Model->mScale = XMFLOAT3(0.6f, 0.6f, 1.0f);

	easy = new NormalObject;
	Model* n_e_Model = easy->GetModel();
	n_e_Model->SetModelData(gModelManager["easy"]);
	n_e_Model->Set2dRender(true);
	n_e_Model->mPos = XMFLOAT3(0.0, 0.3, 0.0);
	n_e_Model->SetDiffuse(XMFLOAT4(1, 1, 1, 0.5f));
	n_e_Model->mScale = XMFLOAT3(0.4f, 0.4f, 1.0f);

	normal = new NormalObject;
	Model* n_no_Model = normal->GetModel();
	n_no_Model->SetModelData(gModelManager["normal"]);
	n_no_Model->Set2dRender(true);
	n_no_Model->mPos = XMFLOAT3(0.0, -0.2, 0.0);
	n_no_Model->SetDiffuse(XMFLOAT4(1, 1, 1, 0.5f));
	n_no_Model->mScale = XMFLOAT3(0.4f, 0.4f, 1.0f);

	hard = new NormalObject;
	Model* n_h_Model = hard->GetModel();
	n_h_Model->SetModelData(gModelManager["hard"]);
	n_h_Model->Set2dRender(true);
	n_h_Model->mPos = XMFLOAT3(0.0, -0.7, 0.0);
	n_h_Model->SetDiffuse(XMFLOAT4(1, 1, 1, 0.5f));
	n_h_Model->mScale = XMFLOAT3(0.4f, 0.4f, 1.0f);

	arrow = new NormalObject;
	Model* n_a_Model = arrow->GetModel();
	n_a_Model->SetModelData(gModelManager["arrow"]);
	n_a_Model->Set2dRender(true);
	n_a_Model->mPos = XMFLOAT3(-0.3, 0.4, 0.0);
	n_a_Model->SetDiffuse(XMFLOAT4(1, 1, 1, 0.5f));
	n_a_Model->mScale = XMFLOAT3(0.4f, 0.4f, 1.0f);

	//�e�N�X�`���ݒ�
	n_sae = new NormalObject();
	Model* saeModel = n_sae->GetModel();
	saeModel->SetModelData(gModelManager["sea"]);
	saeModel->Set2dRender(true);
	saeModel->SetDiffuse(XMFLOAT4(1, 1, 1, 1));
	saeModel->mScale = XMFLOAT3(2.0f, 2.0f, 1.0f);
	saeModel->mPos = XMFLOAT3(-0.0, 0.0, 0.0);


}


nanido::~nanido()
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

void nanido::Update()
{
	if (carsol > 2)
	{
		carsol = 2;
		arrow->GetModel()->mPos.y = -0.7f;
	
	}

	if (carsol < 0)
	{
		carsol = 0;
		arrow->GetModel()->mPos.y = 0.4f;

	}


	if (Input_GetKeyTrigger(VK_UP))
	{
		carsol--;
		arrow->GetModel()->mPos.y += 0.55f;
	}

	if (Input_GetKeyTrigger(VK_DOWN))
	{
		carsol++;
		arrow->GetModel()->mPos.y -= 0.55f;
	}

	if (Input_GetKeyTrigger(VK_RETURN))
	{
		switch (carsol)
		{
		case l_easy:
			level_nonber = l_easy;
			break;

		case l_normal:
			level_nonber = l_normal;
			break;

		case l_hard:
			level_nonber = l_hard;
			break;

		}
		Scenenow = Game_;
	}

}

void nanido::Draw()
{
	// DIRECT3D�\���̂ɃA�N�Z�X����
	DIRECT3D* d3d = Direct3D_Get();
	// ��ʃN���A�i�w��F�œh��Ԃ��j
	// �h��Ԃ��F�̎w��i�����F0.0f�`1.0f�j
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha

	d3d->context->ClearRenderTargetView(d3d->renderTarget, clearColor);

	// Z�o�b�t�@�A�X�e���V���o�b�t�@�N���A
	d3d->context->ClearDepthStencilView(
		d3d->depthStencilView,			// �f�v�X�X�e���V���r���[
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,			// �y�o�b�t�@��1.0�ŃN���A����(0.0f�`1.0f)
		0);				// �X�e���V���o�b�t�@��0�ŃN���A����

		// �Q�[���I�u�W�F�N�g��`��
	d3d->context->OMSetBlendState(d3d->blendAlpha, NULL, 0xffffffff);// �A���t�@�u�����h


	nani->Draw();
	easy->Draw();
	normal->Draw();
	hard->Draw();
	arrow->Draw();
	n_sae->Draw();

	// �_�u���o�b�t�@�̐؂�ւ�
	d3d->swapChain->Present(0, 0);
}

