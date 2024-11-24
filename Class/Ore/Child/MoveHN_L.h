#pragma once
#include "../IOre.h"

/// <summary>
/// 水平移動（左からスタート）する鉱石
/// </summary>
class MoveHN_L : public IOre {
public: // ** メンバ関数 ** //

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="t"></param>
	MoveHN_L(float t) { motion_.Start(true, t); }

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(LWP::Math::Vector2 pos, int* quota) override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;


protected: // ** メンバ変数 ** //

	// 移動量
	float kMove = 2.526f;
	// 移動にかかる秒数
	float kMoveTime = 2.0f;

	// モーション
	LWP::Resource::Motion motion_;


protected: // ** メンバ関数 ** //

	/// <summary>
	/// モーション作成
	/// </summary>
	void CreateMotion(LWP::Math::Vector3 move1, LWP::Math::Vector3 move2);
};