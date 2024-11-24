#include "Weak.h"

using namespace LWP::Math;

void Weak::Init(Vector2 pos, int* quota) {
	// 初期体力を1に
	parameter_.kMaxHP = 1;

	IOre::Init(pos, quota);
	MaxHPModel_.isActive = false;
	LowHPModel_.isActive = true;
}

void Weak::Respawn() {
	IOre::Respawn();
	// 鉱石モデルの表示を逆に
	MaxHPModel_.isActive = false;
	LowHPModel_.isActive = true;
}
