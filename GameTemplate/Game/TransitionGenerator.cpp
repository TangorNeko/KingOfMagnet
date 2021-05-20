#include "stdafx.h"
#include "TransitionGenerator.h"

TransitionGenerator* TransitionGenerator::m_instance = nullptr;

TransitionGenerator::TransitionGenerator()
{
	if (m_instance != nullptr) {
		std::abort(); //すでに出ているためクラッシュ
	}
	//このインスタンスを唯一のインスタンスとして記録する
	m_instance = this;

	m_transitionSpriteRender = NewGO<prefab::CSpriteRender>(100);
	m_transitionSpriteRender->SetSpriteMode(prefab::CSpriteRender::SpriteMode::Transition);
	m_transitionSpriteRender->SetDrawScreen((prefab::CSpriteRender::DrawScreen)2);
	m_transitionSpriteRender->SetPosition(Vector3::Zero);
	m_transitionSpriteRender->Init(Transition_DataList[1].SpriteName, FRAME_BUFFER_W, FRAME_BUFFER_H);

}

TransitionGenerator::~TransitionGenerator()
{
	//インスタンスが破棄されたので、nullptrを代入
	m_instance = nullptr;
}

void TransitionGenerator::Update() {

	if (m_rc == nullptr) {
		m_rc = GameObjectManager::GetInstance()->GetRenderContext();
	}
	if (m_rc == nullptr) {
		return; //レンダリングコンテキストを設定できていないのでこの先へは進まない
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
/// トランジションの開始
/// falseなら表示、trueなら消去
/// </summary>
/// <remarks>
/// 
/// ※サンプルコード
/// 
/// //斜めボックス、60フレームかけて表示
/// TransitionInit(TransitionName::NanameBox, 60, false);
/// 
/// </remarks>
void TransitionGenerator::TransitionInit(
	const TransitionName& name, 
	const int& MoveTime,
	const bool& mode
) {

	//設定を変更する
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