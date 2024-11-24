#include "StageManager.h"

StageManager::StageManager() {
	field_.Init();
}

void StageManager::Init() {
	oreManager_.Init(currentStageLevel);
}
void StageManager::Update() {
	oreManager_.Update();

	// クリアしたら床の当たり判定を消す
	field_.ChangeFloorFlag(!GetIsClear());
}

bool StageManager::NextStage() {
	currentStageLevel++;	// 次のステージへ
	// ステージは10までなので11まで到達したらクリア
	if (currentStageLevel > 10) {
		return false;	// 次のステージへいけなかったのでfalseを返す
	}
	Init();	// 初期化
	return true;
}

float StageManager::GetDropLevelBorder() {
	return oreManager_.GetDropLevelBorder();
}
bool StageManager::GetIsClear() {
	return oreManager_.GetCurrentQuota() <= 0;
}