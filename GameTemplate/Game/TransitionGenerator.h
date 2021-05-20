#pragma once

/// <summary>
/// �g�����W�V���������s
/// �E�V���O���g��
/// </summary>
class TransitionGenerator : public IGameObject
{
	static TransitionGenerator* m_instance;
public:
	//�g�����W�V�������Ăяo���Ƃ��̂����O
	enum TransitionName {
		UnderSlide,	//������t�F�[�h
		NanameBox,	//�i�i���{�b�N�X
		Mesh,		//���b�V��
		Circle,		//�~
		TransitionName_Nom	//�f�[�^��
	};

	TransitionGenerator();
	~TransitionGenerator();
	void Update()override;

	/// <summary>
	/// �C���X�^���X���擾�V���[�Y
	/// </summary>
	/// <returns>�C���X�^���X�ł�</returns>
	static TransitionGenerator* GetInstance() {
		return m_instance;
	}

	//�g�����W�V�����̎��s
	void TransitionInit(
		const TransitionName& name,
		const int& MoveTime,
		const bool& mode
	);

private:
	//Sprite m_transitionSprite;
	prefab::CSpriteRender* m_transitionSpriteRender = nullptr;

	struct Transition_Data {		//�X�v���C�g�p�^�[���f�[�^�\����
		const char* SpriteName;		//�X�v���C�g�t�@�C����
	};

	//�g�����W�V�����̑f�ރX�v���C�g�ꗗ
	Transition_Data Transition_DataList[TransitionName_Nom]{
		{ "Assets/Image/Transition/under.dds" },
		{ "Assets/Image/Transition/naname_box.dds" },
		{ "Assets/Image/Transition/mesh.dds" },
		{ "Assets/Image/Transition/circle_b.dds" },
	};

	//�g�����W�V�����p
	int m_transitionLimit = 0;			//�g�����W�V�������s����
	float m_borderLine = 0.0f;			//�`�悳���{�[�_�[���C��
	bool m_transitionMode = false;		//false�Ȃ�\���Atrue�Ȃ����
	bool m_nowTransitionFlag = false;	//�g�����W�V������
	const float MoveSpeed = 10.0f;		//�v�Z�p
	RenderContext* m_rc = nullptr;		//�`���
	bool m_initFlag = false;			//����ݒ�
};

