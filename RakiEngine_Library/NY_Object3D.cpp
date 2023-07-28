#include "NY_Object3D.h"
#include "FbxLoader.h"
#include "NY_Camera.h"
#include "TexManager.h"
#include "Raki_DX12B.h"
#include "NY_Object3DMgr.h"
#include "DirectionalLight.h"
#include "RenderTargetManager.h"

//----- NY_Object3D -----//

void Object3d::InitObject3D(ID3D12Device *dev)
{

	HRESULT result;
	const auto HEAP_PROP = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	const auto RESDESC = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff);
	
	//定数バッファb0生成
	result =  dev->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0)
	);
	constBuffB0.Get()->SetName(L"Object3d_cbuffB0");

	//定数バッファb1生成
	result = dev->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&RESDESC,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1)
	);
	constBuffB1.Get()->SetName(L"Object3d_cbuffB1");

	auto skinresdesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff);

	result = dev->CreateCommittedResource(
		&HEAP_PROP,
		D3D12_HEAP_FLAG_NONE,
		&skinresdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffSkin)
	);
	constBuffSkin.Get()->SetName(L"Object3d_cbuffSkin");

	//単位行列で初期化
	ConstBufferDataSkin* constMapSkin = nullptr;
	if (SUCCEEDED(constBuffSkin->Map(0, nullptr, (void**)&constMapSkin))) {
		for (int i = 0; i < MAX_BONES; i++) {
			constMapSkin->bones[i] = XMMatrixIdentity();
		}
		constBuffSkin->Unmap(0, nullptr);
	}

	frameTime.SetTime(0, 0, 0, 1, 0, FbxTime::EMode::eFrames60);

}

void Object3d::SetLoadedModelData(Model3D *loadedModel)
{
	InitObject3D(RAKI_DX12B_DEV);

	isDirty = true;

	isThisModel = MODEL_DATA_OBJ;
}

void Object3d::SetLoadedModelData(std::shared_ptr<Model3D> loadedmodel)
{
	InitObject3D(RAKI_DX12B_DEV);

	model = loadedmodel;

	isDirty = true;

	isThisModel = MODEL_DATA_OBJ;
}

void Object3d::SetLoadedModelData(std::shared_ptr<fbxModel> loadedmodel)
{
	InitObject3D(RAKI_DX12B_DEV);

	fbxmodel = loadedmodel;

	isDirty = true;

	isThisModel = MODEL_DATA_FBX;
}

void Object3d::SetWorldMatrix(XMMATRIX matWorld)
{
	//変換行列を設定
	this->matWorld = matWorld;
	//ダーティフラグ有効化
	isDirty = true;
}

void Object3d::SetAffineParam(RVector3 scale, RVector3 rot, RVector3 trans)
{
	//アフィン変換情報を比較し、値が異なる場合更新する
	if (this->scale != scale) {
		this->scale = scale;
		isDirty = true;
	}
	if (this->rotation != rot) {
		this->rotation = rot;
		isDirty = true;
	}
	if (this->position != trans) {
		this->position = trans;
		isDirty = true;
	}

	//ダーティフラグが有効のとき
	if (isDirty == true) {
		//条件に応じてパラメータ更新
		if (isBillBoard != true) {
			UpdateObject3D();
		}
		else {
			UpdateBillBoard3D();
		}
	}
}

void Object3d::SetAffineParamScale(RVector3 scale)
{
	if (this->scale != scale) {
		this->scale = scale;
		isDirty = true;
	}

}

void Object3d::SetAffineParamRotate(RVector3 rot)
{
	if (this->rotation != rot) {
		this->rotation = rot;
		isDirty = true;
	}

}

void Object3d::SetAffineParamTranslate(RVector3 trans)
{
	if (this->position != trans) {
		this->position = trans;
		isDirty = true;
	}
}

void Object3d::UpdateObject3D()
{
	if (isPlay && fbxmodel != nullptr) {
		currentTime += frameTime;

		if (currentTime > endTime) {
			switch (playmode)
			{
			case ANIMATION_PLAYMODE::ANIM_MODE_FIRST:
				StopAnimation();
				break;
			case ANIMATION_PLAYMODE::ANIM_MODE_ROOP:
				currentTime = startTime;
				break;
			default:
				break;
			}
		}
	}

	if (isBillBoard) {
		UpdateBillBoard3D();
		return;
	}

	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行行列の計算

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity();
	matWorld *= matScale;//ワールド行列にスケーリングを反映
	matWorld *= matRot;
	matWorld *= matTrans;

	//定数バッファB0データ転送
	ConstBufferDataB0 *ConstMapB0 = nullptr;
	if (SUCCEEDED(constBuffB0->Map(0, nullptr, (void **)&ConstMapB0)))
	{
		ConstMapB0->mat = matWorld * camera->GetMatrixView() * camera->GetMatrixProjection();
		ConstMapB0->wMat = matWorld;
		ConstMapB0->color = this->color;
		ConstMapB0->lightCamMat = matWorld * DirectionalLight::GetLightCamera() * camera->GetMatrixProjection();
		constBuffB0->Unmap(0, nullptr);
	}

	//定数バッファB1データ転送
	ConstBufferDataB1 *ConstMapB1 = nullptr;
	ConstBufferDataSkin *constMapSkin = nullptr;
	if (isThisModel == MODEL_DATA_FBX) {
		if (SUCCEEDED(constBuffB1->Map(0, nullptr, (void**)&ConstMapB1)))
		{
			ConstMapB1->amdient = fbxmodel->GetMaterial().ambient;
			ConstMapB1->diffuse = fbxmodel->GetMaterial().diffuse;
			ConstMapB1->specular = fbxmodel->GetMaterial().specurar;
			ConstMapB1->alpha = fbxmodel->GetMaterial().alpha;
			constBuffB1->Unmap(0, nullptr);

			auto& bones = fbxmodel->GetBones();

			//現在割り当てられているアニメーション番号を設定
			//更新関数が呼ばれた時点のアニメーションに変更して、定数バッファの転送に備える
			FbxScene* fbxScene = fbxmodel->GetFbxScene();
			FbxAnimStack* animStack = fbxScene->GetSrcObject<FbxAnimStack>(playAnimNum);
			fbxScene->SetCurrentAnimationStack(animStack);

			//メッシュノードのグローバルトランスフォーム
			XMMATRIX grovalScale = XMMatrixScaling(1.f, 1.f, 1.f);
			XMMATRIX grovalRot = XMMatrixIdentity();
			grovalRot *= XMMatrixRotationZ(0);
			grovalRot *= XMMatrixRotationX(0);
			grovalRot *= XMMatrixRotationY(0);
			XMMATRIX grovalTrans = XMMatrixTranslation(0, 0, 0);

			XMMATRIX meshGrovalTrans = grovalScale * grovalRot * grovalTrans;
			XMMATRIX meshGrovalrev = XMMatrixInverse(nullptr, meshGrovalTrans);

			ConstBufferDataSkin* constMapSkin = nullptr;
			if (SUCCEEDED(constBuffSkin->Map(0, nullptr, (void**)&constMapSkin))) {
				for (int i = 0; i < bones.size(); i++) {
					XMMATRIX matcurrentPose;
					FbxAMatrix fbxCurrentPose = bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(currentTime);

					FbxLoader::GetInstance()->ConvertMatrixFromFbx(&matcurrentPose, fbxCurrentPose);

					//メッシュノード対策
					constMapSkin->bones[i] = meshGrovalTrans * 
						bones[i].invInitialBone * 
						matcurrentPose * 
						meshGrovalrev;
				}
				constBuffSkin->Unmap(0, nullptr);
			}
		}
	}
	else {
		if (SUCCEEDED(constBuffB1->Map(0, nullptr, (void**)&ConstMapB1)))
		{
			ConstMapB1->amdient = model->material.ambient;
			ConstMapB1->diffuse = model->material.diffuse;
			ConstMapB1->specular = model->material.specurar;
			ConstMapB1->alpha = model->material.alpha;
		}
	}


	//ダーティフラグリセット
	isDirty = false;
	if (isThisModel == MODEL_DATA_FBX) {

	}
	else {
		model->Update();
	}

}

void Object3d::UpdateBillBoard3D()
{

	//ワールド変換用
	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行行列の計算

	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));

	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity();

	matWorld *= camera->GetMatrixBillBoardY();

	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrans;

	//定数バッファB0データ転送
	ConstBufferDataB0 *ConstMapB0 = nullptr;
	if (SUCCEEDED(constBuffB0->Map(0, nullptr, (void **)&ConstMapB0)))
	{
		ConstMapB0->mat = matWorld * camera->GetMatrixView() * camera->GetMatrixProjection();
		ConstMapB0->wMat = matWorld;
		ConstMapB0->color = this->color;
		constBuffB0->Unmap(0, nullptr);
	}

	//定数バッファB1データ転送
	ConstBufferDataB1* ConstMapB1 = nullptr;
	if (isThisModel == MODEL_DATA_FBX) {
		if (SUCCEEDED(constBuffB1->Map(0, nullptr, (void**)&ConstMapB1)))
		{
			ConstMapB1->amdient = fbxmodel->GetMaterial().ambient;
			ConstMapB1->diffuse = fbxmodel->GetMaterial().diffuse;
			ConstMapB1->specular = fbxmodel->GetMaterial().specurar;
			ConstMapB1->alpha = fbxmodel->GetMaterial().alpha;
			constBuffB1->Unmap(0, nullptr);
		}
	}
	else {
		if (SUCCEEDED(constBuffB1->Map(0, nullptr, (void**)&ConstMapB1)))
		{
			ConstMapB1->amdient = model->material.ambient;
			ConstMapB1->diffuse = model->material.diffuse;
			ConstMapB1->specular = model->material.specurar;
			ConstMapB1->alpha = model->material.alpha;
			constBuffB1->Unmap(0, nullptr);
		}
	}

	model->Update();
}

void Object3d::DrawShadow()
{
	//if (isThisModel == MODEL_DATA_FBX) {
	//	NY_Object3DManager::Get()->SetCommonBeginDrawShadow_FBX();
	//	//定数バッファ設定
	//	RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	//	//定数バッファ設定
	//	RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());
	//	RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(4, constBuffSkin->GetGPUVirtualAddress());

	//	fbxmodel->Draw();

	//	NY_Object3DManager::Get()->ReturnShadowToDifferd();
	//}
	//else {
	//	NY_Object3DManager::Get()->SetCommonBeginDrawShadow();

	//	//頂点バッファ設定
	//	RAKI_DX12B_CMD->IASetVertexBuffers(0, 1, &model->vbView);
	//	//インデックスバッファ設定
	//	RAKI_DX12B_CMD->IASetIndexBuffer(&model->ibview);
	//	//定数バッファ設定
	//	RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	//	//定数バッファ設定
	//	RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());
	//	//描画
	//	RAKI_DX12B_CMD->DrawIndexedInstanced(UINT(model->indices.size()), 1, 0, 0, 0);

	//	NY_Object3DManager::Get()->ReturnShadowToDifferd();
	//}

}

void Object3d::DrawObject()
{
	NY_Object3DManager::Get()->SetRestartObject3D();

	if (isBillBoard) {
		UpdateBillBoard3D();
	}
	else {
		UpdateObject3D();
	}

	if (isDrawShadow) {
		DrawShadow();
	}


	if (isThisModel == MODEL_DATA_FBX) {
		NY_Object3DManager::Get()->SetCommonBeginDrawFBX();

		//定数バッファ設定
		RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
		//定数バッファ設定
		RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());
		RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(3, constBuffSkin->GetGPUVirtualAddress());

		fbxmodel->Draw();
	}
	else {
		NY_Object3DManager::Get()->SetRestartObject3D();

		//頂点バッファ設定
		RAKI_DX12B_CMD->IASetVertexBuffers(0, 1, &model->vbView);
		//インデックスバッファ設定
		RAKI_DX12B_CMD->IASetIndexBuffer(&model->ibview);
		//定数バッファ設定
		RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
		//定数バッファ設定
		RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

		//シェーダーリソースビューをセット
		RAKI_DX12B_CMD->SetGraphicsRootDescriptorTable(2,
			CD3DX12_GPU_DESCRIPTOR_HANDLE(TexManager::texDsvHeap.Get()->GetGPUDescriptorHandleForHeapStart(),
				model->material.texNumber, RAKI_DX12B_DEV->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

		//描画
		RAKI_DX12B_CMD->DrawIndexedInstanced(UINT(model->indices.size()), 1, 0, 0, 0);
	}

}

void Object3d::DrawRTexObject(int rtHandle)
{
	//範囲外参照を検知
	if (RenderTargetManager::GetInstance()->isHandleOutOfRange(rtHandle)) { return; }

	if (RenderTargetManager::GetInstance()->isNullHandle(rtHandle)) { return; }

	UpdateObject3D();

	NY_Object3DManager::Get()->SetCommonBeginDrawObject3DFeatRTex(rtHandle);

	//頂点バッファ設定
	RAKI_DX12B_CMD->IASetVertexBuffers(0, 1, &model->vbView);
	//インデックスバッファ設定
	RAKI_DX12B_CMD->IASetIndexBuffer(&model->ibview);
	//定数バッファ設定
	RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	//定数バッファ設定
	RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

	//シェーダーリソースビューをセット
	RAKI_DX12B_CMD->SetGraphicsRootDescriptorTable(2,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(RenderTargetManager::GetInstance()->renderTextures[rtHandle]->GetDescriptorHeapSRV()->GetGPUDescriptorHandleForHeapStart(),
			0, RAKI_DX12B_DEV->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//描画
	RAKI_DX12B_CMD->DrawIndexedInstanced(UINT(model->indices.size()), 1, 0, 0, 0);
}

void Object3d::DrawRTexObject(RTex* rt)
{
	UpdateObject3D();

	NY_Object3DManager::Get()->SetCommonBeginDrawObject3DFeatRTex(rt);

	//頂点バッファ設定
	RAKI_DX12B_CMD->IASetVertexBuffers(0, 1, &model->vbView);
	//インデックスバッファ設定
	RAKI_DX12B_CMD->IASetIndexBuffer(&model->ibview);
	//定数バッファ設定
	RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	//定数バッファ設定
	RAKI_DX12B_CMD->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());

	//シェーダーリソースビューをセット
	RAKI_DX12B_CMD->SetGraphicsRootDescriptorTable(2,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(rt->GetDescriptorHeapSRV()->GetGPUDescriptorHandleForHeapStart(),
			0, RAKI_DX12B_DEV->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//描画
	RAKI_DX12B_CMD->DrawIndexedInstanced(UINT(model->indices.size()), 1, 0, 0, 0);
}

void Object3d::DrawMultiPassResource()
{




}

void Object3d::LoadAndSetModelData(string modelname)
{
	InitObject3D(RAKI_DX12B_DEV);

	//モデルデータを読み込んで設定する
	model.get()->LoadObjModel(modelname.c_str());

	isThisModel = MODEL_DATA_OBJ;
}

void Object3d::LoadAndSetModelData_Fbx(string filename)
{
	InitObject3D(RAKI_DX12B_DEV);

	//モデルデータをロードして所有権を移行
	fbxModel *fmodel = FbxLoader::GetInstance()->LoadFBXFile(filename);
	fbxmodel.reset(fmodel);

	//ロードしたモデルからアニメーション情報を取得、格納

	isThisModel = MODEL_DATA_FBX;
}

void Object3d::SetAnotherObjectModelData(Object3d *anotherObj)
{
	//別オブジェクトのモデルデータを取得する
	this->model = anotherObj->model;
}

void Object3d::CreateModel_Tile(float x_size, float y_size, float x_uv, float y_uv, UINT useTexNum)
{
	model.get()->CreatePlaneModelXZ(x_size, y_size, x_uv, y_uv, useTexNum, nullptr);
}

void Object3d::CreateModel_Box(float size, float uv_x, float uv_y, UINT useTexNum)
{
	model.get()->CreateBoxModel(size, uv_x, uv_y, useTexNum);
}

void Object3d::PlayAnimation(ANIMATION_PLAYMODE playmode, int animNum)
{
	if (fbxmodel == nullptr) {
		return;
	}

	FbxScene* fbxScene = fbxmodel->GetFbxScene();

	FbxAnimStack* animStack = nullptr;

	if (fbxScene->GetSrcObject<FbxAnimStack>(animNum) != nullptr) {
		animStack = fbxScene->GetSrcObject<FbxAnimStack>(animNum);
		playAnimNum = animNum;
	}
	else {
		animStack = fbxScene->GetSrcObject<FbxAnimStack>(0);
		playAnimNum = 0;
	}

	fbxScene->SetCurrentAnimationStack(animStack);

	const char* animStackName = animStack->GetName();

	FbxTakeInfo* takeInfo = fbxScene->GetTakeInfo(animStackName);

	startTime = takeInfo->mLocalTimeSpan.GetStart();

	endTime = takeInfo->mLocalTimeSpan.GetStop();

	isPlay = true;

	this->playmode = playmode;

}

void Object3d::ResetAnimation()
{
	isPlay = false;
	currentTime = startTime;
}

void Object3d::PauseAnimation()
{
	isPlay = false;
}

void Object3d::StopAnimation()
{
	isPlay = false;

	currentTime = endTime;
}

void Object3d::DisplayObjectStatus(bool isDisplay)
{
	if (!isDisplay) { return; }


	ImguiMgr::Get()->StartDrawImgui("objectStatus", 200, 500);

	ImGui::Checkbox("affine param", &isDisplayAffineParam);

	if (isDisplayAffineParam) {
		ImGui::Text("pos x:%.2f y:%.2f z:%.2f", position.x, position.y, position.z);
		ImGui::SliderFloat("pos x", &position.x, -10000.0f, 10000.0f);
		ImGui::SliderFloat("pos y", &position.y, -10000.0f, 10000.0f);
		ImGui::SliderFloat("pos z", &position.z, -10000.0f, 10000.0f);

		ImGui::Text("rot x:%.2f y:%.2f z:%.2f", rotation.x, rotation.y, rotation.z);
		ImGui::SliderFloat("rot x", &rotation.x, -10000.0f, 10000.0f);
		ImGui::SliderFloat("rot y", &rotation.y, -10000.0f, 10000.0f);
		ImGui::SliderFloat("rot z", &rotation.z, -10000.0f, 10000.0f);

		ImGui::Text("scale x:%.2f y:%.2f z:%.2f", scale.x, scale.y, scale.z);
		ImGui::SliderFloat("scale x", &scale.x, -10000.0f, 10000.0f);
		ImGui::SliderFloat("scale y", &scale.y, -10000.0f, 10000.0f);
		ImGui::SliderFloat("scale z", &scale.z, -10000.0f, 10000.0f);
	}

	ImGui::Checkbox("bone", &isDisplayBone);

	ImguiMgr::Get()->EndDrawImgui();

	SetAffineParam(scale, rotation, position);

}


