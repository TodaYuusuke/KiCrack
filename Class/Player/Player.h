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

	/// <summary>
	/// ステージ開始時に呼び出される処理
	/// </summary>
	void StageStart();

	/// <summary>
	/// 落下攻撃のレベルボーダーを設定
	/// </summary>
	/// <param name="y"></param>
	void SetDropLevelBorder(float y) { dropLevelMaxHeight_ = y; }
	/// <summary>
	/// ステージクリア判定のフラグをセット
	/// </summary>
	/// <param name="b"></param>
	void SetStageClearFlag(bool b) { stageClearFlag_ = b; }

	/// <summary>
	/// 次のステージへ行くかのフラグを返す（ある程度0より下の座標についた場合に次のステージへ）
	/// </summary>
	bool GetNextStageFlag();


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
		float kFallMaxSpeed = -0.33f;	// 自然落下の速度制限

		float kParryJumpPower = kJumpPower * 1.25f;	// パリィジャンプ力
		LWP::Math::Vector3 kParrySizeMin = { -0.3f, -0.5f, -0.5f };	// パリィの当たり判定サイズ
		LWP::Math::Vector3 kParrySizeMax = { 0.3f, 0.15f, 0.5f };
		float kParryReceptionTime = 0.3f;		// パリィの判定時間

		float kDropSpeed = 0.3f;		// ドロップ速度
		LWP::Math::Vector3 kDropSizeMin = kParrySizeMin;	// ドロップの当たり判定サイズ
		LWP::Math::Vector3 kDropSizeMax = kParrySizeMax;
		float kDropLevelMultiply = 2.3f;	// ドロップ攻撃のレベルごとの倍率
		float kDropEndJumpPower = kJumpPower * 0.4f;	// ドロップ終了時のジャンプ力
		float kDropJudgeTime = 0.4f;	// 長押し判定に必要な秒数

		float kCameraOffsetZ = -25.0f;	// カメラの距離
		float kCameraMinBorderY = 3.5f;	// カメラの最低Y座標
		float kCameraDistance = 0.1f;	// カメラとプレイヤーの離れてはいけない距離
		float kHitStopTime = 0.03f;	// ヒットストップ時間
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
		DropStart,	// 落下攻撃開始
		Dropping,	// 落下攻撃中
		Count	// サイズカウント
	};
	LWP::Utility::StatePattern<State, static_cast<int>(State::Count)> statePattern_;
	// 状態をintに変換する関数
	int GetInt(const State& s) { return static_cast<int>(s); }


	// Y軸の速度
	float velocityY_ = 0.0f;
	// ヒットストップ時間
	float hitStop_ = 0.0f;
	// 床のコライダーフラグ
	bool stageClearFlag_ = false;

	// パリィの残り時間
	float parryTime_ = 0.0f;
	// 落下攻撃までの入力時間
	float dropInputTime_ = 0.0f;
	// 落下攻撃のレベル判定用の高さ
	float dropLevelMaxHeight_ = 0.0f;
	// 落下攻撃の当たり判定のポインタ
	LWP::Object::Collider::AABB* dropAABB_;

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
	/// ドロップ処理（長押しカウント）
	/// </summary>
	bool Drop();

	/// <summary>
	/// カメラを追従させる
	/// </summary>
	void CameraMove();

	/// <summary>
	/// 地面を貫通しないかチェックする関数
	/// </summary>
	bool GroundBorderCheck();


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
	void InitDropStart(const State&);
	void UpdateDropStart(std::optional<State>&, const State&);
	void InitDropping(const State&);
	void UpdateDropping(std::optional<State>&, const State&);
#pragma endregion


public: // ** 外部から参照したいパラメーターを定義 ** //

	// パリィ攻撃力
	static const int kParryPower = 1;
	// ドロップ攻撃力
	static const int kDropPower = 2;
};