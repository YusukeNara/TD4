#include "Raki_WinAPI.h"

#include <stdio.h>

int Raki_WinAPI::window_width = 1280;
int Raki_WinAPI::window_height = 720;
wchar_t Raki_WinAPI::window_name[] = L"RakiEngine_Project";

HWND Raki_WinAPI::hwnd;
WNDCLASSEX Raki_WinAPI::wndClass;

LRESULT Raki_WinAPI::WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    // メッセージで分岐
    switch (msg) {
    case WM_DESTROY: // ウィンドウが破棄された
        PostQuitMessage(0); // OSに対して、アプリの終了を伝える
        return 0;
    }
	//imgui用の処理
	ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);

    return DefWindowProc(hwnd, msg, wparam, lparam); // 標準の処理を行う
}

void Raki_WinAPI::CreateGameWindow()
{
	// ウィンドウクラスの設定
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = (WNDPROC)WindowProcedure; // ウィンドウプロシージャ
	wndClass.lpszClassName = window_name;            // ウィンドウクラス名
	wndClass.hInstance = GetModuleHandle(nullptr);   // ウィンドウハンドル
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);  // カーソル指定

	RegisterClassEx(&wndClass); // ウィンドウクラスをOSに登録

	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, window_width, window_height };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // 自動でサイズ補正

	// ウィンドウオブジェクトの生成
	hwnd = CreateWindow(wndClass.lpszClassName, // クラス名
		window_name,			// タイトルバーの文字
		WS_OVERLAPPEDWINDOW,	// タイトルバーと境界線があるウィンドウ
		CW_USEDEFAULT,			// 表示X座標（OSに任せる）
		CW_USEDEFAULT,			// 表示Y座標（OSに任せる）
		wrc.right - wrc.left,	// ウィンドウ横幅
		wrc.bottom - wrc.top,	// ウィンドウ縦幅
		nullptr,				// 親ウィンドウハンドル
		nullptr,				// メニューハンドル
		wndClass.hInstance,			// 呼び出しアプリケーションハンドル
		nullptr);				// オプション

	// ウィンドウ表示
	ShowWindow(hwnd, SW_SHOW);


#ifdef  _DEBUG
		printf("If you are seeing this message, then you have successfully displayed the console window. XD\n");
#endif //  _DEBUG
}

void Raki_WinAPI::CreateConsoleWindow()
{

}

void Raki_WinAPI::DeleteGameWindow()
{
#ifdef _DEBUG
	printf("Close console, see you next time.");
#endif // _DEBUG
	// ウィンドウクラスを登録解除
	UnregisterClass(wndClass.lpszClassName, wndClass.hInstance);
}

void Raki_WinAPI::CloseConsoleWindow()
{

}

bool Raki_WinAPI::ProcessMessage()
{
	MSG msg{};	// メッセージ

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // メッセージがある？
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // ウィンドウプロシージャにメッセージを送る
	}

	if (msg.message == WM_QUIT) // 終了メッセージが来たらループを抜ける
	{
		return true;
	}

	return false;
}
