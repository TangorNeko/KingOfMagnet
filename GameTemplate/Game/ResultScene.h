#pragma once
class ResultScene:public IGameObject
{
	bool Start();
	void Update();
public:
	~ResultScene();
	void ResultDisplay();
	prefab::CFontRender* m_title_fontRender = nullptr;//�^�C�g���B
	prefab::CFontRender* m_replay_fontRender = nullptr;//������x�V��
	prefab::CFontRender* m_detail_fontRender = nullptr;//�ڍו\��
	prefab::CFontRender* m_result_fontRender = nullptr;//�ڍד��e
	int m_menuselect = 1;//���ڑI��
	const Vector4 red = { 1,0,0,1 };//�F
	const Vector4 blue = { 0,0,1,1 };
	float m_titleX = 0.0f;
	float m_titleY = 100.0f;
	float m_replayX = 0.0f;
	float m_replayY = 0.0f;
	float m_detailX = 0.0f;	
	float m_detailY = -100.0f;
};

