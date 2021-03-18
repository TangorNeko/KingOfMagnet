#pragma once
class TitleScene:public IGameObject
{
	bool Start() override;
	void Update() override;
public:
	~TitleScene();
	prefab::CFontRender* m_start_fontRender = nullptr;//�n�߂�
	prefab::CFontRender* m_description_fontRender = nullptr;//�������
	prefab::CFontRender* m_option_fontRender = nullptr;//�I�v�V����
	int m_menuselect = 0;//���ڑI��
	const Vector4 red = { 1,0,0,1 };
	const Vector4 blue = { 0,0,1,1 };

};

