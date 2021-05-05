#pragma once
class MobiusGauge : public IGameObject
{
	~MobiusGauge();
	bool Start() override;
	void Update() override;
	void PostRender(RenderContext& rc, Camera* camera)override;

public:

	//ゲージの座標を設定。
	void SetPosition(const Vector3& pos);

	enum DrawScreen
	{
		Screen1,//左画面
		Screen2,//右画面
		AllScreen//画面全体
	};

	struct GaugeDatas //ゲージのデータ。
	{
		Vector3 spriteStartDir = { 1.0f,0.0f,0.0f };//回転の開始方向　垂直な方向を後から外積で調べるために三次元ベクトルにしている
		float gaugeProgress = 0;//ゲージの進み具合(角度)　0 ~ 360
		Vector2 rotateOrigin = { 0.0f,0.0f };//回転の中心座標
		int isClockwise = 0;//時計回りか? bool型だとうまく渡せなかったのでintにしている
		int colorFlag = -1;//赤色か青色か　赤(-1) 青(1)
	};

	GaugeDatas m_rightGaugeDatas;//右側のゲージのデータ
	GaugeDatas m_leftGaugeDatas;//左側のゲージのデータ

	Sprite m_mobiusBaseSprite;//メビウスゲージの背景。
	Sprite m_mobiusCoverSprite;//メビウスゲージの外側のカバー。
	Sprite m_mobiusGaugeRightSide;//メビウスゲージ(右側)
	Sprite m_mobiusGaugeLeftSide;//メビウスゲージ(左側)

	DrawScreen m_drawScreen = AllScreen;//描画先

	float m_charge;
	bool m_isRed = false;
};

