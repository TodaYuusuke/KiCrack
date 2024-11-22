#include "OreManager.h"

using namespace LWP::Math;
using namespace LWP::Utility;

void OreManager::Init(int level) {
	const float kStageWidth = 950.0f;	// リメイク元のステージの幅
	const float kStageWidthHalf = kStageWidth / 2.0f;
	const float kStageSizeMultiply = 5.0f;	// 今回のステージの幅
	const float kStageHeightReducing = 100.0f;	// リメイク元のステージからの縮小率
	const float kStageHeight = 170.0f;	// リメイク元のステージの高さ

	// 指定されたレベルのCSVを読み込む
	std::ifstream ifs("resources/csv/stage/Stage" + std::to_string(level) + ".csv");
	std::string line;
	while (std::getline(ifs, line)) {
		std::vector<std::string> strvec = Split(line, ',');
		Vector2 pos;
		pos.x = std::stof(strvec[0]);
		pos.y = std::stof(strvec[1]);
		// リメイク元のCSVをそのまま使用するためにここで座標変換を行う
		pos.x = (pos.x - kStageWidthHalf) / (kStageWidthHalf / kStageSizeMultiply);
		pos.y = (pos.y - kStageHeight) / kStageHeightReducing;

		ores_.push_back(new Normal(pos));
	}

	//ores_.push_back(new Normal({ -1.0f, 1.0f }));
	//ores_.push_back(new Normal({ -2.0f, 1.0f }));
	//ores_.push_back(new Normal({ -1.5f, 2.0f }));
	//ores_.push_back(new Normal({ 1.0f, 1.0f }));
	//ores_.push_back(new Normal({ 2.0f, 1.0f }));
	//ores_.push_back(new Normal({ 1.5f, 2.0f }));
}
void OreManager::Update() {

};

float OreManager::GetDropLevelBorder() {
	float resultY = 0.0f;
	for (IOre* o : ores_) {
		Vector3 pos = o->GetWorldPosition();
		resultY = std::max<float>(resultY, pos.y);	// 最大値を返す
	}
	
	return resultY;
}