#include "MoveHN_R.h"

using namespace LWP::Math;
using namespace LWP::Info;

void MoveHN_R::Init(Vector2 pos, int* quota) {
	IOre::Init(pos, quota);
	// モーション作成
	CreateMotion(
		{ -kMove, 0.0f, 0.0f },
		{  kMove, 0.0f, 0.0f }
	);
}