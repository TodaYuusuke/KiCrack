#pragma once
#include "Child/Normal.h"
#include "Child/Weak.h"
#include "Child/MoveHN_L.h"
#include "Child/MoveHN_R.h"
#include "Child/MoveVT_D.h"
#include "Child/MoveVT_U.h"

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
	/// 残りのノルマを返す関数
	/// </summary>
	int GetCurrentQuota() { return quota_; }
	
	/// <summary>
	/// 高さと最大の高さからレベルを返す関数
	/// </summary>
	static int GetHeightLevel(float y, float highestY);


private: // ** メンバ変数 ** //

	// 鉱石たち
	std::vector<IOre*> ores_;
	// ノルマ
	int quota_;

	// 鉱石たちの中で最大の高さ
	float highestY_ = 0.0f;


	// ブロックの種類
	enum class OreType {
		Normal,
		Weak,
		Explosive,
		HorizontalMove_LeftStart,
		HorizontalMove_RightStart,
		VerticalMove_DownStart,
		VerticalMove_UpStart,
	};
};