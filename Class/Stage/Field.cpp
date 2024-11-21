#include "Field.h"

void Field::Init() {
	sun_.rotation = { 0.0f, 0.0f, 0.0f };
	sun_.intensity = 2.0f;

	wall[0].LoadShortPath("stage/Wall.gltf");
	wall[1].LoadShortPath("stage/Wall.gltf");
	wall[0].worldTF.translation.x = -kWallOffset_;
	wall[1].worldTF.translation.x = kWallOffset_;
	wall[0].worldTF.scale = kWallScale_;
	wall[1].worldTF.scale = kWallScale_;
	wall[2].LoadShortPath("stage/Wall.gltf");
	wall[2].worldTF.translation.z = 2.0f;
	wall[2].worldTF.scale = { 10.0f, 10.0f, 1.0f };

	floor.LoadShortPath("stage/Wall.gltf");
	floor.worldTF.translation.y = -1.0f;
	floor.worldTF.scale.x = 5.5f;
}
