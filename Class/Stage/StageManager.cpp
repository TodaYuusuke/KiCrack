#include "StageManager.h"

using namespace LWP::Object;

void StageManager::Init(Camera* camera) {
	field_.Init(camera);
	oreManager_.Init(currentStageLevel);
	oreManager_.SetCamera(camera);
}
void StageManager::Update() {
	oreManager_.Update();
	field_.Update(oreManager_.GetCurrentQuota());

	// クリアしたら床の当たり判定を消す
	field_.ChangeFloorFlag(!GetIsClear());
}

bool StageManager::NextStage() {
	currentStageLevel++;	// 次のステージへ
	// ステージは10までなので11まで到達したらクリア
	if (currentStageLevel > 10) {
		return false;	// 次のステージへいけなかったのでfalseを返す
	}
	oreManager_.Init(currentStageLevel);	// 鉱石初期化
	return true;
}

float StageManager::GetDropLevelBorder() {
	return oreManager_.GetDropLevelBorder();
}
bool StageManager::GetIsClear() {
	return oreManager_.GetCurrentQuota() <= 0;
}