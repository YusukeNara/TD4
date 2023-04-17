#include "NY_Camera.h"
#include "Raki_WinAPI.h"

NY_Camera::NY_Camera(XMFLOAT3 eye_, XMFLOAT3 target_, XMFLOAT3 up_)
{
	//各値の格納
	_eye = eye_;
	_target = target_;
	_up = up_;

	//ビュー行列格納
	_matView = XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_target), XMLoadFloat3(&_up));

	//プロジェクション行列生成
	SetProjecion();

	//ローカル座標生成
	_camRocal = XMMatrixIdentity();

	_camRocal.r[0].m128_f32[0] = _eye.x;
	_camRocal.r[0].m128_f32[1] = _eye.y;
	_camRocal.r[0].m128_f32[2] = _eye.z;
	_camRocal.r[0].m128_f32[3] = 1;

	_camRocal.r[1].m128_f32[0] = _up.x - _eye.x;
	_camRocal.r[1].m128_f32[1] = _up.y - _eye.y;
	_camRocal.r[1].m128_f32[2] = _up.z - _eye.z;
	_camRocal.r[1].m128_f32[3] = 0;

	_camRocal.r[2].m128_f32[0] = _target.x;
	_camRocal.r[2].m128_f32[1] = _target.y;
	_camRocal.r[2].m128_f32[2] = _target.z;
	_camRocal.r[2].m128_f32[3] = 1;

	_camRocal.r[3].m128_f32[3] = 0;

	UpdateViewMat();
}

void NY_Camera::SetProjecion()
{
	//プロジェクション行列生成
	_matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)Raki_WinAPI::window_width / (float)Raki_WinAPI::window_height,
		0.1f, 10000.0f
	);
	_matProjection2D = XMMatrixOrthographicOffCenterLH(
		0.0f, float(Raki_WinAPI::window_width), float(Raki_WinAPI::window_height), 0.0f, 0.0f, 1.0f);
}

void NY_Camera::SetViewStatusEyeTargetUp(XMFLOAT3 &eye, XMFLOAT3 &target, XMFLOAT3 &up)
{
	//パラメータ設定
	_eye	= eye;
	_target = target;
	_up		= up;

	//更新
	UpdateViewMat();
}

void NY_Camera::SetViewStatusEye(XMFLOAT3 &eye)
{
	//パラメータ設定
	_eye = eye;

	//更新
	UpdateViewMat();
}

void NY_Camera::SetViewStatusTarget(XMFLOAT3 &target)
{
	_target = target;

	UpdateViewMat();
}

void NY_Camera::SetViewStatusUp(XMFLOAT3 &up) 
{
	_up = up;

	UpdateViewMat();
}

XMMATRIX NY_Camera::GetMatrixView()
{
	return _matView;
}

XMMATRIX NY_Camera::GetMatrixViewProjection()
{
	return _matViewProj;
}

XMMATRIX NY_Camera::GetMatrixBillBoardAll()
{
	return _matBillBoard;
}
XMMATRIX NY_Camera::GetMatrixBillBoardY()
{
	return _matBillY;
}

XMMATRIX NY_Camera::GetMatrixProjection()
{
	return _matProjection;
}

XMMATRIX NY_Camera::GetMatrixProjection2D()
{
	return _matProjection2D;
}

//void NY_Camera::UpdateFollowing(XMMATRIX followTarget)//引数は追従したいオブジェクトのワールド変換行列
//{
//	//受け取るワールド変換行列をいじれば追従したいオブジェクトがでるか？
//	//カメラのワールド座標行列でできる？
//	_position.z = -30;
//	_position.y = 10;
//
//	XMMATRIX matScale, matRot, matTrans;
//
//	// スケール、回転、平行行列の計算
//
//	matScale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
//
//	matRot = XMMatrixIdentity();
//	matRot *= XMMatrixRotationZ(XMConvertToRadians(_rotation.z));
//	matRot *= XMMatrixRotationX(XMConvertToRadians(_rotation.x));
//	matRot *= XMMatrixRotationY(XMConvertToRadians(_rotation.y));
//
//	matTrans = XMMatrixTranslation(_position.x, _position.y, _position.z);
//
//	_camRocal = XMMatrixIdentity();
//	_camRocal *= matScale;//ワールド行列にスケーリングを反映
//	_camRocal *= matRot;
//	_camRocal *= matTrans;
//
//	_matView = XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_target), XMLoadFloat3(&_up));
//
//	_matView = _matView * _camRocal * followTarget;
//	
//	_matView = XMMatrixInverse(nullptr, _matView);
//	
//
//}
//
//void NY_Camera::UpdateFollowingOnlyZ(XMFLOAT3 followTargetPos, XMFLOAT3 followRange, float XfollowState, float YfollowState)//Z以外使わないなら、オブジェクトのZだけ取るべきでは？
//{
//
//	// 追従位置の指定
//	_position.x = followRange.x;
//	_position.y = followRange.y;
//	_position.z = followRange.z;
//
//	// カメラのワールド行列変換用
//	XMMATRIX matScale, matRot, matTrans;
//
//	// スケール、回転、平行行列の計算
//	matScale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
//
//	matRot =  XMMatrixIdentity();
//	matRot *= XMMatrixRotationZ(XMConvertToRadians(_rotation.z));
//	matRot *= XMMatrixRotationX(XMConvertToRadians(_rotation.x));
//	matRot *= XMMatrixRotationY(XMConvertToRadians(_rotation.y));
//
//	matTrans = XMMatrixTranslation(_position.x, _position.y, _position.z);
//
//	// ワールド変換
//	_camRocal = XMMatrixIdentity();
//	_camRocal *= matScale;//ワールド行列にスケーリングを反映
//	_camRocal *= matRot;
//	_camRocal *= matTrans;
//
//	// 追従オブジェクトの数値加工?追従したいオブジェクトのZ以外を0にした行列を生成
//	XMMATRIX objScale, objRot, objTrans, objProcessed;
//
//	objScale = XMMatrixScaling(1, 1, 1);
//
//	objRot = XMMatrixIdentity();
//	objRot *= XMMatrixRotationZ(XMConvertToRadians(0));
//	objRot *= XMMatrixRotationX(XMConvertToRadians(0));
//	objRot *= XMMatrixRotationY(XMConvertToRadians(0));
//
//	//追従度を用いた計算時、この手法だとカメラのパラメータを用いた処理を実行時ズレが発生する?ため、改善が必要
//	objTrans = XMMatrixTranslation(followTargetPos.x * XfollowState, followTargetPos.y * YfollowState, followTargetPos.z);
//
//	//追従用に加工したワールド行列生成
//	objProcessed = XMMatrixIdentity();
//	objProcessed *= objScale;
//	objProcessed *= objRot;
//	objProcessed *= objTrans;
//
//	//ビュー行列生成
//	_matView = XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_target), XMLoadFloat3(&_up));
//	//追従版変換行列
//	_matView = _matView * _camRocal * objProcessed;
//	//ビュー用に逆行列に変換
//	_matView = XMMatrixInverse(nullptr, _matView);
//}
//
//void NY_Camera::UpdateFollowingOnlyZVer2(XMFLOAT3 followTarget, XMFLOAT3 followRange, float followRad)
//{
//	//追尾ターゲットのセット
//	_target = followTarget;
//	//無回転状態のオフセットベクトル
//	XMVECTOR offVec = { 0.0f,0.0f,followRange.z,0.0f };
//	//Y軸のfollowRad分の回転行列を出す
//	XMMATRIX rot = XMMatrixIdentity();
//	rot *= XMMatrixRotationY(XMConvertToRadians(followRad));
//	//offsetをrotで回転させたベクトルを出す
//	XMVECTOR rotVec = XMVector3TransformNormal(offVec, rot);
//	//targetにrotVecを加算し、eyeを出す。
//	_eye.x = _target.x + rotVec.m128_f32[0];
//	_eye.y = _target.y + rotVec.m128_f32[1];
//	_eye.z = _target.z + rotVec.m128_f32[2];
//
//	//ビュー生成
//	_matView = XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_target), XMLoadFloat3(&_up));
//
//}
//
//void NY_Camera::UpdateFollowingOnlyZWorldMat(XMMATRIX targetWorld)
//{
//	//追従するターゲットのワールドと合成
//	XMMATRIX cam_after = _camRocal * targetWorld;
//
//	//ビュー生成
//	_matView = XMMatrixLookAtLH(
//		XMVECTOR{ cam_after.r[0].m128_f32[0],cam_after.r[0].m128_f32[1], cam_after.r[0].m128_f32[2], },
//		XMVECTOR{ cam_after.r[2].m128_f32[0],cam_after.r[2].m128_f32[1], cam_after.r[2].m128_f32[2], },
//		XMVECTOR{ cam_after.r[1].m128_f32[0],cam_after.r[1].m128_f32[1], cam_after.r[1].m128_f32[2], }
//	);
//
//	//視点、注視点、上方向ベクトルの更新
//	_eye = { cam_after.r[0].m128_f32[0],cam_after.r[0].m128_f32[1], cam_after.r[0].m128_f32[2] };
//	_up = { cam_after.r[1].m128_f32[0],cam_after.r[1].m128_f32[1], cam_after.r[1].m128_f32[2] };
//	_target = { cam_after.r[2].m128_f32[0], cam_after.r[2].m128_f32[1], cam_after.r[2].m128_f32[2] };
//
//}
//
//void NY_Camera::UpdateFollowingViewAndWorld(XMMATRIX target, XMFLOAT3 range)
//{
//	// 追従位置の指定
//	_position.x = range.x;
//	_position.y = range.y;
//	_position.z = range.z;
//
//	// カメラのワールド行列変換用
//	XMMATRIX matScale, matRot, matTrans;
//
//	// スケール、回転、平行行列の計算
//	//matScale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
//
//	matRot = XMMatrixIdentity();
//	matRot *= XMMatrixRotationZ(XMConvertToRadians(_rotation.z));
//	matRot *= XMMatrixRotationX(XMConvertToRadians(_rotation.x));
//	matRot *= XMMatrixRotationY(XMConvertToRadians(_rotation.y));
//
//	matTrans = XMMatrixTranslation(_position.x, _position.y, _position.z);
//
//	//ワールド座標計算
//	_camWorld = matRot * matTrans;
//
//	//追従するターゲットのワールドと合成
//	XMMATRIX cam_after = _camRocal * target;
//
//	//ビュー生成
//	_matView = XMMatrixLookAtLH(
//		XMVECTOR{ cam_after.r[0].m128_f32[0],cam_after.r[0].m128_f32[1], cam_after.r[0].m128_f32[2], },
//		XMVECTOR{ cam_after.r[2].m128_f32[0],cam_after.r[2].m128_f32[1], cam_after.r[2].m128_f32[2], },
//		XMVECTOR{ cam_after.r[1].m128_f32[0],cam_after.r[1].m128_f32[1], cam_after.r[1].m128_f32[2], }
//	);
//
//	//視点、注視点、上方向ベクトルの更新
//	_eye = { cam_after.r[0].m128_f32[0],cam_after.r[0].m128_f32[1], cam_after.r[0].m128_f32[2] };
//	_up = { cam_after.r[1].m128_f32[0],cam_after.r[1].m128_f32[1], cam_after.r[1].m128_f32[2] };
//	_target = { cam_after.r[2].m128_f32[0], cam_after.r[2].m128_f32[1], cam_after.r[2].m128_f32[2] };
//
//	//ビューの最終更新
//	_matView *= _camWorld;
//
//}
//
//void NY_Camera::UpdateFollowingViewAndWorld(XMMATRIX target)
//{
//	// カメラのワールド行列変換用
//	XMMATRIX matScale, matRot, matTrans;
//
//	// スケール、回転、平行行列の計算
//	//matScale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
//
//	matRot = XMMatrixIdentity();
//	matRot *= XMMatrixRotationZ(XMConvertToRadians(_rotation.z));
//	matRot *= XMMatrixRotationX(XMConvertToRadians(_rotation.x));
//	matRot *= XMMatrixRotationY(XMConvertToRadians(_rotation.y));
//
//	matTrans = XMMatrixTranslation(_position.x, _position.y, _position.z);
//
//	//ワールド座標計算
//	_camWorld = matRot * matTrans;
//
//	//追従するターゲットのワールドと合成
//	XMMATRIX cam_after = _camRocal * target;
//
//	//ビュー生成
//	_matView = XMMatrixLookAtLH(
//		XMVECTOR{ cam_after.r[0].m128_f32[0],cam_after.r[0].m128_f32[1], cam_after.r[0].m128_f32[2], },
//		XMVECTOR{ cam_after.r[2].m128_f32[0],cam_after.r[2].m128_f32[1], cam_after.r[2].m128_f32[2], },
//		XMVECTOR{ cam_after.r[1].m128_f32[0],cam_after.r[1].m128_f32[1], cam_after.r[1].m128_f32[2], }
//	);
//
//	//視点、注視点、上方向ベクトルの更新
//	_eye = { cam_after.r[0].m128_f32[0],cam_after.r[0].m128_f32[1], cam_after.r[0].m128_f32[2] };
//	_up = { cam_after.r[1].m128_f32[0],cam_after.r[1].m128_f32[1], cam_after.r[1].m128_f32[2] };
//	_target = { cam_after.r[2].m128_f32[0], cam_after.r[2].m128_f32[1], cam_after.r[2].m128_f32[2] };
//
//	//ビューの最終更新
//	_matView *= _camWorld;
//}

void NY_Camera::UpdateViewMat() {

	_matView = XMMatrixIdentity();

	_matView = XMMatrixLookAtLH(XMLoadFloat3(&_eye), XMLoadFloat3(&_target), XMLoadFloat3(&_up));

	//ビュープロジェクション行列
	_matViewProj = _matView * _matProjection;

	// 以下、ビルボード行列の計算

	//XMVECTORに移動
	XMVECTOR eyeVec = XMLoadFloat3(&_eye);
	XMVECTOR targetVec = XMLoadFloat3(&_target);
	XMVECTOR upVec = XMLoadFloat3(&_up);

	//Z軸計算
	XMVECTOR axisZ = XMVectorSubtract(targetVec, eyeVec);
	axisZ = XMVector3Normalize(axisZ);
	//x軸計算
	XMVECTOR axisX = XMVector3Cross(upVec, axisZ);
	axisX = XMVector3Normalize(axisX);
	//y軸計算
	XMVECTOR axisY = XMVector3Cross(axisZ, axisX);

	//回転行列計算
	XMMATRIX camrot;
	camrot.r[0] = axisX;
	camrot.r[1] = axisY;
	camrot.r[2] = axisZ;
	camrot.r[3] = XMVectorSet(0, 0, 0, 1);
	//y軸ビルボード計算をここに
	XMMATRIX ycamrot = XMMatrixIdentity();
	//Z軸計算
	XMVECTOR yaxisX = axisX;
	//x軸計算
	XMVECTOR yaxisY = XMVector3Normalize(upVec);
	//y軸計算
	XMVECTOR yaxisZ = XMVector3Cross(yaxisX, yaxisY);

	ycamrot.r[0] = yaxisX;
	ycamrot.r[1] = yaxisY;
	ycamrot.r[2] = yaxisZ;
	ycamrot.r[3] = XMVectorSet(0, 0, 0, 1);

	//全方向ビルボード計算
	_matBillBoard	= camrot;
	_matBillY		= ycamrot;

}
