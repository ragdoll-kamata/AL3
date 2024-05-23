#include "Skydome.h"
#include "cassert"
void Skydome::Initialize(Model* model, ViewProjection* viewProjection) { 
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.Initialize();
	worldTransform_.scale_.x = 100.0f;
	worldTransform_.scale_.y = 100.0f;
	worldTransform_.scale_.z = 100.0f;
}

void Skydome::Update() { 
	worldTransform_.TransferMatrix();
}

void Skydome::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_);
}
