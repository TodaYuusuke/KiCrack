#include "OreParticle.h"

#include "Mask.h"

using namespace LWP::Info;
using namespace LWP::Math;
using namespace LWP::Object;
using namespace LWP::Utility;

void OreParticle::Generate(Data& newData) {
	newData.m.materials = model.materials;	// マテリアルがコピーされていない不具合を修正

	// 回転をランダムに
	float radian = static_cast<float>(GenerateRandamNum<int>(0, 614)) / 100.0f;
	newData.m.worldTF.rotation *=
		Quaternion::CreateFromAxisAngle(Vector3::UnitX(), radian) *
		Quaternion::CreateFromAxisAngle(Vector3::UnitY(), radian) *
		Quaternion::CreateFromAxisAngle(Vector3::UnitZ(), radian);

	// 速度ベクトルを生成
	newData.velocity.x = static_cast<float>(GenerateRandamNum<int>(-100, 100)) / 100.0f;
	newData.velocity.y = static_cast<float>(GenerateRandamNum<int>(-100, 100)) / 100.0f;
	newData.velocity.z = static_cast<float>(GenerateRandamNum<int>(-100, 100)) / 100.0f;
	// 勢いをランダムに
	float multiply = static_cast<float>(GenerateRandamNum<int>(100, 1500)) / 10000.0f;
	newData.velocity = newData.velocity.Normalize() * multiply;

	// 当たり判定実装
	Collision* c = new Collision();
	c->SetFollowTarget(&newData.m.worldTF);
	c->mask.SetBelongFrag(KCMask::Particle());
	c->mask.SetHitFrag(KCMask::Wall() | KCMask::BackWall() | KCMask::Ground());
	c->enterLambda = [&newData](Collision* c) {
		// 壁にヒットした場合
		if (c->mask.GetBelongFrag() & KCMask::Wall()) {
			newData.velocity.x *= -0.5f;
		}
		// 奥の壁にヒットした場合
		else if (c->mask.GetBelongFrag() & KCMask::BackWall()) {
			newData.velocity.z *= -0.5f;
		}
		// 床にヒットした場合
		else if (c->mask.GetBelongFrag() & KCMask::Ground()) {
			newData.velocity.y *= -0.4f;
		}
	};
	newData.collider = c;	// データに登録
}

bool OreParticle::UpdateParticle(Data& data) {
	data.velocity.y += -9.8f / 1200.0f;	// 重力を加算
	data.m.worldTF.translation += data.velocity;    // 速度ベクトルを加算
	data.m.worldTF.rotation *=
		Quaternion::CreateFromAxisAngle(Vector3::UnitX(), data.velocity.x) *
		Quaternion::CreateFromAxisAngle(Vector3::UnitY(), data.velocity.y) *
		Quaternion::CreateFromAxisAngle(Vector3::UnitZ(), data.velocity.z);

	// 経過フレーム加算
	data.elapsedTime += GetDeltaTimeF();
	if (data.elapsedTime > 4.0f) { data.elapsedTime = 4.0f; }

	data.m.worldTF.scale = LWP::Utility::Interp::Lerp(model.worldTF.scale, { 0.0f,0.0f,0.0f }, LWP::Utility::Easing::InCubic(data.elapsedTime / 4.0f));
	// 4秒かけて消滅アニメーションを完了したとき消滅する
	if (data.elapsedTime >= 4.0f) {
		return true;
	}

	return false;
}
