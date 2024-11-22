#pragma once
#include "Field.h"
#include "Ore/OreManager.h"

/// <summary>
/// ステージ管理クラス
/// </summary>
class StageManager final {
public: // ** メンバ関数 ** //

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 落下攻撃用の鉱石最大地点を返す関数
	/// </summary>
	float GetDropLevelBorder();


private: // ** メンバ変数 ** //

	// 固定されている地形をまとめたクラス
	Field field_;
	// 鉱石管理クラス
	OreManager oreManager_;
};