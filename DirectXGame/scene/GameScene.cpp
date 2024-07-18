#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	for (std::vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	delete debugCamera_;
	delete modelBlock_;
	delete modelSkydome_;
	delete skydome_;
	delete mapChipField_;
	delete modelPlayer_;
	delete player_;
	delete cameraController_;
	delete modelEnemy_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	delete modelDeathParticles_;
	delete deathParticles_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjecion.Initialize();
	modelBlock_ = Model::CreateFromOBJ("block", true);
	debugCamera_ = new DebugCamera(1280, 720);
	debugCamera_->SetFarZ(1200.0f);
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &viewProjecion);
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/map.csv");
	GenerateBlocks();
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	player_ = new Player();
	player_->Initialize(modelPlayer_, &viewProjecion, mapChipField_->GetMapChipPositionByIndex(2, 15));
	player_->SetMapChipField(mapChipField_);
	cameraController_ = new CameraController();
	cameraController_->SetViewProjection(&viewProjecion);
	cameraController_->SetPlayer(player_);
	cameraController_->Reset();

	modelEnemy_ = Model::CreateFromOBJ("enemy", true);
	for (int32_t i = 0; i < 3; i++) {
		Enemy* enemy = new Enemy();
		enemy->Initialize(modelEnemy_, &viewProjecion, mapChipField_->GetMapChipPositionByIndex(20 + i, 18 - i));
		enemies_.push_back(enemy);
	}

	modelDeathParticles_ = Model::CreateFromOBJ("deathParticle", true);


	phase_ = Phase::kPlay;
}

void GameScene::Update() {
	switch (phase_) {
	case Phase::kPlay:
		skydome_->Update();
		player_->Update();
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		cameraController_->Updata();
#ifdef _DEBUG
		if (input_->TriggerKey(DIK_SPACE)) {
			if (!isDebugCameraActice_) {
				isDebugCameraActice_ = true;
			} else {
				isDebugCameraActice_ = false;
			}
		}
#endif
		if (isDebugCameraActice_) {
			debugCamera_->Update();

			viewProjecion.matView = debugCamera_->GetViewProjection().matView;
			viewProjecion.matProjection = debugCamera_->GetViewProjection().matProjection;
			viewProjecion.TransferMatrix();
		} else {

			viewProjecion.UpdateMatrix();
		}
		for (std::vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				worldTransformBlock->UpdateMatrix();
			}
		}

		CheckAllCollisns();


		break;
	case Phase::kDeath:
		skydome_->Update();
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		if (deathParticles_ != nullptr) {
			deathParticles_->Updata();
		}
		if (isDebugCameraActice_) {
			debugCamera_->Update();

			viewProjecion.matView = debugCamera_->GetViewProjection().matView;
			viewProjecion.matProjection = debugCamera_->GetViewProjection().matProjection;
			viewProjecion.TransferMatrix();
		} else {

			viewProjecion.UpdateMatrix();
		}
		for (std::vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				worldTransformBlock->UpdateMatrix();
			}
		}

		break;
	}
	ChangePhase();
}

void GameScene::Draw() {

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
	switch (phase_) {
	case Phase::kPlay:
		for (std::vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				modelBlock_->Draw(*worldTransformBlock, viewProjecion);
			}
		}
		skydome_->Draw();

		player_->Draw();
		for (Enemy* enemy : enemies_) {
			enemy->Draw();
		}

		break;
	case Phase::kDeath:
		for (std::vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock) {
					continue;
				}
				modelBlock_->Draw(*worldTransformBlock, viewProjecion);
			}
		}
		skydome_->Draw();
		for (Enemy* enemy : enemies_) {
			enemy->Draw();
		}

		if (deathParticles_ != nullptr) {
			deathParticles_->Draw();
		}
		break;
	}

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

void GameScene::GenerateBlocks() {

	const uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirtical();
	const uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	for (uint32_t i = 0; i < kNumBlockVirtical; i++) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				worldTransformBlocks_[i][j] = new WorldTransform();
				worldTransformBlocks_[i][j]->Initialize();
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
				
			}
		}
	}



}

void GameScene::CheckAllCollisns() {
	AABB aabb1, aabb2;
	aabb1 = player_->GetAABB();
	for (Enemy* enemy : enemies_) {
		
		aabb2 = enemy->GetAABB();
		if (AABBCollision(aabb1, aabb2)) {
			player_->OnCollision(enemy);
			enemy->OnCollision(player_);
		}
	}


}

void GameScene::ChangePhase() {
	switch (phase_) { 
	case Phase::kPlay:
		if (player_->IsDead()) {
			phase_ = Phase::kDeath;

			deathParticles_ = new DeathParticles();
			deathParticles_->Initialize(modelDeathParticles_, &viewProjecion, player_->GetWorldPosition());
		}
		break;
	case Phase::kDeath:
		break;
	}






}
