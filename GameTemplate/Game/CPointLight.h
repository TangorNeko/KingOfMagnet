#pragma once

namespace prefab
{
	class CPointLight : public IGameObject
	{
	private:
		struct LigData
		{
			Vector3 ligPos;
			float pad;
			Vector3 ligColor;
			float ligRange;
		};

		LigData ligData;
	public:
		bool Start();
		void Update();

		//�萔�o�b�t�@����̃e�X�g
		LigData* GetLigData() { return &ligData; }
		int GetLigDataSize() { return sizeof(LigData); }

		void SetPosition(Vector3 pos) { ligData.ligPos = pos; }
		Vector3 GetPosition() { return ligData.ligPos; }

		void SetColor(Vector3 color) { ligData.ligColor = color; }
		Vector3 GetColor() { return ligData.ligColor; }

		void SetRange(float range) { ligData.ligRange = range; }
		float GetRange() { return ligData.ligRange; }
	};
}
