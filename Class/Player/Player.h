#pragma once
#include <Adapter.h>

/// <summary>
/// プレイヤークラス
/// </summary>
class Player final {
public: // ** メンバ関数 ** //

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(LWP::Object::Camera* camera);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();


private: // ** メンバ変数 ** //
	// カメラのポインタ
	LWP::Object::Camera* cameraPtr_ = nullptr;

	/// <summary>
	/// プレイヤーのパラメーター
	/// </summary>
	struct Parameter {
		float kWalkSpeed = 0.05f;	// 横移動速度
		float kJumpPower = 0.12f;		// 通常ジャンプ力
		float kGravity = 9.8f / 60.0f / 40.0f;			// 重力の強さ
		float kFallMaxSpeed;	// 自然落下の速度制限

		float kParryJumpPower = kJumpPower * 1.3f;	// パリィジャンプ力
		float kParryReceptionTime = 0.3f;		// パリィの判定時間

		float kDropSpeed;		// ドロップ速度
		float kDropJudgeTime = 0.4f;	// 長押し判定に必要な秒数

		float kCameraOffsetZ = -25.0f;	// カメラの距離
		float kCameraMinBorderY = 3.5f;	// カメラの最低Y座標
		float kCameraDistance = 0.1f;	// カメラとプレイヤーの離れてはいけない距離
		float kFieldBorder = 5.0f;	// ステージのボーダー
	}parameter_;

	// モデル
	LWP::Resource::SkinningModel model_;
	// アニメーション
	LWP::Resource::Animation anim_;

	// パリィコライダー
	LWP::Object::Collision parryCollision_;
	// ドロップコライダー
	LWP::Object::Collision dropCollision_;

	// プレイヤーのステート
	enum class State {
		Idle,	// 無操作
		Walk,	// 歩き
		Jump,	// ジャンプ
		Parry,	// パリィ
		Falling,	// 自由落下
		DropAttak,	// 落下攻撃
		Count	// サイズカウント
	};
	LWP::Utility::StatePattern<State, static_cast<int>(State::Count)> statePattern_;
	// 状態をintに変換する関数
	int GetInt(const State& s) { return static_cast<int>(s); }


	// Y軸の速度
	float velocityY_ = 0.0f;
	// パリィの残り時間
	float parryTime_ = 0.0f;

private: // ** プライベートなメンバ関数 ** //

	/// <summary>
	/// 移動処理（操作をした場合trueを返す）
	/// </summary>
	bool Move();
	/// <summary>
	/// ジャンプ処理（操作をした場合trueを返す）
	/// </summary>
	bool Jump();
	/// <summary>
	/// パリィ処理（パリィが成功した場合trueを返す）
	/// </summary>
	bool Parry();

	/// <summary>
	/// カメラを追従させる
	/// </summary>
	void CameraMove();

	/// <summary>
	/// 地面を貫通しないかチェックする関数
	/// </summary>
	void GroundBorderCheck();


#pragma region 各状態初期化と更新
private: // ** 状態別関数 ** //
	void InitIdle(const State&);
	void UpdateIdle(std::optional<State>&, const State&);
	void InitWalk(const State&);
	void UpdateWalk(std::optional<State>&, const State&);
	void InitJump(const State&);
	void UpdateJump(std::optional<State>&, const State&);
	void InitParry(const State&);
	void UpdateParry(std::optional<State>&, const State&);
	void InitFalling(const State&);
	void UpdateFalling(std::optional<State>&, const State&);
	void InitDropAttak(const State&);
	void UpdateDropAttak(std::optional<State>&, const State&);
#pragma endregion


public: // ** 外部から参照したいパラメーターを定義 ** //

	// パリィ攻撃力
	static const int kParryPower = 1;
	// ドロップ攻撃力
	static const int kDropPower = 2;
};