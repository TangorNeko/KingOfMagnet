#pragma once

namespace prefab
{
	struct SpotLigData //スポットライトのデータ
	{
		Vector3 ligPos;		//ライトの座標
		float pad;			//パディング
		Vector3 ligColor;	//ライトの色
		float ligRange;		//ライトの影響範囲
		Vector3 ligDir;		//ライトの向き
		float ligAngle;		//ライトの影響角度(ラジアン単位)
	};

	class CSpotLight : public IGameObject
	{
		~CSpotLight();
		bool Start();
		void Update();

	public:
		/**
		 * @brief スポットライトのデータを取得
		 * @return スポットライトのデータ
		*/
		SpotLigData* GetLigData() { return &m_spotLigData; }

		/**
		 * @brief スポットライトのデータのサイズを取得
		 * @return スポットライトのデータのサイズ
		*/
		int GetLigDataSize() { return sizeof(SpotLigData); }

		/**
		 * @brief スポットライトの座標を設定
		 * @param pos スポットライトの座標
		*/
		void SetPosition(Vector3 pos) { m_spotLigData.ligPos = pos; }

		/**
		 * @brief スポットライトの座標を取得
		 * @return スポットライトの座標
		*/
		Vector3 GetPosition() { return m_spotLigData.ligPos; }

		/**
		 * @brief スポットライトの色を設定
		 * @param color スポットライトの色
		*/
		void SetColor(Vector3 color) { m_spotLigData.ligColor = color; }

		/**
		 * @brief スポットライトの色を取得
		 * @return スポットライトの色
		*/
		Vector3 GetColor() { return m_spotLigData.ligColor; }

		/**
		 * @brief スポットライトの影響範囲を設定
		 * @param range スポットライトの影響範囲
		*/
		void SetRange(float range) { m_spotLigData.ligRange = range; }

		/**
		 * @brief スポットライトの影響範囲を取得
		 * @return スポットライトの影響範囲
		*/
		float GetRange() { return m_spotLigData.ligRange; }

		/**
		 * @brief スポットライトの向きを設定
		 * @param dir スポットライトの向き
		*/
		void SetDirection(Vector3 dir) { m_spotLigData.ligDir = dir; }

		/**
		 * @brief スポットライトの向きを取得
		 * @return スポットライトの向き
		*/
		Vector3 GetDirection() { return m_spotLigData.ligDir; }

		/**
		 * @brief スポットライトの影響角度を設定(ラジアン単位)
		 * @param angle_rad スポットライトの影響角度
		*/
		void SetAngle(float angle_rad) { m_spotLigData.ligAngle = angle_rad; }

		/**
		 * @brief スポットライトの影響角度を取得(ラジアン単位)
		 * @return スポットライトの影響角度
		*/
		float GetAngle() { return m_spotLigData.ligAngle; }

		/**
		 * @brief スポットライトの影響角度を設定(デグリー単位)
		 * @param angle_deg スポットライトの影響角度
		*/
		void SetAngleDeg(float angle_deg) { m_spotLigData.ligAngle = Math::DegToRad(angle_deg); }

		/**
		 * @brief スポットライトの影響角度を取得(デグリー単位)
		 * @return スポットライトの影響角度
		*/
		float GetAngleDeg() { return Math::RadToDeg(m_spotLigData.ligAngle); }

	private:
		SpotLigData m_spotLigData;	//スポットライトのデータ
		int m_spotLigTag;			//スポットライトのタグ(何番目に作られたライト?)
	};
}

