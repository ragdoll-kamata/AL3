#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
class Player {
public:
	Player();
	~Player();

	void Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection);

	void Update();

	void Draw();

private:

	WorldTransform worldTransform;
	Model* model_ = nullptr;
	ViewProjection* viewProjection_;
	uint32_t textureHandle_ = 0u;
};
