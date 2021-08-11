#include "stdafx.h"
#include "MobiusGauge.h"

namespace
{
    const int SPRITE_GAUGEBASE_WIDTH = 212;                                     //�Q�[�W�̃x�[�X�̕�
    const int SPRITE_GAUGEBASE_HEIGHT = 140;                                    //�Q�[�W�̃x�[�X�̍���
    const int SPRITE_GAUGEHALF_WIDTH = 99;                                      //���E�ɕ��������Q�[�W�̂��ꂼ��̕�
    const int SPRITE_GAUGEHALF_HEIGHT = 77;                                     //���E�ɕ��������Q�[�W�̂��ꂼ��̍���
    const int SPRITE_SPGAUGE_WIDTH = 88;                                        //�K�E�Z�Q�[�W�̕�
    const int SPRITE_SPGAUGE_HEIGHT = 52;                                       //�K�E�Z�Q�[�W�̍���
    const Vector3 SPRITE_RIGHTGAUGE_STARTDIRECTION = { -10.0f,1.5f,0.0f };      //�E���̃Q�[�W�̃X�^�[�g�n�_�̌���
    const Vector3 SPRITE_LEFTGAUGE_STARTDIRECTION = { 10.0f,-1.5f,0.0f };       //�����̃Q�[�W�̃X�^�[�g�n�_�̌���
    const Vector3 SPRITE_SPGAUGE_STARTDIRECTION = { 0.0f,-1.0f,0.0f };          //�K�E�Z�Q�[�W�̃X�^�[�g�n�_�̌���
    const int COLOR_FLAG_RED = -1;                                              //�V�F�[�_�[�ɓn���J���[�t���O�@�ԐF
    const int COLOR_FLAG_BLUE = 1;                                              //�V�F�[�_�[�ɓn���J���[�t���O�@�F
    const float MAGNET_CHARGE_MAX = 1000.0f;                                    //���̓Q�[�W�̍ő�l
    const float SP_CHARGE_MAX = 100.0f;                                         //�K�E�Z�Q�[�W�̍ő�l
    const float MOBIUSGAUGE_PROGRESS_MAX = 720.0f;                              //2���킳�����Q�[�W�Ƃ��Ă̍ő�l(360�x�̃Q�[�W*2��)
    const float MOBIUSGAUGE_PROGRESS_HALF = 360.0f;                             //2���킳�����Q�[�W�̔����̒l
    const float MOBIUSGAUGE_PROGRESS_START_LEFT = 345.0f;                       //�����̃Q�[�W���X�^�[�g����l
    const float SINGLEGAUGE_PROGRESS_MAX = 360.0f;                              //���ꂼ��̃Q�[�W�Ƃ��Ă̍ő�l
    const float SINGLEGAUGE_PROGRESS_ZERO = 0.0f;                               //�Q�[�W�̏����l
    const float SINGLEGAUGE_SYNC_VALUE = 15.0f;                                 //���E�̃Q�[�W�̃^�C�~���O�����킹�邽�߂̒l
    const Vector3 MOBIUSGAUGE_RIGHT_PIVOT = { 50.0f,-20.0f,0.0f };              //�E���̃Q�[�W�̃s�{�b�g
    const Vector3 MOBIUSGAUGE_LEFT_PIVOT = { -49.0f,-20.0f,0.0f };              //�����̃Q�[�W�̃s�{�b�g
    const Vector3 SPGAUGE_PIVOT = { 0.0f,37.5f,0.0f };                          //�K�E�Z�Q�[�W�̃s�{�b�g
    const Vector2 MOBIUSGAUGE_RIGHT_ROTATEORIGIN_OFFSET = { 685.0f,380.0f };    //�E���̃Q�[�W�̉�]�̒��S�̊�_
    const Vector2 MOBIUSGAUGE_LEFT_ROTATEORIGIN_OFFSET = { 595.0f,380.0f };     //�����̃Q�[�W�̉�]�̒��S�̊�_
    const Vector2 SPGAUGE_ROTATEORIGIN_OFFSET = { 645.0f,320.0f };              //�K�E�Z�Q�[�W�̉�]�̒��S�̊�_

}
MobiusGauge::~MobiusGauge()
{

}

bool MobiusGauge::Start()
{
    //�Q�[�W�̔w�i�̏������f�[�^
	SpriteInitData baseInitData;
    baseInitData.m_ddsFilePath[0] = "Assets/Image/Mobius_Base.dds";
    baseInitData.m_fxFilePath = "Assets/shader/sprite.fx";
    baseInitData.m_psEntryPoinFunc = "PSMain";
    baseInitData.m_width = SPRITE_GAUGEBASE_WIDTH;
    baseInitData.m_height = SPRITE_GAUGEBASE_HEIGHT;
    baseInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

    //�Q�[�W�̔w�i�̏�����
    m_mobiusBaseSprite.Init(baseInitData);


    //�Q�[�W�̃J�o�[�̏������f�[�^
    SpriteInitData coverInitData;
    coverInitData.m_ddsFilePath[0] = "Assets/image/Mobius_Cover.dds";
    coverInitData.m_fxFilePath = "Assets/shader/sprite.fx";
    coverInitData.m_psEntryPoinFunc = "PSMain";
    coverInitData.m_width = SPRITE_GAUGEBASE_WIDTH;
    coverInitData.m_height = SPRITE_GAUGEBASE_HEIGHT;
    coverInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

    //�Q�[�W�̃J�o�[�̏�����
    m_mobiusCoverSprite.Init(coverInitData);


    //�E���̃Q�[�W�̏������f�[�^
    SpriteInitData rightGaugeInitData;
    //�Q�[�W�̐؂�ւ��p�ɐԐF�ƐF�̃X�v���C�g��o�^�B
    rightGaugeInitData.m_ddsFilePath[0] = "Assets/image/RightRed.dds";
    rightGaugeInitData.m_ddsFilePath[1] = "Assets/image/RightBlue.dds";
    //��p�̃V�F�[�_�[���g�p����
    rightGaugeInitData.m_fxFilePath = "Assets/shader/ringUI.fx";
    rightGaugeInitData.m_width = SPRITE_GAUGEHALF_WIDTH;
    rightGaugeInitData.m_height = SPRITE_GAUGEHALF_HEIGHT;
    rightGaugeInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

    //�J�n�n�_���������炷
    m_rightGaugeDatas.spriteStartDir = SPRITE_RIGHTGAUGE_STARTDIRECTION;
    m_rightGaugeDatas.spriteStartDir.Normalize();
    //�Q�[�W�̐i�s�����͔����v����
    m_rightGaugeDatas.isClockwise = false;

    rightGaugeInitData.m_expandConstantBuffer[0] = &m_rightGaugeDatas;
    rightGaugeInitData.m_expandConstantBufferSize[0] = sizeof(m_rightGaugeDatas);

    //�E���̃Q�[�W�̏�����
    m_mobiusGaugeRightSide.Init(rightGaugeInitData);


    //�����̃Q�[�W�̏������f�[�^
    SpriteInitData leftGaugeInitData;
    //�Q�[�W�̐؂�ւ��p�ɐԐF�ƐF�̃X�v���C�g��o�^�B
    leftGaugeInitData.m_ddsFilePath[0] = "Assets/image/LeftRed.dds";
    leftGaugeInitData.m_ddsFilePath[1] = "Assets/image/LeftBlue.dds";
    leftGaugeInitData.m_fxFilePath = "Assets/shader/ringUI.fx";
    leftGaugeInitData.m_width = SPRITE_GAUGEHALF_WIDTH;
    leftGaugeInitData.m_height = SPRITE_GAUGEHALF_HEIGHT;
    leftGaugeInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

    //�J�n�n�_���������炷
    m_leftGaugeDatas.spriteStartDir = SPRITE_LEFTGAUGE_STARTDIRECTION;
    m_leftGaugeDatas.spriteStartDir.Normalize();
    //�Q�[�W�̐i�s�����͎��v����
    m_leftGaugeDatas.isClockwise = true;

    leftGaugeInitData.m_expandConstantBuffer[0] = &m_leftGaugeDatas;
    leftGaugeInitData.m_expandConstantBufferSize[0] = sizeof(m_leftGaugeDatas);

    //�����̃Q�[�W�̏�����
    m_mobiusGaugeLeftSide.Init(leftGaugeInitData);

    //�K�E�Z�Q�[�W�̏������f�[�^
    SpriteInitData spInitData;
    spInitData.m_ddsFilePath[0] = "Assets/Image/Mobius_SP_Bar.dds";
    spInitData.m_fxFilePath = "Assets/shader/ringUI.fx";
    spInitData.m_width = SPRITE_SPGAUGE_WIDTH;
    spInitData.m_height = SPRITE_SPGAUGE_HEIGHT;
    spInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

    //NOTE:�ォ��X�^�[�g�ɂ��������㉺�̏�񂪔��΂ɂȂ�̂�y��-1.0f�A�\�L�̐H���Ⴂ���C�ɂȂ�
    m_spGaugeDatas.spriteStartDir = SPRITE_SPGAUGE_STARTDIRECTION;
    //NOTE:�Q�[�W�̐i�s�����͎��v����ɂ�����������͑����Ă����Q�[�W�Ȃ̂�false...�\�L�̐H���Ⴂ���C�ɂȂ�
    m_spGaugeDatas.isClockwise = false;

    spInitData.m_expandConstantBuffer[0] = &m_spGaugeDatas;
    spInitData.m_expandConstantBufferSize[0] = sizeof(m_spGaugeDatas);

    //�K�E�Z�Q�[�W�̏�����
    m_spGaugeSprite.Init(spInitData);

    return true;
}

void MobiusGauge::Update()
{
    //�t���O�ɂ���ĐF��ω�
    if (m_isRed == true)
    {
        m_rightGaugeDatas.colorFlag = COLOR_FLAG_RED;
        m_leftGaugeDatas.colorFlag = COLOR_FLAG_RED;
    }
    else
    {
        m_rightGaugeDatas.colorFlag = COLOR_FLAG_BLUE;
        m_leftGaugeDatas.colorFlag = COLOR_FLAG_BLUE;
    }


    //�Q�[�W��Max���猸�炵�Ă����`�ɂ���
    float count = MOBIUSGAUGE_PROGRESS_MAX - m_magnetCharge / MAGNET_CHARGE_MAX * MOBIUSGAUGE_PROGRESS_MAX;


    //�E���̃Q�[�W
    if (count >= MOBIUSGAUGE_PROGRESS_HALF)
    {
        //�����ȏ�i��ł���΂�����MAX(�܂�Q�[�W�Ȃ�)
        m_rightGaugeDatas.gaugeProgress = SINGLEGAUGE_PROGRESS_MAX;
    }
    else
    {
        //�����ȉ��ł���΃J�E���g�ɉ����ăQ�[�W��i�߂�
        m_rightGaugeDatas.gaugeProgress = count;
    }

    //�����̃Q�[�W
    if (count <= MOBIUSGAUGE_PROGRESS_START_LEFT)
    {
        //�����ȉ��ł���΂܂��Q�[�W������Ȃ��̂�0(�܂�Q�[�WMAX)
        m_leftGaugeDatas.gaugeProgress = SINGLEGAUGE_PROGRESS_ZERO;
    }
    else
    {
        //�����ȏ�ł���΃J�E���g�ɉ����ăQ�[�W��i�߂�
        //�E���̃Q�[�W�Ɠ������Ƃ邽�߂ɏ����v���X���Ă���
        m_leftGaugeDatas.gaugeProgress = count - SINGLEGAUGE_PROGRESS_MAX + SINGLEGAUGE_SYNC_VALUE;
    }


    //�V�F�[�_�[�ɓn���K�E�Z�Q�[�W�̒l�̌v�Z
    m_spGaugeDatas.gaugeProgress = SINGLEGAUGE_PROGRESS_MAX - m_spCharge / SP_CHARGE_MAX * SINGLEGAUGE_PROGRESS_MAX;
}

void MobiusGauge::SetPosition(const Vector3& pos)
{
    //�Q�[�W�̊e�X�v���C�g�̈ʒu��ݒ�B�s�{�b�g�������Ă��Ȃ��̂�position�𒼐ڂ��炵�Ă���
    m_mobiusBaseSprite.Update(pos, Quaternion::Identity, Vector3::One);
    m_mobiusCoverSprite.Update(pos, Quaternion::Identity, Vector3::One);
    m_mobiusGaugeRightSide.Update(pos + MOBIUSGAUGE_RIGHT_PIVOT, Quaternion::Identity, Vector3::One);
    m_mobiusGaugeLeftSide.Update(pos + MOBIUSGAUGE_LEFT_PIVOT, Quaternion::Identity, Vector3::One);

    m_spGaugeSprite.Update(pos + SPGAUGE_PIVOT, Quaternion::Identity, Vector3::One);

    //��]�̃Q�[�W�̉�]�̒��S�ʒu�����炷�B
    m_rightGaugeDatas.rotateOrigin = { MOBIUSGAUGE_RIGHT_ROTATEORIGIN_OFFSET.x + pos.x,MOBIUSGAUGE_RIGHT_ROTATEORIGIN_OFFSET.y - pos.y };
    m_leftGaugeDatas.rotateOrigin = { MOBIUSGAUGE_LEFT_ROTATEORIGIN_OFFSET.x + pos.x,MOBIUSGAUGE_LEFT_ROTATEORIGIN_OFFSET.y - pos.y };
    m_spGaugeDatas.rotateOrigin = { SPGAUGE_ROTATEORIGIN_OFFSET.x + pos.x,SPGAUGE_ROTATEORIGIN_OFFSET.y - pos.y };
}

void MobiusGauge::PostRender(RenderContext& rc, Camera* camera)
{
    //�`���ʂƓ������̂ݕ`��
    if (rc.GetRenderStep() == m_drawScreen)
    {
        m_mobiusBaseSprite.Draw(rc);
        m_mobiusGaugeRightSide.Draw(rc);
        m_mobiusGaugeLeftSide.Draw(rc);
        m_spGaugeSprite.Draw(rc);
        m_mobiusCoverSprite.Draw(rc);
    }
}