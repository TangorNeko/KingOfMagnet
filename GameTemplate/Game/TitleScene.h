#pragma once

class GameOption;
class TitleScene:public IGameObject
{
	bool Start() override;
	void Update() override;
	~TitleScene() override;

	/**
	 * @brief �I�����Ă���R�}���h�ɉ����ă��j���[�o�[���ړ�������
	*/
	void CommandMove();

	/**
	 * @brief �R�}���h�����肵�����̓���
	*/
	void CommandSelectMove();

private:
	//�R�}���h�̎��
	enum TitleCommand {
		TC_Start,	//�X�^�[�g
		TC_Option,	//�I�v�V����
		TC_Exit		//�I��
	};
	TitleCommand m_titleCommand = TC_Start;							//���ݑI�𒆂̃R�}���h
	bool m_selectFlag = false;										//�R�}���h����ρH
	bool m_selectMoveFlag = false;									//�R�}���h�ړ����H
	bool m_comandUpDown = false;									//�R�}���h�̕����͂ǂ���? false:�� true:��
	int m_commandTimer = 0;											//�R�}���h�p�^�C�}�[
	prefab::CSkinModelRender* m_BG_ModelRender = nullptr;			//�w�i�̃��f��
	prefab::CPointLight* m_BGLight = nullptr;						//�w�i�̃��f�����Ƃ炷���C�g
	prefab::CSpriteRender* m_Monitor_SpriteRender = nullptr;		//���j�^�[�̉摜
	prefab::CSpriteRender* m_MonitorLine_SpriteRender = nullptr;	//���j�^�[�̃��C���̉摜
	prefab::CSpriteRender* m_Rogo_SpriteRender = nullptr;			//�^�C�g�����S�̉摜
	prefab::CSpriteRender* m_Option_SpriteRender = nullptr;			//�I�v�V�����̉摜
	prefab::CSpriteRender* m_Start_SpriteRender = nullptr;			//�X�^�[�g�̉摜
	prefab::CSpriteRender* m_Exit_SpriteRender = nullptr;			//�I���̉摜
	prefab::CSpriteRender* m_Arrow_SpriteRender = nullptr;			//�I��p�̖��̉摜
	prefab::CSoundSource* m_titleBGM = nullptr;						//BGM�̃T�E���h�\�[�X
	GameOption* m_option = nullptr;									//�I�v�V�������
};

