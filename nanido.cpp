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

#include <map> // 連想配列
#include <vector> // 可変配列

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

	//テクスチャ設定
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


	nani->Draw();
	easy->Draw();
	normal->Draw();
	hard->Draw();
	arrow->Draw();
	n_sae->Draw();

	// ダブルバッファの切り替え
	d3d->swapChain->Present(0, 0);
}

