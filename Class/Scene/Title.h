#pragma once
#include "scene/IScene.h"
#include "FadeManager.h"

#include "Player/Player.h"
#include "Stage/TutorialManager.h"

class Title final
	: public IScene {
public:
	Title() = default;
	~Title() = default;

	//*** 純粋仮想関数の実体宣言 ***//

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;


private: // ** これより先に必要な処理や変数を記述 ** //

	// プレイヤー
	Player player_;
	// ステージ管理クラス
	TutorialManager tutorialManager_;

	// フェードインアウト管理クラス
	FadeManager fadeManager_;
};