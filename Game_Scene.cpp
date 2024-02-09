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

using namespace DirectX;

typedef std::vector<GameObject*> GameObjectVector;

extern ModelData CreateSquarePolygon(float width, float height, float uvWidth, float uvHeight, const wchar_t* texFile);

GameObjectVector gObjectList;
std::vector <GameObject*> gEnemyList;

// GameObject�N���X�̕ϐ�
GameObject* gpCottage;
GameObject* gpGun;
GameObject* gpShot;
BillboardObject* gp2Dkamome;
BillboardObject* gp2Dsame;
BillboardObject* gp2Diruka;
BillboardObject* gp2Dei;
BillboardObject* gp2Dsyach;
BillboardObject* gp2Dkaziki;
GameObject* gpShadow;

#define MAX_GROUND  10
#define e_Speed 0.0001
#define n_Speed 0.0005
#define h_Speed 0.5

#define ENEMY_ALL 5

int enemycount = 0;
float Speed = n_Speed;

GameObject* gpGround[MAX_GROUND][MAX_GROUND];

// �e�Ǘ��p�z��
GameObjectVector gShotManager;

// �G�t�F�N�g�Ǘ��p�z��
GameObjectVector gEffectManager;

//�V�[���؂�ւ�
extern int Scenenow;

//���x���؂�ւ�
int level_nonber;

// ���f���}�l�[�W���[
// �A�z�z�񁁓Y�����ɐ����ȊO���g����z��
// �e���v���[�g���g���Ƃ��ɓ����̌^�����߂�
// map�̏ꍇ�A�O���Y�����^�A��낪�i�[����v�f�̌^
std::map<std::string, ModelData> gModelManager;

enum hougaku
{
	north, south, east, west
};

// Camera�N���X�̕ϐ�
Camera* gpCamera;

// �f���^�^�C���p�̕ϐ�
extern DWORD gDeltaTime;

void vector_DrawAll(GameObjectVector vec)
{

	for (auto e : vec)
	{
		e->Draw();
	}
}

void vector_UpdateAll(GameObjectVector vec)
{
	for (auto e : vec)
	{
		e->Update();
	}
}

void vector_DeleteAll(GameObjectVector vec)
{
	//�z��̌X�ɓ����Ă���GameObject��delete����

	for (auto e : vec)
	{
		delete e;
	}
	vec.clear();
}

Game_Scene::Game_Scene()
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

	// �J�����̍쐬
	gpCamera = new BackCamera();
	Camera::mMainCamera = gpCamera;
	// �����l�ݒ�
	// ���ӁFeye��focus���������ƃ_��
	// ���ӁFup��xyz�S�ă[�����ƃ_��
	gpCamera->SetEye(XMFLOAT3(0.0f, 0.0f, -2.0f));
	gpCamera->SetFocus(XMFLOAT3(0.0f, 0.0f, 0.0f));
	gpCamera->SetUp(XMFLOAT3(0.0f, 1.0f, 0.0f));

	// ���f���ǂݍ���
	ObjModelLoader loader;
	gModelManager["cottage"] = loader.Load(
		"assets/cottage.obj", L"assets/cottage.png");

	// �e���f���ǂݍ���
	loader = ObjModelLoader();
	gModelManager["gun"] = loader.Load(
		"assets/gun.obj", L"assets/gun.png");

	// �e�pModel�I�u�W�F�N�g����
	gpGun = new NormalObject();
	Model* pGunModel = gpGun->GetModel();
	pGunModel->SetModelData(gModelManager["gun"]);
	pGunModel->SetScale(1.5f);
	pGunModel->mPos.y = 0.4f;
	pGunModel->mPos.z = 1.0f;
	pGunModel->mRotate.y = 90.0f;
	pGunModel->mCamera = gpCamera;

	// �n�ʃ��f���ǂݍ���
	loader = ObjModelLoader();
	gModelManager["ground1"] = loader.Load(
		"assets/ground1.obj", L"assets/mizube.jpg"
	);

	// �n�ʐ���
	for (int i = 0; i < MAX_GROUND; i++)
	{
		for (int j = 0; j < MAX_GROUND; j++)
		{
			gpGround[i][j] = new NormalObject();
			Model* pGroundModel = gpGround[i][j]->GetModel();
			pGroundModel->SetModelData(gModelManager["ground1"]);
			pGroundModel->SetScale(2.0f);
			pGroundModel->mPos.x = -20.0f + 4.0f*j;
			pGroundModel->mPos.z = -10.0f + 4.0f*i;
			pGroundModel->mPos.y = -2.0f;
			pGroundModel->mCamera = gpCamera;

			gObjectList.emplace_back(gpGround[i][j]);
		}
	}

	// �e���f���ǂݍ���
	loader = ObjModelLoader();
	gModelManager["shot"] = loader.Load(
		"assets/billboard.obj", L"assets/effect000.jpg"
	);

	// �e�p�I�u�W�F�N�g����
	gpShot = new BillboardObject();
	Model* pShotModel = gpShot->GetModel();
	pShotModel->SetModelData(gModelManager["shot"]);
	pShotModel->SetScale(0.2f);
	pShotModel->mPos.y = 0.2f;
	pShotModel->mPos.z = 1.0f;
	pShotModel->mCamera = gpCamera;

	// 2D�L�����p�|���S������
	gModelManager["�J����"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/�J����.png");

	// 2D�L�����p�I�u�W�F�N�g����
	gp2Dkamome = new BillboardObject();
	Model* p2Dkamome = gp2Dkamome->GetModel();
	p2Dkamome->SetModelData(gModelManager["�J����"]);
	p2Dkamome->SetScale(1.0f);
	p2Dkamome->mPos.y = 0.6f;
	p2Dkamome->mPos.z = 1.0f;
	p2Dkamome->mPos.x = 2.5f;
	p2Dkamome->direction = north;
	p2Dkamome->mCamera = gpCamera;
	HitSphere* pHit = gp2Dkamome->GetHit();
	pHit->SetHankei(0.8f);// �����苅�̔��a��ݒ�

	gEnemyList.emplace_back(gp2Dkamome);

	// 2D�L�����p�|���S������
	gModelManager["�T��"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/�W���x�G�U��.png");

	// 2D�L�����p�I�u�W�F�N�g����
	gp2Dsame= new BillboardObject();
	Model* p2Dsame = gp2Dsame->GetModel();
	p2Dsame->SetModelData(gModelManager["�T��"]);
	p2Dsame->SetScale(1.0f);
	p2Dsame->mPos.y = 0.6f;
	p2Dsame->mPos.z = -2.0f;
	p2Dsame->mPos.x = -4.5f;
	p2Dsame->direction = south;
	p2Dsame->mCamera = gpCamera;


	gEnemyList.emplace_back(gp2Dsame);

	// 2D�L�����p�|���S������
	gModelManager["�C���J"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/�o���h�E�C���J.png");

	// 2D�L�����p�I�u�W�F�N�g����
	gp2Diruka = new BillboardObject();
	Model* p2Diruka = gp2Diruka->GetModel();
	p2Diruka->SetModelData(gModelManager["�C���J"]);
	p2Diruka->SetScale(1.0f);
	p2Diruka->mPos.y = 0.6f;
	p2Diruka->mPos.z = 0.0f;
	p2Diruka->mPos.x = 5.5f;
	p2Diruka->direction = east;
	p2Diruka->mCamera = gpCamera;
	HitSphere* pHit_2 = gp2Diruka->GetHit();
	pHit_2->SetHankei(0.8f);// �����苅�̔��a��ݒ�

	gEnemyList.emplace_back(gp2Diruka);

	// 2D�L�����p�|���S������
	gModelManager["�G�C"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/�i�����E�}���^.png");

	// 2D�L�����p�I�u�W�F�N�g����
	gp2Dei = new BillboardObject();
	Model* p2Dei = gp2Dei->GetModel();
	p2Dei->SetModelData(gModelManager["�G�C"]);
	p2Dei->SetScale(1.0f);
	p2Dei->mPos.y = 0.6f;
	p2Dei->mPos.z = -3.0f;
	p2Dei->mPos.x = -0.5f;
	p2Dei->direction = west;
	p2Dei->mCamera = gpCamera;
	HitSphere* pHit_3 = gp2Dei->GetHit();
	pHit_3->SetHankei(0.8f);// �����苅�̔��a��ݒ�

	gEnemyList.emplace_back(gp2Dei);

	// 2D�L�����p�|���S������
	gModelManager["�V���`"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/�V���`.png");

	// 2D�L�����p�I�u�W�F�N�g����
	gp2Dsyach = new BillboardObject();
	Model* p2Dsyati = gp2Dsyach->GetModel();
	p2Dsyati->SetModelData(gModelManager["�V���`"]);
	p2Dsyati->SetScale(1.0f);
	p2Dsyati->mPos.y = 0.6f;
	p2Dsyati->mPos.z = -2.0f;
	p2Dsyati->mPos.x = 1.5f;
	p2Dsyati->direction = north;
	p2Dsyati->mCamera = gpCamera;
	HitSphere* pHit_4 = gp2Dsyach->GetHit();
	pHit_4->SetHankei(0.8f);// �����苅�̔��a��ݒ�

	gEnemyList.emplace_back(gp2Dsyach);

	// 2D�L�����p�|���S������
	gModelManager["�J�W�L"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/�o�V���E�J�W�L.png");

	// 2D�L�����p�I�u�W�F�N�g����
	gp2Dkaziki = new BillboardObject();
	Model* p2Dkaziki = gp2Dkaziki->GetModel();
	p2Dkaziki->SetModelData(gModelManager["�J�W�L"]);
	p2Dkaziki->SetScale(1.0f);
	p2Dkaziki->mPos.y = 0.6f;
	p2Dkaziki->mPos.z = 4.0f;
	p2Dkaziki->mPos.x = 2.5f;
	p2Dkaziki->direction = west;
	p2Dkaziki->mCamera = gpCamera;
	HitSphere* pHit_5 = gp2Dkaziki->GetHit();
	pHit_5->SetHankei(0.8f);// �����苅�̔��a��ݒ�

	gEnemyList.emplace_back(gp2Dkaziki);



	gObjectList.emplace_back(gpGun);

	// �R�e�[�W�p�I�u�W�F�N�g����

	gpCottage = new NormalObject();
	Model* pCottageModel = gpCottage->GetModel();
	pCottageModel->SetModelData(gModelManager["cottage"]); // 3D�f�[�^���Z�b�g����
	pCottageModel->SetScale(0.002f);
	pCottageModel->mPos.x = 0.0f;
	pCottageModel->mPos.z = 0.0f;
	pCottageModel->mPos.y = 0.0f;
	gObjectList.emplace_back(gpCottage);


	gModelManager["explosion"] = CreateSquarePolygon(1.0f, 1.0f, 0.25f, 0.25f, L"assets/suibun.png");
	GameObject* tmp = new Explosion();
	Model* pModel = tmp->GetModel();
	pModel->SetModelData(gModelManager["explosion"]);
	gObjectList.emplace_back(tmp);

	switch(level_nonber)
	{
	case l_easy:
		Speed = e_Speed;
		break;

	case l_normal:
		Speed = n_Speed;
		break;

	case l_hard:
		Speed = h_Speed;
		break;
	}

	// �J�����̒Ǐ]�^�[�Q�b�g���w��
	//((BackCamera*)gpCamera)->SetTarget(gpGun);  // C�X�^�C���L���X�g
	dynamic_cast<BackCamera*>(gpCamera)->SetTarget(gpGun); // C++�L���X�g
}


Game_Scene::~Game_Scene()
{
	vector_DeleteAll(gObjectList);

	// �e�Ǘ��z��̒��g��S��delete����
	vector_DeleteAll(gShotManager);

	vector_DeleteAll(gEffectManager);

	COM_SAFE_RELEASE(gpConstBuffer);

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

void Game_Scene::Update()
{
	// �f���^�^�C�����z��O�̒l�ɂȂ����ꍇ�̏���
	if (gDeltaTime >= 100)
	{
		gDeltaTime = 0;
	}
	if (gDeltaTime <= 0)
	{
		gDeltaTime = 1;
	}

	// �J�����ړ��ϐ�
	static float angle = 0.0f; // ��]�p�x
	static float zoom = 3.0f;  // �Y�[��

	// �@�J�����̈ʒu���L�[����ňړ�����
	if (Input_GetKeyDown(VK_RIGHT))
	{
		angle += 0.04f * gDeltaTime;
	}
	if (Input_GetKeyDown(VK_LEFT))
	{
		angle -= 0.04f * gDeltaTime;
	}
	// �Y�[������
	if (Input_GetKeyDown(VK_UP))
		zoom -= 0.01f * gDeltaTime;
	if (Input_GetKeyDown(VK_DOWN))
		zoom += 0.01f * gDeltaTime;

	// �A�J�����̒����_�𒆐S�ɃJ��������]����

	// �J�����ʒuX�@���@sinf(�p�x���W�A��)
	// �J�����ʒuZ�@���@cosf(�p�x���W�A��)
	// ���_�𒆐S�ɔ��a1.0f�̉~����̓_�����߂�
	Model* pCottageModel = gpCottage->GetModel();
	float radian = XMConvertToRadians(angle);
	gpCamera->mEye.x =
		sinf(radian) * zoom + pCottageModel->mPos.x;
	gpCamera->mEye.z =
		cosf(radian) * zoom + pCottageModel->mPos.z;
	gpCamera->mEye.y = 2.0f;

	// �J���������_���R�e�[�W�̈ʒu�ɂ���
	gpCamera->SetFocus(pCottageModel->mPos);

	// �L�����N�^�[�ړ�
	// �L�����N�^�[�������Ă�������ɑO�i����
	// �����ύX��AD�L�[�@�O�i��W�L�[
	// �u�O�����x�N�g���v���v�Z����
	// �ړ����x��W�L�[�Ō��܂�
	gpGun->mSpeed = 0.0f;
	if (Input_GetKeyDown('W'))
		gpGun->mSpeed = 0.001f;

	// �L�����N�^�[�̕����]��
	Model* pGunModel = gpGun->GetModel();
	if (Input_GetKeyDown('A'))
		pGunModel->mRotate.y -= 0.04f * gDeltaTime;
	if (Input_GetKeyDown('D'))
		pGunModel->mRotate.y += 0.04f * gDeltaTime;

	// �e�̔���
	if (Input_GetKeyTrigger(VK_LBUTTON))
	{
		// �e�I�u�W�F�N�g����
		GameObject* pShot = new BillboardObject();
		Model* pShotModel = pShot->GetModel();
		pShotModel->SetModelData(gModelManager["shot"]);
		pShotModel->SetScale(0.2f);
		pShot->GetHit()->SetHankei(0.19f);

		// ���˒n�_�Ɉړ�������
		XMFLOAT3 forwardVec = gpGun->GetForwardVector();

		pShotModel->mPos.x =
			pGunModel->mPos.x + forwardVec.x * 0.8f;
		pShotModel->mPos.y = pGunModel->mPos.y + 0.2f;
		pShotModel->mPos.z =
			pGunModel->mPos.z + forwardVec.z * 0.8f;
		// �e�ɑ��x��ݒ肷��
		pShot->mSpeed = 0.01f;
		// �e����ԕ�����ݒ�
		pShotModel->mRotate.y = pGunModel->mRotate.y;

		// �e�Ǘ��z��ɒǉ�����
		gShotManager.emplace_back(pShot);
	}

	vector_UpdateAll(gObjectList);

	// �e�Ǘ��z��̒��g��S��Update����
	vector_UpdateAll(gShotManager);

	// �e�̉������
	for (int i = 0; i < gShotManager.size(); i++)
	{
		// (a)�J�����̈ʒu
		XMFLOAT3 cameraPos = gpCamera->mEye;
		// (b)�e�̈ʒu
		XMFLOAT3 shotPos = gShotManager[i]->GetModel()->mPos;
		// (b)��(a)�x�N�g��
		XMVECTOR abVector =
			XMLoadFloat3(&cameraPos) - XMLoadFloat3(&shotPos);
		// (b)��(a)�x�N�g���̒������v�Z
		XMVECTOR abVecLength = XMVector3Length(abVector);
		float abLength = XMVectorGetX(abVecLength);
		// abLength�������ȏ�Ȃ�e���������
		if (abLength >= 50.0f)
		{
			// erase�֐������ł͒e��delete�͍s���Ȃ�
			delete gShotManager[i];
			// �e�Ǘ��z�񂩂�e���폜
			gShotManager.erase(gShotManager.begin() + i);
		}
	}

	// �e��NPC�����蔻��
	for (int aw = 0; aw < gEnemyList.size(); aw++)
	{		
		for (int i = 0; i < gShotManager.size(); i++)
		{
			HitSphere* pShotHit = gShotManager[i]->GetHit();
			if (pShotHit->IsHit(gEnemyList[aw]->GetHit()))
			{
				delete gShotManager[i];
				// �e�Ǘ��z�񂩂�e���폜

				gShotManager.erase(gShotManager.begin() + i);
				i--;
				switch (gEnemyList[aw]->GetModel()->direction)
				{
				case north:
					gEnemyList[aw]->GetModel()->direction = east;
					break;

				case south:
					gEnemyList[aw]->GetModel()->direction = west;
					break;

				case east:
					gEnemyList[aw]->GetModel()->direction = south;
					break;

				case west:
					gEnemyList[aw]->GetModel()->direction = north;
					break;
				}

				// emitter
				int num = rand() % 4 + 3;
				for (int j = 0; j < num; j++)
				{
					GameObject* tmp = new Explosion();
					Model* pModel = tmp->GetModel();
					pModel->SetModelData(gModelManager["explosion"]);
					pModel->mPos = gEnemyList[aw]->GetModel()->mPos;
					pModel->mPos.x += static_cast<float>(rand() % 100 - 50) / 100.0f;
					pModel->mPos.y += static_cast<float>(rand() % 100 - 50) / 100.0f;
					pModel->mPos.z += static_cast<float>(rand() % 100 - 50) / 100.0f;
					//tmp->mSpeed = static_cast<float>(rand() % 100) / 100000.0f;
					gEffectManager.emplace_back(tmp);
				}
			}
		}
	}

	for (int i = 0; i < gEnemyList.size(); i++)
	{
		switch (gEnemyList[i]->GetModel()->direction)
		{
		case north:
			gEnemyList[i]->GetModel()->mPos.z += Speed;
			break;

		case south:
			gEnemyList[i]->GetModel()->mPos.z -= Speed;
			break;

		case east:
			gEnemyList[i]->GetModel()->mPos.x += Speed;
			break;

		case west:
			gEnemyList[i]->GetModel()->mPos.x -= Speed;
			break;
		}
	}



	vector_UpdateAll(gEffectManager);

	// �Đ����I������G�t�F�N�g������
	for (int i = 0; i < gEffectManager.size(); i++)
	{
		Explosion* e = dynamic_cast<Explosion*>(gEffectManager[i]);
		if (e->IsFinished())
		{
			delete gEffectManager[i];
			// �e�Ǘ��z�񂩂�e���폜
			gEffectManager.erase(gEffectManager.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < gEnemyList.size(); i++)
	{

		if (gpCottage->GetModel()->mPos.x - 2.0f<gEnemyList[i]->GetModel()->mPos.x&&
			gpCottage->GetModel()->mPos.x + 2.0f > gEnemyList[i]->GetModel()->mPos.x&&
			gpCottage->GetModel()->mPos.z - 1.4f<gEnemyList[i]->GetModel()->mPos.z&&
			gpCottage->GetModel()->mPos.z + 1.4f > gEnemyList[i]->GetModel()->mPos.z
			)
		{
			enemycount++;
		}
	}

	if (enemycount >= gEnemyList.size())
		Scenenow = Goal;
	else
		enemycount = 0;
	

	// �J�����X�V�i�r���[�ϊ��s��v�Z�j
	gpCamera->Update();
}

void Game_Scene::Draw()
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

	// ���@���O�̕`�揈���������ɏ��� *******

	// �Q�[���I�u�W�F�N�g��`��
	d3d->context->OMSetBlendState(d3d->blendAlpha, NULL, 0xffffffff);// �A���t�@�u�����h

	vector_DrawAll(gObjectList);

	vector_DrawAll(gEnemyList);

	d3d->context->OMSetBlendState(d3d->blendAdd, NULL, 0xffffffff);// ���Z����
	// �e�Ǘ��z��̒��g��S��Draw����
	vector_DrawAll(gShotManager);

	vector_DrawAll(gEffectManager);

	// �_�u���o�b�t�@�̐؂�ւ�
	d3d->swapChain->Present(0, 0);
}
