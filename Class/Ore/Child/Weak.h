#pragma once
#include "../IOre.h"

/// <summary>
/// 体力が少ない鉱石
/// </summary>
class Weak final : public IOre {
public: // ** メンバ関数 ** //

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(LWP::Math::Vector2 pos, int* quota) override;


private: // ** メンバ変数 ** //

	/// <summary>
	/// リスポーン時の処理
	/// </summary>
	void Respawn() override;
};