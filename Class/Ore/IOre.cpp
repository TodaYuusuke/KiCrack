#pragma once
#include "IOre.h"
#include "Mask.h"

using namespace LWP::Math;
using namespace LWP::Object;

void IOre::Init(LWP::Math::Vector2 pos) {
	model_.LoadShortPath("stage/ore/Crystal_Big.gltf");
	model_.worldTF.translation.x = pos.x;
	model_.worldTF.translation.y = pos.y;
	model_.worldTF.translation.z = 1.0f;
	model_.worldTF.rotation *= Quaternion::CreateFromAxisAngle(Vector3::UnitX(), -1.57f);
	model_.worldTF.scale = { 0.1f,0.1f, 0.1f };

	collision_.mask.SetBelongFrag(KCMask::Ore());
	collision_.mask.SetHitFrag(KCMask::Parry() | KCMask::Drop());
	Collider::AABB& aabb = collision_.SetBroadShape(Collider::AABB());
	aabb.Create(model_);
};
