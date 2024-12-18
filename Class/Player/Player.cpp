#include "Player.h"
#include "Mask.h"
#include "Ore/OreManager.h"

using namespace LWP::Input;
using namespace LWP::Object;
using namespace LWP::Math;
using namespace LWP::Info;
using namespace LWP::Resource;
using namespace LWP::Utility;

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
	aabb1.min = parameter_.kParrySizeMin;
	aabb1.max = parameter_.kParrySizeMax;
	parryCollision_.mask.SetBelongFrag(KCMask::Parry());
	parryCollision_.mask.SetHitFrag(KCMask::Ore());
	parryCollision_.SetFollowTarget(&model_.worldTF);
	parryCollision_.enterLambda = [this](Collision* c) {
		Jump();	// 鉱石にヒットしていた場合 -> ジャンプ処理を行う
		statePattern_.request = State::Jump;
		parryCollision_.isActive = false;
	};
	parryCollision_.isActive = false;	// コライダーを無効化

	dropAABB_ = &dropCollision_.SetBroadShape(Collider::AABB());
	dropAABB_->min = parameter_.kDropSizeMin;
	dropAABB_->max = parameter_.kDropSizeMax;
	dropCollision_.mask.SetBelongFrag(KCMask::Drop());
	dropCollision_.mask.SetHitFrag(KCMask::Ore());
	dropCollision_.SetFollowTarget(&model_.worldTF);
	dropCollision_.enterLambda = [this](Collision* c) {
		c;	// いったん何もしない
		// ヒットストップ発動
		hitStop_ = parameter_.kHitStopTime;
	};
	dropCollision_.isActive = false;	// コライダーを無効化

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

	// パーティクルの初期設定
	dropParticle_.model.LoadCube();
	dropParticle_.model.worldTF.scale = { 0.09f,0.09f,0.09f };
	dropParticle_.model.materials["Material"].color = Color(255, 100, 0, 255);
	dropParticle_.fallSpeed = -parameter_.kDropSpeed + 0.05f;
	dropParticle_.hitStop = &hitStop_;

#pragma region スプライト
	// ドロップレベル表示UI
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			// テクスチャ読み込み
			if (i == 0) { sprite_[i][j].material.texture = LoadTexture("UI/DropLvBar/DropLv0/DropLvBar.png"); }
			else { sprite_[i][j].material.texture = LoadTexture("UI/DropLvBar/DropLv" + std::to_string(i) + "/DropLvBar0" + std::to_string(j) + ".png"); }
			// 位置調整
			sprite_[i][j].worldTF.translation = { 1465.0f, 600.0f, 0.0f };
			sprite_[i][j].isActive = false;
		}
	}

	// 操作UIの初期化
	moveStrSprite_.material.texture = LoadTexture("Tutorial/Move_string.png");
	jumpStrSprite_.material.texture = LoadTexture("Tutorial/Jump_string.png");
	dropStrSprite_.material.texture = LoadTexture("Tutorial/Drop_string.png");
	moveButtonASprite_[0].material.texture = LoadTexture("Tutorial/Button/ButtonA.png");
	moveButtonASprite_[1].material.texture = LoadTexture("Tutorial/Button/ButtonA_Pressed.png");
	moveButtonDSprite_[0].material.texture = LoadTexture("Tutorial/Button/ButtonD.png");
	moveButtonDSprite_[1].material.texture = LoadTexture("Tutorial/Button/ButtonD_Pressed.png");
	spaceButtonSprite_[0].material.texture = LoadTexture("Tutorial/Button/ButtonSpace.png");
	spaceButtonSprite_[1].material.texture = LoadTexture("Tutorial/Button/ButtonSpace_Pressed.png");
	moveStrSprite_.worldTF.translation = { 0.0f,780.0f,1.0f };
	moveStrSprite_.worldTF.scale = { 0.5f,0.5f,1.0f };
	jumpStrSprite_.worldTF.translation = { 0.0f,865.0f,1.0f };
	jumpStrSprite_.worldTF.scale = { 0.5f,0.5f,1.0f };
	dropStrSprite_.worldTF.translation = { 0.0f,913.0f,1.0f };
	dropStrSprite_.worldTF.scale = { 0.5f,0.5f,1.0f };
	moveButtonASprite_[0].worldTF.translation = { 300.0f,800.0f,1.0f };
	moveButtonASprite_[0].worldTF.scale = { 2.0f,2.0f,1.0f };
	moveButtonASprite_[1].worldTF = moveButtonASprite_[0].worldTF;
	moveButtonDSprite_[0].worldTF.translation = { 370.0f,800.0f,1.0f };
	moveButtonDSprite_[0].worldTF.scale = { 2.0f,2.0f,1.0f };
	moveButtonDSprite_[1].worldTF = moveButtonDSprite_[0].worldTF;
	spaceButtonSprite_[0].worldTF.translation = { 300.0f,900.0f,1.0f };
	spaceButtonSprite_[0].worldTF.scale = { 2.0f,2.0f,1.0f };
	spaceButtonSprite_[1].worldTF = spaceButtonSprite_[0].worldTF;
#pragma endregion
}
void Player::Update() {
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
		ImGui::DragFloat("kDropEndJumpPower", &parameter_.kDropEndJumpPower, 0.01f);
		ImGui::DragFloat("DropJudgeTime", &parameter_.kDropJudgeTime, 0.01f);
		ImGui::DragFloat("kDropParticleLifeTime", &parameter_.kDropParticleLifeTime, 0.01f);
		ImGui::Text("Other");
		ImGui::DragFloat("CameraOffsetZ", &parameter_.kCameraOffsetZ, 0.01f);
		ImGui::DragFloat("kCameraMinBorderY", &parameter_.kCameraMinBorderY, 0.01f);
		ImGui::DragFloat("kCameraDistance", &parameter_.kCameraDistance, 0.01f);
		ImGui::DragFloat("kHitStopTime", &parameter_.kHitStopTime, 0.01f);
		ImGui::DragFloat("FieldBorder", &parameter_.kFieldBorder, 0.01f);
		ImGui::TreePop();
	}
	ImGui::Text("VelocityY : %f", velocityY_);
	ImGui::Text("ParryTime : %f", parryTime_);
	ImGui::Text("DropInputTime : %f", dropInputTime_);
	ImGui::End();
#endif

	// ヒットストップ中は早期リターン
	hitStop_ -= GetDeltaTimeF();
	if (hitStop_ > 0.0f) {return; }
	else { hitStop_ = 0.0f; }

	statePattern_.Update();
	velocityY_ = std::max<float>(velocityY_, parameter_.kFallMaxSpeed);	// ある程度以上の落下速度にならないように設定
	model_.worldTF.translation.y += velocityY_;
	CameraMove();
	DropLevelUpdate();

	// 操作UIのフラグ操作
	bool b = Keyboard::GetPress(DIK_A);
	moveButtonASprite_[0].isActive = !b;
	moveButtonASprite_[1].isActive = b;
	b = Keyboard::GetPress(DIK_D);
	moveButtonDSprite_[0].isActive = !b;
	moveButtonDSprite_[1].isActive = b;
	b = Keyboard::GetPress(DIK_SPACE);
	spaceButtonSprite_[0].isActive = !b;
	spaceButtonSprite_[1].isActive = b;
}

void Player::StageStart() {
	model_.worldTF.translation.y += 25.0f + dropLevelMaxHeight_;
}

bool Player::GetNextStageFlag() {
	// ある程度下層にたどり着いたらならクリア判定を出す
	return model_.worldTF.translation.y < -15.0f;
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
	// モデルの向きを合わせる
	model_.worldTF.rotation = Quaternion::CreateFromAxisAngle(Vector3::UnitY(), 1.57f * x);

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

	// ステージクリアしている場合はこの処理を行わない
	if (!stageClearFlag_) {
		// 地面の下を貫通しないようにカメラを調整
		cameraPtr_->worldTF.translation.y = std::max<float>(cameraPtr_->worldTF.translation.y, parameter_.kCameraMinBorderY);
	}
}
void Player::DropLevelUpdate() {
	// 落下中はレベルを更新しない
	if (statePattern_.GetCurrentBehavior() != State::Dropping) {
		// 落下攻撃のレベルを設定
		int pre = dropLevel_;
		dropLevel_ = OreManager::GetHeightLevel(model_.worldTF.translation.y, dropLevelMaxHeight_) - 1;
		// 値が変わったときの処理
		if (pre != dropLevel_) {
			// フラグ初期化
			for (int i = 0; i < 4; i++) { sprite_[pre][i].isActive = false; }
		}
	}

	// フラグを初期化
	for (int i = 0; i < 4; i++) { sprite_[dropLevel_][i].isActive = false; }
	spriteT_ = std::fmod(spriteT_ + GetDeltaTimeF(), 2.0f);	// アニメーションタイムを更新
	// 適切なUIのフラグをtrueに
	int index = static_cast<int>((spriteT_ - 1.8f) / 0.05f);
	if (index < 0) { index = 0; }
	sprite_[dropLevel_][index].isActive = true;
}
bool Player::GroundBorderCheck() {
	// もしノルマクリアによって床の判定が消えた場合 -> チェックしない
	if (stageClearFlag_) { return false; }

	if (model_.worldTF.translation.y < 0.0f) {
		model_.worldTF.translation.y = 0.0f;
		velocityY_ = 0.0f;
		if (statePattern_.request == std::nullopt) {	// リクエストがなにもなければIdleにする
			statePattern_.request = State::Idle;
		}
		return true;
	}
	return false;
}


#pragma region 各状態初期化と更新
void Player::InitIdle(const State& pre) {
	anim_.Play("idle", true);
	parryCollision_.isActive = false;
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
	GroundBorderCheck();
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
	GroundBorderCheck();
}
void Player::InitFalling(const State& pre) {
	anim_.Play("falling");
}
void Player::UpdateFalling(std::optional<State>& req, const State& pre) {
	Move();
	velocityY_ -= parameter_.kGravity;	// 重力を加算
	if (Parry()) { req = State::Parry; }
	if (Drop()) { req = State::DropStart; }
	GroundBorderCheck();
}
void Player::InitDropStart(const State& pre) {
	anim_.Play("drop");
	velocityY_ *= 0.2f;	// 速度を急激に落とす
}
void Player::UpdateDropStart(std::optional<State>& req, const State& pre) {
	// アニメーションを再生し終わったら開始
	if (!anim_.GetPlaying("drop")) {
		req = State::Dropping;
		dropAABB_->min = parameter_.kDropSizeMin;
		dropAABB_->max = parameter_.kDropSizeMax;
		dropParticle_.lifeTime = parameter_.kDropParticleLifeTime;	// パーティクルの寿命を設定
		// レベル0以上ならサイズに倍率をかける
		if (dropLevel_ > 0) {
			dropAABB_->min.x *= dropLevel_ * parameter_.kDropLevelMultiply;
			dropAABB_->max.x *= dropLevel_ * parameter_.kDropLevelMultiply;
			dropParticle_.lifeTime *= dropLevel_ * parameter_.kDropLevelMultiply;
		}
	}
	GroundBorderCheck();
}
void Player::InitDropping(const State& pre) {
	// 落下速度固定
	velocityY_ = -parameter_.kDropSpeed;
	dropCollision_.isActive = true;	// コライダーを有効化
}
void Player::UpdateDropping(std::optional<State>& req, const State& pre) {
	// パーティクルを生成
	Vector3 p = model_.worldTF.GetWorldPosition();
	p.y -= 0.1f;
	dropParticle_.Add(4, p);

	// もし地面についたら跳ねる
   	if (GroundBorderCheck()) {
		model_.worldTF.translation.y = 0.0f;
		req = State::Jump;
		dropCollision_.isActive = false;	// コライダーを無効化
	}
}
#pragma endregion
