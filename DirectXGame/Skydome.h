#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"

class Skydome {
public:


	void Initialize(Model* model, ViewProjection* viewProjection);


	void Update();

	
	void Draw();


private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	ViewProjection* viewProjection_ = nullptr;

};
