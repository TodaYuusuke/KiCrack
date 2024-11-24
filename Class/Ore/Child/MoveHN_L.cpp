#include "MoveHN_L.h"

using namespace LWP::Math;
using namespace LWP::Info;

void MoveHN_L::Init(Vector2 pos, int* quota) {
	IOre::Init(pos, quota);
	// モーション作成
	CreateMotion(
		{  kMove, 0.0f, 0.0f },
		{ -kMove, 0.0f, 0.0f }
	);
}

void MoveHN_L::Update() {
	IOre::Update();
}

void MoveHN_L::CreateMotion(Vector3 move1, Vector3 move2) {
	motion_.Add(
		&spawnPointModel_.worldTF.translation,
		move1,
		0.0f, kMoveTime
	);
	motion_.Add(
		&spawnPointModel_.worldTF.translation,
		move2,
		kMoveTime, kMoveTime
	);
}
