#pragma once
#include "IOre.h"

/// <summary>
/// 通常の鉱石
/// </summary>
class Normal final : public IOre {
public: // ** メンバ関数 ** //

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Normal(LWP::Math::Vector2 pos) { Init(pos); }

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;
};