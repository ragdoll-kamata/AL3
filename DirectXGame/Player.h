#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "input.h"
enum class LRDirection {
	kRight,
	kLeft,
};
class Player {
public:
	Player();
	~Player();

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3 position);

	void Update();

	void Draw();

	float Lape(float strat, float end, float t);

	WorldTransform* GetWorldTransfoam() { return &worldTransform; }
	const Vector3& GetVelocity() const { return velocity_; }

private:

	WorldTransform worldTransform;
	Model* model_ = nullptr;
	ViewProjection* viewProjection_;
	uint32_t textureHandle_ = 0u;
	LRDirection lrDierction = LRDirection::kRight;
	Vector3 velocity_ = {};

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;

	bool onGround_ = true;



	static inline const float kAcceleration = 0.05f;
	static inline const float kAttenuation = 0.1f;
	static inline const float kLimitRunSpeed_ = 0.5f;
	static inline const float kTimeTurn_ = 0.3f;

	static inline const float kGravityAcceleration = 0.1f;
	static inline const float kLimitFallSpeed = 1.0f;
	static inline const float kJumpAcceleration = 1.0f;
};
