#pragma once

/// <summary>
/// プレイヤークラス
/// </summary>
class Player final {
public: // ** メンバ関数 ** //

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();


private: // ** メンバ変数 ** //

	/// <summary>
	/// プレイヤーのパラメーター(csvで定義)
	/// </summary>
	struct Parameter {
		float kParryJumpPower;	// パリィジャンプ力
		float kParryPower;		// パリィ攻撃力

		float kDropSpeed;		// ドロップ速度
		float kDropPower;		// ドロップ攻撃力
		float kDropJudgeTime;	// 長押し判定に必要な秒数

		float kJumpPower;		// 通常ジャンプ力
		float kGravity;			// 重力の強さ
		float kFallMaxSpeed;	// 自然落下の速度制限
	}parameter_;
};