#pragma once

/// <summary>
/// 鉱石の基底クラス
/// </summary>
class IOre {
public: // ** メンバ関数 ** //

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;


private: // ** メンバ変数 ** //

	/// <summary>
	/// 鉱石のパラメーター(csvで定義)
	/// </summary>
	struct Parameter {
		float kCollisionScale;	// 当たり判定スケール
		int kHealth;			// 体力
		float kRespawnTime;		// リポップ時間

		float kMoveSpeed;		// 移動速度
	}parameter_;
};