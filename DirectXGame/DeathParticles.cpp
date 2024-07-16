#include "DeathParticles.h"
#include "Affine.h"
#include <algorithm>

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3 position) {
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
	model_ = model;
	viewProjection_ = viewProjection;
	objectColor_.Initialize();
	color_ = {1.0f, 1.0f, 1.0f, 1.0f};
}

void DeathParticles::Updata() {

	if (isFinished_) {
		return;
	}

	for (uint32_t i = 0; i < kNumParticles; ++i) {
		Vector3 velocity = {kSpeed, 0.0f, 0.0f};
		float angle = kAngleUnit * i;
		Matrix4x4 matrixRotate = MakeRotateZMatrix(angle);
		velocity = Transform(velocity, matrixRotate);
		worldTransforms_[i].translation_ += velocity;
	}

	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
	objectColor_.SetColor(color_);
	objectColor_.TransferMatrix();

	counter_ += 1.0f / 60.0f;
	if (counter_ >= kDuration) {
		isFinished_ = true;
	}

	for (auto& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}
}

void DeathParticles::Draw() {

	if (isFinished_) {
		return;
	}

	for (auto& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, *viewProjection_, &objectColor_);
	}
}
