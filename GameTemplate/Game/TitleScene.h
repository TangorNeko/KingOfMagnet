#pragma once

class GameOption;
class TitleScene:public IGameObject
{
	bool Start() override;
	void Update() override;
	~TitleScene();
public:
	prefab::CSkinModelRender* m_BG_ModelRender = nullptr;
	prefab::CPointLight* m_BGLight = nullptr;
	prefab::CSpriteRender* m_Monitor_SpriteRender = nullptr;
	prefab::CSpriteRender* m_MonitorLine_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Rogo_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Option_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Start_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Exit_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Arrow_SpriteRender = nullptr;

	Vector4 m_semitrans = { 1, 1, 1, 0.7f };//������
	Vector3 m_smallScale = { 0.7f,0.7f,1.0f };//�㉺�̃��j���[�o�[�̊g�嗦
	//���j���[�o�[�̈ʒu
	Vector3 m_topPos = { 440.0f,200.0f,0.0f };//��
	Vector3 m_centerPos = { 320.0f,0.0f,0.0f };//��
	Vector3 m_bottomPos = { 440.0f,-200.0f,0.0f };//��

	//�I�v�V�������
	GameOption* m_option = nullptr;

	//BGM
	prefab::CSoundSource* ssBGM = nullptr;
	
private:
	void CommandMove();
	void CommandSelectMove();

	//�R�}���h�̎��
	enum TitleCommand {
		TC_Start,
		TC_Option,
		TC_Exit
	};
	TitleCommand m_titleCommand = TC_Start;		//���ݑI�𒆂̃R�}���h
	bool m_selectFlag = false;					//�R�}���h����ρH
	bool m_selectMoveFlag = false;				//�R�}���h�ړ����H
	bool m_comandUpDown = false;				//false:�� true:��

	int m_commandTimer = 0;

};

