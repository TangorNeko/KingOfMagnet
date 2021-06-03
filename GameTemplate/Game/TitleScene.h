#pragma once

class GameOption;
class TitleScene:public IGameObject
{
	bool Start() override;
	void Update() override;
	~TitleScene();

	/**
	 * @brief �I�����Ă���R�}���h�ɉ����ă��j���[�o�[���ړ�������
	*/
	void CommandMove();

	/**
	 * @brief �R�}���h�����肵�����̓���
	*/
	void CommandSelectMove();

private:
	prefab::CSkinModelRender* m_BG_ModelRender = nullptr;
	prefab::CPointLight* m_BGLight = nullptr;
	prefab::CSpriteRender* m_Monitor_SpriteRender = nullptr;
	prefab::CSpriteRender* m_MonitorLine_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Rogo_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Option_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Start_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Exit_SpriteRender = nullptr;
	prefab::CSpriteRender* m_Arrow_SpriteRender = nullptr;

	//���j���[�o�[�̈ʒu
	Vector3 m_topPos = { 440.0f,200.0f,0.0f };//��
	Vector3 m_centerPos = { 320.0f,0.0f,0.0f };//��
	Vector3 m_bottomPos = { 440.0f,-200.0f,0.0f };//��

	//�I�v�V�������
	GameOption* m_option = nullptr;

	//BGM
	prefab::CSoundSource* ssBGM = nullptr;

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

