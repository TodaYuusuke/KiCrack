#include "Field.h"

#include "Mask.h"

using namespace LWP::Object;

void Field::Init(LWP::Object::Camera* camera) {
	cameraPtr_ = camera;

	sun_.rotation = { 0.0f, 0.0f, 0.0f };
	sun_.intensity = 2.0f;

	wall_[0].worldTF.translation.x = -kWallOffset_;
	wall_[1].worldTF.translation.x = kWallOffset_;
	wall_[2].worldTF.translation.z = 2.0f;
	wall_[0].worldTF.scale = kWallScale_;
	wall_[1].worldTF.scale = kWallScale_;
	wall_[2].worldTF.scale = { 6.0f, 10.0f, 1.0f };

	for (int i = 0; i < 3; i++) {
		wall_[i].LoadShortPath("stage/Wall.gltf");
		wallCollision_[i].SetFollowTarget(&wall_[i].worldTF);
		wallCollision_[i].mask.SetBelongFrag(KCMask::Wall());
		wallCollision_[i].mask.SetHitFrag(KCMask::Particle());
		Collider::AABB& aabb = wallCollision_[i].SetBroadShape(Collider::AABB());
		aabb.Create(wall_[i]);
	}
	wallCollision_[2].mask.SetBelongFrag(KCMask::BackWall());	// 奥の壁だけコライダーを変える

	floor_.LoadShortPath("stage/Wall.gltf");
	floor_.worldTF.translation.y = -1.0f;
	floor_.worldTF.scale.x = 5.5f;
	floorCollision_.SetFollowTarget(&floor_.worldTF);
	floorCollision_.mask.SetBelongFrag(KCMask::Ground());
	floorCollision_.mask.SetHitFrag(KCMask::Particle());
	Collider::AABB& aabb = floorCollision_.SetBroadShape(Collider::AABB());
	aabb.Create(floor_);
}

void Field::Update() {
	// カメラと高さを合わせる
	for (int i = 0; i < 3; i++) {
		wall_[i].worldTF.translation.y = cameraPtr_->worldTF.GetWorldPosition().y - 3.5f;
	}
}