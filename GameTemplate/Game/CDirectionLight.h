#pragma once

namespace prefab
{
	class CDirectionLight : public IGameObject
	{
	private:
		struct LigData
		{
			Vector3 ligDir;
			float pad0;
			Vector3 ligColor;

		};

		ConstantBuffer cb;
		DescriptorHeap ds;
		LigData ligData;
	public:
		bool Start();
		void Update();

		//定数バッファ周りのテスト
		LigData* GetLigData() { return &ligData; }


		void SetDirection(Vector3 dir) { ligData.ligDir = dir; }
		void SetColor(Vector3 color) { ligData.ligColor = color; }
	};
}

