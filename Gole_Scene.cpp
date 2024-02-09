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

#include <map> // 連想配列
#include <vector> // 可変配列

extern ID3D11Buffer* gpConstBuffer;

extern std::map<std::string, ModelData> gModelManager;

//	Spriteの変数(イラストの宣言) 
GameObject* gpGoal;
GameObject* gpG_Enter;
GameObject* gpGsae;

extern ModelData CreateSquarePolygon(float width, float height, float uvWidth, float uvHeight, const wchar_t* texFile);

using namespace DirectX;

extern int Scenenow;

Gole_Scene::Gole_Scene()
{// 定数バッファ作成
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

	// 2Dキャラ用ポリゴン生成
	gModelManager["goal"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.00f,
			L"assets/kuria.png");

	gModelManager["goal_enter"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.00f,
			L"assets/goal.png");

	gModelManager["sea"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/umi.jpg");

	//テクスチャ設定
	gpGoal = new NormalObject();
	Model* G_G_Model = gpGoal->GetModel();
	G_G_Model->SetModelData(gModelManager["goal"]);
	G_G_Model->Set2dRender(true);
	G_G_Model->SetDiffuse(XMFLOAT4(1, 1, 1, 1));
	G_G_Model->mPos = XMFLOAT3(0.0, 0.0, 0.0);
	G_G_Model->mScale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	//テクスチャ設定
	gpG_Enter = new NormalObject();
	Model* G_entModel = gpG_Enter->GetModel();
	G_entModel->SetModelData(gModelManager["goal_enter"]);
	G_entModel->Set2dRender(true);
	G_entModel->SetDiffuse(XMFLOAT4(1, 1, 1, 0.5f));
	G_entModel->mPos = XMFLOAT3(0.0, -0.6, 0.0);
	G_entModel->mScale = XMFLOAT3(1.0f, 0.7f, 1.0f);

	//テクスチャ設定
	gpGsae = new NormalObject();
	Model* G_saeModel = gpGsae->GetModel();
	G_saeModel->SetModelData(gModelManager["sea"]);
	G_saeModel->Set2dRender(true);
	G_saeModel->SetDiffuse(XMFLOAT4(1, 1, 1, 1));
	G_saeModel->mScale = XMFLOAT3(2.0f, 2.0f, 2.0f);

}


Gole_Scene::~Gole_Scene()
{	
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

void Gole_Scene::Update()
{
	if (Input_GetKeyTrigger(VK_RETURN))
		Scenenow = Title;
}

void Gole_Scene::Draw()
{
	// DIRECT3D構造体にアクセスする
	DIRECT3D* d3d = Direct3D_Get();
	// 画面クリア（指定色で塗りつぶし）
	// 塗りつぶし色の指定（小数：0.0f～1.0f）
	float clearColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; //red,green,blue,alpha

	d3d->context->ClearRenderTargetView(d3d->renderTarget, clearColor);

	// Zバッファ、ステンシルバッファクリア
	d3d->context->ClearDepthStencilView(
		d3d->depthStencilView,			// デプスステンシルビュー
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,			// Ｚバッファを1.0でクリアする(0.0f～1.0f)
		0);				// ステンシルバッファを0でクリアする

		// ゲームオブジェクトを描画
	d3d->context->OMSetBlendState(d3d->blendAlpha, NULL, 0xffffffff);// アルファブレンド

	gpGoal->Draw();
	gpG_Enter->Draw();
	gpGsae->Draw();

	// ダブルバッファの切り替え
	d3d->swapChain->Present(0, 0);
}
