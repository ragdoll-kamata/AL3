#include "OverScene.h"
#include <cmath>
#include <numbers>

OverScene::OverScene() {}

OverScene::~OverScene() { delete modelGameOver_; }

void OverScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjecion.Initialize();
	worldTransform.Initialize();
	viewProjecion.translation_.z = -10.0f;
	viewProjecion.UpdateMatrix();
	modelGameOver_ = Model::CreateFromOBJ("GameOver", true);
}

void OverScene::Update() {
	if (input_->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
	radian += std::numbers::pi_v<float> / 60.0f;
	worldTransform.translation_.y = std::cos(radian) * 0.5f + 1;
	worldTransform.UpdateMatrix();
}

void OverScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	modelGameOver_->Draw(worldTransform, viewProjecion);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}