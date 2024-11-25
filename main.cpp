#include <Adapter.h>
#include "Scene/Title.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	LWP::System::StartUp(L"キックラック");
	LWP::Window::ChangeFullScreenMode();	// フルスクリーン
	LWP::System::Run(new Title);
	return 0;
}