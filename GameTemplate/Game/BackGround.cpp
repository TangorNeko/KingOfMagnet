#include "stdafx.h"
#include "BackGround.h"
#include "PopRandItem.h"
#include "Repulsion.h"

BackGround::~BackGround()
{
	DeleteGO(m_skinModelRender);
	//DeleteGO(popranditem);
}

bool BackGround::Start()
{
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);

	m_skinModelRender->SetShadowCasterFlag(true);	m_sLigR = NewGO<prefab::CSpotLight>(0);
	m_sLigR->SetPosition({ 0.0f,-20.0f,270.0f });
	m_sLigR->SetColor({ 1.0f,1.0f,1.0f });
	m_sLigR->SetRange(1000);
	m_sLigR->SetDirection({ 0.0f,1.0f,0.0f });
	m_sLigR->SetAngleDeg(180.0f);
	m_sLigL = NewGO<prefab::CSpotLight>(0);
	m_sLigL->SetPosition({ 0.0f,-20.0f,-270.0f });
	m_sLigL->SetColor({ 1.0f,1.0f,1.0f });
	m_sLigL->SetRange(1000);
	m_sLigL->SetDirection({ 0.0f,1.0f,0.0f });
	m_sLigL->SetAngleDeg(180.0f);	//アイテムをランダムに出現させる
	//popranditem = NewGO<PopRandItem>(0, "popranditem");

	/*
	m_pLig = NewGO<prefab::CPointLight>(0);
	m_pLig->SetPosition({ 100.0f,0.0f,0.0f });
	m_pLig->SetColor({ 1.0f,1.0f,1.0f });
	m_pLig->SetRange(500);
	*/
	m_skinModelRender->Init("Assets/modelData/stage00.tkm");
	//m_skinModelRender->Init("Assets/modelData/ministage.tkm");

	m_physicsStaticObject.CreateFromModel(m_skinModelRender->GetModel(), m_skinModelRender->GetModel().GetWorldMatrix());

	
	m_level.Init("Assets/modelData/Level_00.tkl", [&](prefab::LevelObjectData& objData) {
		if (strcmp(objData.name, "ResPos") == 0) {//リスポーン地点
			
			char buff[256];
			sprintf(buff, "x = %f, y = %f, z = %f\n", objData.position.x, objData.position.y, objData.position.z);
			OutputDebugStringA(buff);

			//リスポーン地点の候補に追加していく。
			m_respawnPoints.push_back({ objData.position.x,0.0f,objData.position.z });
			
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

Vector3 BackGround::GetRespawnPoint(const Vector3& enemyPos)
{
	Vector3 respawnpoint;
	float distance = 0;
	for (auto ResPos : m_respawnPoints)
	{
		Vector3 diff = ResPos - enemyPos;

		if (diff.Length() > distance)
		{
			respawnpoint = ResPos;
			distance = diff.Length();
		}
	}

	char buff[256];
	sprintf(buff, "決定地点、x = %f, y = %f, z = %f\n", respawnpoint.x, respawnpoint.y, respawnpoint.z);
	OutputDebugStringA(buff);

	return respawnpoint;
}