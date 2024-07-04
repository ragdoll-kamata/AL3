#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "input.h"

class Enemy {
public:

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3 position);

	void Update();
	
	void Draw();

private:
	WorldTransform worldTransform;
	Model* model_ = nullptr;
	ViewProjection* viewProjection_;
	Vector3 velocity_ = {};

	float walkTimer_ = 0.0f;

	static inline const float kWalkSpeed = 0.05f;

	static inline const float kWalkMotionAngleStart = 20.0f;
	static inline const float kWalkMotionAngleEnd = -45.0f;
	static inline const float kWalkMotionTime = 1.0f;
};
