#include "Field.h"

void Field::Init(LWP::Object::Camera* camera) {
	cameraPtr_ = camera;

	sun_.rotation = { 0.0f, 0.0f, 0.0f };
	sun_.intensity = 2.0f;

	for (int i = 0; i < 3; i++) {
		wall[i].LoadShortPath("stage/Wall.gltf");
	}

	wall[0].worldTF.translation.x = -kWallOffset_;
	wall[1].worldTF.translation.x = kWallOffset_;
	wall[0].worldTF.scale = kWallScale_;
	wall[1].worldTF.scale = kWallScale_;
	wall[2].worldTF.translation.z = 2.0f;
	wall[2].worldTF.scale = { 10.0f, 10.0f, 1.0f };

	floor.LoadShortPath("stage/Wall.gltf");
	floor.worldTF.translation.y = -1.0f;
	floor.worldTF.scale.x = 5.5f;
}

void Field::Update() {
	// カメラと高さを合わせる
	for (int i = 0; i < 3; i++) {
		wall[i].worldTF.translation.y = cameraPtr_->worldTF.GetWorldPosition().y - 3.5f;
	}
}