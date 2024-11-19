#include "StageManager.h"

void StageManager::Init() {
	field_.Init();
	oreManager_.Init(1);
}
void StageManager::Update() {
	oreManager_.Update();
}
