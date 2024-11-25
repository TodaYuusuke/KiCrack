#pragma once
#include "Field.h"
#include "Ore/OreManager.h"
#include "Player/Player.h"

/// <summary>
/// チュートリアル専用のステージ管理クラス
/// </summary>
class TutorialManager final {
public: // ** メンバ関数 ** //

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(LWP::Object::Camera* camera, Player* player);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ステージをクリアしたかどうか返す
	/// </summary>
	/// <returns></returns>
	bool GetIsClear();

private: // ** メンバ変数 ** //
	// プレイヤーのポインタ
	Player* playerPtr_;

	// 固定されている地形をまとめたクラス
	Field field_;
	// 鉱石管理クラス
	OreManager oreManager_;
	
	enum class State {
		Move,
		Jump,
		Parry,
		Drop,
		Count
	};
	LWP::Utility::StatePattern<State, static_cast<int>(State::Count)> statePattern_;
	// 状態をintに変換する関数
	int GetInt(const State& s) { return static_cast<int>(s); }

	// チュートリアル用UI
	LWP::Primitive::Surface backMoniter_;
	LWP::Primitive::Surface gage_;

	// 各種操作
	static const int kMoveSize = 9;
	static const int kJumpSize = 10;
	static const int kParrySize = 9;
	static const int kDropSize = 14;
	LWP::Primitive::Surface move_[kMoveSize];
	LWP::Primitive::Surface jump_[kJumpSize];
	LWP::Primitive::Surface parry_[kParrySize];
	LWP::Primitive::Surface drop_[kDropSize];

	// チュートリアルの進捗
	float complete_ = 0.0f;
	// チュートリアル表示のアニメーションタイム
	float t_ = 0.0f;
	// プレイヤーの前回の状態
	Player::State prePlayerState_ = Player::State::Idle;

#pragma region 各状態初期化と更新
private: // ** 状態別関数 ** //
	void InitMove(const State&);
	void UpdateMove(std::optional<State>&, const State&);
	void InitJump(const State&);
	void UpdateJump(std::optional<State>&, const State&);
	void InitParry(const State&);
	void UpdateParry(std::optional<State>&, const State&);
	void InitDrop(const State&);
	void UpdateDrop(std::optional<State>&, const State&);
#pragma endregion

	/// <summary>
	/// プレイヤーのステートが引数と同じになったか判断する関数
	/// </summary>
	/// <param name="s"></param>
	/// <returns></returns>
	bool CheckState(Player::State s);
};