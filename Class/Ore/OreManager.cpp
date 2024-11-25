#include "OreManager.h"

using namespace LWP::Math;
using namespace LWP::Resource;
using namespace LWP::Utility;

OreManager::OreManager() {
	// パーティクル初期設定
	particle_.model.LoadCube();
	particle_.model.worldTF.scale = { 0.05f,0.05f,0.05f };
	particle_.model.materials["Material"].color = Color(82, 158, 255, 255);

	// スプライト
	for (int i = 0; i < 3; i++) {
		sprite3D_[i].material.texture = LoadTexture("UI/LevelUpBorder/LevelUpBorder.png");
		sprite3D_[i].worldTF.translation.z = 1.0f;
		sprite3D_[i].worldTF.scale.x = 15.0f;
		sprite3D_[i].name = "LevelUpBorder" + std::to_string(i);
	}
	quotaArrowSprite_.material.texture = LoadTexture("UI/Quota/Arrow.png");
	quotaArrowSprite_.material.color.A = 127;
	quotaArrowSprite_.anchorPoint = { 0.5f,0.5f };
	quotaArrowSprite_.worldTF.translation = { 960.0f, 970.0f, 1.0f };
	quotaGoSprite_.material.texture = LoadTexture("UI/Quota/GO.png");
	quotaGoSprite_.material.color.A = 127;
	quotaGoSprite_.anchorPoint = { 0.5f,0.5f };
	quotaGoSprite_.worldTF.Parent(&quotaArrowSprite_.worldTF);
	quotaGoSprite_.worldTF.translation = { 0.0f, -60.0f, 1.0f };
	quotaGoSprite_.worldTF.scale = { 1.3f,1.3f,1.0f };
	quotaGoSprite_.isActive = false;
	for (int i = 0; i < 10; i++) {
		numSprite100_[i].material.texture = LoadTexture("UI/Numbers/" + std::to_string(i) + ".png");
		numSprite10_[i].material.texture = LoadTexture("UI/Numbers/" + std::to_string(i) + ".png");
		numSprite1_[i].material.texture = LoadTexture("UI/Numbers/" + std::to_string(i) + ".png");
		numSprite100_[i].material.color.A = 127;
		numSprite10_[i].material.color.A = 127;
		numSprite1_[i].material.color.A = 127;

		numSprite100_[i].anchorPoint = { 0.5f,0.5f };
		numSprite10_[i].anchorPoint = { 0.5f,0.5f };
		numSprite1_[i].anchorPoint = { 0.5f,0.5f };

		numSprite100_[i].worldTF.Parent(&quotaArrowSprite_.worldTF);
		numSprite10_[i].worldTF.Parent(&quotaArrowSprite_.worldTF);
		numSprite1_[i].worldTF.Parent(&quotaArrowSprite_.worldTF);
		numSprite100_[i].worldTF.translation = { -40.0f, -60.0f, 1.0f };
		numSprite10_[i].worldTF.translation = { 0.0f, -60.0f, 1.0f };
		numSprite1_[i].worldTF.translation = { 40.0f, -60.0f, 1.0f };

		float s = 0.7f;
		numSprite100_[i].worldTF.scale = { s,s,s };
		numSprite10_[i].worldTF.scale = { s,s,s };
		numSprite1_[i].worldTF.scale = { s,s,s };
	}
}

OreManager::~OreManager() {
	// 鉱石クリア
	for (IOre* o : ores_) { delete o; }
	ores_.clear();
}

void OreManager::Init(int level) {
	// 鉱石クリア
	for (IOre* o : ores_) { delete o; }
	ores_.clear();

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
		// 最後の行はノルマしか書かれていないので特殊な処理
		if (strvec.size() <= 1) {
			quota_ = std::stof(strvec[0]);
			break;
		}

		Vector2 pos;
		pos.x = std::stof(strvec[0]);
		pos.y = std::stof(strvec[1]);
		// リメイク元のCSVをそのまま使用するためにここで座標変換を行う
		pos.x = (pos.x - kStageWidthHalf) / (kStageWidthHalf / kStageSizeMultiply);
		pos.y = (pos.y - kStageHeight) / kStageHeightReducing;
		// 移動の初期地点をずらすためのt
		float t = std::stof(strvec[3]) / 60.0f;	// 秒に戻す

		// 鉱石の種類決定
		IOre* o = nullptr;
		switch (static_cast<OreType>(std::stoi(strvec[2]))) {
		case OreType::Normal:
			o = new Normal();
			break;
		case OreType::Weak:
			o = new Weak();
			break;
		case OreType::Explosive:
			// リメイク前の未実装部分（CSVの値を書き換えるのが面倒なのでそのまま）
			o = new Normal();
			break;
		case OreType::HorizontalMove_LeftStart:
			o = new MoveHN_L(t);
			break;
		case OreType::HorizontalMove_RightStart:
			o = new MoveHN_R(t);
			break;
		case OreType::VerticalMove_DownStart:
			o = new MoveVT_D(t);
			break;
		case OreType::VerticalMove_UpStart:
			o = new MoveVT_U(t);
			break;
		}

		std::function<void()> f = []() {};

		o->Init(pos, &quota_);
		o->SetParticleFunction(
			[this](Vector3 pos) { particle_.Add(16, pos); },
			[this](Vector3 pos) { particle_.Add(16, pos); }
		);
		ores_.push_back(o);

		// 一番高い位置を求める
		highestY_ = std::max<float>(highestY_, pos.y);	// 最大値を返す
	}
	// チュートリアル用の特殊な処理
	if (level == 0) {
		highestY_ = 10000.0f;
		quotaGoSprite_.material.texture = LoadTexture("UI/Numbers/Infinity.png");
		quotaGoSprite_.worldTF.scale = { 1.0f,1.0f,1.0f };
	}

	// スコアを付与する
	for (IOre* o : ores_) {
		float y = o->GetWorldPosition().y;
		o->SetScore(OreManager::GetHeightLevel(y, highestY_));
	}

	// レベルアップボーダーを配置する
	float unit = highestY_ / 3.0f;
	for (int i = 0; i < 3; i++) {
		sprite3D_[i].worldTF.translation.y = unit * (i + 1);
	}
}
void OreManager::Update() {
	for (IOre* o : ores_) { o->Update(); }

	// フラグ初期化
	for (int i = 0; i < 10; i++) {
		numSprite100_[i].isActive = false;
		numSprite10_[i].isActive = false;
		numSprite1_[i].isActive = false;
	}
	quotaArrowSprite_.isActive = false;
	quotaGoSprite_.isActive = false;

	// カメラがある程度下に行ったら非表示にする
	if (cameraPtr_->worldTF.GetWorldPosition().y <= 3.5f) { return; }

	// アローを縦揺れさせる
	spriteRadian_ += 0.07f;
	if (spriteRadian_ > 6.28f) {
		spriteRadian_ -= 6.28f;
	}
	quotaArrowSprite_.worldTF.translation.y = 970.0f + sinf(spriteRadian_) * 3.0f;
	quotaArrowSprite_.isActive = true;
	// スコアを表示
	if (quota_ > 0) {
		numSprite100_[quota_ / 100].isActive = true;
		numSprite10_[quota_ % 100 / 10].isActive = true;
		numSprite1_[quota_ % 100 % 10].isActive = true;
	}
	// クリア済みなのでGoを表示する
	else {
		quotaGoSprite_.isActive = true;
	}
};

int OreManager::GetHeightLevel(float y, float highestY) {
	// 0除算を回避
	if (y <= 0) { return 1; }
	
	float unit = highestY / 3.0f;
	int level = static_cast<int>((y / unit)) + 1;
	return std::min<int>(level, 4);	// 3レベル以上にはしない
}