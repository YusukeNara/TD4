#include "FbxLoader.h"
#include <iostream>
#include <algorithm>

const std::string FbxLoader::baseDir = "Resources/";

const std::string FbxLoader::defaultTexName = "white1x1.png";

using namespace DirectX;

FbxLoader* FbxLoader::GetInstance()
{
    static FbxLoader instance;
    return &instance;
}

void FbxLoader::Initialize()
{
    assert(fbxmgr == nullptr);

    fbxmgr = FbxManager::Create();

    FbxIOSettings* ios = FbxIOSettings::Create(fbxmgr, IOSROOT);
    fbxmgr->SetIOSettings(ios);

    fbxImporter = FbxImporter::Create(fbxmgr, "");

}

void FbxLoader::Finalize()
{
    fbxImporter->Destroy();
    fbxmgr->Destroy();
}

fbxModel* FbxLoader::LoadFBXFile(string filename)
{
    const string dirPass = baseDir + filename + "/";

    folderPass = filename + "/";

    const string modelname = filename + ".fbx";

    const string fullpath = dirPass + modelname;

    std::cout << "Loading :" << fullpath << std::endl;

    if (!fbxImporter->Initialize(fullpath.c_str(), -1, fbxmgr->GetIOSettings())) {
        assert(0);
    }

    FbxScene* fbxscene = FbxScene::Create(fbxmgr, "fbxScene");

    fbxImporter->Import(fbxscene);

    fbxModel* model = new fbxModel();
    model->name = filename;

    int nodecount = fbxscene->GetNodeCount();

    model->nodes.reserve(nodecount);

    ParseNodeRecursive(model, fbxscene->GetRootNode());

    model->CreateBuffers();

    model->fbxScene = fbxscene;

    return model;
}

void FbxLoader::ParseNodeRecursive(fbxModel* model, FbxNode* fbxnode, Node* parent)
{
    string name = fbxnode->GetName();

    model->nodes.emplace_back();
    Node& node = model->nodes.back();

    node.name = fbxnode->GetName();

    FbxDouble3 rotate = fbxnode->LclRotation.Get();
    FbxDouble3 scale = fbxnode->LclScaling.Get();
    FbxDouble3 trans = fbxnode->LclTranslation.Get();

    node.rotation = { (float)rotate[0],(float)rotate[1],(float)rotate[2],0.0f };
    node.scaling = { (float)scale[0],(float)scale[1],(float)scale[2],0.0f };
    node.translation = { (float)trans[0],(float)trans[1],(float)trans[2],1.0f };

    node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
    node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
    node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);

    XMMATRIX matscale, matrot, mattrans;
    matscale = XMMatrixScalingFromVector(node.scaling);
    matrot = XMMatrixRotationRollPitchYawFromVector(node.rotation);
    mattrans = XMMatrixTranslationFromVector(node.translation);

    node.transform = XMMatrixIdentity();
    node.transform *= matscale;
    node.transform *= matrot;
    node.transform *= mattrans;
    node.globalTransform = node.transform;

    if (parent) {
        node.parent = parent;

        node.globalTransform *= parent->globalTransform;
    }


    FbxNodeAttribute* fbxNodeAttribute = fbxnode->GetNodeAttribute();
    if (fbxNodeAttribute) {
        if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
            model->meshNode = &node;
            ParseMesh(model, fbxnode);
        }
    }

    std::cout << fbxnode->GetChildCount() << std::endl;
    //子ノードに対し再帰呼び出し
    for (int i = 0; i < fbxnode->GetChildCount(); i++) {
        ParseNodeRecursive(model, fbxnode->GetChild(i), &node);
    }

}

void FbxLoader::ParseMesh(fbxModel* model, FbxNode* fbxnode)
{
    FbxMesh* fbxmesh = fbxnode->GetMesh();

    //ParseMeshVertices(model, fbxmesh);

    ParseMeshFaces(model, fbxmesh);

    ParseMaterial(model, fbxnode);

    ParseSkin(model, fbxmesh);

}

std::string FbxLoader::ExtractFileName(const std::string& path)
{
    size_t pos1;

    pos1 = path.rfind('\\');
    if (pos1 != string::npos) {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }

    pos1 = path.rfind('/');
    if (pos1 != string::npos) {
        return path.substr(pos1 + 1, path.size() - pos1 - 1);
    }

    return path;

}

void FbxLoader::ParseMeshVertices(fbxModel* model, FbxMesh* mesh)
{
    auto& vertices = model->vertices;

    const int ctrlPointCount = mesh->GetControlPointsCount();

    fbxVertex v{};
    model->vertices.resize(ctrlPointCount, v);

    FbxVector4* pCoord = mesh->GetControlPoints();

    for (int i = 0; i < ctrlPointCount; i++) {
        fbxVertex& vertex = vertices[i];
        vertex.pos.x = (float)pCoord[i][0];
        vertex.pos.y = (float)pCoord[i][1];
        vertex.pos.z = (float)pCoord[i][2];
    }
}

void FbxLoader::ParseMeshFaces(fbxModel* model, FbxMesh* mesh)
{
    auto& vertices = model->vertices;
    auto& indices = model->indices;

    assert(indices.size() == 0);

    const int polygonCount = mesh->GetPolygonCount();
    const int texUVCount = mesh->GetTextureUVCount();

    FbxStringList uvNames;
    mesh->GetUVSetNames(uvNames);

    //コントロールポイント格納配列
    FbxVector4* pCoord = mesh->GetControlPoints();
    fbxVertex v{};

    for (int i = 0; i < polygonCount; i++) {
        const int polygonSize = mesh->GetPolygonSize(i);
        if (polygonSize > 3)
        {
            assert(polygonSize <= 3);
        }

        for (int j = 0; j < polygonSize; j++) {
            //制御点のインデックスを取得
            int index = mesh->GetPolygonVertex(i, j);
            assert(index >= 0);

            //制御点配列のindex番目を参照
            v.pos.x = (float)(*(pCoord + index))[0];
            v.pos.y = (float)(*(pCoord + index))[1];
            v.pos.z = (float)(*(pCoord + index))[2];

            //fbxVertex& v = vertices[index];

            FbxVector4 normal;
            if (mesh->GetPolygonVertexNormal(i, j, normal)) {
                v.normal.x = (float)normal[0];
                v.normal.y = (float)normal[1];
                v.normal.z = (float)normal[2];
            }

            if (texUVCount > 0) {
                FbxVector2 uvs;
                bool lUnmappedUV;

                if (mesh->GetPolygonVertexUV(i, j, uvNames[0], uvs, lUnmappedUV)) {
                    v.uv.x = (float)uvs[0];
                    v.uv.y = 1.0f - (float)uvs[1];
                }
            }

            //制御点インデックス番号を格納
            v.controlPointIndex = index;

            //if (j < 3) {
            //    indices.push_back(index);
            //}
            //else {
            //    int index2 = indices[indices.size() - 1];
            //    int index3 = index;
            //    int index0 = indices[indices.size() - 3];
            //    indices.push_back(index2);
            //    indices.push_back(index3);
            //    indices.push_back(index0);
            //}

            //頂点とインデックスを動的に増やす
            indices.push_back(unsigned short(vertices.size()));
            vertices.push_back(v);
        }
    }

}

void FbxLoader::ParseMaterial(fbxModel* model, FbxNode* node)
{
    const int materialCount = node->GetMaterialCount();
    bool textureLoaded = false;
    if (materialCount > 0) {

        FbxSurfaceMaterial* material = node->GetMaterial(0);

        if (material) {
            if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
                //FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);
                FbxSurfacePhong* phong = static_cast<FbxSurfacePhong*>(material);

                FbxPropertyT<FbxDouble3> ambient = phong->Ambient;
                model->material.ambient.x = (float)ambient.Get()[0];
                model->material.ambient.y = (float)ambient.Get()[1];
                model->material.ambient.z = (float)ambient.Get()[2];

                FbxPropertyT<FbxDouble3> diffuse = phong->Diffuse;
                model->material.diffuse.x = (float)diffuse.Get()[0];
                model->material.diffuse.y = (float)diffuse.Get()[1];
                model->material.diffuse.z = (float)diffuse.Get()[2];

                FbxPropertyT<FbxDouble3> specular = phong->Specular;
                model->material.specurar.x = (float)specular.Get()[0];
                model->material.specurar.y = (float)specular.Get()[1];
                model->material.specurar.z = (float)specular.Get()[2];
            }

            const FbxProperty diffuseProperty = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
            if (diffuseProperty.IsValid()) {
                const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
                if (texture) {
                    const char* filepath = texture->GetFileName();

                    string path_str = filepath;
                    string name = ExtractFileName(path_str);

                    model->material.texNumber = TexManager::LoadTexture(baseDir + folderPass + name);

                    if (model->material.texNumber != -1) {
                        textureLoaded = true;
                    }
                }
            }
        }

    }

    if (!textureLoaded) { model->material.texNumber = TexManager::LoadTexture(baseDir + defaultTexName); }
}

void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dst->r[i].m128_f32[j] = float(src.Get(i, j));
        }
    }
}

FbxLoader::~FbxLoader()
{
}

void FbxLoader::ParseSkin(fbxModel* model, FbxMesh* fbxMesh)
{
    FbxSkin* skin = static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));

    if (skin == nullptr) {
        for (int i = 0; i < model->vertices.size(); i++) {
            model->vertices[i].boneIndex[0] = 0;
            model->vertices[i].boneWeight[0] = 1.0f;
        }
        return;
    }

    std::vector<Bone>& bones = model->bones;

    int clusterCount = skin->GetClusterCount();
    bones.reserve(clusterCount);

    //すべてのボーン
    for (int i = 0; i < clusterCount; i++) {
        //i本目のボーン
        FbxCluster* fbxCluster = skin->GetCluster(i);
        //ボーンのノードの名前
        const char* boneName = fbxCluster->GetLink()->GetName();

        bones.emplace_back(Bone(boneName));

        Bone& bone = bones.back();

        bone.fbxCluster = fbxCluster;

        FbxAMatrix fbxMat;
        fbxCluster->GetTransformLinkMatrix(fbxMat);

        XMMATRIX initialPose;
        ConvertMatrixFromFbx(&initialPose, fbxMat);

        bone.invInitialBone = XMMatrixInverse(nullptr, initialPose);

        struct WeightSet {
            UINT index;
            float weight;
        };

        //二次元配列（vector：全制御点　list:頂点が影響を受けるボーンのリスト）
        std::vector<std::list<WeightSet>> weightLists(fbxMesh->GetControlPointsCount());

        //i番ボーン
        for (int i = 0; i < clusterCount; i++) {
            FbxCluster* fbxCluster = skin->GetCluster(i);
            //制御点インデックスとウェイト配列の総数
            int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();

            model->ctrlPointIndicesCount = controlPointIndicesCount;

            //制御点インデックス配列
            int* controlPointIndices = fbxCluster->GetControlPointIndices();
            //制御点のウェイト配列
            double* controlPointWeights = fbxCluster->GetControlPointWeights();

            //影響を受けるすべての制御点
            for (int j = 0; j < controlPointIndicesCount; j++) {
                //制御点j番のインデックスとウェイトを取得
                int vertIndex = controlPointIndices[j];
                float weight = (float)controlPointWeights[j];

                //制御点j番にi番ボーン、ウェイトを格納
                weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i,weight });
            }

            //影響を受けるすべての頂点について
            //for (int v = 0; v < model->vertices.size(); v++) {
            //    //v番目の頂点のインデックス番号を取得
            //    int vIndexNum = model->vertices[v].controlPointIndex;
            //    //制御点v番のインデックスとウェイトを取得
            //    int vertIndex = controlPointIndices[vIndexNum];
            //    float weight = (float)controlPointWeights[vIndexNum];

            //    //vertIndex番の頂点ウェイトリストに、ボーンインデックスiとウェイト値を格納
            //    weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i,weight });
            //}

            auto& vertices = model->vertices;

            //全頂点について
            for (int i = 0; i < vertices.size(); i++) {
                //頂点i番の制御点インデックスのウェイトリストを取得
                auto& weightList = weightLists[vertices[i].controlPointIndex];

                //もっとも影響度の高いボーン4つに絞る
                weightList.sort([](auto const& l, auto const& r) {
                    return l.weight > r.weight;
                    });

                int weightArrayIndex = 0;

                //ソート済のウェイトリストを
                for (auto& weightSet : weightList) {
                    //頂点に書き込む
                    //このとき参照するインデックスは
                    vertices[i].boneIndex[weightArrayIndex] = weightSet.index;
                    vertices[i].boneWeight[weightArrayIndex] = weightSet.weight;

                    if (++weightArrayIndex >= fbxModel::BONE_INDICES_MAX) {
                        float weight = 0.0f;

                        for (int j = 1; j < fbxModel::BONE_INDICES_MAX; j++) {
                            weight += vertices[i].boneWeight[j];
                        }

                        vertices[i].boneWeight[0] = 1.0f - weight;
                        break;
                    }
                }
            }
        }
    }
}

void FbxLoader::SaveAnimationData(fbxModel* model, FbxMesh* fbxMesh)
{
    ////アニメーション総数分ループ
    //int animationDataCount = fbxImporter->GetAnimStackCount();

    //for (int i = 0; i < animationDataCount; i++) {
    //    //アニメーション情報を取得する
    //    FbxTakeInfo* takeInfo = fbxImporter->GetTakeInfo(i);

    //    //基準点からの差を取得
    //    auto importOffset = takeInfo->mImportOffset;
    //    auto startTime = takeInfo->mLocalTimeSpan.GetStart();
    //    auto stopTime = takeInfo->mLocalTimeSpan.GetStop();	

    //    float start = (importOffset.Get() + startTime.Get()) / FbxTime::GetOneFrameValue(FbxTime::eFrames60);	//60フレームでの開始時間を設定
    //    float end = (importOffset.Get() + stopTime.Get()) / FbxTime::GetOneFrameValue(FbxTime::eFrames60);	//60フレームでの終了時間を設定

    //    SkinAnimationPlayInfo info{};
    //    info.start = start;
    //    info.end = end;
    //    info.name = takeInfo->mName;

    //    model->animinfo.push_back(info);
    //}

}
