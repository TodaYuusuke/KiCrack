#include "MoveVT_D.h"

using namespace LWP::Math;
using namespace LWP::Info;

void MoveVT_D::Init(Vector2 pos, int* quota) {
	IOre::Init(pos, quota);
	// モーション作成
	CreateMotion(
		{ 0.0f,  kMove, 0.0f },
		{ 0.0f, -kMove, 0.0f }
	);
}