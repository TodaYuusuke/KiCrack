#pragma once
#include "IOre.h"
#include "Mask.h"

#include "Player/Player.h"

using namespace LWP::Math;
using namespace LWP::Object;
using namespace LWP::Utility;
using namespace LWP::Info;

void IOre::Init(LWP::Math::Vector2 pos) {
	spawnPointModel_.LoadShortPath("stage/ore/Block_Crystal.gltf");
	spawnPointModel_.worldTF.translation.x = pos.x;
	spawnPointModel_.worldTF.translation.y = pos.y;
	spawnPointModel_.worldTF.translation.z = 1.0f;
	spawnPointModel_.worldTF.rotation *= Quaternion::CreateFromAxisAngle(Vector3::UnitX(), -1.57f);
	spawnPointModel_.worldTF.scale = { 0.3f,0.3f, 0.3f };
	spawnPointModel_.materials["Atlas"].color = Color(0x7D7D7DFF);

	MaxHPModel_.LoadShortPath("stage/ore/Crystal_Big.gltf");
	MaxHPModel_.worldTF.Parent(&spawnPointModel_.worldTF);
	MaxHPModel_.worldTF.translation.y = 0.9f;
	MaxHPModel_.worldTF.scale = { 0.3f,0.3f, 0.3f };
	LowHPModel_.LoadShortPath("stage/ore/Crystal_Small.gltf");
	LowHPModel_.worldTF.Parent(&MaxHPModel_.worldTF);

	// 鉱石に追従するカメラ
	collision_.SetFollowTarget(&MaxHPModel_.worldTF);
	collision_.mask.SetBelongFrag(KCMask::Ore());
	collision_.mask.SetHitFrag(KCMask::Parry() | KCMask::Drop());
	collision_.enterLambda = [this](Collision* c) {
		// パリィ所属のコライダーにヒットした場合
		if (c->mask.GetBelongFrag() & KCMask::Parry()) {
			// 体力を攻撃力分減らす
			hp_ -= Player::kParryPower;
		}
		// ドロップ所属のコライダーにヒットした場合
		else if (c->mask.GetBelongFrag() & KCMask::Drop()) {
			// 体力を攻撃力分減らす
			hp_ -= Player::kDropPower;
		}

		// 体力が1になった場合 -> モデルを変更
		if (hp_ == 1) {
			MaxHPModel_.isActive = false;
			LowHPModel_.isActive = true;
		}
		// 体力が0以下になった場合 -> モデル消失
		else if (hp_ <= 0) {
			MaxHPModel_.isActive = false;
			LowHPModel_.isActive = false;
			collision_.isActive = false;
			respawnTime_ = parameter_.kRespawnTime;	// リスポーンタイマー初期化
		}
	};
	Collider::AABB& aabb = collision_.SetBroadShape(Collider::AABB());
	aabb.min = { -0.25f, -0.25f, -0.56f };
	aabb.max = { 0.25f, 0.25f, 0.0f };
	
};

void IOre::Update() {
	// 体力が0になっていたらリスポーンタイマー更新
	if (hp_ <= 0) {
		respawnTime_ -= GetDeltaTimeF();
		// 残り0秒以下になったらリスポーン
		if (respawnTime_ <= 0.0f) {
			hp_ = parameter_.kMaxHP;
			// モデルとコライダーをtrueに
			MaxHPModel_.isActive = true;
			collision_.isActive = true;
		}
	}
}
