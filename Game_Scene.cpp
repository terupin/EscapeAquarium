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

#include <map> // 連想配列
#include <vector> // 可変配列

extern ID3D11Buffer* gpConstBuffer;

using namespace DirectX;

typedef std::vector<GameObject*> GameObjectVector;

extern ModelData CreateSquarePolygon(float width, float height, float uvWidth, float uvHeight, const wchar_t* texFile);

GameObjectVector gObjectList;
std::vector <GameObject*> gEnemyList;

// GameObjectクラスの変数
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

// 弾管理用配列
GameObjectVector gShotManager;

// エフェクト管理用配列
GameObjectVector gEffectManager;

//シーン切り替え
extern int Scenenow;

//レベル切り替え
int level_nonber;

// モデルマネージャー
// 連想配列＝添え字に整数以外も使える配列
// テンプレート＝使うときに内部の型を決める
// mapの場合、前が添え字型、後ろが格納する要素の型
std::map<std::string, ModelData> gModelManager;

enum hougaku
{
	north, south, east, west
};

// Cameraクラスの変数
Camera* gpCamera;

// デルタタイム用の変数
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
	//配列の個々に入っているGameObjectをdeleteする

	for (auto e : vec)
	{
		delete e;
	}
	vec.clear();
}

Game_Scene::Game_Scene()
{
	// 定数バッファ作成
// コンスタントバッファとして作成するための情報設定
	D3D11_BUFFER_DESC contstat_buffer_desc;
	contstat_buffer_desc.ByteWidth = 512;	// バッファのサイズ（4x4行列x4個）
	contstat_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;		// 使用方法
	contstat_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	// バッファの種類(コンスタントバッファ)
	contstat_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;			// CPUアクセス設定
	contstat_buffer_desc.MiscFlags = 0;				// その他のオプション
	contstat_buffer_desc.StructureByteStride = 0;			// 構造体サイズ(行列を使うが今回は0でも動作することを実証する)

	Direct3D_Get()->device->CreateBuffer(&contstat_buffer_desc,
		nullptr, &gpConstBuffer);

	// カメラの作成
	gpCamera = new BackCamera();
	Camera::mMainCamera = gpCamera;
	// 初期値設定
	// 注意：eyeとfocusが同じだとダメ
	// 注意：upのxyz全てゼロだとダメ
	gpCamera->SetEye(XMFLOAT3(0.0f, 0.0f, -2.0f));
	gpCamera->SetFocus(XMFLOAT3(0.0f, 0.0f, 0.0f));
	gpCamera->SetUp(XMFLOAT3(0.0f, 1.0f, 0.0f));

	// モデル読み込み
	ObjModelLoader loader;
	gModelManager["cottage"] = loader.Load(
		"assets/cottage.obj", L"assets/cottage.png");

	// 銃モデル読み込み
	loader = ObjModelLoader();
	gModelManager["gun"] = loader.Load(
		"assets/gun.obj", L"assets/gun.png");

	// 銃用Modelオブジェクト生成
	gpGun = new NormalObject();
	Model* pGunModel = gpGun->GetModel();
	pGunModel->SetModelData(gModelManager["gun"]);
	pGunModel->SetScale(1.5f);
	pGunModel->mPos.y = 0.4f;
	pGunModel->mPos.z = 1.0f;
	pGunModel->mRotate.y = 90.0f;
	pGunModel->mCamera = gpCamera;

	// 地面モデル読み込み
	loader = ObjModelLoader();
	gModelManager["ground1"] = loader.Load(
		"assets/ground1.obj", L"assets/mizube.jpg"
	);

	// 地面生成
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

	// 弾モデル読み込み
	loader = ObjModelLoader();
	gModelManager["shot"] = loader.Load(
		"assets/billboard.obj", L"assets/effect000.jpg"
	);

	// 弾用オブジェクト生成
	gpShot = new BillboardObject();
	Model* pShotModel = gpShot->GetModel();
	pShotModel->SetModelData(gModelManager["shot"]);
	pShotModel->SetScale(0.2f);
	pShotModel->mPos.y = 0.2f;
	pShotModel->mPos.z = 1.0f;
	pShotModel->mCamera = gpCamera;

	// 2Dキャラ用ポリゴン生成
	gModelManager["カモメ"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/カモメ.png");

	// 2Dキャラ用オブジェクト生成
	gp2Dkamome = new BillboardObject();
	Model* p2Dkamome = gp2Dkamome->GetModel();
	p2Dkamome->SetModelData(gModelManager["カモメ"]);
	p2Dkamome->SetScale(1.0f);
	p2Dkamome->mPos.y = 0.6f;
	p2Dkamome->mPos.z = 1.0f;
	p2Dkamome->mPos.x = 2.5f;
	p2Dkamome->direction = north;
	p2Dkamome->mCamera = gpCamera;
	HitSphere* pHit = gp2Dkamome->GetHit();
	pHit->SetHankei(0.8f);// 当たり球の半径を設定

	gEnemyList.emplace_back(gp2Dkamome);

	// 2Dキャラ用ポリゴン生成
	gModelManager["サメ"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/ジンベエザメ.png");

	// 2Dキャラ用オブジェクト生成
	gp2Dsame= new BillboardObject();
	Model* p2Dsame = gp2Dsame->GetModel();
	p2Dsame->SetModelData(gModelManager["サメ"]);
	p2Dsame->SetScale(1.0f);
	p2Dsame->mPos.y = 0.6f;
	p2Dsame->mPos.z = -2.0f;
	p2Dsame->mPos.x = -4.5f;
	p2Dsame->direction = south;
	p2Dsame->mCamera = gpCamera;


	gEnemyList.emplace_back(gp2Dsame);

	// 2Dキャラ用ポリゴン生成
	gModelManager["イルカ"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/バンドウイルカ.png");

	// 2Dキャラ用オブジェクト生成
	gp2Diruka = new BillboardObject();
	Model* p2Diruka = gp2Diruka->GetModel();
	p2Diruka->SetModelData(gModelManager["イルカ"]);
	p2Diruka->SetScale(1.0f);
	p2Diruka->mPos.y = 0.6f;
	p2Diruka->mPos.z = 0.0f;
	p2Diruka->mPos.x = 5.5f;
	p2Diruka->direction = east;
	p2Diruka->mCamera = gpCamera;
	HitSphere* pHit_2 = gp2Diruka->GetHit();
	pHit_2->SetHankei(0.8f);// 当たり球の半径を設定

	gEnemyList.emplace_back(gp2Diruka);

	// 2Dキャラ用ポリゴン生成
	gModelManager["エイ"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/ナンヨウマンタ.png");

	// 2Dキャラ用オブジェクト生成
	gp2Dei = new BillboardObject();
	Model* p2Dei = gp2Dei->GetModel();
	p2Dei->SetModelData(gModelManager["エイ"]);
	p2Dei->SetScale(1.0f);
	p2Dei->mPos.y = 0.6f;
	p2Dei->mPos.z = -3.0f;
	p2Dei->mPos.x = -0.5f;
	p2Dei->direction = west;
	p2Dei->mCamera = gpCamera;
	HitSphere* pHit_3 = gp2Dei->GetHit();
	pHit_3->SetHankei(0.8f);// 当たり球の半径を設定

	gEnemyList.emplace_back(gp2Dei);

	// 2Dキャラ用ポリゴン生成
	gModelManager["シャチ"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/シャチ.png");

	// 2Dキャラ用オブジェクト生成
	gp2Dsyach = new BillboardObject();
	Model* p2Dsyati = gp2Dsyach->GetModel();
	p2Dsyati->SetModelData(gModelManager["シャチ"]);
	p2Dsyati->SetScale(1.0f);
	p2Dsyati->mPos.y = 0.6f;
	p2Dsyati->mPos.z = -2.0f;
	p2Dsyati->mPos.x = 1.5f;
	p2Dsyati->direction = north;
	p2Dsyati->mCamera = gpCamera;
	HitSphere* pHit_4 = gp2Dsyach->GetHit();
	pHit_4->SetHankei(0.8f);// 当たり球の半径を設定

	gEnemyList.emplace_back(gp2Dsyach);

	// 2Dキャラ用ポリゴン生成
	gModelManager["カジキ"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/バショウカジキ.png");

	// 2Dキャラ用オブジェクト生成
	gp2Dkaziki = new BillboardObject();
	Model* p2Dkaziki = gp2Dkaziki->GetModel();
	p2Dkaziki->SetModelData(gModelManager["カジキ"]);
	p2Dkaziki->SetScale(1.0f);
	p2Dkaziki->mPos.y = 0.6f;
	p2Dkaziki->mPos.z = 4.0f;
	p2Dkaziki->mPos.x = 2.5f;
	p2Dkaziki->direction = west;
	p2Dkaziki->mCamera = gpCamera;
	HitSphere* pHit_5 = gp2Dkaziki->GetHit();
	pHit_5->SetHankei(0.8f);// 当たり球の半径を設定

	gEnemyList.emplace_back(gp2Dkaziki);



	gObjectList.emplace_back(gpGun);

	// コテージ用オブジェクト生成

	gpCottage = new NormalObject();
	Model* pCottageModel = gpCottage->GetModel();
	pCottageModel->SetModelData(gModelManager["cottage"]); // 3Dデータをセットする
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

	// カメラの追従ターゲットを指定
	//((BackCamera*)gpCamera)->SetTarget(gpGun);  // Cスタイルキャスト
	dynamic_cast<BackCamera*>(gpCamera)->SetTarget(gpGun); // C++キャスト
}


Game_Scene::~Game_Scene()
{
	vector_DeleteAll(gObjectList);

	// 弾管理配列の中身を全てdeleteする
	vector_DeleteAll(gShotManager);

	vector_DeleteAll(gEffectManager);

	COM_SAFE_RELEASE(gpConstBuffer);

	// モデルマネージャーが持つ全モデルを解放
	for (auto i = gModelManager.begin();//連想配列の先頭情報
		i != gModelManager.end();//連想配列の最後尾情報
		i++)
	{
		// first … 添え字
		// second … 要素そのもの
		COM_SAFE_RELEASE(i->second.mSRV);//テクスチャ
		COM_SAFE_RELEASE(i->second.mVertexBuffer);//頂点バッファ
	}
	// 連想配列の全要素を削除する（要素数０にする）
	gModelManager.clear();
}

void Game_Scene::Update()
{
	// デルタタイムが想定外の値になった場合の処理
	if (gDeltaTime >= 100)
	{
		gDeltaTime = 0;
	}
	if (gDeltaTime <= 0)
	{
		gDeltaTime = 1;
	}

	// カメラ移動変数
	static float angle = 0.0f; // 回転角度
	static float zoom = 3.0f;  // ズーム

	// ①カメラの位置をキー操作で移動する
	if (Input_GetKeyDown(VK_RIGHT))
	{
		angle += 0.04f * gDeltaTime;
	}
	if (Input_GetKeyDown(VK_LEFT))
	{
		angle -= 0.04f * gDeltaTime;
	}
	// ズーム操作
	if (Input_GetKeyDown(VK_UP))
		zoom -= 0.01f * gDeltaTime;
	if (Input_GetKeyDown(VK_DOWN))
		zoom += 0.01f * gDeltaTime;

	// ②カメラの注視点を中心にカメラを回転する

	// カメラ位置X　＝　sinf(角度ラジアン)
	// カメラ位置Z　＝　cosf(角度ラジアン)
	// 原点を中心に半径1.0fの円周上の点を求める
	Model* pCottageModel = gpCottage->GetModel();
	float radian = XMConvertToRadians(angle);
	gpCamera->mEye.x =
		sinf(radian) * zoom + pCottageModel->mPos.x;
	gpCamera->mEye.z =
		cosf(radian) * zoom + pCottageModel->mPos.z;
	gpCamera->mEye.y = 2.0f;

	// カメラ注視点をコテージの位置にする
	gpCamera->SetFocus(pCottageModel->mPos);

	// キャラクター移動
	// キャラクターが向いている方向に前進する
	// 向き変更＝ADキー　前進＝Wキー
	// 「前向きベクトル」を計算する
	// 移動速度＝Wキーで決まる
	gpGun->mSpeed = 0.0f;
	if (Input_GetKeyDown('W'))
		gpGun->mSpeed = 0.001f;

	// キャラクターの方向転換
	Model* pGunModel = gpGun->GetModel();
	if (Input_GetKeyDown('A'))
		pGunModel->mRotate.y -= 0.04f * gDeltaTime;
	if (Input_GetKeyDown('D'))
		pGunModel->mRotate.y += 0.04f * gDeltaTime;

	// 弾の発射
	if (Input_GetKeyTrigger(VK_LBUTTON))
	{
		// 弾オブジェクト生成
		GameObject* pShot = new BillboardObject();
		Model* pShotModel = pShot->GetModel();
		pShotModel->SetModelData(gModelManager["shot"]);
		pShotModel->SetScale(0.2f);
		pShot->GetHit()->SetHankei(0.19f);

		// 発射地点に移動させる
		XMFLOAT3 forwardVec = gpGun->GetForwardVector();

		pShotModel->mPos.x =
			pGunModel->mPos.x + forwardVec.x * 0.8f;
		pShotModel->mPos.y = pGunModel->mPos.y + 0.2f;
		pShotModel->mPos.z =
			pGunModel->mPos.z + forwardVec.z * 0.8f;
		// 弾に速度を設定する
		pShot->mSpeed = 0.01f;
		// 弾が飛ぶ方向を設定
		pShotModel->mRotate.y = pGunModel->mRotate.y;

		// 弾管理配列に追加する
		gShotManager.emplace_back(pShot);
	}

	vector_UpdateAll(gObjectList);

	// 弾管理配列の中身を全てUpdateする
	vector_UpdateAll(gShotManager);

	// 弾の回収処理
	for (int i = 0; i < gShotManager.size(); i++)
	{
		// (a)カメラの位置
		XMFLOAT3 cameraPos = gpCamera->mEye;
		// (b)弾の位置
		XMFLOAT3 shotPos = gShotManager[i]->GetModel()->mPos;
		// (b)→(a)ベクトル
		XMVECTOR abVector =
			XMLoadFloat3(&cameraPos) - XMLoadFloat3(&shotPos);
		// (b)→(a)ベクトルの長さを計算
		XMVECTOR abVecLength = XMVector3Length(abVector);
		float abLength = XMVectorGetX(abVecLength);
		// abLengthが○○以上なら弾を回収する
		if (abLength >= 50.0f)
		{
			// erase関数だけでは弾のdeleteは行われない
			delete gShotManager[i];
			// 弾管理配列から弾を削除
			gShotManager.erase(gShotManager.begin() + i);
		}
	}

	// 弾とNPC当たり判定
	for (int aw = 0; aw < gEnemyList.size(); aw++)
	{		
		for (int i = 0; i < gShotManager.size(); i++)
		{
			HitSphere* pShotHit = gShotManager[i]->GetHit();
			if (pShotHit->IsHit(gEnemyList[aw]->GetHit()))
			{
				delete gShotManager[i];
				// 弾管理配列から弾を削除

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

	// 再生が終わったエフェクトを消す
	for (int i = 0; i < gEffectManager.size(); i++)
	{
		Explosion* e = dynamic_cast<Explosion*>(gEffectManager[i]);
		if (e->IsFinished())
		{
			delete gEffectManager[i];
			// 弾管理配列から弾を削除
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
	

	// カメラ更新（ビュー変換行列計算）
	gpCamera->Update();
}

void Game_Scene::Draw()
{
	// DIRECT3D構造体にアクセスする
	DIRECT3D* d3d = Direct3D_Get();

	// 画面クリア（指定色で塗りつぶし）
	// 塗りつぶし色の指定（小数：0.0f～1.0f）
	float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; //red,green,blue,alpha

	d3d->context->ClearRenderTargetView(d3d->renderTarget, clearColor);

	// Zバッファ、ステンシルバッファクリア
	d3d->context->ClearDepthStencilView(
		d3d->depthStencilView,			// デプスステンシルビュー
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,			// Ｚバッファを1.0でクリアする(0.0f～1.0f)
		0);				// ステンシルバッファを0でクリアする

	// ↓　自前の描画処理をここに書く *******

	// ゲームオブジェクトを描画
	d3d->context->OMSetBlendState(d3d->blendAlpha, NULL, 0xffffffff);// アルファブレンド

	vector_DrawAll(gObjectList);

	vector_DrawAll(gEnemyList);

	d3d->context->OMSetBlendState(d3d->blendAdd, NULL, 0xffffffff);// 加算合成
	// 弾管理配列の中身を全てDrawする
	vector_DrawAll(gShotManager);

	vector_DrawAll(gEffectManager);

	// ダブルバッファの切り替え
	d3d->swapChain->Present(0, 0);
}
