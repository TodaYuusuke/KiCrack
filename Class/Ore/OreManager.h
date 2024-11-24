#pragma once
#include "Normal.h"

/// <summary>
/// 鉱石管理クラス
/// </summary>
class OreManager final {
public: // ** メンバ関数 ** //

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(int level);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 落下攻撃用の鉱石最大地点を返す関数
	/// </summary>
	float GetDropLevelBorder() { return highestY_; }
	
	/// <summary>
	/// 高さと最大の高さからレベルを返す関数
	/// </summary>
	static int GetHeightLevel(float y, float highestY);

private: // ** メンバ変数 ** //

	// 鉱石たち
	std::vector<IOre*> ores_;

	// 鉱石たちの中で最大の高さ
	float highestY_ = 0.0f;
};