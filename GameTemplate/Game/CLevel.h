#pragma once
#include "tklFile.h"
#include "CMapChipRender.h"

namespace prefab
{
	//レベルのオブジェクト一つ一つのデータ
	struct LevelObjectData
	{
		Vector3 position = Vector3::Zero;
		Quaternion rotation = Quaternion::Identity;
		Vector3 scale = Vector3::One;
		char* name = nullptr;
	};

	class CLevel
	{
	private:
		//読み込んだtklファイルのデータが格納される。
		tklFile m_tklFile;
		
		//ボーンのユニークポインタ。
		typedef std::unique_ptr<Bone> CBone;

		//ボーンのユニークポインタの可変長配列
		std::vector<CBone> m_bone;

		//行列のユニークポインタ。
		std::unique_ptr<Matrix[]> m_boneMatrixs;

		//初期化が完了したか?
		bool m_isInited = false;

		//ボーンを作ってm_boneに渡す
		void BuildBone();

		std::map<unsigned int, CMapChipRender*> m_mapChipRenderPtrs;
	public:
		~CLevel();

		//レベルの初期化
		bool Init(const char* levelFilePath,std::function<bool(LevelObjectData& objectData)> hookFunc);

		//
		CMapChipRender* CreateMapChipRenderOrAddRenderObject(const LevelObjectData& objData);
	};
}

