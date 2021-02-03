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
			float pad1;
			Vector3 eyePos;
			float pad2;
		};

		//ConstantBuffer cb;
		//DescriptorHeap ds;
		LigData ligData[2];
	public:
		bool Start();
		void Update();

		//定数バッファ周りのテスト
		void* GetLigData() { return &ligData; }
		int GetLigDataSize() { return sizeof(ligData[0]); }

		void SetDirection(Vector3 dir1, Vector3 dir2) { ligData[0].ligDir = dir1;ligData[1].ligDir = dir2; }
		//Vector3 GetDirecion() { return ligData.ligDir; }

		void SetColor(Vector3 color1, Vector3 color2) { ligData[0].ligColor = color1;ligData[1].ligColor = color2; }
		//Vector3 GetColor() { return ligData.ligColor; }
	};
}

