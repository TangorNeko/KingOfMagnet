#include "stdafx.h"
#include "BackGround.h"
#include "PopRandItem.h"
#include "Repulsion.h"

BackGround::~BackGround()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(popranditem);
}

bool BackGround::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	//アイテムをランダムに出現させる
	popranditem = NewGO<PopRandItem>(0, "popranditem");

	/*
	m_pLig = NewGO<prefab::CPointLight>(0);
	m_pLig->SetPosition({ 100.0f,0.0f,0.0f });
	m_pLig->SetColor({ 1.0f,1.0f,1.0f });
	m_pLig->SetRange(500);
	*/
	m_skinModelRender->Init("Assets/modelData/protostage.tkm");

	m_physicsStaticObject.CreateFromModel(m_skinModelRender->GetModel(), m_skinModelRender->GetModel().GetWorldMatrix());

	m_level.Init("Assets/modelData/level_00.tkl", [&](prefab::LevelObjectData& objData) {
		if (strcmp(objData.name,"repulsion") == 0) {//斥力床
			Repulsion* gimmick = NewGO<Repulsion>(0, "repulsion");
			gimmick->m_position = objData.position;//
			gimmick->m_rot = objData.rotation;
			gimmick->m_scale = objData.scale;
			m_repulsion.push_back(gimmick);
			return true;
		}
		return false;
	});
	return true;
}
void BackGround::Update()
{
	m_skinModelRender->SetPosition(m_position);
}
bool BackGround::isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint)
{
	return m_skinModelRender->isLineHitModel(start, end, crossPoint);
}