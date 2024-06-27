#include "CameraController.h"
#include "Player.h"
#include "WorldTransform.h"
#include <iostream>
#include <algorithm>

void CameraController::Intialize() { 
	viewProjection_->Initialize();
}

void CameraController::Updata() {
	const WorldTransform* targetWorldTransform = target_->GetWorldTransfoam();

	targetTranslation_ = targetOffset_ + targetWorldTransform->translation_;
	const Vector3 targetVelocity = target_->GetVelocity();
	targetTranslation_.x += targetVelocity.x * kVelocityBias;
	targetTranslation_.y += targetVelocity.y * kVelocityBias;
	targetTranslation_.z += targetVelocity.z * kVelocityBias;


	viewProjection_->translation_ = Vectord3Larp(viewProjection_->translation_, targetTranslation_, kInterpolationRate);

	viewProjection_->translation_.x = std::clamp(viewProjection_->translation_.x, targetWorldTransform->translation_.x + margin_.left, targetWorldTransform->translation_.x + margin_.right);
	viewProjection_->translation_.y = std::clamp(viewProjection_->translation_.y, targetWorldTransform->translation_.y + margin_.bottom, targetWorldTransform->translation_.y + margin_.top);



	viewProjection_->translation_.x = std::clamp(viewProjection_->translation_.x, movableArea_.left, movableArea_.right);
	viewProjection_->translation_.y = std::clamp(viewProjection_->translation_.y, movableArea_.bottom, movableArea_.top);

	viewProjection_->UpdateMatrix();

}

void CameraController::Reset() { 
	const WorldTransform* targetWorldTransform = target_->GetWorldTransfoam();
	viewProjection_->translation_ = targetOffset_ + targetWorldTransform->translation_;
}

float CameraController::Larp(float start, float end, float t) {
	return (1.0f -t) * start + t * end; 
}

Vector3 CameraController::Vectord3Larp(Vector3 start, Vector3 end, float t) { 
	return Vector3(Larp(start.x, end.x, t), Larp(start.y, end.y, t), Larp(start.z, end.z, t));
}
