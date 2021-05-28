#include "stdafx.h"
#include "GameOption.h"

GameOption::~GameOption()
{
	//スプライト、フォントの削除
	DeleteGO(m_optionBackGroundSprite);
	DeleteGO(m_BGMVolumeFont);
	DeleteGO(m_SEVolumeFont);
	DeleteGO(m_1PSensitivityFont);
	DeleteGO(m_2PSensitivityFont);
}

bool GameOption::Start()
{
	//背景スプライト
	m_optionBackGroundSprite = NewGO<prefab::CSpriteRender>(0);
	m_optionBackGroundSprite->Init("Assets/Image/White.dds", 960, 540);
	m_optionBackGroundSprite->SetScale({ 0.0f,0.0f,0.0f });

	//オプションフォント
	m_optionFont = NewGO<prefab::CFontRender>(0);
	m_optionFont->SetScale({ 0.0f,0.0f });
	m_optionFont->SetPosition({ -125.0f,220.0f });
	m_optionFont->SetText(L"OPTION");

	//BGMボリュームフォント
	m_BGMVolumeFont = NewGO<prefab::CFontRender>(0);
	m_BGMVolumeFont->SetScale({ 0.0f,0.0f });
	m_BGMVolumeFont->SetPosition({ -450.0f,120.0f });


	//SEボリュームフォント
	m_SEVolumeFont = NewGO<prefab::CFontRender>(0);
	m_SEVolumeFont->SetScale({ 0.0f,0.0f });
	m_SEVolumeFont->SetPosition({ -450.0f,20.0f });


	//1P操作感度フォント
	m_1PSensitivityFont = NewGO<prefab::CFontRender>(0);
	m_1PSensitivityFont->SetScale({ 0.0f,0.0f });
	m_1PSensitivityFont->SetPosition({ -450.0f,-80.0f });


	//2P操作感度フォント
	m_2PSensitivityFont = NewGO<prefab::CFontRender>(0);
	m_2PSensitivityFont->SetScale({ 0.0f,0.0f });
	m_2PSensitivityFont->SetPosition({ -450.0f,-180.0f });
	return true;
}

void GameOption::Update()
{
	//オプションが開いていて、拡大が終わっている。
	if (m_optionBackGroundSprite->m_spriteSupporter.GetSpriteScaleListLen() == 0 && isOpen == true)
	{
		//背景を灰色に
		m_optionBackGroundSprite->SetMulColor({ 0.0f,0.75f,0.75f,0.8f });

		//フォントの拡大率を1倍にすることで擬似的に表示
		m_optionFont->SetScale({ 1.0f,1.0f });
		m_BGMVolumeFont->SetScale({ 1.0f,1.0f });
		m_SEVolumeFont->SetScale({ 1.0f,1.0f });
		m_1PSensitivityFont->SetScale({ 1.0f,1.0f });
		m_2PSensitivityFont->SetScale({ 1.0f,1.0f });

		//選択したフォント以外を白にするため一旦全部白にしている(switch文の中で選択しているやつ以外を白にする形でもいいかも)
		m_BGMVolumeFont->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		m_SEVolumeFont->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		m_1PSensitivityFont->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		m_2PSensitivityFont->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		//項目の選択中なら
		if (m_selectingState == enItem)
		{
			//下を押すと下の項目へ
			if (g_pad[0]->IsTrigger(enButtonDown))
			{
				if (++m_selectingItem == 4)
				{
					m_selectingItem = 0;
				}
			}
			else if(g_pad[0]->IsTrigger(enButtonUp))//上を押すと上の項目へ
			{
				if (--m_selectingItem == -1)
				{
					m_selectingItem = 3;
				}
			}

			//選んでいる項目をポインタにセット
			//NOTE:switch文の多用を避けたくてポインタにセットしたが、設定項目によって数値の幅が違う以上微妙かも...
			switch (m_selectingItem)
			{
			case 0://BGMボリューム
				m_selectingItemValue = &m_BGMVolume;
				m_selectingItemFont = m_BGMVolumeFont;
				break;
			case 1://SEボリューム
				m_selectingItemValue = &m_SEVolume;
				m_selectingItemFont = m_SEVolumeFont;
				break;
			case 2://1Pカメラ感度
				m_selectingItemValue = &m_1PSensitivity;
				m_selectingItemFont = m_1PSensitivityFont;
				break;
			case 3://2Pカメラ感度
				m_selectingItemValue = &m_2PSensitivity;
				m_selectingItemFont = m_2PSensitivityFont;
				break;
			}

			//選択されている項目を赤黒色に
			m_selectingItemFont->SetColor({ 0.5f,0.0f,0.0f,1.0f });

			//Aボタンを押すと選択している項目の数値を設定するモードに移行
			if (g_pad[0]->IsTrigger(enButtonA))
			{
				m_selectingState = enNumeric;

				//数値設定前の値を記録
				m_selectItemTemporaryValue = *m_selectingItemValue;
			}
		}
		else if(m_selectingState == enNumeric)//項目の数値を設定するモードなら
		{
			//下を押すと数値を減らす
			if (g_pad[0]->IsPress(enButtonDown))
			{
				(*m_selectingItemValue) -= 0.1f;
			}
			else if(g_pad[0]->IsPress(enButtonUp))//上を押すと数値を増やす
			{
				(*m_selectingItemValue) += 0.1f;
			}

			//選択されている項目を青黒色に
			m_selectingItemFont->SetColor({ 0.0f,0.0f,0.5f,1.0f });

			if (g_pad[0]->IsTrigger(enButtonA))
			{
				m_selectingState = enItem;
			}
			else if (g_pad[0]->IsTrigger(enButtonB))
			{
				m_selectingState = enItem;

				*m_selectingItemValue = m_selectItemTemporaryValue;
			}
		}

		//変更された数値を元にフォントの描画文字列を変更
		swprintf_s(m_buffer, L"BGMVOLUME = %.2f", m_BGMVolume);
		m_BGMVolumeFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"SEVOLUME = %.2f", m_SEVolume);
		m_SEVolumeFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"1PSENSITIVITY = %.2f", m_1PSensitivity);
		m_1PSensitivityFont->SetText(m_buffer);
		swprintf_s(m_buffer, L"2PSENSITIVITY = %.2f", m_2PSensitivity);
		m_2PSensitivityFont->SetText(m_buffer);

	}
	else//それ以外
	{
		//背景を白色に
		m_optionBackGroundSprite->SetMulColor({ 1.0f,1.0f,1.0f,1.0f });

		//フォントの拡大率を0倍にすることで擬似的に消去
		m_optionFont->SetScale({ 0.0f,0.0f });
		m_BGMVolumeFont->SetScale({ 0.0f,0.0f });
		m_SEVolumeFont->SetScale({ 0.0f,0.0f });
		m_1PSensitivityFont->SetScale({ 0.0f,0.0f });
		m_2PSensitivityFont->SetScale({ 0.0f,0.0f });
	}
}

void GameOption::Open()
{
	//背景の拡大率を1倍にするキューをセット
	m_optionBackGroundSprite->m_spriteSupporter.SpriteScale(Vector3::One, 12, 1);

	//開いたフラグをオン
	isOpen = true;
}

void GameOption::Close()
{
	//背景の拡大率を0倍にするキューをセット
	m_optionBackGroundSprite->m_spriteSupporter.SpriteScale(Vector3::Zero, 12, 1);

	//開いたフラグをオフ
	isOpen = false;

	//選択項目をリセット
	m_selectingItem = 0;
}