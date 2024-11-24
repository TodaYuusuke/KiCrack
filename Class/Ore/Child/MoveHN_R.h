#pragma once
#include "MoveHN_L.h"

/// <summary>
/// 水平移動（右からスタート）する鉱石
/// </summary>
class MoveHN_R : public MoveHN_L {
public: // ** メンバ関数 ** //

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="t"></param>
	MoveHN_R(float t) : MoveHN_L(t) {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(LWP::Math::Vector2 pos, int* quota) override;
};