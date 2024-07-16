#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "input.h"
#include <numbers>

class DeathParticles {
public:

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3 position);

	void Updata();

	void Draw();

private:
	Model* model_;
	ViewProjection* viewProjection_;
	static inline const uint32_t kNumParticles = 8;

	std::array<WorldTransform, kNumParticles> worldTransforms_;

	static inline const float kDuration = 2.0f;
	static inline const float kSpeed = 0.05f;
	static inline const float kAngleUnit = 2.0f * std::numbers::pi_v<float> / kNumParticles;

	bool isFinished_ = false;
	float counter_ = 0.0f;

	ObjectColor objectColor_;

	Vector4 color_;
};
