#include "Affine.h"
#include <cmath>

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 m3 = {};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				m3.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return m3;
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	return Matrix4x4{1.0f, 0.0f, 0.0f, 0.0f, 0.0f, std::cos(radian), std::sin(radian), 0.0f, 0.0f, -std::sin(radian), std::cos(radian), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	return Matrix4x4{std::cos(radian), 0.0f, -std::sin(radian), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, std::sin(radian), 0.0f, std::cos(radian), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	return Matrix4x4{std::cos(radian), std::sin(radian), 0.0f, 0.0f, -std::sin(radian), std::cos(radian), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
}
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 matrixRotate = Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));
	Matrix4x4 a = {
	    scale.x * matrixRotate.m[0][0],
	    scale.x * matrixRotate.m[0][1],
	    scale.x * matrixRotate.m[0][2],
	    0.0f,
	    scale.y * matrixRotate.m[1][0],
	    scale.y * matrixRotate.m[1][1],
	    scale.y * matrixRotate.m[1][2],
	    0.0f,
	    scale.z * matrixRotate.m[2][0],
	    scale.z * matrixRotate.m[2][1],
	    scale.z * matrixRotate.m[2][2],
	    0.0f,
	    translate.x,
	    translate.y,
	    translate.z,
	    1.0f};
	return a;
}
