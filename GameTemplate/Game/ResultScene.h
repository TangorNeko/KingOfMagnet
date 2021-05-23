#pragma once
class ResultScene:public IGameObject
{
	bool Start();
	void Update();
public:
	~ResultScene();
	void ResultDisplay();
	prefab::CFontRender* m_title_fontRender = nullptr;//タイトル。
	prefab::CFontRender* m_replay_fontRender = nullptr;//もう一度遊ぶ
	prefab::CFontRender* m_detail_fontRender = nullptr;//詳細表示
	prefab::CFontRender* m_result_fontRender = nullptr;//詳細内容
	int m_menuselect = 1;//項目選択
	const Vector4 red = { 1,0,0,1 };//色
	const Vector4 blue = { 0,0,1,1 };
	float m_titleX = 0.0f;
	float m_titleY = 100.0f;
	float m_replayX = 0.0f;
	float m_replayY = 0.0f;
	float m_detailX = 0.0f;	
	float m_detailY = -100.0f;
};

