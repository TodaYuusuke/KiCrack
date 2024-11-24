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
	virtual void Init(LWP::Math::Vector2 pos, int* quota);
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// この鉱石を破壊したときのスコアを設定する
	/// </summary>
	/// <param name="s"></param>
	void SetScore(int s) { score_ = s; }

	/// <summary>
	/// ワールド座標を返す関数
	/// </summary>
	/// <returns>ワールド座標</returns>
	LWP::Math::Vector3 GetWorldPosition() { return spawnPointModel_.worldTF.GetWorldPosition(); }

protected: // ** メンバ変数 ** //

	// 鉱石が生える地点を示すモデル
	LWP::Resource::RigidModel spawnPointModel_;
	// 鉱石自体のモデル
	LWP::Resource::RigidModel MaxHPModel_;
	LWP::Resource::RigidModel LowHPModel_;
	// コライダー
	LWP::Object::Collision collision_;

	/// <summary>
	/// 鉱石のパラメーター
	/// </summary>
	struct Parameter {
		float kCollisionScale = 1.0f;	// 当たり判定スケール
		int kMaxHP = 2;			// 最大体力
		float kRespawnTime = 3.0f;		// リポップ時間

		float kMoveSpeed = 0.0f;		// 移動速度
	}parameter_;

	// 体力
	int hp_ = parameter_.kMaxHP;
	// リスポーンまでの時間
	float respawnTime_ = 0.0f;

	// この鉱石のスコア
	int score_ = 1;
};