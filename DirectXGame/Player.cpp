#define NOMINMAX
#include "Player.h"
#include "MapChipField.h"
#include "cassert"
#include <algorithm>
#include <numbers>
#include "DebugText.h"
#include "Enemy.h"

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
	
	MoveInput();

	CollisionMapInfo collisionMapInfo;

	collisionMapInfo.velocity = velocity_;

	MapCollision(collisionMapInfo);

	Move(collisionMapInfo);

	IsCeilingCollisionProcess(collisionMapInfo);

	OnGroundSwitch(collisionMapInfo);

	
	if (turnTimer_ > 0.0f) {
		turnTimer_ -= 1.0f / 60.0f;
		float destinationRotationYTable[] = {
			std::numbers::pi_v<float> / 2.0f,
			std::numbers::pi_v<float> * 3.0f / 2.0f
		};
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDierction)];

		worldTransform.rotation_.y = Lape(turnFirstRotationY_, destinationRotationY, 1.0f - turnTimer_ / kTimeTurn_);
	}

	worldTransform.UpdateMatrix();


}

void Player::MoveInput() {

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
	}
}

void Player::MapCollision(CollisionMapInfo& info) { 
	MapCollisionTop(info);
	MapCollisionBottom(info);
	MapCollisionLeft(info);
	MapCollisionRight(info);

}

void Player::MapCollisionTop(CollisionMapInfo& info) { 


	if (info.velocity.y <= 0.0f) {
		return;
	}

	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform.translation_ + Vector3(0.0f, info.velocity.y, 0.0f), static_cast<Corner>(i));
	} 
	
	MapChipType mapChipType;

	bool hit = false;

	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.y = std::max(0.0f, (rect.bottom - worldTransform.translation_.y) - (kHeight / 2.0f + kBlank));
		info.isCeilingCollision = true;
	}
}

void Player::MapCollisionBottom(CollisionMapInfo& info) {
	if (info.velocity.y >= 0.0f) {
		return;
	}

	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform.translation_ + Vector3(0.0f,info.velocity.y,0.0f), static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	bool hit = false;

	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.y = std::min(0.0f, (rect.top - worldTransform.translation_.y) + (kHeight / 2.0f + kBlank));
		info.landing = true;
	}
}

void Player::MapCollisionLeft(CollisionMapInfo& info) {
	if (info.velocity.x >= 0.0f) {
		return;
	}

	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform.translation_ + info.velocity, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	bool hit = false;

	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kLeftBottom]);
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.x = std::min(0.0f, (rect.right - worldTransform.translation_.x) + (kWidth /2.0f + kBlank));
		info.isWallTouch = true;
	}
}

void Player::MapCollisionRight(CollisionMapInfo& info) {
	if (info.velocity.x <= 0.0f) {
		return;
	}

	std::array<Vector3, kNumCorner> positionNew;

	for (uint32_t i = 0; i < positionNew.size(); ++i) {
		positionNew[i] = CornerPosition(worldTransform.translation_ + info.velocity, static_cast<Corner>(i));
	}

	MapChipType mapChipType;

	bool hit = false;

	MapChipField::IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionNew[kRightTop]);
		MapChipField::Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.velocity.x = std::max(0.0f, (rect.left - worldTransform.translation_.x) - (kWidth + kBlank));
		info.isWallTouch = true;
	}
}

void Player::Move(CollisionMapInfo& info) { 
	worldTransform.translation_ += info.velocity; 
}

void Player::IsCeilingCollisionProcess(const CollisionMapInfo& info) { 
	if (info.isCeilingCollision) {
		DebugText::GetInstance()->ConsolePrintf("hitCeiling\n");
		velocity_.y = 0.0f;
	}
}

void Player::OnGroundSwitch(const CollisionMapInfo& info) {
	if (onGround_) {
		MapChipType mapChipType;

		bool hit = false;

		MapChipField::IndexSet indexSet;
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(CornerPosition(worldTransform.translation_ + Vector3(0.0f, -kMin, 0.0f), kLeftBottom));
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(CornerPosition(worldTransform.translation_ + Vector3(0.0f, -kMin, 0.0f), kRightBottom));
		mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
		if (mapChipType == MapChipType::kBlock) {
			hit = true;
		}
		if (!hit) {
			onGround_ = false;
		}

	} else {
		if (info.landing) {
			velocity_.x *= (1.0f - kAttenuation);
			velocity_.y = 0.0f;
			onGround_ = true;
		}
	}
}

void Player::WallTouchProcess(const CollisionMapInfo& info) { 
	if (info.isWallTouch) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

void Player::Draw() { 
	model_->Draw(worldTransform, *viewProjection_); 
}

float Player::Lape(float strat, float end, float t) { 
	return (1.0f - t) * strat + t * end; }

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	
	Vector3 offsetTable[kNumCorner] = {
		{+kWidth / 2.0f, -kHeight / 2.0f, 0.0f},
		{-kWidth / 2.0f, -kHeight / 2.0f, 0.0f},
		{+kWidth / 2.0f, +kHeight / 2.0f, 0.0f},
		{-kWidth / 2.0f, +kHeight / 2.0f, 0.0f}
	};

	return offsetTable[static_cast<int>(corner)] + center;
}

Vector3 Player::GetWorldPosition() { 
	Vector3 worldPos;

	worldPos.x = worldTransform.matWorld_.m[3][0];
	worldPos.y = worldTransform.matWorld_.m[3][1];
	worldPos.z = worldTransform.matWorld_.m[3][2];

	return worldPos;
}

AABB Player::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};

	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Player::OnCollision(const Enemy* enemy) {
	Vector3 velocity = enemy->GetVelocity() + enemy->GetVelocity();

	velocity.y = 0.7f;

	velocity_ = velocity;
}

