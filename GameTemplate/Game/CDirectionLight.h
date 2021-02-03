#pragma once

namespace prefab
{
	struct DirLigData
	{
		Vector3 ligDir;
		float pad0;
		Vector3 ligColor;
		float pad1;
		Vector3 eyePos;
		float pad2;
	};

	class CDirectionLight : public IGameObject
	{
	private:

		//ConstantBuffer cb;
		//DescriptorHeap ds;
		DirLigData dirLigData;

		int m_lightNum;
	public:
		~CDirectionLight();
		bool Start();
		void Update();

		//定数バッファ周りのテスト
		DirLigData* GetLigData() { return &dirLigData; }
		int GetLigDataSize() { return sizeof(dirLigData); }

		void SetDirection(Vector3 dir) { dirLigData.ligDir = dir; }
		Vector3 GetDirecion() { return dirLigData.ligDir; }

		void SetColor(Vector3 color) { dirLigData.ligColor = color; }
		Vector3 GetColor() { return dirLigData.ligColor; }
	};
}

