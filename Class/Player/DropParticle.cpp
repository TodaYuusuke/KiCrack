#include "DropParticle.h"

#include "Mask.h"

using namespace LWP::Info;
using namespace LWP::Math;
using namespace LWP::Object;
using namespace LWP::Utility;

void DropParticle::Generate(Data& newData) {
	newData.m.materials = model.materials;	// マテリアルがコピーされていない不具合を修正

	// 速度ベクトルを生成
	newData.velocity = {0.1f, fallSpeed, 0.0f};
	if (GenerateRandamNum<int>(0, 1)) {
		newData.velocity.x *= -1.0f;	// 2分の1で反転
	}
}

bool DropParticle::UpdateParticle(Data& data) {
	if (*hitStop > 0.0f) { return false; }	// プレイヤーのヒットストップと同期

	data.m.worldTF.translation += data.velocity;    // 速度ベクトルを加算

	// 経過フレーム加算
	data.elapsedTime += GetDeltaTimeF();
	if (data.elapsedTime > lifeTime) { data.elapsedTime = lifeTime; }

	data.m.worldTF.scale = Interp::Lerp(model.worldTF.scale, { 0.0f,0.0f,0.0f }, Easing::InCubic(data.elapsedTime / lifeTime));
	data.m.materials["Material"].color.G = 100 - (100 * Easing::InCubic(data.elapsedTime / lifeTime));
	// lifeTimeかけて消滅アニメーションを完了したとき消滅する
	if (data.elapsedTime >= lifeTime) {
		return true;
	}

	return false;
}
