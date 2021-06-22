#pragma once
class SkyBoard : public IGameObject
{
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc, Camera* camera)override;

private:
	//空のデータ(シェーダーに渡される)
	struct SkyData
	{
		float textureSize;//テクスチャのサイズ(正方形を想定しているのでfloat。)
		float scrollingValue = 0;//スクロール量
	};

	Model m_skyModel[2];	//空のモデル、2画面用に2つ用意する。
	SkyData m_skyData;		//シェーダーに渡される空のデータ
};

