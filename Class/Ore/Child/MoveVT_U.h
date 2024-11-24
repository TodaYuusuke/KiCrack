#pragma once
#include "MoveHN_L.h"

/// <summary>
/// 垂直移動（下からスタート）する鉱石
/// </summary>
class MoveVT_U : public MoveHN_L {
public: // ** メンバ関数 ** //

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="t"></param>
	MoveVT_U(float t) : MoveHN_L(t) {};

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(LWP::Math::Vector2 pos, int* quota) override;
};