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
	private:
		PointLigData pointligData;

		int m_pointLigNum;
	public:
		~CPointLight();
		bool Start();
		void Update();

		//ポイントライトのデータを取得
		PointLigData* GetLigData() { return &pointligData; }

		//ポイントライトのサイズを取得
		int GetLigDataSize() { return sizeof(PointLigData); }

		//座標の設定と取得
		void SetPosition(Vector3 pos) { pointligData.ligPos = pos; }
		Vector3 GetPosition() { return pointligData.ligPos; }

		//色の設定と取得
		void SetColor(Vector3 color) { pointligData.ligColor = color; }
		Vector3 GetColor() { return pointligData.ligColor; }

		//影響範囲の設定と取得
		void SetRange(float range) { pointligData.ligRange = range; }
		float GetRange() { return pointligData.ligRange; }
	};
}
