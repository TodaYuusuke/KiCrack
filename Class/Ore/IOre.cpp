#pragma once
#include "IOre.h"
#include "Mask.h"

#include "Player/Player.h"

using namespace LWP::Math;
using namespace LWP::Object;

void IOre::Init(LWP::Math::Vector2 pos) {
	spawnPointModel_.LoadShortPath("stage/ore/Block_Crystal.gltf");
	spawnPointModel_.worldTF.translation.x = pos.x;
	spawnPointModel_.worldTF.translation.y = pos.y;
	spawnPointModel_.worldTF.translation.z = 1.0f;
	spawnPointModel_.worldTF.rotation *= Quaternion::CreateFromAxisAngle(Vector3::UnitX(), -1.57f);
	spawnPointModel_.worldTF.scale = { 0.3f,0.3f, 0.3f };
	model_[1].LoadShortPath("stage/ore/Crystal_Big.gltf");
	model_[1].worldTF.Parent(&spawnPointModel_.worldTF);
	model_[1].worldTF.translation.y = 0.9f;
	model_[1].worldTF.scale = { 0.3f,0.3f, 0.3f };
	model_[0].LoadShortPath("stage/ore/Crystal_Small.gltf");
	model_[0].worldTF = model_[1].worldTF;	// コピー

	// 鉱石に追従するカメラ
	collision_.SetFollowTarget(&model_[1].worldTF);
	collision_.mask.SetBelongFrag(KCMask::Ore());
	collision_.mask.SetHitFrag(KCMask::Parry() | KCMask::Drop());
	collision_.enterLambda = [this](Collision* c) {
		// パリィ所属のコライダーにヒットした場合
		if (c->mask.GetBelongFrag() & KCMask::Parry()) {
			// 体力を攻撃力分減らす
			hp_ -= Player::kParryPower;
		}
		// ドロップ所属のコライダーにヒットした場合
		else if (c->mask.GetBelongFrag() & KCMask::Parry()) {
			// 体力を攻撃力分減らす
			hp_ -= Player::kDropPower;
		}

		// 体力が1になった場合 -> モデルを変更
		if (hp_ == 1) {
			model_.LoadFullPath(parameter_.kLowHealthModelPath_);
		}
		// 体力が0以下になった場合 -> モデル消失
		else if (hp_ <= 0) {
			model_.isActive = false;
			collision_.isActive = false;
		}
	};
	Collider::AABB& aabb = collision_.SetBroadShape(Collider::AABB());
	aabb.min = { -0.25f, -0.25f, -0.56f };
	aabb.max = { 0.25f, 0.25f, 0.0f };
	//aabb.Create(model_);
};
