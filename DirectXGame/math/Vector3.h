#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
class Vector3 final {
public:

	void operator+=(const Vector3& a) {
		x += a.x;
		y += a.y;
		z += a.z;
	}

	float x;
	float y;
	float z;
};