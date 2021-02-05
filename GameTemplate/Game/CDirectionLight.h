#pragma once

namespace prefab
{
	struct DirLigData
	{
		Vector3 ligDir;		//ライトの向き
		float pad0;			//パディング
		Vector3 ligColor;	//ライトの色
		float pad1;			//パディング(単体だと必要ないがこれがないとDirLigDataを配列にした際HLSLで4バイトずつずれていくことになる?)
	};

	class CDirectionLight : public IGameObject
	{
	private:
		DirLigData dirLigData;

		int m_dirLigNum;
	public:
		~CDirectionLight();
		bool Start();
		void Update();

		//ディレクションライトのデータを取得
		DirLigData* GetLigData() { return &dirLigData; }
		
		//ディレクションライトのサイズを取得
		int GetLigDataSize() { return sizeof(dirLigData); }

		//向きの設定と取得
		void SetDirection(Vector3 dir) { dirLigData.ligDir = dir; }
		Vector3 GetDirecion() { return dirLigData.ligDir; }

		//色の設定と取得
		void SetColor(Vector3 color) { dirLigData.ligColor = color; }
		Vector3 GetColor() { return dirLigData.ligColor; }
	};
}

