#pragma once
class ResultScene:public IGameObject
{
	bool Start();
	void Update();
	~ResultScene();

public:

	/**
	 * @brief �s�҂̔ԍ���ݒ�
	 * @param playerNum �s�҂̃v���C���[�ԍ�
	*/
	void SetLoserNum(int playerNum)
	{
		m_loserNum = playerNum;
	}

	enum {
		enAnimationClip_Win,
		enAnimationClip_Lose,
		enAnimationClip_Num,
	};

private:
	AnimationClip m_animationClips[enAnimationClip_Num];		//�L�����N�^�[�̃A�j���[�V����
	prefab::CSkinModelRender* m_P1_skinModelRender = nullptr;	//�v���C���[1�̃��f��
	prefab::CSkinModelRender* m_P2_skinModelRender = nullptr;	//�v���C���[2�̃��f��
	prefab::CDirectionLight* m_directionLight = nullptr;		//���҂��Ƃ炷���C�g
	prefab::CSpriteRender* m_Down_SpriteRender = nullptr;		//�����̃V���b�^�[�̃X�v���C�g
	prefab::CSpriteRender* m_Up_SpriteRender = nullptr;			//�㑤�̃V���b�^�[�̃X�v���C�g
	prefab::CSpriteRender* m_Under_SpriteRender = nullptr;		//�v���C���[��\�����鑋�̃X�v���C�g
	prefab::CSpriteRender* m_Lose_SpriteRender = nullptr;		//LOSE�̃X�v���C�g
	prefab::CSpriteRender* m_Win_SpriteRender = nullptr;		//WIN�̃X�v���C�g
	prefab::CSpriteRender* m_Command_SpriteRender = nullptr;	//���g���C�A�^�C�g���ւ̃X�v���C�g
	prefab::CSpriteRender* m_BG_SpriteRender = nullptr;			//�w�i�̃X�v���C�g
	bool m_RetryOn = true;										//���g���C�̃R�}���h�I��?(false�Ȃ�^�C�g���ւ̃R�}���h�I��)
	int m_loserNum = 0;											//�s�҂̃v���C���[�ԍ�
	bool m_moveEndFlag = false;									//�ړ�����
	int m_moveTimer = 0;										//�X�v���C�g�̈ړ��^�C�}�[
	bool m_win_lose_MoveFlag = false;							//Win�̕������g��k��������?
	prefab::CSoundSource* m_resultBGM = nullptr;				//���U���gBGM
};

