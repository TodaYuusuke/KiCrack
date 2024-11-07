#pragma once

/// <summary>
/// 鉱石の基底クラス
/// </summary>
class IOre {
public: // ** メンバ関数 ** //

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;


private: // ** メンバ変数 ** //


};