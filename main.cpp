#include <Adapter.h>
#include "Scene/Title.h"
#include "Scene/GameScene.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LWP::System::StartUp(L"キックラック");
	//LWP::Window::ChangeFullScreenMode();	// フルスクリーン
	LWP::System::Run(new GameScene);
	return 0;
}