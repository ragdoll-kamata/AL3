#define NOMINMAX
#include "Player.h"
#include "cassert"
#include <algorithm>
#include <numbers>
Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3 position)
{ 
	assert(model);
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform.Initialize();
	worldTransform.translation_ = position;
}

void Player::Update() { 
	bool landing = false;
	if (velocity_.y < 0.0f) {
	
		if (worldTransform.translation_.y <= 1.0f) {
			landing = true;
		}
	}

	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			Vector3 acceleration = {};
			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				acceleration.x += kAcceleration;
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				if (lrDierction != LRDirection::kRight) {
					lrDierction = LRDirection::kRight;

					turnFirstRotationY_ = worldTransform.rotation_.y;
					turnTimer_ = kTimeTurn_;
				}
			}
			if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				acceleration.x -= kAcceleration;
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				if (lrDierction != LRDirection::kLeft) {
					lrDierction = LRDirection::kLeft;
					turnFirstRotationY_ = worldTransform.rotation_.y;
					turnTimer_ = kTimeTurn_;
				}
			}
			velocity_ += acceleration;
			
			velocity_.x = std::clamp<float>(velocity_.x, -kLimitRunSpeed_, kLimitRunSpeed_);

		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_ += Vector3(0.0f, kJumpAcceleration, 0.0f);
		}
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		}

	} else {
		velocity_ += Vector3(0.0f, -kGravityAcceleration, 0.0f);
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
		if (landing) {
			worldTransform.translation_.y = 1.0f;
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}


	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;
		float destinationRotationYTable[] = {
			std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float> * 3.0f / 2.0f
		};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDierction)];

		worldTransform.rotation_.y = Lape(turnFirstRotationY_, destinationRotationY, 1.0f - turnTimer_ / kTimeTurn_);
	}

	worldTransform.translation_ += velocity_;

	worldTransform.UpdateMatrix();

}

void Player::Draw() { 
	model_->Draw(worldTransform, *viewProjection_, textureHandle_); 
}

float Player::Lape(float strat, float end, float t) { 
	return (1.0f - t) * strat + t * end;
}

