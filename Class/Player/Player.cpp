#include "Player.h"
#include "Mask.h"

using namespace LWP::Input;
using namespace LWP::Object;
using namespace LWP::Math;
using namespace LWP::Info;

void Player::Init(LWP::Object::Camera* camera) {
	// カメラ位置微調整
	cameraPtr_ = camera;
	cameraPtr_->worldTF.translation.y = parameter_.kCameraMinBorderY;
	cameraPtr_->worldTF.translation.z = parameter_.kCameraOffsetZ;

	model_.LoadShortPath("player/Player.gltf");
	model_.worldTF.rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY(), 1.57f);
	model_.worldTF.scale = { 0.02f, 0.02f, 0.02f };
	anim_.LoadFullPath("resources/model/player/Player.gltf", &model_);
	anim_.playbackSpeed = 2.0f;

	// 当たり判定の設定
	Collider::AABB& aabb1 = parryCollision_.SetBroadShape(Collider::AABB());
	aabb1.min = { -0.3f, -0.5f, -0.5f };
	aabb1.max = { 0.3f, 0.15f, 0.5f };
	parryCollision_.mask.SetBelongFrag(KCMask::Parry());
	parryCollision_.mask.SetHitFrag(KCMask::Ore());
	parryCollision_.SetFollowTarget(&model_.worldTF);
	parryCollision_.enterLambda = [this](Collision* c) {
		Jump();	// 鉱石にヒットしていた場合 -> ジャンプ処理を行う
		statePattern_.request = State::Jump;
		parryCollision_.isActive = false;
	};

	Collider::AABB& aabb2 = dropCollision_.SetBroadShape(Collider::AABB());
	aabb2.min = { -0.3f, -0.5f, -0.5f };
	//aabb2.max = { 0.3f, 0.15f, 0.5f };
	dropCollision_.mask.SetBelongFrag(KCMask::Drop());
	dropCollision_.mask.SetHitFrag(KCMask::Ore());
	dropCollision_.SetFollowTarget(&model_.worldTF);
	dropCollision_.enterLambda = [this](Collision* c) {
		c;	// いったん何もしない
	};

#pragma region 状態管理の関数登録
	statePattern_.initFunction[GetInt(State::Idle)] = [this](const State& pre) { InitIdle(pre); };
	statePattern_.updateFunction[GetInt(State::Idle)] = [this](std::optional<State>& req, const State& pre) { UpdateIdle(req, pre); };
	statePattern_.initFunction[GetInt(State::Walk)] = [this](const State& pre) { InitWalk(pre); };
	statePattern_.updateFunction[GetInt(State::Walk)] = [this](std::optional<State>& req, const State& pre) { UpdateWalk(req, pre); };
	statePattern_.initFunction[GetInt(State::Jump)] = [this](const State& pre) { InitJump(pre); };
	statePattern_.updateFunction[GetInt(State::Jump)] = [this](std::optional<State>& req, const State& pre) { UpdateJump(req, pre); };
	statePattern_.initFunction[GetInt(State::Parry)] = [this](const State& pre) { InitParry(pre); };
	statePattern_.updateFunction[GetInt(State::Parry)] = [this](std::optional<State>& req, const State& pre) { UpdateParry(req, pre); };
	statePattern_.initFunction[GetInt(State::Falling)] = [this](const State& pre) { InitFalling(pre); };
	statePattern_.updateFunction[GetInt(State::Falling)] = [this](std::optional<State>& req, const State& pre) { UpdateFalling(req, pre); };
	statePattern_.initFunction[GetInt(State::DropStart)] = [this](const State& pre) { InitDropStart(pre); };
	statePattern_.updateFunction[GetInt(State::DropStart)] = [this](std::optional<State>& req, const State& pre) { UpdateDropStart(req, pre); };
	statePattern_.initFunction[GetInt(State::Dropping)] = [this](const State& pre) { InitDropping(pre); };
	statePattern_.updateFunction[GetInt(State::Dropping)] = [this](std::optional<State>& req, const State& pre) { UpdateDropping(req, pre); };
#if DEMO
	// 名前を登録しておく
	statePattern_.name[GetInt(State::Idle)] = "Idle";
	statePattern_.name[GetInt(State::Walk)] = "Walk";
	statePattern_.name[GetInt(State::Jump)] = "Jump";
	statePattern_.name[GetInt(State::Parry)] = "Parry";
	statePattern_.name[GetInt(State::Falling)] = "Falling";
	statePattern_.name[GetInt(State::DropStart)] = "DropStart";
	statePattern_.name[GetInt(State::Dropping)] = "Dropping";
	
#endif

#pragma endregion
}
void Player::Update() {
	statePattern_.Update();
	model_.worldTF.translation.y += velocityY_;
	GroundBorderCheck();
	CameraMove();

#if DEMO
	ImGui::Begin("Player");
	if (ImGui::TreeNode("Model")) {
		model_.DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Animation")) {
		anim_.DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("State")) {
		statePattern_.DebugGUI();
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Parameter")) {
		ImGui::Text("Common");
		ImGui::DragFloat("WalkSpeed", &parameter_.kWalkSpeed, 0.01f);
		ImGui::DragFloat("JumpPower", &parameter_.kJumpPower, 0.01f);
		ImGui::DragFloat("Gravity", &parameter_.kGravity, 0.01f);
		ImGui::DragFloat("FallMaxSpeed", &parameter_.kFallMaxSpeed, 0.01f);
		ImGui::Text("Parry");
		ImGui::DragFloat("ParryJumpPower", &parameter_.kParryJumpPower, 0.01f);
		ImGui::Text("Drop");
		ImGui::DragFloat("DropSpeed", &parameter_.kDropSpeed, 0.01f);
		ImGui::DragFloat("DropJudgeTime", &parameter_.kDropJudgeTime, 0.01f);
		ImGui::Text("Other");
		ImGui::DragFloat("CameraOffsetZ", &parameter_.kCameraOffsetZ, 0.01f);
		ImGui::DragFloat("kCameraMinBorderY", &parameter_.kCameraMinBorderY, 0.01f);
		ImGui::DragFloat("kCameraDistance", &parameter_.kCameraDistance, 0.01f);
		ImGui::DragFloat("FieldBorder", &parameter_.kFieldBorder, 0.01f);
		ImGui::TreePop();
	}
	ImGui::Text("VelocityY : %f", velocityY_);
	ImGui::Text("ParryTime : %f", parryTime_);
	ImGui::Text("DropInputTime : %f", dropInputTime_);
	ImGui::End();
#endif
}

bool Player::Jump() {
	// スペースキーを押している場合にtrueを返す
	return Keyboard::GetTrigger(DIK_SPACE);
}
bool Player::Move() {
	bool pressD = Keyboard::GetPress(DIK_D);
	bool pressA = Keyboard::GetPress(DIK_A);
	// どちらも押されていない場合早期リターン
	if (!(pressA || pressD)) { return false; }
	float x = 0.0f;	// 移動する向き
	float& posX = model_.worldTF.translation.x;	// モデルの位置

	// キー入力
	if (pressD) { x += 1.0f; }
	if (pressA) { x -= 1.0f; }

	// モデルを移動
	posX += x * parameter_.kWalkSpeed;
	// エリア外に行かないように調整
	posX = std::clamp(posX, -parameter_.kFieldBorder, parameter_.kFieldBorder);
	
	return true;
}
bool Player::Parry() {
	// スペースキーを押している場合にtrueを返す
	return Keyboard::GetTrigger(DIK_SPACE);
}
bool Player::Drop() {
	if (Keyboard::GetPress(DIK_SPACE)) {
		dropInputTime_ += GetDeltaTimeF();	// 長押し秒数加算
		// 長押しに必要な時間以上押した場合にtrueを返す
		if (dropInputTime_ >= parameter_.kDropJudgeTime) {
			dropInputTime_ = 0.0f;
			return true;
		}
	}
	else {
		dropInputTime_ = 0.0f;	// 長押し秒数初期化
	}

	return false;
}

void Player::CameraMove() {
	// カメラとプレイヤーのY軸の差がある程度以上広がらないように調整
	float distance = model_.worldTF.translation.y - cameraPtr_->worldTF.translation.y;
	if(distance > parameter_.kCameraDistance) {
		cameraPtr_->worldTF.translation.y += distance - parameter_.kCameraDistance;
	}
	else if (distance < -parameter_.kCameraDistance) {
		cameraPtr_->worldTF.translation.y += distance + parameter_.kCameraDistance;
	}

	// 地面の下を貫通しないようにカメラを調整
	cameraPtr_->worldTF.translation.y = std::max<float>(cameraPtr_->worldTF.translation.y, parameter_.kCameraMinBorderY);
}
void Player::GroundBorderCheck() {
	if (model_.worldTF.translation.y < 0.0f) {
		model_.worldTF.translation.y = 0.0f;
		velocityY_ = 0.0f;
		statePattern_.request = State::Idle;
	}
}


#pragma region 各状態初期化と更新
void Player::InitIdle(const State& pre) {
	anim_.Play("idle", true);
}
void Player::UpdateIdle(std::optional<State>& req, const State& pre) {
	if (Move()) { req = State::Walk; }
	if (Jump()) { req = State::Jump; }
}
void Player::InitWalk(const State& pre) {
	anim_.Play("walk", true);
}
void Player::UpdateWalk(std::optional<State>& req, const State& pre) {
	if (!Move()) { req = State::Idle; }
	if (Jump()) { req = State::Jump; }
}
void Player::InitJump(const State& pre) {
	anim_.Play("jump");
	switch (pre) {
		// 前がパリィ状態ならジャンプ力変更
		case Player::State::Parry:
			velocityY_ = parameter_.kParryJumpPower;
			break;
		// 前がら落下状態ならジャンプ力変更
		case Player::State::Dropping:
			velocityY_ = parameter_.kDropEndJumpPower;
			break;
		// ジャンプ力を速度に付与
		default:
			velocityY_ = parameter_.kJumpPower;
			break;
	}
}
void Player::UpdateJump(std::optional<State>& req, const State& pre) {
	Move();
	velocityY_ -= parameter_.kGravity;	// 重力を加算
	if (Parry()) { req = State::Parry; }
	if (Drop()) { req = State::DropStart; }
	if (velocityY_ < 0.0f) { req = State::Falling; }
}
void Player::InitParry(const State& pre) {
	anim_.Play("parry");
	parryTime_ = parameter_.kParryReceptionTime;
	parryCollision_.isActive = true;
}
void Player::UpdateParry(std::optional<State>& req, const State& pre) {
	Move();
	parryTime_ -= GetDeltaTimeF();
	velocityY_ -= parameter_.kGravity;	// 重力を加算
	if(parryTime_ < 0.0f) {	// 受付時間が経過したら終了
		req = State::Falling;
		parryCollision_.isActive = false;
	}
	if (Drop()) { req = State::DropStart; }
}
void Player::InitFalling(const State& pre) {
	anim_.Play("falling");
}
void Player::UpdateFalling(std::optional<State>& req, const State& pre) {
	Move();
	velocityY_ -= parameter_.kGravity;	// 重力を加算
	if (Parry()) { req = State::Parry; }
	if (Drop()) { req = State::DropStart; }
}
void Player::InitDropStart(const State& pre) {
	anim_.Play("drop");
}
void Player::UpdateDropStart(std::optional<State>& req, const State& pre) {
	// アニメーションを再生し終わったら開始（今はアニメーションがないのでそのまま開始する）
	if (!anim_.GetPlaying("drop")) {
		req = State::Dropping;
	}

	dropCollision_.worldTF.scale;
}
void Player::InitDropping(const State& pre) {
	// 落下速度固定
	velocityY_ = -parameter_.kDropSpeed;
}
void Player::UpdateDropping(std::optional<State>& req, const State& pre) {
	// もし地面についたら跳ねる
	if (model_.worldTF.translation.y < 0.0f) {
		model_.worldTF.translation.y = 0.0f;
		req = State::Jump;
	}
}
#pragma endregion
