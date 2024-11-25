#include "Field.h"

#include "Mask.h"

using namespace LWP::Math;
using namespace LWP::Object;
using namespace LWP::Resource;

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

	// ノルマ表示用テクスチャ
	numSprite10_[0].worldTF.translation = { 0.0f,-0.7f,-1.0f };
	numSprite10_[0].worldTF.scale = { 0.8f,0.8f,1.0f };
	numSprite10_[0].name = "Center";
	for (int i = 0; i < 10; i++) {
		numSprite100_[i].material.texture = LoadTexture("UI/Numbers/" + std::to_string(i) + ".png");
		numSprite10_[i].material.texture = LoadTexture("UI/Numbers/" + std::to_string(i) + ".png");
		numSprite1_[i].material.texture = LoadTexture("UI/Numbers/" + std::to_string(i) + ".png");

		numSprite100_[i].worldTF.Parent(&numSprite10_[0].worldTF);
		if (i != 0) { numSprite10_[i].worldTF.Parent(&numSprite10_[0].worldTF); }
		numSprite1_[i].worldTF.Parent(&numSprite10_[0].worldTF);
		numSprite100_[i].worldTF.translation.x = -0.8f;
		numSprite1_[i].worldTF.translation.x = 0.8f;
	}
	infinitySprite_.material.texture = LoadTexture("UI/Numbers/Infinity.png");
	infinitySprite_.worldTF.Parent(&numSprite10_[0].worldTF);
	infinitySprite_.worldTF.scale = { 1.3f,1.3f, 1.3f };
}

void Field::Update(int quota) {
	// カメラと高さを合わせる
	for (int i = 0; i < 3; i++) {
		wall_[i].worldTF.translation.y = cameraPtr_->worldTF.GetWorldPosition().y - 3.5f;
	}

	// フラグ初期化
	for (int i = 0; i < 10; i++) {
		numSprite100_[i].isActive = false;
		numSprite10_[i].isActive = false;
		numSprite1_[i].isActive = false;
	}
	infinitySprite_.isActive = false;

	// スコアを表示
	if (quota < -1000) {
		// ノルマがチュートリアル用のものである場合の特殊な処理
		infinitySprite_.isActive = true;
	}
	else if (quota > 0) {
		numSprite100_[quota / 100].isActive = true;
		numSprite10_[quota % 100 / 10].isActive = true;
		numSprite1_[quota % 100 % 10].isActive = true;
	}
}