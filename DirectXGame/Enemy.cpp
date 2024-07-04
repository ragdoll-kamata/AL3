#include "Enemy.h"
#include "cassert"
#include <algorithm>
#include <numbers>
void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3 position) {
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform.Initialize();
	worldTransform.translation_ = position;
	worldTransform.rotation_.y = std::numbers::pi_v<float> * 3.0f / 2.0f;
	walkTimer_ = 0.0f;
}

void Enemy::Update() {
	velocity_ = {-kWalkSpeed, 0.0f, 0.0f };

	worldTransform.translation_ += velocity_;

	walkTimer_ += 1.0f / 60.0f;

	float param = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);

	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd*(param + 1.0f) / 2.0f;

	worldTransform.rotation_.z = radian * (std::numbers::pi_v<float> / 180.0f);

	worldTransform.UpdateMatrix();
}

void Enemy::Draw() {
	model_->Draw(worldTransform, *viewProjection_);
}
