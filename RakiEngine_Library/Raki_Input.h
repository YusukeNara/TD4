#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <xinput.h>
#include <wrl.h>
#include <utility>

#include "RVector.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib,"dxguid.lib")


#pragma region InputKeyCode
// キーボードはやらないかも

// マウス入力
#define MOUSE_L			0x00		//マウス左
#define MOUSE_R			0x01		//マウス右
#define MOUSE_CENTER	0x02		//マウス中央

//ゲームパッド識別コード
enum XPAD_INPUT_CODE
{
	XPAD_BUTTON_A,
	XPAD_BUTTON_B,
	XPAD_BUTTON_X,
	XPAD_BUTTON_Y,

	XPAD_BUTTON_CROSS_UP,
	XPAD_BUTTON_CROSS_DOWN,
	XPAD_BUTTON_CROSS_LEFT,
	XPAD_BUTTON_CROSS_RIGHT,

	XPAD_TRIGGER_LB,
	XPAD_TRIGGER_LT,
	XPAD_TRIGGER_RB,
	XPAD_TRIGGER_RT,

	XPAD_BUTTON_LSTICK,
	XPAD_BUTTON_RSTICK,

	XPAD_BUTTON_OPTION_R,
	XPAD_BUTTON_OPTION_L,
};
//xInputスティック傾き方向
enum XPAD_STICK_DIRECTION_CODE
{
	///4方向入力
	XPAD_LSTICK_DIR_UP,
	XPAD_LSTICK_DIR_DOWN,
	XPAD_LSTICK_DIR_LEFT,
	XPAD_LSTICK_DIR_RIGHT,

	///斜め入力
	XPAD_LSTICK_DIR_UR,
	XPAD_LSTICK_DIR_UL,
	XPAD_LSTICK_DIR_DR,
	XPAD_LSTICK_DIR_DL,

	///4方向入力
	XPAD_RSTICK_DIR_UP,
	XPAD_RSTICK_DIR_DOWN,
	XPAD_RSTICK_DIR_LEFT,
	XPAD_RSTICK_DIR_RIGHT,

	///斜め入力
	XPAD_RSTICK_DIR_UR,
	XPAD_RSTICK_DIR_UL,
	XPAD_RSTICK_DIR_DR,
	XPAD_RSTICK_DIR_DL,
};


#pragma endregion InputKeyCode

struct XPAD_STICKTILT_FLAG
{
	bool isRStickTiltUp;
	bool isRStickTiltDown;
	bool isRStickTiltRight;
	bool isRStickTiltLeft;

	bool isRStickTiltLeftUp;
	bool isRStickTiltRightUp;
	bool isRStickTiltLeftDown;
	bool isRStickTiltRightDown;

	bool isLStickTiltUp;
	bool isLStickTiltDown;
	bool isLStickTiltRight;
	bool isLStickTiltLeft;

	bool isLStickTiltLeftUp;
	bool isLStickTiltRightUp;
	bool isLStickTiltLeftDown;
	bool isLStickTiltRightDown;

	void Reset();
};

using namespace Microsoft::WRL;
/// <summary>
/// シングルトンパターン採用、キー入力クラス
/// </summary>
class Input final
{
private:
	/// デバイス

	//入力デバイス
	static IDirectInput8       *dinput      ;
	//キーボード
	static IDirectInputDevice8 *devkeyBoard ;
	//マウス
	static IDirectInputDevice8 *devMouse;


	/// 各種入力情報

	//キーボード
	static BYTE keys[256];
	static BYTE oldkeys[256];
	//マウス
	static DIMOUSESTATE mouseState;		//マウス入力情報
	static DIMOUSESTATE oldMouseState;	//1F前のマウス入力情報
	static POINT		pos;			//マウス座標
	//xinput
	static XINPUT_STATE xInputState;	//xinputの入力状態
	static XINPUT_STATE oldxInputState;	//1F前
	static SHORT XPAD_RS_DEADZONE, XPAD_LS_DEADZONE;
	static XPAD_STICKTILT_FLAG xpadTiltFlags;
	static XPAD_STICKTILT_FLAG oldTiltFlags;

	//コンストラクタ、デストラクタを隠蔽
	Input() {}
	~Input() {}

	

public:

	/// <summary>
	/// 入力系初期化
	/// </summary>
	/// <param name="w">ウィンドウクラス</param>
	/// <param name="hwnd">ウィンドウハンドル</param>
	/// <returns>成否</returns>
	bool Init(WNDCLASSEX w, HWND hwnd);

	//入力情報の取得開始
	static void StartGetInputState();

	// ---------- キー入力 ---------- //

	//キーが入力されているか？
	static bool isKey(int key);
	//トリガー判定
	static bool isKeyTrigger(int key);
	//リリース判定
	static bool isKeyReleased(int key);

	// ---------- マウス入力 ---------- //

	static bool isMouseClicking(int keyCode);

	static bool isMouseClickTrigger(int keyCode);

	static bool isMouseClicked(int keyCode);

	static XMFLOAT2 getMousePos();

	static XMFLOAT2 getMouseVelocity();

	//----------- xinputコントローラー入力 ----------//

	static bool isXpadButtonPushing(XPAD_INPUT_CODE code);

	static bool isXpadButtonPushTrigger(XPAD_INPUT_CODE code);

	static bool isXpadButtonPushed(XPAD_INPUT_CODE code);

	static int GetXpadRTStrength();

	static int GetXpadLTStrength();

	static bool isXpadStickTilt(XPAD_STICK_DIRECTION_CODE dircode);

	static bool isXpadStickTiltTrigger(XPAD_STICK_DIRECTION_CODE dircode);

	static bool isXpadStickTiltReleased(XPAD_STICK_DIRECTION_CODE dircode);

	struct StickTiltParam
	{
		int x = 0;
		int y = 0;
		float x_rate = 0.0f;
		float y_rate = 0.0f;
	};

	static StickTiltParam GetXpadRStickTilt();

	static StickTiltParam GetXpadLStickTilt();

	//インスタンス取得
	static Input *Get();

	//コピーコンストラクタ、代入演算子無効化
	Input(const Input &obj) = delete;
	Input &operator=(const Input &obj) = delete;

private:
	//1F前の入力を確認
	static bool isOldXpadPushing(XPAD_INPUT_CODE code);

	//スティックの傾きをデッドゾーン範囲にまとめる
	static void XpadStickTiltRoundOffToDeadzone();

	//スティックの傾き方向を確認

	static bool isRightStickTiltRight();
	static bool isRightStickTiltLeft();
	static bool isRightStickTiltUp();
	static bool isRightStickTiltDown();
	static bool isRightStickTiltRightUp();
	static bool isRightStickTiltRightDown();
	static bool isRightStickTiltLeftUp();
	static bool isRightStickTiltLeftDown();

	static bool isLeftStickTiltRight();
	static bool isLeftStickTiltLeft();
	static bool isLeftStickTiltUp();
	static bool isLeftStickTiltDown();
	static bool isLeftStickTiltLeftUp();
	static bool isLeftStickTiltLeftDown();
	static bool isLeftStickTiltRightUp();
	static bool isLeftStickTiltRightDown();
};

//マウス入力（ラップ関数）
namespace mouse {



}

//ゲームパッド入力
namespace pad {




}
