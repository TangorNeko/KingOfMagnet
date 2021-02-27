#pragma once
#include "tklFile.h"
#include "CMapChipRender.h"

namespace prefab
{
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

		std::list<CMapChipRender*> m_mapChipRenderPtrs;
	public:
		~CLevel();

		//レベルの初期化
		bool Init(const char* levelFilePath,std::function<bool(LevelObjectData& objectData)> hookFunc);
	};
}

