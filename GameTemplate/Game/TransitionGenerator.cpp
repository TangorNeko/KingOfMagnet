#include "stdafx.h"
#include "TransitionGenerator.h"

TransitionGenerator* TransitionGenerator::m_instance = nullptr;

TransitionGenerator::TransitionGenerator()
{
	if (m_instance != nullptr) {
		std::abort(); //���łɏo�Ă��邽�߃N���b�V��
	}
	//���̃C���X�^���X��B��̃C���X�^���X�Ƃ��ċL�^����
	m_instance = this;

	m_transitionSpriteRender = NewGO<prefab::CSpriteRender>(100);
	m_transitionSpriteRender->SetSpriteMode(prefab::CSpriteRender::SpriteMode::Transition);
	m_transitionSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_transitionSpriteRender->SetPosition(Vector3::Zero);
	m_transitionSpriteRender->Init(Transition_DataList[1].SpriteName, FRAME_BUFFER_W, FRAME_BUFFER_H);

}

TransitionGenerator::~TransitionGenerator()
{
	//�C���X�^���X���j�����ꂽ�̂ŁAnullptr����
	m_instance = nullptr;
}

void TransitionGenerator::Update() {

	if (m_rc == nullptr) {
		m_rc = GameObjectManager::GetInstance()->GetRenderContext();
	}
	if (m_rc == nullptr) {
		return; //�����_�����O�R���e�L�X�g��ݒ�ł��Ă��Ȃ��̂ł��̐�ւ͐i�܂Ȃ�
	}

	if (m_nowTransitionFlag == true) {
		if (m_borderLine < 0.0f) {
			m_nowTransitionFlag = false;
		}
		if (m_borderLine > MoveSpeed) {
			m_nowTransitionFlag = false;
		}

		if (m_transitionMode == false) {
			m_borderLine += (1.0f / m_transitionLimit)*MoveSpeed;
		}
		else {
			m_borderLine -= (1.0f / m_transitionLimit)*MoveSpeed;
		}
		m_transitionSpriteRender->SetTransitionBorder(m_borderLine);
	}

}

/// <summary>
/// �g�����W�V�����̊J�n
/// false�Ȃ�\���Atrue�Ȃ����
/// </summary>
/// <remarks>
/// 
/// ���T���v���R�[�h
/// 
/// //�΂߃{�b�N�X�A60�t���[�������ĕ\��
/// TransitionInit(TransitionName::NanameBox, 60, false);
/// 
/// </remarks>
void TransitionGenerator::TransitionInit(
	const TransitionName& name, 
	const int& MoveTime,
	const bool& mode
) {

	//�ݒ��ύX����
	m_transitionSpriteRender->Init(Transition_DataList[name].SpriteName, FRAME_BUFFER_W, FRAME_BUFFER_H);

	m_nowTransitionFlag = true;
	m_transitionLimit = MoveTime;
	m_transitionMode = mode;
	if (mode == false) {
		m_borderLine = 0.0f;
	}
	else {
		m_borderLine = MoveSpeed / 2.0f;
	}
}