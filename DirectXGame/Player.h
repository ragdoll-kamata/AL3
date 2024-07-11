#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "input.h"
#include "AABB.h"

class MapChipField;
class Enemy;

enum class LRDirection {
	kRight,
	kLeft,
};

struct CollisionMapInfo {
	bool isCeilingCollision = false;
	bool landing = false;
	bool isWallTouch = false;
	Vector3 velocity;
};

enum Corner {
	kRightBottom,
	kLeftBottom,
	kRightTop,
	kLeftTop,
	kNumCorner
};

class Player {
public:
	Player();
	~Player();

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3 position);

	void Update();

	void MoveInput();

	void MapCollision(CollisionMapInfo& info);

	void MapCollisionTop(CollisionMapInfo& info);
	void MapCollisionBottom(CollisionMapInfo& info);
	void MapCollisionLeft(CollisionMapInfo& info);
	void MapCollisionRight(CollisionMapInfo& info);
	
	void Move(CollisionMapInfo& info);

	void IsCeilingCollisionProcess(const CollisionMapInfo& info);

	void OnGroundSwitch(const CollisionMapInfo& info);

	void WallTouchProcess(const CollisionMapInfo& info);

	void Draw();

	float Lape(float strat, float end, float t);

	WorldTransform* GetWorldTransfoam() { return &worldTransform; }
	const Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	Vector3 CornerPosition(const Vector3& center, Corner corner);
	
	Vector3 GetWorldPosition();

	AABB GetAABB();

	void OnCollision(const Enemy* enemy);

private:

	WorldTransform worldTransform;
	Model* model_ = nullptr;
	ViewProjection* viewProjection_;
	LRDirection lrDierction = LRDirection::kRight;
	Vector3 velocity_ = {};

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;

	bool onGround_ = true;

	MapChipField* mapChipField_ = nullptr;

	static inline const float kAcceleration = 0.02f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kAttenuationWall = 0.2f;

	static inline const float kLimitRunSpeed_ = 0.3f;
	static inline const float kTimeTurn_ = 0.3f;

	static inline const float kGravityAcceleration = 0.025f;
	static inline const float kLimitFallSpeed = 1.0f;
	static inline const float kJumpAcceleration = 0.5f;

	static inline const float kWidth = 0.9f;
	static inline const float kHeight = 0.7f;
	static inline const float kBlank = 0.2f;

	static inline const float kMin = 0.1f;
};
