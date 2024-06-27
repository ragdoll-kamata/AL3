#pragma once
#include "ViewProjection.h"
class Player;

struct Rect {
	float left;
	float right;
	float bottom;
	float top;
};

class CameraController {
public:
	void Intialize();

	void Updata();

	void Reset();

	void SetPlayer(Player* target) { target_ = target; }
	void SetViewProjection(ViewProjection* target) { viewProjection_ = target; }
	void SetMovebleArea(Rect area) { movableArea_ = area; }

	
private:
	ViewProjection* viewProjection_;
	Player* target_;

	Vector3 targetOffset_ = {0.0f, 0.0f, -20.0f};
	Rect movableArea_ = {13.0f, 100.0f, 7.0f, 50.0f};
	Rect margin_ = {-5.0f, 5.0f, -5.0f, 5.0f};

	Vector3 targetTranslation_;

	static inline const float kInterpolationRate = 0.5f;
	static inline const float kVelocityBias = 5.0f;

	float Larp(float start, float end, float t);
	Vector3 Vectord3Larp(Vector3 start, Vector3 end, float t);
};
