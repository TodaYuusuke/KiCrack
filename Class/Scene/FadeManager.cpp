#include "FadeManager.h"
#include "NullScene.h"

using namespace LWP;
using namespace LWP::Input;
using namespace LWP::Primitive;
using namespace LWP::Math;
using namespace LWP::Utility;
using namespace LWP::Object;

FadeManager::~FadeManager() {}

void FadeManager::Init() {
	sprite_.isUI = true;
	sprite_.material.enableLighting = false;
	sprite_.material.color = ColorPattern::BLACK;
	sprite_.worldTF.scale = { 10.0f,5.5f,0.0f };

	startOut = false;
	endIn = false;
	endOut = false;

	// �t�F�[�h�C���͎����ŊJ�n
	time_ = 0.0f;
}
void FadeManager::Update() {
	if (startOut && !endOut) {
		time_ += LWP::Info::GetDeltaTimeF();
		if (time_ > totalTime_) {
			time_ = totalTime_;
			endOut = true;
		}

		// �t�F�[�h�A�E�g����
		sprite_.material.color.A = int(ResultLerp(0.0f, 255.0f, time_ / totalTime_));
	}
	else if (!endIn) {
		time_ += LWP::Info::GetDeltaTimeF();
		if (time_ > totalTime_) {
			time_ = totalTime_;
			endIn = true;
		}

		// �t�F�[�h�C������
		sprite_.material.color.A = int(ResultLerp(255.0f, 0.0f, time_ / totalTime_));
	}
}

void FadeManager::Out() {
	if (startOut) { return; }
	startOut = true;
	time_ = 0.0f;
}