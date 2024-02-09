#include "Title_Scene.h"
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

#include <map> // 連想配列
#include <vector> // 可変配列

extern ID3D11Buffer* gpConstBuffer;

using namespace DirectX;

extern std::map<std::string, ModelData> gModelManager;

//	Spriteの変数(イラストの宣言) 
GameObject* gpTitle;
GameObject* gpenter;
GameObject* sae;


extern ModelData CreateSquarePolygon(float width, float height, float uvWidth, float uvHeight, const wchar_t* texFile);


extern int Scenenow;


Title_Scene::Title_Scene()
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

	// モデル読み込み
	// 2Dキャラ用ポリゴン生成
	gModelManager["title"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.00f,
			L"assets/title.png");

	gModelManager["enter"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.00f,
			L"assets/Start.png");

	gModelManager["sea"] =
		CreateSquarePolygon(1.0f, 1.0f, 1.0f, 1.0f,
			L"assets/umi.jpg");

	//テクスチャ設定
	gpTitle = new NormalObject();
	Model* pModel = gpTitle->GetModel();
	pModel->SetModelData(gModelManager["title"]);
	pModel->Set2dRender(true);
	pModel->SetDiffuse(XMFLOAT4(1, 1, 1, 1));
	pModel->mScale = XMFLOAT3(1.4f, 0.7f, 1.0f);

	//テクスチャ設定
	gpenter = new NormalObject();
	Model* entModel = gpenter->GetModel();
	entModel->SetModelData(gModelManager["enter"]);
	entModel->Set2dRender(true);
	entModel->SetDiffuse(XMFLOAT4(1, 1, 1, 0.5f));
	entModel->mPos = XMFLOAT3(0.0, -0.7, 0.0);
	entModel->mScale = XMFLOAT3(0.7f, 0.4f, 1.0f);

	//テクスチャ設定
	sae = new NormalObject();
	Model* saeModel = sae->GetModel();
	saeModel->SetModelData(gModelManager["sea"]);
	saeModel->Set2dRender(true);
	saeModel->SetDiffuse(XMFLOAT4(1, 1, 1, 1));
	saeModel->mScale = XMFLOAT3(2.0f, 2.0f, 2.0f);
}

Title_Scene::~Title_Scene()
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

void Title_Scene::Update()
{	
	if (Input_GetKeyTrigger(VK_RETURN))
		Scenenow = Stage;

}

void Title_Scene::Draw()
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

		// ゲームオブジェクトを描画
	d3d->context->OMSetBlendState(d3d->blendAlpha, NULL, 0xffffffff);// アルファブレンド


	gpTitle->Draw();
	gpenter->Draw();
	sae->Draw();

	// ダブルバッファの切り替え
	d3d->swapChain->Present(0, 0);
}
