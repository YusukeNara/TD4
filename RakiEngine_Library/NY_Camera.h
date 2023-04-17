#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class NY_Camera final
{
private:


private:
	//ビュー変換情報
	XMFLOAT3 _eye;//視点
	XMFLOAT3 _target;//注視点
	XMFLOAT3 _up;//上ベクトル

	//ビュー行列
	XMMATRIX _matView;
	//射影変換行列
	XMMATRIX _matProjection;
	//2D射影変換
	XMMATRIX _matProjection2D;
	//ビュープロジェクション
	XMMATRIX _matViewProj;
	//ビルボード行列
	XMMATRIX _matBillBoard;
	XMMATRIX _matBillY;
	//カメラローカル
	XMMATRIX _camRocal;
	//カメラワールド
	XMMATRIX _camWorld;

	//コンストラクタ
	NY_Camera(XMFLOAT3 eye_, XMFLOAT3 target_, XMFLOAT3 up_);
	NY_Camera() {
		_eye = XMFLOAT3(0, 45, -45);
		_target = XMFLOAT3(0, 0, 0);
		_up = XMFLOAT3(0, 1, 0);

		SetProjecion();

		UpdateViewMat();
	};
	//デストラクタ
	~NY_Camera() {};

public:
	/// インスタンス取得
	static NY_Camera *Get() {
		static NY_Camera ins;
		return &ins;
	}


	///セッタ

	/// <summary>
	/// プロジェクション行列生成
	/// </summary>
	void SetProjecion();

	/// <summary>
	/// ビュー行列設定
	/// </summary>
	/// <param name="eye">視点座標</param>
	/// <param name="target">注視点座標</param>
	/// <param name="up">上方向ベクトル</param>
	void SetViewStatusEyeTargetUp(XMFLOAT3 &eye, XMFLOAT3 &target, XMFLOAT3 &up);

	/// <summary>
	/// ビュー行列設定
	/// </summary>
	/// <param name="eye">視点座標</param>
	void SetViewStatusEye(XMFLOAT3 &eye);

	/// <summary>
	/// ビュー行列設定
	/// </summary>
	/// <param name="target">注視点座標</param>
	void SetViewStatusTarget(XMFLOAT3 &target);

	/// <summary>
	/// ビュー行列設定
	/// </summary>
	/// <param name="up">上方向ベクトル設定</param>
	void SetViewStatusUp(XMFLOAT3 &up);


	/// 各種ゲッタ

	/// <summary>
	/// ビュー行列取得
	/// </summary>
	/// <returns>ビュー行列返却</returns>
	XMMATRIX GetMatrixView();

	/// <summary>
	/// 射影変換行列取得
	/// </summary>
	/// <returns>射影変換（プロジェクション）行列</returns>
	XMMATRIX GetMatrixProjection();

	XMMATRIX GetMatrixProjection2D();

	/// <summary>
	/// ビルボード行列取得
	/// </summary>
	/// <returns>全方向ビルボード行列返却</returns>
	XMMATRIX GetMatrixBillBoardAll();

	XMMATRIX GetMatrixBillBoardY();

	/// <summary>
	/// ビュープロジェクション行列取得
	/// </summary>
	/// <returns>ビューと射影を合成した行列</returns>
	XMMATRIX GetMatrixViewProjection();

	XMFLOAT3 GetEye() { return _eye; }


	/// <summary>
	/// ビュー行列更新（セッタ実行時に自動で実行するのでユーザーが呼び出さなくてよい）
	/// </summary>
	void UpdateViewMat();
};

//ゲッタを省略
#define camera (NY_Camera::Get())
