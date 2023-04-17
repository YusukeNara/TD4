#pragma once
#include <Windows.h>
#include <io.h>
#include <fcntl.h>

#include "imgui/imgui_impl_win32.h"

//imgui関数プロトタイプ宣言
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class Raki_WinAPI
{
public:
	//ウィンドウサイズ
	static int window_width; //縦幅
	static int window_height;//横幅
	static wchar_t window_name[];//ウィンドウの名前

public:
	//---------------------メンバ関数--------------------//

	//静的メンバ関数

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd">ウィンドウハンドル</param>
	/// <param name="msg">メッセージ</param>
	/// <param name="wparam">メッセージ1</param>
	/// <param name="lparam">メッセージ2</param>
	/// <returns>ウィンドウ生成の成否</returns>
	static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	//メンバ関数
	
	/// <summary>
	/// ゲームウィンドウの生成
	/// </summary>
	void CreateGameWindow();

	/// <summary>
	/// コンソールウィンドウの表示
	/// </summary>
	void CreateConsoleWindow();

	/// <summary>
	/// ゲームウィンドウの破棄
	/// </summary>
	void DeleteGameWindow();

	/// <summary>
	/// コンソールウィンドウを閉じる
	/// </summary>
	void CloseConsoleWindow();

	/// <summary>
	/// メッセージ処理（終了かどうか）
	/// </summary>
	bool ProcessMessage();

	/// <summary>
	/// ウィンドウハンドル取得
	/// </summary>
	static HWND GetHWND() { return hwnd; }

	static WNDCLASSEX GetWNDCLASSEX() { return wndClass; }

	/// <summary>
	/// ウィンドウクラスのインスタンス取得
	/// </summary>
	HINSTANCE GetInstance() { return wndClass.hInstance; }

private:
	//プライベートメンバ変数
	static HWND hwnd;  //ウィンドウハンドル
	static WNDCLASSEX wndClass;//ウィンドウクラス

};

