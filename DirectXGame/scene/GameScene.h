#pragma once
#include "Scene.h"
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
#include "DeathParticles.h"

enum class Phase {
	kPlay,
	kDeath,
};

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene : public Scene {

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
	void Initialize() override;

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

	void GenerateBlocks();

	void CheckAllCollisns();

	void ChangePhase();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	WorldTransform GoolTransform;
	ViewProjection viewProjecion;
	Model* modelBlock_ = nullptr;
	Model* modelGoal_ = nullptr;

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

	Model* modelDeathParticles_ = nullptr;
	DeathParticles* deathParticles_ = nullptr;

	Phase phase_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
