#pragma once
#include <Adapter.h>

/// <summary>
/// 鉱石の基底クラス
/// </summary>
class IOre {
public: // ** メンバ関数 ** //

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init(LWP::Math::Vector2 pos);
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;


protected: // ** メンバ変数 ** //

	// モデル
	LWP::Resource::RigidModel model_;
	// コライダー
	LWP::Object::Collision collision_;

	/// <summary>
	/// 鉱石のパラメーター
	/// </summary>
	struct Parameter {
		float kCollisionScale = 1.0f;	// 当たり判定スケール
		int kHealth = 2;			// 体力
		float kRespawnTime = 3.0f;		// リポップ時間

		float kMoveSpeed = 0.0f;		// 移動速度
	}parameter_;
};