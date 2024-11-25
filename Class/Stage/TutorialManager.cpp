#include "TutorialManager.h"

using namespace LWP::Object;
using namespace LWP::Resource;
using namespace LWP::Info;

void TutorialManager::Init(Camera* camera, Player* player) {
	playerPtr_ = player;

	field_.Init(camera);
	oreManager_.Init(0);	// レベル0（チュートリアルステージ読み込み）
	oreManager_.SetCamera(camera);

	// チュートリアル用スプライト
#pragma region テクスチャ読み込み
	backMoniter_.material.texture = LoadTexture("Tutorial/Monitor/Monitor04.png");
	backMoniter_.worldTF.translation = { 0.0f,7.0f,1.0f };
	backMoniter_.worldTF.scale = { 12.0f,7.5f,1.0f };
	backMoniter_.name = "Moniter";

	gage_.material.texture = LoadTexture("Tutorial/Gage.png");
	gage_.vertices[0].position = { 0.0f, 1.0f, 0.0f };
	gage_.vertices[1].position = { 6.3f, 1.0f, 0.0f };
	gage_.vertices[2].position = { 6.3f, 0.0f, 0.0f };
	gage_.vertices[3].position = { 0.0f, 0.0f, 0.0f };
	gage_.worldTF.translation = { -3.13f,5.26, 1.0f };
	gage_.worldTF.scale.y = 0.18f;


	for (int i = 0; i < kMoveSize; i++) {
		move_[i].material.texture = LoadTexture("Tutorial/Move/Tutorial0" + std::to_string(i) + ".png");
		move_[i].worldTF.Parent(&backMoniter_.worldTF);
		move_[i].isActive = false;
	}
	for (int i = 0; i < kJumpSize; i++) {
		jump_[i].material.texture = LoadTexture("Tutorial/Jump/Tutorial0" + std::to_string(i) + ".png");
		jump_[i].worldTF.Parent(&backMoniter_.worldTF);
		jump_[i].isActive = false;
	}
	for (int i = 0; i < kParrySize; i++) {
		parry_[i].material.texture = LoadTexture("Tutorial/Parry/Tutorial0" + std::to_string(i) + ".png");
		parry_[i].worldTF.Parent(&backMoniter_.worldTF);
		parry_[i].isActive = false;
	}
	for (int i = 0; i < kDropSize; i++) {
		drop_[i].material.texture = LoadTexture("Tutorial/Drop/Tutorial" + std::to_string(i / 10) + std::to_string(i % 10) + ".png");
		drop_[i].worldTF.Parent(&backMoniter_.worldTF);
		drop_[i].isActive = false;
	}
#pragma endregion


#pragma region 状態管理の関数登録
	statePattern_.initFunction[GetInt(State::Move)] = [this](const State& pre) { InitMove(pre); };
	statePattern_.updateFunction[GetInt(State::Move)] = [this](std::optional<State>& req, const State& pre) { UpdateMove(req, pre); };
	statePattern_.initFunction[GetInt(State::Jump)] = [this](const State& pre) { InitJump(pre); };
	statePattern_.updateFunction[GetInt(State::Jump)] = [this](std::optional<State>& req, const State& pre) { UpdateJump(req, pre); };
	statePattern_.initFunction[GetInt(State::Parry)] = [this](const State& pre) { InitParry(pre); };
	statePattern_.updateFunction[GetInt(State::Parry)] = [this](std::optional<State>& req, const State& pre) { UpdateParry(req, pre); };
	statePattern_.initFunction[GetInt(State::Drop)] = [this](const State& pre) { InitDrop(pre); };
	statePattern_.updateFunction[GetInt(State::Drop)] = [this](std::optional<State>& req, const State& pre) { UpdateDrop(req, pre); };
#if DEMO
	// 名前を登録しておく
	statePattern_.name[GetInt(State::Move)] = "Move";
	statePattern_.name[GetInt(State::Jump)] = "Jump";
	statePattern_.name[GetInt(State::Parry)] = "Parry";
	statePattern_.name[GetInt(State::Drop)] = "Drop";
#endif
#pragma endregion
}
void TutorialManager::Update() {
	oreManager_.Update();
	field_.Update(oreManager_.GetCurrentQuota());

	statePattern_.Update();
	gage_.worldTF.scale.x = complete_;

	// クリアしたら床の当たり判定を消す
	field_.ChangeFloorFlag(!GetIsClear());

	ImGui::Begin("a");
	statePattern_.DebugGUI();
	ImGui::End();
}

bool TutorialManager::GetIsClear() {
	return (statePattern_.GetCurrentBehavior() == State::Drop) && (oreManager_.GetCurrentQuota() <= 0);
}

#pragma region 各状態初期化と更新
void TutorialManager::InitMove(const State& pre) {
	t_ = 0.0f;
	complete_ = 0.0f;
}
void TutorialManager::UpdateMove(std::optional<State>& req, const State& pre) {
	t_ += GetDeltaTimeF();
	for (int i = 0; i < kMoveSize; i++) { move_[i].isActive = false; }
	move_[static_cast<int>(t_ / 0.5f) % kMoveSize].isActive = true;
	
	if (playerPtr_->GetCurrentState() == Player::State::Walk) {
		complete_ += GetDeltaTimeF();
	}
	if (complete_ > 1.0f) {
		complete_ = 1.0f;
		req = State::Jump;
	}
}
void TutorialManager::InitJump(const State& pre) {
	t_ = 0.0f;
	complete_ = 0.0f;
	for (int i = 0; i < kMoveSize; i++) { move_[i].isActive = false; }
}
void TutorialManager::UpdateJump(std::optional<State>& req, const State& pre) {
	t_ += GetDeltaTimeF();
	for (int i = 0; i < kJumpSize; i++) { jump_[i].isActive = false; }
	jump_[static_cast<int>(t_ / 0.5f) % kJumpSize].isActive = true;

	if (CheckState(Player::State::Jump)) {
		complete_ += 0.34f;
	}
	if (complete_ > 1.0f) {
		complete_ = 1.0f;
		req = State::Parry;
	}
}
void TutorialManager::InitParry(const State& pre) {
	t_ = 0.0f;
	complete_ = 0.0f;
	for (int i = 0; i < kJumpSize; i++) { jump_[i].isActive = false; }
}
void TutorialManager::UpdateParry(std::optional<State>& req, const State& pre) {
	t_ += GetDeltaTimeF();
	for (int i = 0; i < kParrySize; i++) { parry_[i].isActive = false; }
	parry_[static_cast<int>(t_ / 0.5f) % kParrySize].isActive = true;

	if (CheckState(Player::State::Parry)) {
		complete_ += 0.34f;
	}
	if (complete_ > 1.0f) {
		complete_ = 1.0f;
		req = State::Drop;
	}
}
void TutorialManager::InitDrop(const State& pre) {
	t_ = 0.0f;
	complete_ = 0.0f;
	oreManager_.SetQuota(3);	// ノルマを3に
	for (int i = 0; i < kParrySize; i++) { parry_[i].isActive = false; }
}
void TutorialManager::UpdateDrop(std::optional<State>& req, const State& pre) {
	t_ += GetDeltaTimeF();
	for (int i = 0; i < kDropSize; i++) { drop_[i].isActive = false; }
	drop_[static_cast<int>(t_ / 0.5f) % kDropSize].isActive = true;
}
#pragma endregion

bool TutorialManager::CheckState(Player::State s) {
	bool b = (prePlayerState_ != s) && (playerPtr_->GetCurrentState() == s);
	prePlayerState_ = playerPtr_->GetCurrentState();
	return b;
}
