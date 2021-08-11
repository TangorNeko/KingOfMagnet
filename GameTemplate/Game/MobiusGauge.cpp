#include "stdafx.h"
#include "MobiusGauge.h"

namespace
{
    const int SPRITE_GAUGEBASE_WIDTH = 212;                                     //ゲージのベースの幅
    const int SPRITE_GAUGEBASE_HEIGHT = 140;                                    //ゲージのベースの高さ
    const int SPRITE_GAUGEHALF_WIDTH = 99;                                      //左右に分割したゲージのそれぞれの幅
    const int SPRITE_GAUGEHALF_HEIGHT = 77;                                     //左右に分割したゲージのそれぞれの高さ
    const int SPRITE_SPGAUGE_WIDTH = 88;                                        //必殺技ゲージの幅
    const int SPRITE_SPGAUGE_HEIGHT = 52;                                       //必殺技ゲージの高さ
    const Vector3 SPRITE_RIGHTGAUGE_STARTDIRECTION = { -10.0f,1.5f,0.0f };      //右側のゲージのスタート地点の向き
    const Vector3 SPRITE_LEFTGAUGE_STARTDIRECTION = { 10.0f,-1.5f,0.0f };       //左側のゲージのスタート地点の向き
    const Vector3 SPRITE_SPGAUGE_STARTDIRECTION = { 0.0f,-1.0f,0.0f };          //必殺技ゲージのスタート地点の向き
    const int COLOR_FLAG_RED = -1;                                              //シェーダーに渡すカラーフラグ　赤色
    const int COLOR_FLAG_BLUE = 1;                                              //シェーダーに渡すカラーフラグ　青色
    const float MAGNET_CHARGE_MAX = 1000.0f;                                    //磁力ゲージの最大値
    const float SP_CHARGE_MAX = 100.0f;                                         //必殺技ゲージの最大値
    const float MOBIUSGAUGE_PROGRESS_MAX = 720.0f;                              //2つ合わさったゲージとしての最大値(360度のゲージ*2個分)
    const float MOBIUSGAUGE_PROGRESS_HALF = 360.0f;                             //2つ合わさったゲージの半分の値
    const float MOBIUSGAUGE_PROGRESS_START_LEFT = 345.0f;                       //左側のゲージがスタートする値
    const float SINGLEGAUGE_PROGRESS_MAX = 360.0f;                              //それぞれのゲージとしての最大値
    const float SINGLEGAUGE_PROGRESS_ZERO = 0.0f;                               //ゲージの初期値
    const float SINGLEGAUGE_SYNC_VALUE = 15.0f;                                 //左右のゲージのタイミングを合わせるための値
    const Vector3 MOBIUSGAUGE_RIGHT_PIVOT = { 50.0f,-20.0f,0.0f };              //右側のゲージのピボット
    const Vector3 MOBIUSGAUGE_LEFT_PIVOT = { -49.0f,-20.0f,0.0f };              //左側のゲージのピボット
    const Vector3 SPGAUGE_PIVOT = { 0.0f,37.5f,0.0f };                          //必殺技ゲージのピボット
    const Vector2 MOBIUSGAUGE_RIGHT_ROTATEORIGIN_OFFSET = { 685.0f,380.0f };    //右側のゲージの回転の中心の基点
    const Vector2 MOBIUSGAUGE_LEFT_ROTATEORIGIN_OFFSET = { 595.0f,380.0f };     //左側のゲージの回転の中心の基点
    const Vector2 SPGAUGE_ROTATEORIGIN_OFFSET = { 645.0f,320.0f };              //必殺技ゲージの回転の中心の基点

}
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
    baseInitData.m_width = SPRITE_GAUGEBASE_WIDTH;
    baseInitData.m_height = SPRITE_GAUGEBASE_HEIGHT;
    baseInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

    //ゲージの背景の初期化
    m_mobiusBaseSprite.Init(baseInitData);


    //ゲージのカバーの初期化データ
    SpriteInitData coverInitData;
    coverInitData.m_ddsFilePath[0] = "Assets/image/Mobius_Cover.dds";
    coverInitData.m_fxFilePath = "Assets/shader/sprite.fx";
    coverInitData.m_psEntryPoinFunc = "PSMain";
    coverInitData.m_width = SPRITE_GAUGEBASE_WIDTH;
    coverInitData.m_height = SPRITE_GAUGEBASE_HEIGHT;
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
    rightGaugeInitData.m_width = SPRITE_GAUGEHALF_WIDTH;
    rightGaugeInitData.m_height = SPRITE_GAUGEHALF_HEIGHT;
    rightGaugeInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

    //開始地点を少しずらす
    m_rightGaugeDatas.spriteStartDir = SPRITE_RIGHTGAUGE_STARTDIRECTION;
    m_rightGaugeDatas.spriteStartDir.Normalize();
    //ゲージの進行方向は反時計周り
    m_rightGaugeDatas.isClockwise = false;

    rightGaugeInitData.m_expandConstantBuffer[0] = &m_rightGaugeDatas;
    rightGaugeInitData.m_expandConstantBufferSize[0] = sizeof(m_rightGaugeDatas);

    //右側のゲージの初期化
    m_mobiusGaugeRightSide.Init(rightGaugeInitData);


    //左側のゲージの初期化データ
    SpriteInitData leftGaugeInitData;
    //ゲージの切り替え用に赤色と青色のスプライトを登録。
    leftGaugeInitData.m_ddsFilePath[0] = "Assets/image/LeftRed.dds";
    leftGaugeInitData.m_ddsFilePath[1] = "Assets/image/LeftBlue.dds";
    leftGaugeInitData.m_fxFilePath = "Assets/shader/ringUI.fx";
    leftGaugeInitData.m_width = SPRITE_GAUGEHALF_WIDTH;
    leftGaugeInitData.m_height = SPRITE_GAUGEHALF_HEIGHT;
    leftGaugeInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

    //開始地点を少しずらす
    m_leftGaugeDatas.spriteStartDir = SPRITE_LEFTGAUGE_STARTDIRECTION;
    m_leftGaugeDatas.spriteStartDir.Normalize();
    //ゲージの進行方向は時計周り
    m_leftGaugeDatas.isClockwise = true;

    leftGaugeInitData.m_expandConstantBuffer[0] = &m_leftGaugeDatas;
    leftGaugeInitData.m_expandConstantBufferSize[0] = sizeof(m_leftGaugeDatas);

    //左側のゲージの初期化
    m_mobiusGaugeLeftSide.Init(leftGaugeInitData);

    //必殺技ゲージの初期化データ
    SpriteInitData spInitData;
    spInitData.m_ddsFilePath[0] = "Assets/Image/Mobius_SP_Bar.dds";
    spInitData.m_fxFilePath = "Assets/shader/ringUI.fx";
    spInitData.m_width = SPRITE_SPGAUGE_WIDTH;
    spInitData.m_height = SPRITE_SPGAUGE_HEIGHT;
    spInitData.m_alphaBlendMode = AlphaBlendMode_Trans;

    //NOTE:上からスタートにしたいが上下の情報が反対になるのでyは-1.0f、表記の食い違いが気になる
    m_spGaugeDatas.spriteStartDir = SPRITE_SPGAUGE_STARTDIRECTION;
    //NOTE:ゲージの進行方向は時計周りにしたいが今回は増えていくゲージなのでfalse...表記の食い違いが気になる
    m_spGaugeDatas.isClockwise = false;

    spInitData.m_expandConstantBuffer[0] = &m_spGaugeDatas;
    spInitData.m_expandConstantBufferSize[0] = sizeof(m_spGaugeDatas);

    //必殺技ゲージの初期化
    m_spGaugeSprite.Init(spInitData);

    return true;
}

void MobiusGauge::Update()
{
    //フラグによって色を変化
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


    //ゲージをMaxから減らしていく形にする
    float count = MOBIUSGAUGE_PROGRESS_MAX - m_magnetCharge / MAGNET_CHARGE_MAX * MOBIUSGAUGE_PROGRESS_MAX;


    //右側のゲージ
    if (count >= MOBIUSGAUGE_PROGRESS_HALF)
    {
        //半分以上進んでいればずっとMAX(つまりゲージなし)
        m_rightGaugeDatas.gaugeProgress = SINGLEGAUGE_PROGRESS_MAX;
    }
    else
    {
        //半分以下であればカウントに応じてゲージを進める
        m_rightGaugeDatas.gaugeProgress = count;
    }

    //左側のゲージ
    if (count <= MOBIUSGAUGE_PROGRESS_START_LEFT)
    {
        //半分以下であればまだゲージが減らないので0(つまりゲージMAX)
        m_leftGaugeDatas.gaugeProgress = SINGLEGAUGE_PROGRESS_ZERO;
    }
    else
    {
        //半分以上であればカウントに応じてゲージを進める
        //右側のゲージと同期をとるために少しプラスしている
        m_leftGaugeDatas.gaugeProgress = count - SINGLEGAUGE_PROGRESS_MAX + SINGLEGAUGE_SYNC_VALUE;
    }


    //シェーダーに渡す必殺技ゲージの値の計算
    m_spGaugeDatas.gaugeProgress = SINGLEGAUGE_PROGRESS_MAX - m_spCharge / SP_CHARGE_MAX * SINGLEGAUGE_PROGRESS_MAX;
}

void MobiusGauge::SetPosition(const Vector3& pos)
{
    //ゲージの各スプライトの位置を設定。ピボットが効いていないのでpositionを直接ずらしている
    m_mobiusBaseSprite.Update(pos, Quaternion::Identity, Vector3::One);
    m_mobiusCoverSprite.Update(pos, Quaternion::Identity, Vector3::One);
    m_mobiusGaugeRightSide.Update(pos + MOBIUSGAUGE_RIGHT_PIVOT, Quaternion::Identity, Vector3::One);
    m_mobiusGaugeLeftSide.Update(pos + MOBIUSGAUGE_LEFT_PIVOT, Quaternion::Identity, Vector3::One);

    m_spGaugeSprite.Update(pos + SPGAUGE_PIVOT, Quaternion::Identity, Vector3::One);

    //回転のゲージの回転の中心位置もずらす。
    m_rightGaugeDatas.rotateOrigin = { MOBIUSGAUGE_RIGHT_ROTATEORIGIN_OFFSET.x + pos.x,MOBIUSGAUGE_RIGHT_ROTATEORIGIN_OFFSET.y - pos.y };
    m_leftGaugeDatas.rotateOrigin = { MOBIUSGAUGE_LEFT_ROTATEORIGIN_OFFSET.x + pos.x,MOBIUSGAUGE_LEFT_ROTATEORIGIN_OFFSET.y - pos.y };
    m_spGaugeDatas.rotateOrigin = { SPGAUGE_ROTATEORIGIN_OFFSET.x + pos.x,SPGAUGE_ROTATEORIGIN_OFFSET.y - pos.y };
}

void MobiusGauge::PostRender(RenderContext& rc, Camera* camera)
{
    //描画画面と同じ時のみ描画
    if (rc.GetRenderStep() == m_drawScreen)
    {
        m_mobiusBaseSprite.Draw(rc);
        m_mobiusGaugeRightSide.Draw(rc);
        m_mobiusGaugeLeftSide.Draw(rc);
        m_spGaugeSprite.Draw(rc);
        m_mobiusCoverSprite.Draw(rc);
    }
}