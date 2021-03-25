#include "stdafx.h"
#include "Model.h"
#include "Material.h"

void Model::Init(const ModelInitData& initData)
{
	MY_ASSERT(
		initData.m_fxFilePath, 
		"error : initData.m_fxFilePathが指定されていません。"
	);
	MY_ASSERT(
		initData.m_tkmFilePath,
		"error : initData.m_tkmFilePathが指定されていません。"
	);
	//内部のシェーダーをロードする処理が求めているのが
	//wchar_t型の文字列なので、ここで変換しておく。
	wchar_t wfxFilePath[256] = {L""};
	if (initData.m_fxFilePath != nullptr) {
		//MessageBoxA(nullptr, "fxファイルパスが指定されていません。", "エラー", MB_OK);
		//std::abort();
		mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);
	}
	
	if (initData.m_skeleton != nullptr) {
		//スケルトンが指定されている。
		m_meshParts.BindSkeleton(*initData.m_skeleton);
	}
	
	m_modelUpAxis = initData.m_modelUpAxis;

	m_tkmFile = tkmFileManager::GetInstance()->GetTkmFile(initData.m_tkmFilePath);
	
	if (m_tkmFile == nullptr)
	{
		m_tkmFile->Load(initData.m_tkmFilePath);

		m_meshParts.InitFromTkmFile(
			*m_tkmFile,
			wfxFilePath,
			initData.m_vsEntryPointFunc,
			initData.m_vsSkinEntryPointFunc,
			initData.m_psEntryPointFunc,
			initData.m_expandConstantBuffer,
			initData.m_expandConstantBufferSize,
			initData.m_expandShaderResoruceView
		);
	}
	else
	{
		
		//TkmFileはoperator=が定義されていないので代入できない。
		//m_tkmFile = initData.m_tkmFile;
		m_meshParts.InitFromTkmFile(
			*m_tkmFile,
			wfxFilePath,
			initData.m_vsEntryPointFunc,
			initData.m_vsSkinEntryPointFunc,
			initData.m_psEntryPointFunc,
			initData.m_expandConstantBuffer,
			initData.m_expandConstantBufferSize,
			initData.m_expandShaderResoruceView
		);
		
	}

	UpdateWorldMatrix(g_vec3Zero, g_quatIdentity, g_vec3One);
	
}

void Model::UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
{
	Matrix mBias;
	if (m_modelUpAxis == enModelUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(Math::PI * -0.5f);
	}
	Matrix mTrans, mRot, mScale;
	mTrans.MakeTranslation(pos);
	mRot.MakeRotationFromQuaternion(rot);
	mScale.MakeScaling(scale);
	m_world = mBias * mScale * mRot * mTrans;
}

void Model::UpdateWorldMatrix(Matrix world)
{
	m_world = world;
}

void Model::ChangeAlbedoMap(const char* materialName, Texture& albedoMap)
{
	m_meshParts.QueryMeshs([&](const SMesh& mesh) {
		//todo マテリアル名をtkmファイルに出力したなかった・・・。
		//todo 今は全マテリアル差し替えます
		for (Material* material : mesh.m_materials) {
			material->GetAlbedoMap().InitFromD3DResource(albedoMap.Get());
		}
	});
	//ディスクリプタヒープの再作成。
	m_meshParts.CreateDescriptorHeaps();
	
}
void Model::Draw(RenderContext& rc,Camera* camera)
{
	m_meshParts.Draw(
		rc, 
		m_world, 
		camera->GetViewMatrix(), 
		camera->GetProjectionMatrix()
	);
}

bool Model::isLineHitModel(const Vector3& start, const Vector3& end,Vector3& minCrossPoint)
{
	const auto& meshParts = m_tkmFile->GetMeshParts();

	bool isHit = false;
	float distance = FLT_MAX;

	for (const auto& mesh : meshParts)
	{
		//まずは16ビット版から。
		for (const auto& indexBuffer : mesh.indexBuffer16Array)
		{
			//インデックスの数からポリゴンの数を計算する。
			int numPolygon = indexBuffer.indices.size() / 3;
			for (int polygonNo = 0;polygonNo < numPolygon;polygonNo++)
			{
				Vector3 vertexPosition[3];
				//ポリゴンを構成する頂点番号をインデックスバッファから取得する。
				int vertexNo_0 = indexBuffer.indices[polygonNo * 3 + 0];
				int vertexNo_1 = indexBuffer.indices[polygonNo * 3 + 1];
				int vertexNo_2 = indexBuffer.indices[polygonNo * 3 + 2];

				vertexPosition[0] = mesh.vertexBuffer[vertexNo_0].pos;
				vertexPosition[1] = mesh.vertexBuffer[vertexNo_1].pos;
				vertexPosition[2] = mesh.vertexBuffer[vertexNo_2].pos;

				m_world.Apply(vertexPosition[0]);
				m_world.Apply(vertexPosition[1]);
				m_world.Apply(vertexPosition[2]);


				//ここから線との接触判定
				//1.ポリゴンを含む無限平面との交差判定

				//3つの頂点からm_vertex[0]から伸びる法線を求める
				Vector3 v1 = vertexPosition[1] - vertexPosition[0];
				Vector3 v2 = vertexPosition[2] - vertexPosition[0];

				Vector3 normal;
				normal.Cross(v1, v2);
				normal.Normalize();

				//法線と、m_vertex[0]から判定する線分の開始地点と終了地点で内積をとる。
				Vector3 toStart = start - vertexPosition[0];
				toStart.Normalize();

				Vector3 toEnd = end - vertexPosition[0];
				toEnd.Normalize();

				//内積の結果が0以上なら交差していないので次のループへ。
				if (normal.Dot(toStart) * normal.Dot(toEnd) > 0)
				{
					continue;
				}

				//2.交差している座標を求める
				toStart = start - vertexPosition[0];

				toEnd = end - vertexPosition[0];

				Vector3 test = start - end;

				float a = normal.Dot(toStart);

				normal *= -1;

				float b = normal.Dot(toEnd);

				Vector3 crossPoint = toStart - toEnd;
				crossPoint *= b / (a + b);
				crossPoint += end;

				//3.交点が三角形の中にあるかどうかの判定

				Vector3 vA = vertexPosition[1] - vertexPosition[0];
				Vector3 vB = vertexPosition[2] - vertexPosition[1];
				Vector3 vC = vertexPosition[0] - vertexPosition[2];
				Vector3 vD = crossPoint - vertexPosition[0];
				Vector3 vE = crossPoint - vertexPosition[1];
				Vector3 vF = crossPoint - vertexPosition[2];

				vA.Cross(vD);
				vA.Normalize();
				vB.Cross(vE);
				vB.Normalize();
				vC.Cross(vF);
				vC.Normalize();

				if (vA.Dot(vB) > 0 && vA.Dot(vC) > 0)
				{
					//1回でも接触している。
					isHit = true;

					//接触しているので、スタートとの距離を求める
					Vector3 dist = crossPoint - start;

					//現在の最長距離よりも短いなら更新。
					if (dist.Length() < distance)
					{
						minCrossPoint = crossPoint;
						distance = dist.Length();
					}
				}
				else
				{
					//交点が三角形の中にない。
					continue;
				}
			}
		}
	}
	return isHit;
}
