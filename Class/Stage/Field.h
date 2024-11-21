#pragma once
#include <Adapter.h>

/// <summary>
/// フィールドクラス
/// </summary>
class Field final {
public: // ** メンバ関数 ** //

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 床のテクスチャ変更
	/// </summary>
	void ChangeFloorTexture(int level);


private: // ** メンバ定数 ** //

	// 壁のパラメータ
	float kWallOffset_ = 7.5f;
	LWP::Math::Vector3 kWallScale_ = { 2.3f, 30.0f, 1.0f };


private: // ** メンバ変数 ** //

	// 太陽
	LWP::Object::DirectionLight sun_;

	// 壁のモデル
	LWP::Resource::RigidModel wall[3];
	// 床のモデル
	LWP::Resource::RigidModel floor;
};