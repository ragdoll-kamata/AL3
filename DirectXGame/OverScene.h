#pragma once
#include "Scene.h"
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class OverScene : public Scene {
public:
	/// <summary>
	/// コンストクラタ
	/// </summary>
	OverScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~OverScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	ViewProjection viewProjecion;
	WorldTransform worldTransform;
	Model* modelGameOver_ = nullptr;

	float radian = 0.0f;
};
