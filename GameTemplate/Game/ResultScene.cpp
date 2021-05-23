#include "stdafx.h"
#include "ResultScene.h"
#include "SampleScene.h"
#include "TitleScene.h"
ResultScene::~ResultScene()
{	
	DeleteGO(m_Down_SpriteRender);
	DeleteGO(m_Up_SpriteRender);
	DeleteGO(m_Under_SpriteRender);
	DeleteGO(m_Lose_SpriteRender);
	DeleteGO(m_Win_SpriteRender);
	DeleteGO(m_Command_SpriteRender);
}
bool ResultScene::Start()
{
	m_Down_SpriteRender = NewGO<prefab::CSpriteRender>(1);
	m_Down_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Down_SpriteRender->Init("Assets/Image/Result_Down.dds", 1280, 720);
	m_Down_SpriteRender->SetPosition({ m_DownPos });


	m_Up_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Up_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Up_SpriteRender->Init("Assets/Image/Result_Up.dds", 1280, 720);
	m_Up_SpriteRender->SetPosition({ m_UpPos });


	m_Under_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Under_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Under_SpriteRender->Init("Assets/Image/Result_Under.dds", 1280, 720);
	m_Under_SpriteRender->SetPosition({ m_UnderPos });

	m_Lose_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Lose_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Lose_SpriteRender->Init("Assets/Image/Result_Lose.dds", 372, 132);
	m_Lose_SpriteRender->SetPosition({ m_LosePos });

	m_Win_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Win_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Win_SpriteRender->Init("Assets/Image/Result_Win.dds", 300, 112);
	m_Win_SpriteRender->SetPosition({ m_WinPos });

	m_Command_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Command_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Command_SpriteRender->Init("Assets/Image/Result_Command_Retry.dds", 400, 76);
	m_Command_SpriteRender->SetPosition({ m_CommandPos });

	return true;
}
void ResultScene::Update()
{
	//上か下ボタンを押すとリトライと、タイトルが切り替える
	if (g_pad[0]->IsTrigger(enButtonUp) || g_pad[1]->IsTrigger(enButtonDown) || g_pad[0]->IsTrigger(enButtonDown) || g_pad[1]->IsTrigger(enButtonUp)) {
		if (m_RetryOn == true)
		{
			m_Command_SpriteRender->Init("Assets/Image/Result_Command_Title.dds", 400, 76);
			m_RetryOn = false;
		}
		else
		{
			m_Command_SpriteRender->Init("Assets/Image/Result_Command_Retry.dds", 400, 76);
			m_RetryOn = true;
		}
	}

	//スタートでAボタンを押すとキャラ選択画面に遷移する
	if (g_pad[0]->IsTrigger(enButtonA) || g_pad[1]->IsTrigger(enButtonA)) {

		if (m_RetryOn == true) {
			SampleScene* samplescene = NewGO<SampleScene>(0, "gamescene");
			DeleteGO(this);
		}
		if (m_RetryOn == false) {
			TitleScene* titlescene = NewGO<TitleScene>(0, "titlescene");
			DeleteGO(this);
		}
	}
}

//void ResultScene::ResultDisplay()
//{
//	if (m_resultFirstTime == true)
//	{		
//		m_resultFirstTime = false;		
//		//ステータス表示の初期化
//		m_result_fontRender = NewGO<prefab::CFontRender>(1);
//		m_result_fontRender->SetDrawScreen((prefab::CFontRender::DrawScreen)m_playerNum);
//		m_result_fontRender->SetColor({ 1,1,1,1 });
//		
//	}
//	if (m_playerNum == 0)
//	{
//		//命中率を計算(0で割るとバグるため＋１をしておく)
//		m_HitRate = ((m_enemy->m_TakeAttackNum+1) / (m_AttackNum+1))*100;
//		if (m_resultPos.x <= -600)
//		{
//			m_resultPos.x++;
//		}
//		m_result_fontRender->SetPosition({ m_resultPos });
//
//		//体力、チャージ、現在の自分の磁力の状態の表示
//		wchar_t HitRate[256];
//		swprintf_s(HitRate, L"\n命中率%d%%", int(m_HitRate));
//		m_result_fontRender->SetText(L"攻撃回数" + std::to_wstring(int(m_AttackNum)) + HitRate + L"\nバースト回数" + std::to_wstring(m_BurstNum)
//			+ L"\n弾を奪った回数" + std::to_wstring(m_StealNum) + L"\n穴から落ちた回数" + std::to_wstring(m_LandingNum)
//			+ L"\n受けたダメージ量" + std::to_wstring(m_ReceivedDamage) + L"\n溜まった必殺技ポイント" + std::to_wstring(m_SaveSP));
//
//	}
//	if (m_playerNum == 1)
//	{
//		//命中率を計算(0で割るとバグるため＋１をしておく)
//		m_HitRate = ((m_enemy->m_TakeAttackNum + 1) / (m_AttackNum + 1)) * 100;
//		if (m_resultPos.x <= -600)
//		{
//			m_resultPos.x++;
//		}
//		m_result_fontRender->SetPosition({ m_resultPos });
//
//		//体力、チャージ、現在の自分の磁力の状態の表示
//		wchar_t HitRate[256];
//		swprintf_s(HitRate, L"\n命中率%d%%", int(m_HitRate));
//		m_result_fontRender->SetText(L"攻撃回数" + std::to_wstring(int(m_AttackNum)) + HitRate + L"\nバースト回数" + std::to_wstring(m_BurstNum)
//			+ L"\n弾を奪った回数" + std::to_wstring(m_StealNum) + L"\n穴から落ちた回数" + std::to_wstring(m_LandingNum)
//			+ L"\n受けたダメージ量" + std::to_wstring(m_ReceivedDamage) + L"\n溜まった必殺技ポイント" + std::to_wstring(m_SaveSP));
//
//	}
//}
