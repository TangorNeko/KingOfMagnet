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
	DeleteGO(m_BG_SpriteRender);
	DeleteGO(m_P1_skinModelRender);
	DeleteGO(m_P2_skinModelRender);
	DeleteGO(m_winnerDirectionLight);
}			 
bool ResultScene::Start()
{	
	TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::NanameBox, 30, true);

	//�w�i
	m_BG_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_BG_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)0);
	m_BG_SpriteRender->SetPosition({ Vector3::Zero });
	m_BG_SpriteRender->Init("Assets/Image/TitleBG.dds", 1280, 720);
	//���f���̕`��O�ɃX�v���C�g��`��ł���悤�ɂ���B
	m_BG_SpriteRender->SetPostRenderMode(false);
	m_BG_SpriteRender->SetPosition({ m_UpPos });

	m_Up_SpriteRender = NewGO<prefab::CSpriteRender>(1);
	m_Up_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)0);
	m_Up_SpriteRender->Init("Assets/Image/Result_Up.dds", 1280, 720);
	//���f���̕`��O�ɃX�v���C�g��`��ł���悤�ɂ���B
	m_Up_SpriteRender->SetPostRenderMode(false);
	m_Up_SpriteRender->SetPosition({ m_UpPos.x,m_UpPos.y + CoverMove,m_UpPos.z });

	m_Under_SpriteRender = NewGO<prefab::CSpriteRender>(1);
	m_Under_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)0);
	m_Under_SpriteRender->Init("Assets/Image/Result_Under.dds", 1280, 720);
	//���f���̕`��O�ɃX�v���C�g��`��ł���悤�ɂ���B
	m_Under_SpriteRender->SetPostRenderMode(false);
	m_Under_SpriteRender->SetPosition({ m_UnderPos });
	m_Under_SpriteRender->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });

	m_Lose_SpriteRender = NewGO<prefab::CSpriteRender>(1);
	m_Lose_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Lose_SpriteRender->Init("Assets/Image/Result_Lose.dds", 372, 132);
	m_Lose_SpriteRender->SetPosition({ m_LosePos });
	m_Lose_SpriteRender->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });

	m_Win_SpriteRender = NewGO<prefab::CSpriteRender>(1);
	m_Win_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Win_SpriteRender->Init("Assets/Image/Result_Win.dds", 300, 112);
	m_Win_SpriteRender->SetPosition({ m_WinPos });
	m_Win_SpriteRender->SetMulColor({ 1.0f,1.0f,1.0f,0.0f });

	m_Command_SpriteRender = NewGO<prefab::CSpriteRender>(1);
	m_Command_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Command_SpriteRender->Init("Assets/Image/Result_Command_Retry.dds", 400, 76);
	m_Command_SpriteRender->SetPosition({ m_CommandPos.x,m_CommandPos.y - CoverMove,m_CommandPos.z });
	m_Command_SpriteRender->SetMulColor({ 0.0f,0.0f,0.0f,1.0f });

	//���ҁA�s�҂̃��f��
	m_animationClips[enAnimationClip_Win].Load("Assets/animData/Mage_SpecialAttack2.tka");
	m_animationClips[enAnimationClip_Win].SetLoopFlag(true);
	m_animationClips[enAnimationClip_Lose].Load("Assets/animData/Mage_Stunned.tka");
	m_animationClips[enAnimationClip_Lose].SetLoopFlag(true);
	m_P1_skinModelRender = NewGO<prefab::CSkinModelRender>(3);
	m_P1_skinModelRender->Init("Assets/modelData/Player1.tkm", "Assets/modelData/Mage.tks",m_animationClips,enAnimationClip_Num);
	m_P2_skinModelRender = NewGO<prefab::CSkinModelRender>(3);
	m_P2_skinModelRender->Init("Assets/modelData/Player2.tkm", "Assets/modelData/Mage.tks", m_animationClips, enAnimationClip_Num);
	//�������v���C���[�ŕ���
	if (m_loserNum == 0)
	{
		m_P1_skinModelRender->SetPosition(m_LoserPos);
		m_P2_skinModelRender->SetPosition(m_WinnerPos);
	}
	else
	{
		m_P1_skinModelRender->SetPosition(m_WinnerPos);
		m_P2_skinModelRender->SetPosition(m_LoserPos);
	}

	m_winnerDirectionLight = NewGO<prefab::CDirectionLight>(0);
	m_winnerDirectionLight->SetDirection({ 0.0f,-1.0f,-1.0f });
	m_winnerDirectionLight->SetColor({ 1.0f,1.0f,1.0f });

	m_Down_SpriteRender = NewGO<prefab::CSpriteRender>(2);
	m_Down_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Down_SpriteRender->Init("Assets/Image/Result_Down.dds", 1280, 720);
	m_Down_SpriteRender->SetPosition({ m_UpPos.x,m_UpPos.y - CoverMove,m_UpPos.z });

	g_camera3D[0]->SetPosition(m_cameraPos);
	g_camera3D[0]->SetTarget({ 0,90,0 });

	/*m_Down_SpriteRender->SetScale({ 0,0,0 });
	m_Up_SpriteRender->SetScale({ 0,0,0 });
	m_Under_SpriteRender->SetScale({ 0,0,0 });
	m_Lose_SpriteRender->SetScale({ 0,0,0 });
	m_Win_SpriteRender->SetScale({ 0,0,0 });
	m_Command_SpriteRender->SetScale({ 0,0,0 });*/

	return true;
}
void ResultScene::Update()
{
	if (m_loserNum == 0)
	{
		m_P1_skinModelRender->PlayAnimation(enAnimationClip_Lose);
		m_P2_skinModelRender->PlayAnimation(enAnimationClip_Win);
	}
	else
	{
		m_P1_skinModelRender->PlayAnimation(enAnimationClip_Win);
		m_P2_skinModelRender->PlayAnimation(enAnimationClip_Lose);
	}

	if (m_moveEndFlag == false) {
		if (m_moveTimer == 0) {
			//�ړ��\��
			m_Up_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,-CoverMove - 20.0f }, 30, 0, true);
			m_Up_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,40.0f }, 12, 30, true);
			m_Up_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,-20.0f }, 12, 42, true);

			m_Down_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,CoverMove + 20.0f }, 30, 0, true);
			m_Down_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,-40.0f }, 12, 30, true);
			m_Down_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,20.0f }, 12, 42, true);
			m_Command_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,CoverMove + 20.0f }, 30, 0, true);
			m_Command_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,-40.0f }, 12, 30, true);
			m_Command_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,20.0f }, 12, 42, true);

		}
		else if (m_moveTimer == 60) {
			m_Under_SpriteRender->m_spriteSupporter.SpriteColor(Vector4::White, 12, 0);
			m_Win_SpriteRender->m_spriteSupporter.SpriteColor(Vector4::White, 12, 0);
			m_Lose_SpriteRender->m_spriteSupporter.SpriteColor(Vector4::White, 12, 0);
			m_Command_SpriteRender->m_spriteSupporter.SpriteColor(Vector4::White, 12, 0);
			m_Lose_SpriteRender->m_spriteSupporter.SpriteShake({20.0f,0.0f}, 24, 0);		//�V�F�C�N
			m_win_lose_MoveFlag = true;

			//BGM
			ssBGM = NewGO<prefab::CSoundSource>(0);;
			ssBGM->Init(L"Assets/sound/���U���g��.wav");
			ssBGM->SetVolume(0.2f);
			ssBGM->Play(true);
		}

		//SE
		if (m_moveTimer == 22) {
			prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
			ss->Init(L"Assets/sound/���U���gSE1.wav");
			ss->SetVolume(1.2f);
			ss->Play(false);
		}
		else if (m_moveTimer == 52)
		{
			prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
			ss->Init(L"Assets/sound/���U���gSE1.wav");
			ss->SetVolume(1.2f);
			ss->Play(false);
		}

		m_moveTimer++;
		if (m_moveTimer >= MoveLimit) {
			m_moveEndFlag = true;
		}
	}
	else if(m_moveEndFlag == true){
		//�ォ���{�^���������ƃ��g���C�ƁA�^�C�g�����؂�ւ���
		if (g_pad[0]->IsTrigger(enButtonUp) || g_pad[1]->IsTrigger(enButtonDown) || g_pad[0]->IsTrigger(enButtonDown) || g_pad[1]->IsTrigger(enButtonUp)) {
			if (m_RetryOn == true)
			{
				m_Command_SpriteRender->Init("Assets/Image/Result_Command_Title.dds", 400, 76);
				m_Command_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,5.0f }, 6, 0, true);
				m_Command_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,-5.0f }, 6, 6, true);
				m_RetryOn = false;

				//SE
				prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
				ss->Init(L"Assets/sound/�J�[�\���ړ���.wav");
				ss->SetVolume(1.0f);
				ss->Play(false);
			}
			else
			{
				m_Command_SpriteRender->Init("Assets/Image/Result_Command_Retry.dds", 400, 76);
				m_Command_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,5.0f }, 6, 0, true);
				m_Command_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,-5.0f }, 6, 6, true);
				m_RetryOn = true;

				//SE
				prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
				ss->Init(L"Assets/sound/�J�[�\���ړ���.wav");
				ss->SetVolume(1.0f);
				ss->Play(false);
			}
		}

		//�X�^�[�g��A�{�^���������ƃL�����I����ʂɑJ�ڂ���
		if (g_pad[0]->IsTrigger(enButtonA) || g_pad[1]->IsTrigger(enButtonA)) {

			if (m_RetryOn == true) {
				DeleteGO(ssBGM);
				//SE
				prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
				ss->Init(L"Assets/sound/���U���g��ʌ��艹.wav");
				ss->SetVolume(1.0f);
				ss->Play(false);

				//�g�����W�V����
				TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::NanameBox, 3, false);
				SampleScene* samplescene = NewGO<SampleScene>(0, "gamescene");
				DeleteGO(this);
			}
			if (m_RetryOn == false) {
				DeleteGO(ssBGM);
				//SE
				prefab::CSoundSource* ss = NewGO<prefab::CSoundSource>(0);;
				ss->Init(L"Assets/sound/���U���g��ʌ��艹.wav");
				ss->SetVolume(1.0f);
				ss->Play(false);

				//�g�����W�V����
				TransitionGenerator::GetInstance()->TransitionInit(TransitionGenerator::TransitionName::Circle, 15, false);
				TitleScene* titlescene = NewGO<TitleScene>(0, "titlescene");
				DeleteGO(this);
			}
		}
	}

	if (m_win_lose_MoveFlag == true) {

		if (m_Win_SpriteRender->m_spriteSupporter.GetSpriteMoveListLen() == 0) {
			m_Win_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,-10.0f }, 12, 0, true);
			m_Win_SpriteRender->m_spriteSupporter.SpriteScale({ 1.2f,0.8f,1.0f }, 12, 0);
			m_Win_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,60.0f }, 12, 12, true);
			m_Win_SpriteRender->m_spriteSupporter.SpriteScale({ 0.8f,1.2f,1.0f }, 12, 12);
			m_Win_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,-60.0f }, 6, 12, true);
			m_Win_SpriteRender->m_spriteSupporter.SpriteScale({ 1.5f,0.5f,1.0f }, 6, 12);
			m_Win_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,20.0f }, 12, 18, true);
			m_Win_SpriteRender->m_spriteSupporter.SpriteScale({ 0.6f,1.4f,1.0f }, 12, 18);
			m_Win_SpriteRender->m_spriteSupporter.SpriteMove({ m_WinPos.x,m_WinPos.y }, 6, 30);
			m_Win_SpriteRender->m_spriteSupporter.SpriteScale({ 1.2f,0.8f,1.0f }, 6, 30);
			m_Win_SpriteRender->m_spriteSupporter.SpriteMove({ 0.0f,0.0f }, 90, 30, true);
			m_Win_SpriteRender->m_spriteSupporter.SpriteScale({ 0.9f,1.1f,1.0f }, 6, 36);
			m_Win_SpriteRender->m_spriteSupporter.SpriteScale(1.0f, 6, 42);
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
