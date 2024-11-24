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
	void Init(LWP::Object::Camera* camera);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 床のフラグ切り替え
	/// </summary>
	/// <param name="level"></param>
	void ChangeFloorFlag(bool b) { floor.isActive = b; }
	/// <summary>
	/// 床のテクスチャ変更
	/// </summary>
	//void ChangeFloorTexture(int level);


private: // ** メンバ定数 ** //

	// 壁のパラメータ
	float kWallOffset_ = 7.5f;
	LWP::Math::Vector3 kWallScale_ = { 2.3f, 10.0f, 1.0f };


private: // ** メンバ変数 ** //
	// カメラのポインタ
	LWP::Object::Camera* cameraPtr_ = nullptr;

	// 太陽
	LWP::Object::DirectionLight sun_;

	// 壁のモデル
	LWP::Resource::RigidModel wall[3];
	LWP::Object::Collision wallCollision_[3];	// 当たり判定
	// 床のモデル
	LWP::Resource::RigidModel floor;
	LWP::Object::Collision floorCollision_;	// 当たり判定
};