#pragma once

namespace prefab
{
	struct PointLigData
	{
		Vector3 ligPos;
		float pad;
		Vector3 ligColor;
		float ligRange;
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

		//定数バッファ周りのテスト
		PointLigData* GetLigData() { return &pointligData; }
		int GetLigDataSize() { return sizeof(PointLigData); }

		void SetPosition(Vector3 pos) { pointligData.ligPos = pos; }
		Vector3 GetPosition() { return pointligData.ligPos; }

		void SetColor(Vector3 color) { pointligData.ligColor = color; }
		Vector3 GetColor() { return pointligData.ligColor; }

		void SetRange(float range) { pointligData.ligRange = range; }
		float GetRange() { return pointligData.ligRange; }
	};
}
