#include "stdafx.h"
#include "ResultScene.h"
#include "SampleScene.h"
#include "TitleScene.h"
ResultScene::~ResultScene()
{
	DeleteGO(m_title_fontRender);
	DeleteGO(m_replay_fontRender);
	DeleteGO(m_detail_fontRender);
}
bool ResultScene::Start()
{
	m_title_fontRender = NewGO<prefab::CFontRender>(1);
	m_title_fontRender->SetPosition({ m_titleX, m_titleY });//��
	m_title_fontRender->SetText(L"�^�C�g����");

	m_replay_fontRender = NewGO<prefab::CFontRender>(1);
	m_replay_fontRender->SetPosition({ m_replayX, m_replayY });//��
	m_replay_fontRender->SetText(L"������x");

	m_detail_fontRender = NewGO<prefab::CFontRender>(1);
	m_detail_fontRender->SetPosition({ m_detailX, m_detailY });//��
	m_detail_fontRender->SetText(L"�ڍו\��");

	return true;
}
void ResultScene::Update()
{
	if (g_pad[0]->IsTrigger(enButtonUp) || g_pad[1]->IsTrigger(enButtonDown)) {
		m_menuselect--;
		if (m_menuselect <= -1) {
			m_menuselect = 2;//���܂ōs���Əォ��ɂȂ�
		}
	}
	if (g_pad[0]->IsTrigger(enButtonDown) || g_pad[1]->IsTrigger(enButtonUp)) {
		m_menuselect++;

		if (m_menuselect >= 3) {
			m_menuselect = 0;//��܂ōs���Ɖ�����ɂȂ�
		}
	}
	m_titleX = 0.0f;
	m_replayX = 0.0f;
	m_detailX = 0.0f;
	//�I�����Ă��镔�����ԂɂȂ�
	switch (m_menuselect)
	{
	case 0://�オ��
		m_title_fontRender->SetColor({ red });
		m_replay_fontRender->SetColor({ blue });
		m_detail_fontRender->SetColor({ blue });
		m_titleX -= 50.0f;
		break;
	case 1://������
		m_title_fontRender->SetColor({ blue });
		m_replay_fontRender->SetColor({ red });
		m_detail_fontRender->SetColor({ blue });
		m_replayX -= 50.0f;
		break;
	case 2://������
		m_title_fontRender->SetColor({ blue });
		m_replay_fontRender->SetColor({ blue });
		m_detail_fontRender->SetColor({ red });
		m_detailX -= 50.0f;
		break;
	default:
		break;
	}
	m_title_fontRender->SetPosition({ m_titleX,m_titleY });
	m_replay_fontRender->SetPosition({ m_replayX,m_replayY });
	m_detail_fontRender->SetPosition({ m_detailX,m_detailY });
	//�X�^�[�g��A�{�^���������ƃL�����I����ʂɑJ�ڂ���
	if (g_pad[0]->IsTrigger(enButtonA) || g_pad[1]->IsTrigger(enButtonA)) {
		
		if (m_menuselect == 0) {
			TitleScene* titlescene = NewGO<TitleScene>(0, "titlescene");
			DeleteGO(this);
		}
		if (m_menuselect == 1) {
			SampleScene* samplescene = NewGO<SampleScene>(0, "gamescene");
			DeleteGO(this);
		}
		if (m_menuselect == 2) {
			//���U���g�\��
			//ResultDisplay();
		}		
	}
}

//void ResultScene::ResultDisplay()
//{
//	if (m_resultFirstTime == true)
//	{		
//		m_resultFirstTime = false;		
//		//�X�e�[�^�X�\���̏�����
//		m_result_fontRender = NewGO<prefab::CFontRender>(1);
//		m_result_fontRender->SetDrawScreen((prefab::CFontRender::DrawScreen)m_playerNum);
//		m_result_fontRender->SetColor({ 1,1,1,1 });
//		
//	}
//	if (m_playerNum == 0)
//	{
//		//���������v�Z(0�Ŋ���ƃo�O�邽�߁{�P�����Ă���)
//		m_HitRate = ((m_enemy->m_TakeAttackNum+1) / (m_AttackNum+1))*100;
//		if (m_resultPos.x <= -600)
//		{
//			m_resultPos.x++;
//		}
//		m_result_fontRender->SetPosition({ m_resultPos });
//
//		//�̗́A�`���[�W�A���݂̎����̎��͂̏�Ԃ̕\��
//		wchar_t HitRate[256];
//		swprintf_s(HitRate, L"\n������%d%%", int(m_HitRate));
//		m_result_fontRender->SetText(L"�U����" + std::to_wstring(int(m_AttackNum)) + HitRate + L"\n�o�[�X�g��" + std::to_wstring(m_BurstNum)
//			+ L"\n�e��D������" + std::to_wstring(m_StealNum) + L"\n�����痎������" + std::to_wstring(m_LandingNum)
//			+ L"\n�󂯂��_���[�W��" + std::to_wstring(m_ReceivedDamage) + L"\n���܂����K�E�Z�|�C���g" + std::to_wstring(m_SaveSP));
//
//	}
//	if (m_playerNum == 1)
//	{
//		//���������v�Z(0�Ŋ���ƃo�O�邽�߁{�P�����Ă���)
//		m_HitRate = ((m_enemy->m_TakeAttackNum + 1) / (m_AttackNum + 1)) * 100;
//		if (m_resultPos.x <= -600)
//		{
//			m_resultPos.x++;
//		}
//		m_result_fontRender->SetPosition({ m_resultPos });
//
//		//�̗́A�`���[�W�A���݂̎����̎��͂̏�Ԃ̕\��
//		wchar_t HitRate[256];
//		swprintf_s(HitRate, L"\n������%d%%", int(m_HitRate));
//		m_result_fontRender->SetText(L"�U����" + std::to_wstring(int(m_AttackNum)) + HitRate + L"\n�o�[�X�g��" + std::to_wstring(m_BurstNum)
//			+ L"\n�e��D������" + std::to_wstring(m_StealNum) + L"\n�����痎������" + std::to_wstring(m_LandingNum)
//			+ L"\n�󂯂��_���[�W��" + std::to_wstring(m_ReceivedDamage) + L"\n���܂����K�E�Z�|�C���g" + std::to_wstring(m_SaveSP));
//
//	}
//}
