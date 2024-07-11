#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "debugCamera.h"
#include "Skydome.h"
#include "MapChipField.h"
#include "Player.h"
#include "Enemy.h"
#include "CameraController.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void GenerateBlocks();

	void CheckAllCollisns();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	ViewProjection viewProjecion;
	Model* modelBlock_ = nullptr;

	bool isDebugCameraActice_ = false;
	DebugCamera* debugCamera_ = nullptr;

	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	MapChipField* mapChipField_;

	Model* modelPlayer_ = nullptr;
	Player* player_ = nullptr;

	Model* modelEnemy_ = nullptr;
	std::list<Enemy*> enemies_;

	CameraController* cameraController_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
