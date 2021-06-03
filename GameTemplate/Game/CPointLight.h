#pragma once

namespace prefab
{
	struct PointLigData
	{
		Vector3 ligPos;		//ライトの座標
		float pad;			//パディング
		Vector3 ligColor;	//ライトの色
		float ligRange;		//ライトの影響範囲
	};

	class CPointLight : public IGameObject
	{
		~CPointLight();
		bool Start();
		void Update();

	public:
		/**
		 * @brief ポイントライトのデータを取得
		 * @return ポイントライトのデータ
		*/
		PointLigData* GetLigData() { return &m_pointLigData; }

		/**
		 * @brief ポイントライトのデータのサイズを取得
		 * @return ポイントライトのデータのサイズ
		*/
		int GetLigDataSize() { return sizeof(PointLigData); }

		/**
		 * @brief ポイントライトの座標を設定
		 * @param pos ポイントライトの座標
		*/
		void SetPosition(Vector3 pos) { m_pointLigData.ligPos = pos; }

		/**
		 * @brief ポイントライトの座標を取得
		 * @return ポイントライトの座標 
		*/
		Vector3 GetPosition() { return m_pointLigData.ligPos; }

		/**
		 * @brief ポイントライトの色を設定
		 * @param color ポイントライトの色
		*/
		void SetColor(Vector3 color) { m_pointLigData.ligColor = color; }

		/**
		 * @brief ポイントライトの色を取得
		 * @return ポイントライトの色
		*/
		Vector3 GetColor() { return m_pointLigData.ligColor; }

		/**
		 * @brief ポイントライトの影響範囲を設定
		 * @param range ポイントライトの影響範囲
		*/
		void SetRange(float range) { m_pointLigData.ligRange = range; }

		/**
		 * @brief ポイントライトの影響範囲を取得
		 * @return ポイントライトの影響範囲
		*/
		float GetRange() { return m_pointLigData.ligRange; }

	private:
		PointLigData m_pointLigData;	//ポイントライトのデータ
		int m_pointLigTag;			//ポイントライトのタグ(何番目に作られたライト?)
	};
}
