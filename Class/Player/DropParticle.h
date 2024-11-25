#pragma once
#include <Adapter.h>

/// <summary>
/// 落下攻撃のパーティクル
/// </summary>
class DropParticle : public LWP::Object::Particle {
public: // ** メンバ変数 ** //

	// 生存時間
	float lifeTime = 0.0f;
	// 落下速度（プレイヤーの落下に遅れないように）
	float fallSpeed = 0.0f;

	// プレイヤーのヒットストップと同期
	float* hitStop = nullptr;

private: // ** 純粋仮想関数のオーバーライド ** //

	/// <summary>
	/// パーティクルを１つ生成する度に呼ばれる関数
	/// </summary>
	/// <param name="newData">生成された実体の参照（mに基準となるmodelは既にコピーされている）</param>
	/// <returns></returns>
	void Generate(Data& newData) override;
	/// <summary>
	/// パーティクルの更新処理
	/// </summary>
	/// <param name="data">更新する実態の参照</param>
	/// <returns></returns>
	bool UpdateParticle(Data& data) override;
};
