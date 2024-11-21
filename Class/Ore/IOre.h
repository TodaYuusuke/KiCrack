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

	// 鉱石が生える地点を示すモデル
	LWP::Resource::RigidModel spawnPointModel_;
	// 鉱石自体のモデル
	LWP::Resource::RigidModel model_[2];
	// コライダー
	LWP::Object::Collision collision_;

	/// <summary>
	/// 鉱石のパラメーター
	/// </summary>
	struct Parameter {
		float kCollisionScale = 1.0f;	// 当たり判定スケール
		int kMaxHealth = 2;			// 最大体力
		float kRespawnTime = 3.0f;		// リポップ時間

		float kMoveSpeed = 0.0f;		// 移動速度
	}parameter_;

	// 体力
	int hp_ = parameter_.kMaxHealth;
};