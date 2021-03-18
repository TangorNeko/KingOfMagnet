#pragma once
class TitleScene:public IGameObject
{
	bool Start() override;
	void Update() override;
public:
	~TitleScene();
	prefab::CFontRender* m_start_fontRender = nullptr;//始める
	prefab::CFontRender* m_description_fontRender = nullptr;//操作説明
	prefab::CFontRender* m_option_fontRender = nullptr;//オプション
	int m_menuselect = 0;//項目選択
	const Vector4 red = { 1,0,0,1 };
	const Vector4 blue = { 0,0,1,1 };

};

