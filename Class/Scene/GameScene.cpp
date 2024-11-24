#include "GameScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;
using namespace LWP::Info;

// 初期化
void GameScene::Initialize() {
	player_.Init(&mainCamera);
	stageManager_.Init();
	player_.SetDropLevelBorder(stageManager_.GetDropLevelBorder());	// 落下攻撃レベルボーダー設定
}

// 更新
void GameScene::Update() {
	player_.Update();
	stageManager_.Update();

	// ステージをクリアしたら床のコリジョンをオフにする
	player_.SetStageClearFlag(stageManager_.GetIsClear());

	// プレイヤーがある程度下層まで到達したら次のステージへ
	if (player_.GetNextStageFlag()) {
		// 次のステージへいけなかった場合はゲームクリアなので終了
		if (!stageManager_.NextStage()) {}
		// 初期設定
		player_.SetDropLevelBorder(stageManager_.GetDropLevelBorder());	// 落下攻撃レベルボーダー設定
		player_.StageStart();	// ステージ開始時に必要な処理をまとめた関数を呼び出す
	}
}