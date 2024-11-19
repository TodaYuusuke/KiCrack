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
}

// 更新
void GameScene::Update() {
	player_.Update();
	stageManager_.Update();
}