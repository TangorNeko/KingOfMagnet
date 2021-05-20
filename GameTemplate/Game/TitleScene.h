#pragma once
class TitleScene:public IGameObject
{
	bool Start() override;
	void Update() override;
public:
	~TitleScene();
	prefab::CFontRender* m_start_fontRender = nullptr;//始める
	prefab::CFontRender* m_exit_fontRender = nullptr;//操作説明
	prefab::CFontRender* m_option_fontRender = nullptr;//オプション
	prefab::CFontRender* m_dot_fontRender = nullptr;//オプション

	prefab::CSpriteRender* m_titleSpriteRender = nullptr;

	int m_menuselect = 1;//項目選択
	const Vector4 red = { 1,0,0,1 };//色
	const Vector4 blue = { 0,0,1,1 };
	/*Vector2 m_option={};
	Vector2 m_start={};
	Vector2 m_exit={};*/
	float m_optionX = 400.0f;
	float m_startX = 400.0f;
	float m_exitX = 400.0f;
	float m_optionY = 100.0f;
	float m_startY = 0.0f;
	float m_exitY = -100.0f;
};

