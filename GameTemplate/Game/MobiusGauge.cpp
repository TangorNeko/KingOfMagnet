#include "stdafx.h"
#include "MobiusGauge.h"

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
    baseInitData.m_width = 212.0f;//197.0f
    baseInitData.m_height = 140.0f;//77.0f
    baseInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

    //�Q�[�W�̔w�i�̏�����
    m_mobiusBaseSprite.Init(baseInitData);


    //�Q�[�W�̃J�o�[�̏������f�[�^
    SpriteInitData coverInitData;
    coverInitData.m_ddsFilePath[0] = "Assets/image/Mobius_Cover.dds";
    coverInitData.m_fxFilePath = "Assets/shader/sprite.fx";
    coverInitData.m_psEntryPoinFunc = "PSMain";
    coverInitData.m_width = 212.0f;
    coverInitData.m_height = 140.0f;   
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
    rightGaugeInitData.m_width = 99.0f;
    rightGaugeInitData.m_height = 77.0f;
    rightGaugeInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

    //�J�n�n�_���������炷
    m_rightGaugeDatas.spriteStartDir = { -10.0f,1.5f,0.0f };
    m_rightGaugeDatas.spriteStartDir.Normalize();
    //�Q�[�W�̐i�s�����͔����v����
    m_rightGaugeDatas.isClockwise = false;

    rightGaugeInitData.m_expandConstantBuffer = &m_rightGaugeDatas;
    rightGaugeInitData.m_expandConstantBufferSize = sizeof(m_rightGaugeDatas);

    //�E���̃Q�[�W�̏�����
    m_mobiusGaugeRightSide.Init(rightGaugeInitData);


    //�����̃Q�[�W�̏������f�[�^
    SpriteInitData leftGaugeInitData;
    //�Q�[�W�̐؂�ւ��p�ɐԐF�ƐF�̃X�v���C�g��o�^�B
    leftGaugeInitData.m_ddsFilePath[0] = "Assets/image/LeftRed.dds";
    leftGaugeInitData.m_ddsFilePath[1] = "Assets/image/LeftBlue.dds";
    leftGaugeInitData.m_fxFilePath = "Assets/shader/ringUI.fx";
    leftGaugeInitData.m_width = 99.0f;
    leftGaugeInitData.m_height = 77.0f;
    leftGaugeInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

    //�J�n�n�_���������炷
    m_leftGaugeDatas.spriteStartDir = { 10.0f,-1.5f,0.0f };
    m_leftGaugeDatas.spriteStartDir.Normalize();
    //�Q�[�W�̐i�s�����͎��v����
    m_leftGaugeDatas.isClockwise = true;

    leftGaugeInitData.m_expandConstantBuffer = &m_leftGaugeDatas;
    leftGaugeInitData.m_expandConstantBufferSize = sizeof(m_leftGaugeDatas);

    //�����̃Q�[�W�̏�����
    m_mobiusGaugeLeftSide.Init(leftGaugeInitData);

    //�K�E�Z�Q�[�W�̏������f�[�^
    SpriteInitData spInitData;
    spInitData.m_ddsFilePath[0] = "Assets/Image/Mobius_SP_Bar.dds";
    spInitData.m_fxFilePath = "Assets/shader/ringUI.fx";
    spInitData.m_width = 88.0f;
    spInitData.m_height = 52.0f;
    spInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

    //NOTE:�ォ��X�^�[�g�ɂ��������㉺�̏�񂪔��΂ɂȂ�̂�y��-1.0f�A�\�L�̐H���Ⴂ���C�ɂȂ�
    m_spGaugeDatas.spriteStartDir = { 0.0f,-1.0f,0.0f };
    //NOTE:�Q�[�W�̐i�s�����͎��v����ɂ�����������͑����Ă����Q�[�W�Ȃ̂�false...�\�L�̐H���Ⴂ���C�ɂȂ�
    m_spGaugeDatas.isClockwise = false;

    spInitData.m_expandConstantBuffer = &m_spGaugeDatas;
    spInitData.m_expandConstantBufferSize = sizeof(m_spGaugeDatas);

    //�K�E�Z�Q�[�W�̏�����
    m_spGaugeSprite.Init(spInitData);

    return true;
}

void MobiusGauge::Update()
{
    //�t���O�ɂ���ĐF��ω�
    if (m_isRed == true)
    {
        m_rightGaugeDatas.colorFlag = -1;
        m_leftGaugeDatas.colorFlag = -1;
    }
    else
    {
        m_rightGaugeDatas.colorFlag = 1;
        m_leftGaugeDatas.colorFlag = 1;
    }


    //�Q�[�W��Max���猸�炵�Ă����`�ɂ���
    float count = 720 - m_charge / 1000 * 720;


    //�E���̃Q�[�W
    if (count >= 360)
    {
        //�����ȏ�i��ł���΂�����MAX(�܂�Q�[�W�Ȃ�)
        m_rightGaugeDatas.gaugeProgress = 360.0f;
    }
    else
    {
        //�����ȉ��ł���΃J�E���g�ɉ����ăQ�[�W��i�߂�
        m_rightGaugeDatas.gaugeProgress = count;
    }

    //�����̃Q�[�W
    if (count <= 345)
    {
        //�����ȉ��ł���΂܂��Q�[�W������Ȃ��̂�0(�܂�Q�[�WMAX)
        m_leftGaugeDatas.gaugeProgress = 0;
    }
    else
    {
        //�����ȏ�ł���΃J�E���g�ɉ����ăQ�[�W��i�߂�
        //�E���̃Q�[�W�Ɠ������Ƃ邽�߂�+15.0���Ă���
        m_leftGaugeDatas.gaugeProgress = count - 360.0f + 15.0f;
    }


    //�K�E�Z�Q�[�W�̕\��

    m_spGaugeDatas.gaugeProgress = 360 - m_spCharge / 100 * 360;
}

void MobiusGauge::SetPosition(const Vector3& pos)
{
    //�Q�[�W�̊e�X�v���C�g�̈ʒu��ݒ�B�s�{�b�g�������Ă��Ȃ��̂�position�𒼐ڂ��炵�Ă���
    m_mobiusBaseSprite.Update(pos, Quaternion::Identity, Vector3::One, { 0.5f,0.5f });
    m_mobiusCoverSprite.Update({ pos.x,pos.y,pos.z }, Quaternion::Identity, Vector3::One, { 0.5f,0.5f });
    m_mobiusGaugeRightSide.Update({ pos.x + 50.0f,pos.y - 20.0f,pos.z }, Quaternion::Identity, Vector3::One, { 0.0f,0.5f });
    m_mobiusGaugeLeftSide.Update({ pos.x - 49.0f,pos.y - 20.0f,pos.z }, Quaternion::Identity, Vector3::One, { 1.0f,0.5f });

    m_spGaugeSprite.Update({ pos.x,pos.y + 37.5f,pos.z }, Quaternion::Identity, Vector3::One, { 0.5f,0.5f });

    //��]�̃Q�[�W�̉�]�̒��S�ʒu�����炷�B
    m_rightGaugeDatas.rotateOrigin = { 685.0f + pos.x,380.0f - pos.y };//360.0f
    m_leftGaugeDatas.rotateOrigin = { 595.0f + pos.x,380.0f - pos.y };
    m_spGaugeDatas.rotateOrigin = { 645.0f + pos.x,320.0f - pos.y };
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