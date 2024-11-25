#include "Title.h"
#include "GameScene.h"
#include "NullScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;
using namespace LWP::Info;

// 初期化
void Title::Initialize() {
	player_.Init(&mainCamera);
	tutorialManager_.Init(&mainCamera, &player_);
	player_.SetDropLevelBorder(10000.0f);	// 落下攻撃レベルボーダー設定

	fadeManager_.Init();
}

// 更新
void Title::Update() {
	player_.Update();
	tutorialManager_.Update();

	// ステージをクリアしたら床のコリジョンをオフにする
	player_.SetStageClearFlag(tutorialManager_.GetIsClear());

	// プレイヤーがある程度下層まで到達したら次のステージへ
	if (player_.GetNextStageFlag()) {
		fadeManager_.Out();	// フェードアウト開始
	}	

	// フェードアウトが終わったら次のシーンへ
	fadeManager_.Update();
	if (fadeManager_.GetOut()) {
		nextSceneFunction = []() { return new NullScene([]() { return new GameScene(); }); };
	}
}
