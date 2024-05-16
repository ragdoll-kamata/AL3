#include "Player.h"
#include "cassert"

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) { 
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	worldTransform.Initialize();

}

void Player::Update() { 
	worldTransform.TransferMatrix();

}

void Player::Draw() { 
	model_->Draw(worldTransform, *viewProjection_, textureHandle_); 
}

