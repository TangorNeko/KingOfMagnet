#include "stdafx.h"
#include "CLevel.h"

namespace prefab
{
	CLevel::~CLevel()
	{
		for (auto mapChipRender : m_mapChipRenderPtrs)
		{
			DeleteGO(mapChipRender);
		}
	}

	bool CLevel::Init(const char* levelFilePath, std::function<bool(LevelObjectData& objectData)> hookFunc)
	{
		m_tklFile.Load(levelFilePath);

		BuildBone();

		for (auto i = 1; i < m_bone.size();i++)
		{
			//骨を取得
			auto bone = m_bone[i].get();

			if (bone->GetParentBoneNo() == 0)
			{
				LevelObjectData objData;

				//objDataの各メンバにボーンから値を入れる
				//座標、回転、拡大率
				bone->CalcWorldTRS(objData.position, objData.rotation, objData.scale);

				//名前。
				objData.name = m_tklFile.m_objects[i].boneName.get();

				//3dsMaxとは軸が違うので、補正を入れる。
				float t = objData.position.y;
				objData.position.y = objData.position.z;
				objData.position.z = -t;

				t = objData.rotation.y;
				objData.rotation.y = objData.rotation.z;
				objData.rotation.z = -t;

				std::swap(objData.scale.y, objData.scale.z);

				auto isHook = false;

				if (hookFunc != nullptr)
				{
					//hook関数が指定されているのでHook関数を呼び出す。
					isHook = hookFunc(objData);
				}

				if (isHook == false)
				{
					//マップチップレンダーを作成する。
					CMapChipRender* pMapChipRender = NewGO<CMapChipRender>(0);
					//ボーンから取得したオブジェクトのデータを取得
					pMapChipRender->AddRenderObject(objData);
					//レベルのマップチップレンダーの可変長配列に追加
					m_mapChipRenderPtrs.push_back(pMapChipRender);
				}
			}
		}

		//すべてのマップチップレンダーを初期化する
		for (auto& mapChipRender : m_mapChipRenderPtrs)
		{
			mapChipRender->Init();
		}
		return true;
	}

	void CLevel::BuildBone()
	{
		m_tklFile.QueryObject([&](tklFile::SObject& tklObj)
			{
				//バインドポーズの行列
				//初期のスケルトンのポーズ、TポーズやAポーズみたいなものらしい。
				Matrix bindPoseMatrix;
				memcpy(bindPoseMatrix.m[0], &tklObj.bindPose[0], sizeof(tklObj.bindPose[0]));
				memcpy(bindPoseMatrix.m[1], &tklObj.bindPose[1], sizeof(tklObj.bindPose[1]));
				memcpy(bindPoseMatrix.m[2], &tklObj.bindPose[2], sizeof(tklObj.bindPose[2]));
				memcpy(bindPoseMatrix.m[3], &tklObj.bindPose[3], sizeof(tklObj.bindPose[3]));

				bindPoseMatrix.m[0][3] = 0.0f;
				bindPoseMatrix.m[1][3] = 0.0f;
				bindPoseMatrix.m[2][3] = 0.0f;
				bindPoseMatrix.m[3][3] = 1.0f;

				//バインドポーズの逆行列。
				Matrix invBindPoseMatrix;
				memcpy(invBindPoseMatrix.m[0], &tklObj.invBindPose[0], sizeof(tklObj.invBindPose[0]));
				memcpy(invBindPoseMatrix.m[1], &tklObj.invBindPose[1], sizeof(tklObj.invBindPose[1]));
				memcpy(invBindPoseMatrix.m[2], &tklObj.invBindPose[2], sizeof(tklObj.invBindPose[2]));
				memcpy(invBindPoseMatrix.m[3], &tklObj.invBindPose[3], sizeof(tklObj.invBindPose[3]));
				invBindPoseMatrix.m[0][3] = 0.0f;
				invBindPoseMatrix.m[1][3] = 0.0f;
				invBindPoseMatrix.m[2][3] = 0.0f;
				invBindPoseMatrix.m[3][3] = 1.0f;

				//ボーン名。
				wchar_t boneName[256];

				//tklのオブジェクト名の型をワイド文字に変換。
				mbstowcs(boneName, tklObj.boneName.get(), 256);

				//ボーンを作成
				CBone bone = std::make_unique<Bone>(
					boneName,
					bindPoseMatrix,
					invBindPoseMatrix,
					tklObj.parentBoneNo,
					tklObj.boneNo
					);

				//作成したボーンを追加。(ムーブ)
				m_bone.push_back(std::move(bone));


				//ボーンの計算?
				for (auto& bone : m_bone)
				{
					if (bone->GetParentBoneNo() != -1)
					{
						m_bone.at(bone->GetParentBoneNo())->AddChild(bone.get());
						//ローカルマトリクス計算
						const Matrix& parentMatrix = m_bone.at(bone->GetParentBoneNo())->GetInvBindPoseMatrix();
						Matrix localMatrix;
						//平行移動行列はかき消す。
						localMatrix = bone->GetBindPoseMatrix() * parentMatrix;
						bone->SetLocalMatrix(localMatrix);
					}
					else
					{
						//これ以上親がいない。
						bone->SetLocalMatrix(bone->GetBindPoseMatrix());
					}
				}

				//ボーン行列を確保
				m_boneMatrixs = std::make_unique<Matrix[]>(m_bone.size());

				//初期化終わり。
				m_isInited = true;
			}
	);
	}
}