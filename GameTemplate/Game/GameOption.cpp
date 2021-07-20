#include "stdafx.h"
#include "GameOption.h"
#include <algorithm>


namespace
{
	const Vector2 FONT_OPTION_POSITION = { -125.0f,240.0f };
	const Vector2 FONT_BGMVOLUME_POSITION = { -450.0f,170.0f };
	const Vector2 FONT_SEVOLUME_POSITION = { -450.0f,100.0f };
	const Vector2 FONT_P1SENSITIVITY_POSITION = { -450.0f,30.0f };
	const Vector2 FONT_P2SENSITIVITY_POSITION = { -450.0f,-40.0f };
	const Vector2 FONT_GAMETIMELIMIT_POSITION = { -450.0f,-110.0f };
	const Vector2 FONT_ROUNDTOWIN_POSITION = { -450.0f,-180.0f };
	const Vector4 FONT_SHADOWCOLOR_BLACK = { 0,0,0,1 };
	const float FONT_SHADOWOFFSET = 2.0f;
	const Vector2 FONT_SCALE_CLOSE = { 0.0f,0.0f };
	const Vector2 FONT_SCALE_OPEN = { 0.8f,0.8f };
	const Vector4 FONT_COLOR_WHITE = { 1.0f,1.0f,1.0f,1.0f };
	const Vector4 FONT_COLOR_BLUE = { 0.1f,0.1f,1.0f,1.0f };
	const Vector4 FONT_COLOR_RED = { 1.0f,0.25f,0.25f,1.0f };

	const Vector4 SPRITE_COLOR_LIGHTBLUE = { 0.0f,0.75f,0.75f,0.8f };
	const Vector3 SPRITE_SCALE_CLOSE = { 0.0f,0.0f,0.0f };
	const Vector3 SPRITE_SCALE_OPEN = { 1.0f,1.0f,1.0f };
	const int SPRITE_SCALING_TIME = 12;
	const int SPRITE_SCALING_DELAY = 1;

	const float OPTION_VALUE_CHANGERATE = 0.05f;
	const float OPTION_GAMETIME_CHANGERATE = 10.0f;
	const float OPTION_ROUNDTOWIN_CHANGERATE = 2.0f;
	const int OPTION_ITEM_UNDERRANGE = -1;
	const int OPTION_ITEM_BGM = 0;
	const int OPTION_ITEM_SE = 1;
	const int OPTION_ITEM_P1SENSITIVITY = 2;
	const int OPTION_ITEM_P2SENSITIVITY = 3;
	const int OPTION_ITEM_GAMETIMELIMIT = 4;
	const int OPTION_ITEM_ROUNDTOWIN = 5;
	const int OPTION_ITEM_OVERRANGE = 6;

	const float SOUND_VOLUME_MIN = 0.0f;
	const float SOUND_VOLUME_MAX = 1.0f;

	const float CONTROL_SENSITIVITY_MIN = 0.1f;
	const float CONTROL_SENSITIVITY_MAX = 5.0f;
	
	const float GAME_TIMELIMIT_MIN = 30.0f;
	const float GAME_TIMELIMIT_MAX = 90.0f;

	const float GAME_ROUNDTOWIN_MIN = 1.0f;
	const float GAME_ROUNDTOWIN_MAX = 3.0f;

	const float SOUND_OPTIONDISPLAY_VOLUME = 0.5f;
}

bool GameOption::m_isInited = false;							//初期化されたか
OptionValue GameOption::m_BGMVolume;							//BGMのボリューム					
OptionValue GameOption::m_SEVolume;							//効果音のボリューム							
OptionValue GameOption::m_P1Sensitivity;						//プレイヤー1のカメラ感度							
OptionValue GameOption::m_P2Sensitivity;						//プレイヤー2のカメラ感度							
OptionValue GameOption::m_gameTimeLimit;						//ゲームの制限時間								
OptionValue GameOption::m_roundToWin;							//勝利に必要なラウンド数

GameOption::~GameOption()
{
	//スプライト、フォントの削除
	DeleteGO(m_optionBackGroundSprite);
	DeleteGO(m_BGMVolumeFont);
	DeleteGO(m_SEVolumeFont);
	DeleteGO(m_1PSensitivityFont);
	DeleteGO(m_2PSensitivityFont);
	DeleteGO(m_gameTimeLimitFont);
	DeleteGO(m_roundToWinFont);
}

bool GameOption::Start()
{
	//背景スプライト
	m_optionBackGroundSprite = NewGO<prefab::CSpriteRender>(0);
	m_optionBackGroundSprite->Init("Assets/Image/White.dds", 960, 540);
	m_optionBackGroundSprite->SetScale(SPRITE_SCALE_CLOSE);
	m_optionBackGroundSprite->SetMulColor(SPRITE_COLOR_LIGHTBLUE);

	//オプションフォント
	m_optionFont = NewGO<prefab::CFontRender>(0);
	m_optionFont->SetScale(FONT_SCALE_CLOSE);
	m_optionFont->SetPosition(FONT_OPTION_POSITION);
	m_optionFont->SetText(L"OPTION");
	m_optionFont->SetShadowFlag(true);
	m_optionFont->SetShadowColor(FONT_SHADOWCOLOR_BLACK);
	m_optionFont->SetShadowOffset(FONT_SHADOWOFFSET);

	//BGMボリュームフォント
	m_BGMVolumeFont = NewGO<prefab::CFontRender>(0);
	m_BGMVolumeFont->SetScale(FONT_SCALE_CLOSE);
	m_BGMVolumeFont->SetPosition(FONT_BGMVOLUME_POSITION);
	m_BGMVolumeFont->SetShadowFlag(true);
	m_BGMVolumeFont->SetShadowColor(FONT_SHADOWCOLOR_BLACK);
	m_BGMVolumeFont->SetShadowOffset(FONT_SHADOWOFFSET);


	//SEボリュームフォント
	m_SEVolumeFont = NewGO<prefab::CFontRender>(0);
	m_SEVolumeFont->SetScale(FONT_SCALE_CLOSE);
	m_SEVolumeFont->SetPosition(FONT_SEVOLUME_POSITION);
	m_SEVolumeFont->SetShadowFlag(true);
	m_SEVolumeFont->SetShadowColor(FONT_SHADOWCOLOR_BLACK);
	m_SEVolumeFont->SetShadowOffset(FONT_SHADOWOFFSET);


	//1P操作感度フォント
	m_1PSensitivityFont = NewGO<prefab::CFontRender>(0);
	m_1PSensitivityFont->SetScale(FONT_SCALE_CLOSE);
	m_1PSensitivityFont->SetPosition(FONT_P1SENSITIVITY_POSITION);
	m_1PSensitivityFont->SetShadowFlag(true);
	m_1PSensitivityFont->SetShadowColor(FONT_SHADOWCOLOR_BLACK);
	m_1PSensitivityFont->SetShadowOffset(FONT_SHADOWOFFSET);


	//2P操作感度フォント
	m_2PSensitivityFont = NewGO<prefab::CFontRender>(0);
	m_2PSensitivityFont->SetScale(FONT_SCALE_CLOSE);
	m_2PSensitivityFont->SetPosition(FONT_P2SENSITIVITY_POSITION);
	m_2PSensitivityFont->SetShadowFlag(true);
	m_2PSensitivityFont->SetShadowColor(FONT_SHADOWCOLOR_BLACK);
	m_2PSensitivityFont->SetShadowOffset(FONT_SHADOWOFFSET);
	

	//ゲーム制限時間フォント
	m_gameTimeLimitFont = NewGO<prefab::CFontRender>(0);
	m_gameTimeLimitFont->SetScale(FONT_SCALE_CLOSE);
	m_gameTimeLimitFont->SetPosition(FONT_GAMETIMELIMIT_POSITION);
	m_gameTimeLimitFont->SetShadowFlag(true);
	m_gameTimeLimitFont->SetShadowColor(FONT_SHADOWCOLOR_BLACK);
	m_gameTimeLimitFont->SetShadowOffset(FONT_SHADOWOFFSET);
	
	//勝利に必要なラウンド数フォント
	m_roundToWinFont = NewGO<prefab::CFontRender>(0);
	m_roundToWinFont->SetScale(FONT_SCALE_CLOSE);
	m_roundToWinFont->SetPosition(FONT_ROUNDTOWIN_POSITION);
	m_roundToWinFont->SetShadowFlag(true);
	m_roundToWinFont->SetShadowColor(FONT_SHADOWCOLOR_BLACK);
	m_roundToWinFont->SetShadowOffset(FONT_SHADOWOFFSET);

	return true;
}

void GameOption::Update()
{
	//オプションが開いていて、拡大が終わっている。
	if (isQueuing() == false && m_isOpen == true)
	{
		//フォントの拡大率を1倍にすることで擬似的に表示
		m_optionFont->SetScale(FONT_SCALE_OPEN);
		m_BGMVolumeFont->SetScale(FONT_SCALE_OPEN);
		m_SEVolumeFont->SetScale(FONT_SCALE_OPEN);
		m_1PSensitivityFont->SetScale(FONT_SCALE_OPEN);
		m_2PSensitivityFont->SetScale(FONT_SCALE_OPEN);
		m_gameTimeLimitFont->SetScale(FONT_SCALE_OPEN);
		m_roundToWinFont->SetScale(FONT_SCALE_OPEN);

		//選択したフォント以外を白にするため一旦全部白にしている
		m_BGMVolumeFont->SetColor(Vector4::White);
		m_SEVolumeFont->SetColor(Vector4::White);
		m_1PSensitivityFont->SetColor(Vector4::White);
		m_2PSensitivityFont->SetColor(Vector4::White);
		m_gameTimeLimitFont->SetColor(Vector4::White);
		m_roundToWinFont->SetColor(Vector4::White);

		//項目の選択中なら
		if (m_selectingState == enItem)
		{
			//下を押すと下の項目へ
			if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonDown) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonDown))
			{
				if (++m_selectingItem == OPTION_ITEM_OVERRANGE)
				{
					m_selectingItem = OPTION_ITEM_BGM;
				}
				SoundOneShotPlay(L"Assets/sound/OptionValueChange.wav", SOUND_OPTIONDISPLAY_VOLUME);

			}
			else if(g_pad[PAD_PLAYER1]->IsTrigger(enButtonUp) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonUp))//上を押すと上の項目へ
			{
				if (--m_selectingItem == OPTION_ITEM_UNDERRANGE)
				{
					m_selectingItem = OPTION_ITEM_ROUNDTOWIN;
				}
				SoundOneShotPlay(L"Assets/sound/OptionValueChange.wav", SOUND_OPTIONDISPLAY_VOLUME);

			}

			//選んでいる項目をポインタにセット
			switch (m_selectingItem)
			{
			case OPTION_ITEM_BGM://BGMボリューム
				m_selectingItemValue = &m_BGMVolume;
				m_selectingItemFont = m_BGMVolumeFont;
				break;
			case OPTION_ITEM_SE://SEボリューム
				m_selectingItemValue = &m_SEVolume;
				m_selectingItemFont = m_SEVolumeFont;
				break;
			case OPTION_ITEM_P1SENSITIVITY://1Pカメラ感度
				m_selectingItemValue = &m_P1Sensitivity;
				m_selectingItemFont = m_1PSensitivityFont;
				break;
			case OPTION_ITEM_P2SENSITIVITY://2Pカメラ感度
				m_selectingItemValue = &m_P2Sensitivity;
				m_selectingItemFont = m_2PSensitivityFont;
				break;
			case OPTION_ITEM_GAMETIMELIMIT://ゲーム制限時間
				m_selectingItemValue = &m_gameTimeLimit;
				m_selectingItemFont = m_gameTimeLimitFont;
				break;
			case OPTION_ITEM_ROUNDTOWIN://勝利に必要なラウンド
				m_selectingItemValue = &m_roundToWin;
				m_selectingItemFont = m_roundToWinFont;
				break;
			}

			//選択されている項目を青色に
			m_selectingItemFont->SetColor(FONT_COLOR_BLUE);

			//Aボタンを押すと選択している項目の数値を設定するモードに移行
			if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonA) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonA))
			{
				m_selectingState = enNumeric;

				//数値設定前の値を記録
				m_selectingItemTemporaryValue = m_selectingItemValue->GetValue();
				SoundOneShotPlay(L"Assets/sound/OptionValueChange.wav", SOUND_OPTIONDISPLAY_VOLUME);

			}
		}
		else if(m_selectingState == enNumeric)//項目の数値を設定するモードなら
		{
			//設定項目の増減モードで分岐
			switch (m_selectingItemValue->GetValueChangeMode())
			{
			case OptionValue::enTrigger:
				if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonDown) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonDown))
				{
					m_selectingItemValue->DecreaseValue();//値の減少
					SoundOneShotPlay(L"Assets/sound/OptionValueChange.wav", SOUND_OPTIONDISPLAY_VOLUME);
				}
				else if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonUp) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonUp))//上を押すと数値を増やす
				{
					m_selectingItemValue->IncreaseValue();//値の増加
					SoundOneShotPlay(L"Assets/sound/OptionValueChange.wav", SOUND_OPTIONDISPLAY_VOLUME);

				}
				break;
			case OptionValue::enPress:
				if (g_pad[PAD_PLAYER1]->IsPress(enButtonDown) || g_pad[PAD_PLAYER2]->IsPress(enButtonDown))
				{
					m_selectingItemValue->DecreaseValue();//値の減少
					SoundOneShotPlay(L"Assets/sound/OptionValueChange.wav", SOUND_OPTIONDISPLAY_VOLUME);
				}
				else if (g_pad[PAD_PLAYER1]->IsPress(enButtonUp) || g_pad[PAD_PLAYER2]->IsPress(enButtonUp))//上を押すと数値を増やす
				{
					m_selectingItemValue->IncreaseValue();//値の増加
					SoundOneShotPlay(L"Assets/sound/OptionValueChange.wav", SOUND_OPTIONDISPLAY_VOLUME);

				}
			}
			

			//選んでいる値がBGMの音量なら
			if (m_selectingItemValue == &m_BGMVolume)
			{
				//設定値をBGMの音量としてセット
				CSoundEngine::GetInstance()->SetBGMVolume(m_selectingItemValue->GetValue());
			}
			else if (m_selectingItemValue == &m_SEVolume)//選んでいる値がSEの音量なら
			{
				//設定値をSEの音量としてセット
				CSoundEngine::GetInstance()->SetSEVolume(m_selectingItemValue->GetValue());
			}
			//選択されている項目を赤色に
			m_selectingItemFont->SetColor(FONT_COLOR_RED);

			//Aボタンを押すと値そのまま項目選択へ(値の決定)
			if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonA) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonA))
			{
				m_selectingState = enItem;
				SoundOneShotPlay(L"Assets/sound/OptionSelect.wav", SOUND_OPTIONDISPLAY_VOLUME);

			}
			else if (g_pad[PAD_PLAYER1]->IsTrigger(enButtonB) || g_pad[PAD_PLAYER2]->IsTrigger(enButtonB))//Bボタンを押すと値を変更前に戻して項目選択へ(値のキャンセル)
			{
				m_selectingState = enItem;

				m_selectingItemValue->SetValue(m_selectingItemTemporaryValue);

				if (m_selectingItemValue == &m_BGMVolume)
				{
					//設定値をBGMの音量としてセット
					CSoundEngine::GetInstance()->SetBGMVolume(m_selectingItemValue->GetValue());
				}
				else if (m_selectingItemValue == &m_SEVolume)//選んでいる値がSEの音量なら
				{
					//設定値をSEの音量としてセット
					CSoundEngine::GetInstance()->SetSEVolume(m_selectingItemValue->GetValue());
				}

				SoundOneShotPlay(L"Assets/sound/OptionCancel.wav", SOUND_OPTIONDISPLAY_VOLUME);

			}
		}

		//変更された数値を元にフォントの描画文字列を変更
		swprintf_s(m_buffer, L"BGM VOLUME     = %.2f", m_BGMVolume.GetValue());
		m_BGMVolumeFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"SE  VOLUME     = %.2f", m_SEVolume.GetValue());
		m_SEVolumeFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"1P SENSITIVITY = %.2f", m_P1Sensitivity.GetValue());
		m_1PSensitivityFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"2P SENSITIVITY = %.2f", m_P2Sensitivity.GetValue());
		m_2PSensitivityFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"TIMELIMIT      = %.2f", m_gameTimeLimit.GetValue());
		m_gameTimeLimitFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"ROUNDTOWIN     = %.0f", m_roundToWin.GetValue());
		m_roundToWinFont->SetText(m_buffer);

	}
	else//それ以外
	{
		//フォントの拡大率を0倍にすることで擬似的に消去
		m_optionFont->SetScale(FONT_SCALE_CLOSE);
		m_BGMVolumeFont->SetScale(FONT_SCALE_CLOSE);
		m_SEVolumeFont->SetScale(FONT_SCALE_CLOSE);
		m_1PSensitivityFont->SetScale(FONT_SCALE_CLOSE);
		m_2PSensitivityFont->SetScale(FONT_SCALE_CLOSE);
		m_gameTimeLimitFont->SetScale(FONT_SCALE_CLOSE);
		m_roundToWinFont->SetScale(FONT_SCALE_CLOSE);
	}
}

void GameOption::Open()
{
	if (isQueuing() == false)
	{
		//背景の拡大率を1倍にするキューをセット
		m_optionBackGroundSprite->GetSpriteSupporter().SpriteScale(SPRITE_SCALE_OPEN, SPRITE_SCALING_TIME, SPRITE_SCALING_DELAY);

		//開くサウンドを再生
		SoundOneShotPlay(L"Assets/sound/OptionDisplayOn.wav", SOUND_OPTIONDISPLAY_VOLUME);

		//開いたフラグをオン
		m_isOpen = true;
	}
}

void GameOption::Close()
{
	if (isQueuing() == false)
	{
		//背景の拡大率を0倍にするキューをセット
		m_optionBackGroundSprite->GetSpriteSupporter().SpriteScale(SPRITE_SCALE_CLOSE, SPRITE_SCALING_TIME, SPRITE_SCALING_DELAY);

		//閉じるサウンドを再生
		SoundOneShotPlay(L"Assets/sound/OptionDisplayOff.wav", SOUND_OPTIONDISPLAY_VOLUME);

		//開いたフラグをオフ
		m_isOpen = false;

		//選択項目をリセット
		m_selectingItem = OPTION_ITEM_BGM;

		//書き込み
		WriteOption();
	}
}

bool GameOption::ReadOption()
{
	//オープンに失敗した時用に初期化
	Init();

	FILE* fp = fopen("gameoption.dat", "rb");

	if (fp == nullptr)
	{
		return false;
	}

	//直接OptionValueに格納できなかったので(参照を返す形にすればいけそうだけど...)ローカル変数に一旦格納
	float BGMVolume,SEVolume, P1Sensitivity, P2Sensitivity, gameTimeLimit, roundToWin;
	fread(&BGMVolume,sizeof(float),1,fp);
	fread(&SEVolume,sizeof(float),1,fp);
	fread(&P1Sensitivity,sizeof(float),1,fp);
	fread(&P2Sensitivity,sizeof(float),1,fp);
	fread(&gameTimeLimit,sizeof(float),1,fp);
	fread(&roundToWin,sizeof(float),1,fp);

	fclose(fp);

	//読み込んだ各値をセット
	m_BGMVolume.SetValue(BGMVolume);
	m_SEVolume.SetValue(SEVolume);
	m_P1Sensitivity.SetValue(P1Sensitivity);
	m_P2Sensitivity.SetValue(P2Sensitivity);
	m_gameTimeLimit.SetValue(gameTimeLimit);
	m_roundToWin.SetValue(roundToWin);

	//音量は読み込んだ時にセット
	CSoundEngine::GetInstance()->SetBGMVolume(m_BGMVolume.GetValue());
	CSoundEngine::GetInstance()->SetSEVolume(m_SEVolume.GetValue());

	return true;
}

bool GameOption::WriteOption()
{
	FILE* fp = fopen("gameoption.dat", "wb");

	if (fp == nullptr)
	{
		return false;
	}

	//OptionValueから直接書き込めなかったので一旦ローカル変数に格納
	float BGMVolume, SEVolume, P1Sensitivity, P2Sensitivity, gameTimeLimit, roundToWin;

	BGMVolume = m_BGMVolume.GetValue();
	SEVolume = m_SEVolume.GetValue();
	P1Sensitivity = m_P1Sensitivity.GetValue();
	P2Sensitivity = m_P2Sensitivity.GetValue();
	gameTimeLimit = m_gameTimeLimit.GetValue();
	roundToWin = m_roundToWin.GetValue();

	//書き込み
	fwrite(&BGMVolume, sizeof(float), 1, fp);
	fwrite(&SEVolume, sizeof(float), 1, fp);
	fwrite(&P1Sensitivity, sizeof(float), 1, fp);
	fwrite(&P2Sensitivity, sizeof(float), 1, fp);
	fwrite(&gameTimeLimit, sizeof(float), 1, fp);
	fwrite(&roundToWin, sizeof(float), 1, fp);

	fclose(fp);

	return true;
}

void GameOption::Init()
{
	if (m_isInited == true)
	{
		return;
	}

	//BGM
	m_BGMVolume.Init(OPTION_VALUE_CHANGERATE, SOUND_VOLUME_MIN, SOUND_VOLUME_MAX, OptionValue::enPress);
	m_BGMVolume.SetValue(1.0f);
	//SE
	m_SEVolume.Init(OPTION_VALUE_CHANGERATE, SOUND_VOLUME_MIN, SOUND_VOLUME_MAX, OptionValue::enPress);
	m_SEVolume.SetValue(1.0f);
	//プレイヤー1のカメラ感度
	m_P1Sensitivity.Init(OPTION_VALUE_CHANGERATE, CONTROL_SENSITIVITY_MIN, CONTROL_SENSITIVITY_MAX, OptionValue::enPress);
	m_P1Sensitivity.SetValue(2.0f);
	//プレイヤー2のカメラ感度
	m_P2Sensitivity.Init(OPTION_VALUE_CHANGERATE, CONTROL_SENSITIVITY_MIN, CONTROL_SENSITIVITY_MAX, OptionValue::enPress);
	m_P2Sensitivity.SetValue(2.0f);
	//ゲームの制限時間
	m_gameTimeLimit.Init(OPTION_GAMETIME_CHANGERATE, GAME_TIMELIMIT_MIN, GAME_TIMELIMIT_MAX, OptionValue::enTrigger);
	m_gameTimeLimit.SetValue(40.0f);
	//勝利に必要なラウンド数
	m_roundToWin.Init(OPTION_ROUNDTOWIN_CHANGERATE, GAME_ROUNDTOWIN_MIN, GAME_ROUNDTOWIN_MAX, OptionValue::enTrigger);
	m_roundToWin.SetValue(1.0f);

	m_isInited = true;
}