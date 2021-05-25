#include "stdafx.h"
#include "TitleScene.h"
#include "CharacterSelect.h"
#include "SampleScene.h"
TitleScene::~TitleScene() 
{
	DeleteGO(m_BG_ModelRender);
	DeleteGO(m_BGLight);
	DeleteGO(m_Monitor_SpriteRender);
	DeleteGO(m_MonitorLine_SpriteRender);
	DeleteGO(m_Rogo_SpriteRender);
	DeleteGO(m_Option_SpriteRender);
	DeleteGO(m_Start_SpriteRender);
	DeleteGO(m_Exit_SpriteRender);
	DeleteGO(m_Arrow_SpriteRender);
}
bool TitleScene::Start()
{	
	//�w�i�̃��f��
	m_BG_ModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_BG_ModelRender->Init("Assets/modelData/TitleCylinder.tkm");
	m_BG_ModelRender->SetPosition({ 60.0f,-300.0f,-75.0f });
	m_BG_ModelRender->SetScale({ 1.2f,1.2f ,1.2f });

	//�w�i�̃��f�����Ƃ炷�|�C���g���C�g
	m_BGLight = NewGO<prefab::CPointLight>(0);
	m_BGLight->SetRange(1500.0f);
	m_BGLight->SetPosition(Vector3::Zero);
	m_BGLight->SetColor(Vector3::One);

	//���j�^�[
	m_Monitor_SpriteRender =NewGO<prefab::CSpriteRender>(0);
	m_Monitor_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Monitor_SpriteRender->SetPosition({ -265,-20,0 });
	m_Monitor_SpriteRender->Init("Assets/Image/Title_Monitor.dds", 752, 620);

	//���j�^�[���C��
	m_MonitorLine_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_MonitorLine_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_MonitorLine_SpriteRender->SetPosition({ -290,-21,0 });
	m_MonitorLine_SpriteRender->Init("Assets/Image/Title_MonitorLine.dds", 704, 560);

	//�^�C�g�����S
	m_Rogo_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Rogo_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Rogo_SpriteRender->SetPosition({ -320,10,0 });
	m_Rogo_SpriteRender->Init("Assets/Image/Zikai_Rogo.dds", 660, 495);

	//�I�v�V����
	m_Option_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Option_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Option_SpriteRender->SetPosition({ m_topPos });
	m_Option_SpriteRender->Init("Assets/Image/Title_Option.dds", 500, 188);
	m_Option_SpriteRender->SetMulColor({ m_semitrans });
	m_Option_SpriteRender->SetScale({ m_smallScale });
	//�n�߂�
	m_Start_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Start_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Start_SpriteRender->SetPosition({ m_centerPos });
	m_Start_SpriteRender->Init("Assets/Image/Title_Start.dds", 500, 188);
	
	//��߂�
	m_Exit_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Exit_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Exit_SpriteRender->SetPosition({ m_bottomPos });
	m_Exit_SpriteRender->Init("Assets/Image/Title_Exit.dds", 500, 188);
	m_Exit_SpriteRender->SetMulColor({ m_semitrans });
	m_Exit_SpriteRender->SetScale({ m_smallScale });

	//���
	m_Arrow_SpriteRender = NewGO<prefab::CSpriteRender>(0);
	m_Arrow_SpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_Arrow_SpriteRender->SetPosition({ 600,0,0 });
	m_Arrow_SpriteRender->Init("Assets/Image/Title_Arrow.dds", 48, 88);
	return true;
}
void TitleScene::Update()
{
	//������]
	Quaternion rot = m_BG_ModelRender->GetRotation();
	Vector3 AxisY = { 0.0f,  1.0f,  0.0f };
	rot.Apply(AxisY);
	rot.SetRotationDeg(Vector3::AxisY, -0.2f);
	m_BG_ModelRender->SetRotation(m_BG_ModelRender->GetRotation() * rot);

	if (m_selectFlag == false) {

		//����
		if ((g_pad[0]->IsTrigger(enButtonA) || g_pad[1]->IsTrigger(enButtonA)) && m_selectMoveFlag == false) {
			m_commandTimer = 0;
			m_selectFlag = true;
		}

		//�R�}���h�ړ� ��
		if ((g_pad[0]->IsTrigger(enButtonUp)) && m_selectMoveFlag == false) {
			m_selectMoveFlag = true;
			m_comandUpDown = false;
			m_commandTimer = 0;
		}
		//�R�}���h�ړ� ��
		if ((g_pad[0]->IsTrigger(enButtonDown)) && m_selectMoveFlag == false) {
			m_selectMoveFlag = true;
			m_comandUpDown = true;
			m_commandTimer = 0;
		}

		if (m_selectMoveFlag == true) {
			//�R�}���h���o
			CommandMove();
		}

	}
	else {
		//���肵���̂ŃR�}���h���o
		CommandSelectMove();
	}
}

void TitleScene::CommandMove() {

	if (m_commandTimer == 0) {
		if (m_comandUpDown == false) {
			//��
			switch (m_titleCommand)
			{
			case TitleScene::TC_Start:
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x,m_topPos.y }, 24, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 24, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 24, 0);
				//
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({50.0f,-50.0f }, 12, 0,true);
				m_Option_SpriteRender->m_spriteSupporter.SpriteScale(0.5f, 12, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteColor({1.0f,1.0f,1.0f,0.0f}, 12, 0);
				//
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ m_centerPos.x,m_centerPos.y }, 24, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteScale(1.0f, 24, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteColor(Vector4::White, 24, 0);
				break;
			case TitleScene::TC_Option:
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x,m_topPos.y }, 24, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 24, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 24, 0);
				//
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ 50.0f,-50.0f }, 12, 0, true);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteScale(0.5f, 12, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteColor({ 1.0f,1.0f,1.0f,0.0f }, 12, 0);
				//
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ m_centerPos.x,m_centerPos.y }, 24, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteScale(1.0f, 24, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteColor(Vector4::White, 24, 0);
				break;
			case TitleScene::TC_Exit:
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x,m_topPos.y }, 24, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 24, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 24, 0);
				//
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ 50.0f,-50.0f }, 12, 0, true);
				m_Start_SpriteRender->m_spriteSupporter.SpriteScale(0.5f, 12, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteColor({ 1.0f,1.0f,1.0f,0.0f }, 12, 0);
				//
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ m_centerPos.x,m_centerPos.y }, 24, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteScale(1.0f, 24, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteColor(Vector4::White, 24, 0);
				break;
			}
		}
		else {
			//��
			switch (m_titleCommand)
			{
			case TitleScene::TC_Start:
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x,m_bottomPos.y }, 24, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 24, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 24, 0);
				//
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ 50.0f,50.0f }, 12, 0, true);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteScale(0.5f, 12, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteColor({ 1.0f,1.0f,1.0f,0.0f }, 12, 0);
				//
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ m_centerPos.x,m_centerPos.y }, 24, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteScale(1.0f, 24, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteColor(Vector4::White, 24, 0);
				break;
			case TitleScene::TC_Option:
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x,m_bottomPos.y }, 24, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 24, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 24, 0);
				//
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ 50.0f,50.0f }, 12, 0, true);
				m_Start_SpriteRender->m_spriteSupporter.SpriteScale(0.5f, 12, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteColor({ 1.0f,1.0f,1.0f,0.0f }, 12, 0);
				//
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ m_centerPos.x,m_centerPos.y }, 24, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteScale(1.0f, 24, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteColor(Vector4::White, 24, 0);
				break;
			case TitleScene::TC_Exit:
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x,m_bottomPos.y }, 24, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 24, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 24, 0);
				//
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ 50.0f,50.0f }, 12, 0, true);
				m_Option_SpriteRender->m_spriteSupporter.SpriteScale(0.5f, 12, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteColor({ 1.0f,1.0f,1.0f,0.0f }, 12, 0);
				//
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ m_centerPos.x,m_centerPos.y }, 24, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteScale(1.0f, 24, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteColor(Vector4::White, 24, 0);
				break;
			}
		}
	}

	if (m_commandTimer == 12) {
		//��
		if (m_comandUpDown == false) {
			switch (m_titleCommand)
			{
			case TitleScene::TC_Start:
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x + 50.0f ,m_bottomPos.y + 50.0f }, 1, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x,m_bottomPos.y }, 12, 1);
				m_Option_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 12, 1);
				m_Option_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 12, 1);
				m_titleCommand = TC_Exit;
				break;
			case TitleScene::TC_Option:
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x + 50.0f ,m_bottomPos.y + 50.0f }, 1, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x,m_bottomPos.y }, 12, 1);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 12, 1);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 12, 1);
				m_titleCommand = TC_Start;
				break;
			case TitleScene::TC_Exit:
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x + 50.0f ,m_bottomPos.y + 50.0f }, 1, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ m_bottomPos.x,m_bottomPos.y }, 12, 1);
				m_Start_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 12, 1);
				m_Start_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 12, 1);
				m_titleCommand = TC_Option;
				break;
			}
		}
		else {
			//��
			switch (m_titleCommand)
			{
			case TitleScene::TC_Start:
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x + 50.0f ,m_topPos.y - 50.0f }, 1, 0);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x,m_topPos.y }, 12, 1);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 12, 1);
				m_Exit_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 12, 1);
				m_titleCommand = TC_Option;
				break;
			case TitleScene::TC_Option:
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x + 50.0f ,m_topPos.y - 50.0f }, 1, 0);
				m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x,m_topPos.y }, 12, 1);
				m_Start_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 12, 1);
				m_Start_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 12, 1);
				m_titleCommand = TC_Exit;
				break;
			case TitleScene::TC_Exit:
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x + 50.0f ,m_topPos.y - 50.0f }, 1, 0);
				m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ m_topPos.x,m_topPos.y }, 12, 1);
				m_Option_SpriteRender->m_spriteSupporter.SpriteScale(m_smallScale, 12, 1);
				m_Option_SpriteRender->m_spriteSupporter.SpriteColor(m_semitrans, 12, 1);
				m_titleCommand = TC_Start;
				break;
			}
		}
	}

	m_commandTimer++;

	if (m_commandTimer >= 30) {
		//�R�}���h���o�I��
		m_selectMoveFlag = false;
	}
}

void TitleScene::CommandSelectMove() {

	if (m_commandTimer == 0) {
		switch (m_titleCommand)
		{
		case TitleScene::TC_Start:
			//�J�n
			m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ 50.0f,0.0f }, 12, 0, true);
			m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ -125.0f,0.0f }, 12, 6, true);
			m_Start_SpriteRender->m_spriteSupporter.SpriteMove({ 20.0f,0.0f }, 6, 18, true);
			break;
		case TitleScene::TC_Option:
			//�ݒ�
			m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ 50.0f,0.0f }, 12, 0, true);
			m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ -125.0f,0.0f }, 12, 6, true);
			m_Option_SpriteRender->m_spriteSupporter.SpriteMove({ 20.0f,0.0f }, 6, 18, true);
			break;
		case TitleScene::TC_Exit:
			//�I��
			m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ 50.0f,0.0f }, 12, 0, true);
			m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ -125.0f,0.0f }, 12, 6, true);
			m_Exit_SpriteRender->m_spriteSupporter.SpriteMove({ 20.0f,0.0f }, 6, 18, true);
			break;
		}

	}

	m_commandTimer++;

	if (m_commandTimer == 60) {
		switch (m_titleCommand)
		{
		case TitleScene::TC_Start:
			//�J�n
			NewGO<SampleScene>(0, "gamescene");
			DeleteGO(this);
			break;
		case TitleScene::TC_Option:
			//�ݒ�
			DeleteGO(this);
			break;
		case TitleScene::TC_Exit:
			//�I��
			DeleteGO(this);
			break;
		}
	}

}