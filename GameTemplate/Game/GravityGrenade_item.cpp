#include "stdafx.h"
#include "GravityGrenade_item.h"
#include "Character_base.h"

GravityGrenade_item::~GravityGrenade_item()
{
	DeleteGO(m_skinModelRender);
}
bool GravityGrenade_item::Start()
{
	//モデルを作成
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	m_skinModelRender->Init("Assets/modelData/Gravity.tkm");
	//m_skinModelRender->SetScale({ 5.0f, 5.0f, 5.0f });
	m_collider.SetStartPoint(m_position);
	topPos = m_position;
	topPos.y += 50;
	m_collider.SetEndPoint(topPos);
	m_collider.SetRadius(15.0f);
	//m_skinModelRender->SetPosition(m_position);

	return true;
}
void GravityGrenade_item::Update()
{
	//各プレイヤーを検索
	QueryGOs<Character_base>("Player", [this](Character_base* player)->bool
		{
			//プレイヤーが近ければ

			if (player->m_collider.isHitCapsule(m_collider))
			{
				player->m_GravityGrenadeHave = true;

				DeleteGO(this);
			}
			return true;
		});
	ItemMotion();

}