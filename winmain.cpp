#undef UNICODE  // Unicodeではなく、マルチバイト文字を使う

#include <Windows.h>
#include "Direct3d.h"
#include "input.h"
#include <DirectXMath.h>
#include "WICTextureLoader.h"
#include "ObjModelLoader.h"
#include "Model.h"
#include "BackCamera.h"
#include "NormalObject.h"
#include "Explosion.h"
#include <map> // 連想配列
#include <vector> // 可変配列
#include"Game_Scene.h"
#include "Title_Scene.h"
#include"nanido.h"
#include"Gole_Scene.h"

#pragma comment (lib, "winmm.lib") // timeGetTime関数のため

// マクロ定義
#define CLASS_NAME    "DX21Smpl"// ウインドウクラスの名前
#define WINDOW_NAME  "スケルトンプログラム"// ウィンドウの名前

#define SCREEN_WIDTH (1024)	// ウインドウの幅
#define SCREEN_HEIGHT (576+30)	// ウインドウの高さ


// 関数のプロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// 頂点バッファ用の変数
ID3D11Buffer* gpVertexBuffer;

// 頂点数を持つ変数
int gNumVertex;

// テクスチャ用の変数
ID3D11ShaderResourceView* gpTextureHalOsaka; // HAL OSAKA

//画面遷移用変数
int Scenenow = Title;

extern ID3D11Buffer* gpConstBuffer;

using namespace DirectX;

bool Game_Scene_Start = false;

// デルタタイム用の変数
DWORD gDeltaTime;

// ビルボード用のポリゴンを作成し、テクスチャも読み込む
ModelData CreateSquarePolygon(float width, float height, float uvWidth, float uvHeight, const wchar_t* texFile)
{
	// モデル頂点データ作成
	const float w = width / 2.0f;
	const float h = height / 2.0f;
	ModelVertex vx[] = {
		{ -w,  h, 0, 0.0f, 0.0f },
		{  w,  h, 0, uvWidth, 0.0f },
		{  w, -h, 0, uvWidth, uvHeight },

		{  w, -h, 0, uvWidth, uvHeight },
		{ -w, -h, 0, 0.0f, uvHeight },
		{ -w,  h, 0, 0.0f, 0.0f },
	};

	vx[0].u = 0.0f;
	vx[0].v = 0.0f;
	vx[1].u = uvWidth;
	vx[1].v = 0.0f;
	vx[2].u = uvWidth;
	vx[2].v = uvHeight;
	vx[3].u = uvWidth;
	vx[3].v = uvHeight;
	vx[4].u = 0.0f;
	vx[4].v = uvHeight;
	vx[5].u = 0.0f;
	vx[5].v = 0.0f;

	ModelData modelData;

	modelData.mNumVertex = sizeof(vx) / sizeof(ModelVertex); // 頂点数を計算

	// モデル用の頂点バッファ作成
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(vx);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	// バッファの初期値指定
	D3D11_SUBRESOURCE_DATA initialVertexData;
	// 頂点バッファの初期値
	initialVertexData.pSysMem = vx;
	// 頂点バッファでは使用しない
	initialVertexData.SysMemPitch = 0;
	initialVertexData.SysMemSlicePitch = 0;

	HRESULT hr = Direct3D_Get()->device->CreateBuffer(&bufferDesc, &initialVertexData, &modelData.mVertexBuffer);

	if (FAILED(hr))
	{
		throw hr;
	}

	Direct3D_LoadTexture(texFile, &modelData.mSRV);

	return modelData;
}

// WinMain関数を作る
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc; // WND = Window

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc); // Windowsにウインドウ情報を登録

	HWND hWnd; // H=Handle=ポインタ WND=Window
	hWnd = CreateWindowEx(0,// 拡張ウィンドウスタイル
		CLASS_NAME,// ウィンドウクラスの名前
		WINDOW_NAME,// ウィンドウの名前
		WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,// ウィンドウスタイル
		CW_USEDEFAULT,// ウィンドウの左上Ｘ座標
		CW_USEDEFAULT,// ウィンドウの左上Ｙ座標 
		SCREEN_WIDTH,// ウィンドウの幅
		SCREEN_HEIGHT,// ウィンドウの高さ
		NULL,// 親ウィンドウのハンドル
		NULL,// メニューハンドルまたは子ウィンドウID
		hInstance,// インスタンスハンドル
		NULL);// ウィンドウ作成データ

	// 指定されたウィンドウの表示状態を設定(ウィンドウを表示)
	ShowWindow(hWnd, nCmdShow);
	// ウィンドウの状態を直ちに反映(ウィンドウのクライアント領域を更新)
	UpdateWindow(hWnd);

	// Direct3Dの初期化関数を呼び出す
	Direct3D_Init(hWnd);

	Title_Scene*Title_S = new Title_Scene;
	Game_Scene*Game = new Game_Scene;
	Gole_Scene*GOAL = new Gole_Scene;
	nanido*STAGE = new nanido;

	MSG msg;
	// メインループ
	for (;;) {
		// 前回のループからユーザー操作があったか調べる
		BOOL doesMessageExist = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

		if (doesMessageExist)
		{
			// 間接的にウインドウプロシージャを呼び出す
			DispatchMessage(&msg);

			// アプリ終了命令が来た
			if (msg.message == WM_QUIT) {
				break;
			}
		}
		else
		{
			// デルタタイムの計測
			static DWORD lastTime = timeGetTime(); // 前回計測時の時間
			timeBeginPeriod(1); // 精度を１ミリ秒に上げる
			DWORD nowTime = timeGetTime(); // 現在の時間
			timeEndPeriod(1); // 精度を元に戻す
			gDeltaTime = nowTime - lastTime; // 差分がデルタタイム
			lastTime = nowTime; // 前回計測時間として保存

			// ゲームループ
			switch (Scenenow)
			{
			case Title:
				Title_S->Update();
				Title_S->Draw();
				break;
			
			case Stage:
				STAGE->Update();
				STAGE->Draw();
				break;

			case Game_:
				Game->Update();
				Game->Draw();
				break;

			case Goal:
				GOAL->Update();
				GOAL->Draw();

				break;
			}

			Input_Refresh(); // キー状態の更新
		}
	}

	delete Game;
	delete Title_S;
	delete GOAL;
	// Direct3Dの解放関数を呼び出す
	Direct3D_Release();

	UnregisterClass(CLASS_NAME, hInstance);

	return (int)msg.wParam;
}


// ウインドウプロシージャ関数を作る
// ※関数を作れるのはグローバル領域(=どの関数の中でもない場所)だけ！
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// uMsg（この関数の第2引数）が、ユーザー操作のID情報
	switch (uMsg)
	{
	case WM_DESTROY:// ウィンドウ破棄のメッセージ
		PostQuitMessage(0);// “WM_QUIT”メッセージを送る　→　アプリ終了
		break;

	case WM_CLOSE:  // xボタンが押されたら
		DestroyWindow(hWnd);  // “WM_DESTROY”メッセージを送る
		break;

	case WM_LBUTTONDOWN: // 左クリックされたとき
		Input_SetKeyDown(VK_LBUTTON);
		break;

	case WM_LBUTTONUP: // 左クリックされたとき
		Input_SetKeyUp(VK_LBUTTON);
		break;

	case WM_RBUTTONDOWN: // 左クリックされたとき
		Input_SetKeyDown(VK_RBUTTON);
		break;

	case WM_RBUTTONUP: // 左クリックされたとき
		Input_SetKeyUp(VK_RBUTTON);
		break;

	case WM_MOUSEMOVE: // マウスカーソルが動いたとき
		break;

	case WM_KEYDOWN:
		// キーが押された時のリアクションを書く
		// ESCが押されたのかどうかチェック
		if (LOWORD(wParam) == VK_ESCAPE)
		{
			// メッセージボックスで修了確認
			int result;
			result = MessageBox(NULL, "終了してよろしいですか？",
				"終了確認", MB_YESNO | MB_ICONQUESTION);
			if (result == IDYES) // 「はい」ボタンが押された時
			{
				// xボタンが押されたのと同じ効果を発揮する
				PostMessage(hWnd, WM_CLOSE, wParam, lParam);
			}
		}
		Input_SetKeyDown(LOWORD(wParam));
		break;

	case WM_KEYUP: // キーが離されたイベント
		Input_SetKeyUp(LOWORD(wParam));
		break;

	default:
		// 上のcase以外の場合の処理を実行
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	return 0;
}




