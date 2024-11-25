#pragma once
#include <Config.h>

namespace KCMask {

    //** マスクフラグを設定している **//

    // プレイヤー
    static int Player() { return LWP::Config::Collider::MaskLayer::Player; }
    // 鉱石
    static int Ore() { return LWP::Config::Collider::MaskLayer::Enemy; }
    // パリィ攻撃
    static int Parry() { return LWP::Config::Collider::MaskLayer::Bullet; }
    // ドロップ攻撃
    static int Drop() { return LWP::Config::Collider::MaskLayer::Layer5; }
    // パーティクル
    static int Particle() { return LWP::Config::Collider::MaskLayer::Particle; }
    // 壁
    static int Wall() { return LWP::Config::Collider::MaskLayer::Layer6; }
    // 奥の壁
    static int BackWall() { return LWP::Config::Collider::MaskLayer::Layer7; }
    // 地面
    static int Ground() { return LWP::Config::Collider::MaskLayer::Ground; }
    static int ALL() { return LWP::Config::Collider::MaskLayer::ALL; }

}