#pragma once
class MobiusGauge : public IGameObject
{
	~MobiusGauge();
	bool Start() override;
	void Update() override;
	void PostRender(RenderContext& rc, Camera* camera)override;

public:

	//�Q�[�W�̍��W��ݒ�B
	void SetPosition(const Vector3& pos);

	enum DrawScreen
	{
		Screen1,//�����
		Screen2,//�E���
		AllScreen//��ʑS��
	};

private:

	struct GaugeDatas //�Q�[�W�̃f�[�^�B
	{
		Vector3 spriteStartDir = { 1.0f,0.0f,0.0f };//��]�̊J�n�����@�����ȕ������ォ��O�ςŒ��ׂ邽�߂ɎO�����x�N�g���ɂ��Ă���
		float gaugeProgress = 0;//�Q�[�W�̐i�݋(�p�x)�@0 ~ 360
		Vector2 rotateOrigin = { 0.0f,0.0f };//��]�̒��S���W
		int isClockwise = 0;//���v��肩? bool�^���Ƃ��܂��n���Ȃ������̂�int�ɂ��Ă���
		int colorFlag = -1;//�ԐF���F���@��(-1) ��(1)
	};

	GaugeDatas m_rightGaugeDatas;//�E���̃Q�[�W�̃f�[�^
	GaugeDatas m_leftGaugeDatas;//�����̃Q�[�W�̃f�[�^
	GaugeDatas m_spGaugeDatas;//�K�E�Z�Q�[�W�̃f�[�^

	Sprite m_mobiusBaseSprite;//���r�E�X�Q�[�W�̔w�i�B
	Sprite m_mobiusCoverSprite;//���r�E�X�Q�[�W�̊O���̃J�o�[�B
	Sprite m_mobiusGaugeRightSide;//���r�E�X�Q�[�W(�E��)
	Sprite m_mobiusGaugeLeftSide;//���r�E�X�Q�[�W(����)
	Sprite m_spGaugeSprite;//�K�E�Z�Q�[�W

	DrawScreen m_drawScreen = AllScreen;//�`���

public:
	float m_charge;//���̓Q�[�W�̗��܂�
	bool m_isRed = false;//���̓Q�[�W�͐ԐF(�˗�)��?
	float m_spCharge;//�K�E�Z�Q�[�W�̗��܂�
};

