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


private: // ** メンバ変数 ** //

	// 鉱石たち
	std::vector<IOre*> ores_;
};