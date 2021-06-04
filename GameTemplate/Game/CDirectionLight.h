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
		~CDirectionLight();
		bool Start();
		void Update();

	public:
		/**
		 * @brief ディレクションライトのデータを取得
		 * @return ディレクションライトのデータ
		*/
		DirLigData* GetLigData() { return &m_dirLigData; }
		
		/**
		 * @brief ディレクションライトのデータのサイズを取得
		 * @return ディレクションライトのデータのサイズ
		*/
		int GetLigDataSize() { return sizeof(m_dirLigData); }

		/**
		 * @brief ディレクションライトの向きを設定
		 * @param dir ディレクションライトの向き
		*/
		void SetDirection(Vector3 dir) { 
			m_dirLigData.ligDir = dir;
			m_dirLigData.ligDir.Normalize();
		}

		/**
		 * @brief ディレクションライトの向きを取得
		 * @return ディレクションライトの向き
		*/
		Vector3 GetDirecion() { return m_dirLigData.ligDir; }

		/**
		 * @brief ディレクションライトの色を設定
		 * @param color ディレクションライトの色
		*/
		void SetColor(Vector3 color) { m_dirLigData.ligColor = color; }

		/**
		 * @brief ディレクションのライトの色を取得
		 * @return ディレクションライトの色
		*/
		Vector3 GetColor() { return m_dirLigData.ligColor; }

	private:
		DirLigData m_dirLigData;	//ディレクションライトのデータ
		int m_dirLigTag;			//ディレクションライトの番号(何番目に作られたライト?)
	};
}

