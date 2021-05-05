#include "stdafx.h"
#include "MobiusGauge.h"

MobiusGauge::~MobiusGauge()
{

}

bool MobiusGauge::Start()
{
    //ゲージの背景の初期化データ
	SpriteInitData baseInitData;
    baseInitData.m_ddsFilePath[0] = "Assets/Image/Mobius_Base.dds";
    baseInitData.m_fxFilePath = "Assets/shader/sprite.fx";
    baseInitData.m_psEntryPoinFunc = "PSMain";
    baseInitData.m_width = 197.0f;
    baseInitData.m_height = 77.0f;
    baseInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

    //ゲージの背景の初期化
    m_mobiusBaseSprite.Init(baseInitData);


    //ゲージのカバーの初期化データ
    SpriteInitData coverInitData;
    coverInitData.m_ddsFilePath[0] = "Assets/image/Mobius_Cover.dds";
    coverInitData.m_fxFilePath = "Assets/shader/sprite.fx";
    coverInitData.m_psEntryPoinFunc = "PSMain";
    coverInitData.m_width = 208.0f;
    coverInitData.m_height = 94.0f;
    coverInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

    //ゲージのカバーの初期化
    m_mobiusCoverSprite.Init(coverInitData);


    //右側のゲージの初期化データ
    SpriteInitData rightGaugeInitData;
    //ゲージの切り替え用に赤色と青色のスプライトを登録。
    rightGaugeInitData.m_ddsFilePath[0] = "Assets/image/RightRed.dds";
    rightGaugeInitData.m_ddsFilePath[1] = "Assets/image/RightBlue.dds";
    //専用のシェーダーを使用する
    rightGaugeInitData.m_fxFilePath = "Assets/shader/ringUI.fx";
    rightGaugeInitData.m_width = 99.0f;
    rightGaugeInitData.m_height = 77.0f;
    rightGaugeInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

    //開始地点を少しずらす
    m_rightGaugeDatas.spriteStartDir = { -10.0f,1.5f,0.0f };
    m_rightGaugeDatas.spriteStartDir.Normalize();
    //ゲージの進行方向は反時計周り
    m_rightGaugeDatas.isClockwise = false;

    rightGaugeInitData.m_expandConstantBuffer = &m_rightGaugeDatas;
    rightGaugeInitData.m_expandConstantBufferSize = sizeof(m_rightGaugeDatas);

    //右側のゲージの初期化
    m_mobiusGaugeRightSide.Init(rightGaugeInitData);


    //左側のゲージの初期化データ
    SpriteInitData leftGaugeInitData;
    //ゲージの切り替え用に赤色と青色のスプライトを登録。
    leftGaugeInitData.m_ddsFilePath[0] = "Assets/image/LeftRed.dds";
    leftGaugeInitData.m_ddsFilePath[1] = "Assets/image/LeftBlue.dds";
    leftGaugeInitData.m_fxFilePath = "Assets/shader/ringUI.fx";
    leftGaugeInitData.m_width = 99.0f;
    leftGaugeInitData.m_height = 77.0f;
    leftGaugeInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

    //開始地点を少しずらす
    m_leftGaugeDatas.spriteStartDir = { 10.0f,-1.5f,0.0f };
    m_leftGaugeDatas.spriteStartDir.Normalize();
    //ゲージの進行方向は時計周り
    m_leftGaugeDatas.isClockwise = true;

    leftGaugeInitData.m_expandConstantBuffer = &m_leftGaugeDatas;
    leftGaugeInitData.m_expandConstantBufferSize = sizeof(m_leftGaugeDatas);

    //左側のゲージの初期化
    m_mobiusGaugeLeftSide.Init(leftGaugeInitData);


	return true;
}

void MobiusGauge::Update()
{
    //フラグによって色を変化
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


    //ゲージをMaxから減らしていく形にする
    float count = 720 - m_charge / 1000 * 720;


    //右側のゲージ
    if (count >= 360)
    {
        //半分以上進んでいればずっとMAX(つまりゲージなし)
        m_rightGaugeDatas.gaugeProgress = 360.0f;
    }
    else
    {
        //半分以下であればカウントに応じてゲージを進める
        m_rightGaugeDatas.gaugeProgress = count;
    }

    //左側のゲージ
    if (count <= 345)
    {
        //半分以下であればまだゲージが減らないので0(つまりゲージMAX)
        m_leftGaugeDatas.gaugeProgress = 0;
    }
    else
    {
        //半分以上であればカウントに応じてゲージを進める
        //右側のゲージと同期をとるために+15.0している
        m_leftGaugeDatas.gaugeProgress = count - 360.0f + 15.0f;
    }
}

void MobiusGauge::SetPosition(const Vector3& pos)
{
    //ゲージの各スプライトの位置を設定。ピボットが効いていないのでpositionを直接ずらしている
    m_mobiusBaseSprite.Update(pos, Quaternion::Identity, Vector3::One, { 0.5f,0.5f });
    m_mobiusCoverSprite.Update({ pos.x,pos.y -3.0f,pos.z }, Quaternion::Identity, Vector3::One, { 0.5f,0.5f });
    m_mobiusGaugeRightSide.Update({ pos.x + 50.0f,pos.y,pos.z }, Quaternion::Identity, Vector3::One, { 0.0f,0.5f });
    m_mobiusGaugeLeftSide.Update({ pos.x - 49.0f,pos.y,pos.z }, Quaternion::Identity, Vector3::One, { 1.0f,0.5f });

    //回転のゲージの回転の中心位置もずらす。
    m_rightGaugeDatas.rotateOrigin = { 685.0f + pos.x,360.0f - pos.y };
    m_leftGaugeDatas.rotateOrigin = { 595.0f + pos.x,360.0f - pos.y };
}

void MobiusGauge::PostRender(RenderContext& rc, Camera* camera)
{
    //描画画面と同じ時のみ描画
    if (rc.GetRenderStep() == m_drawScreen)
    {
        m_mobiusBaseSprite.Draw(rc);
        m_mobiusGaugeRightSide.Draw(rc);
        m_mobiusGaugeLeftSide.Draw(rc);
        m_mobiusCoverSprite.Draw(rc);
    }
}