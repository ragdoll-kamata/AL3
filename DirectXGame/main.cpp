#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "WinApp.h"

#include "TitleScene.h"
#include "ClearScene.h"
#include "OverScene.h"

Scene* scene = nullptr;
enum class SceneSelect {
	kUnknown = 0,

	kTitle,
	kGame,
	kGameClear,
	kGameOver,
};
SceneSelect sceneSelect = SceneSelect::kUnknown;

void ChangeScene();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;
	

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"追加試験");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	// ゲームシーンの初期化
	sceneSelect = SceneSelect::kTitle;
	scene = new TitleScene();
	scene->Initialize();

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();
		// ゲームシーンの毎フレーム処理
		scene->Update();
		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();
		// ゲームシーンの描画
		scene->Draw();
		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
		if (scene->IsFinished()) {
			ChangeScene();
		}
	}

	// 各種解放
	delete scene;
	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}

void ChangeScene() {
	switch (sceneSelect) {
	case SceneSelect::kTitle:
		sceneSelect = SceneSelect::kGame;
		delete scene;
		scene = nullptr;
		scene = new GameScene;
		scene->Initialize();
		break;
	case SceneSelect::kGame:
		if (scene->IsClear()) {
			sceneSelect = SceneSelect::kGameClear;
			delete scene;
			scene = nullptr;
			scene = new ClearScene;
			scene->Initialize();
		} else {
			sceneSelect = SceneSelect::kGameOver;
			delete scene;
			scene = nullptr;
			scene = new OverScene;
			scene->Initialize();
		}
		break;
	case SceneSelect::kGameClear:
		sceneSelect = SceneSelect::kTitle;
		delete scene;
		scene = nullptr;
		scene = new TitleScene;
		scene->Initialize();
		break;
	case SceneSelect::kGameOver:
		sceneSelect = SceneSelect::kTitle;
		delete scene;
		scene = nullptr;
		scene = new TitleScene;
		scene->Initialize();
		break;
	}
}
