#include "stdafx.h"
#include "BackGround.h"
#include "Repulsion.h"
#include "DebrisBlock.h"

namespace
{
	const Vector3 SPOTLIGHT_NORTH_POSITION = { 0.0f,0.0f,270.0f };	//ステージ北側のスポットライトの座標
	const Vector3 SPOTLIGHT_SOUTH_POSITION = { 0.0f,0.0f,-270.0f };	//ステージ南側のスポットライトの座標
	const Vector3 SPOTLIGHT_DIRECTION = { 0.0f,-1.0f,0.0f };		//スポットライトの向き
	const Vector3 SPOTLIGHT_COLOR_WHITE = { 1.0f,1.0f,1.0f };		//スポットライトの色　白色
	const float SPOTLIGHT_RANGE = 1000.0f;							//スポットライトの影響範囲
	const float SPOTLIGHT_ANGLE = 90.0f;							//スポットライトの影響角度
	const float RESPAWNPOINT_INIT_DISTANCE = 0.0f;					//スポーン地点選定の初期距離						
}

BackGround::~BackGround()
{
	DeleteGO(m_skinModelRender);
	DeleteGO(m_northSpotLight);
	DeleteGO(m_southSpotLight);
}

bool BackGround::Start()
{
	//ステージを作成。
	m_skinModelRender = NewGO<prefab::CSkinModelRender>(0);
	//ステージは影を発生させる
	m_skinModelRender->SetShadowCasterFlag(true);
	m_skinModelRender->Init("Assets/modelData/stage00.tkm");

	//ステージのモデルの静的物理モデルを作成
	m_physicsStaticObject.CreateFromModel(m_skinModelRender->GetModel(), m_skinModelRender->GetModel().GetWorldMatrix());

	//ステージの北の穴を照らすスポットライト
	m_northSpotLight = NewGO<prefab::CSpotLight>(0);
	m_northSpotLight->SetPosition(SPOTLIGHT_NORTH_POSITION);
	m_northSpotLight->SetColor(SPOTLIGHT_COLOR_WHITE);
	m_northSpotLight->SetRange(SPOTLIGHT_RANGE);
	m_northSpotLight->SetDirection(SPOTLIGHT_DIRECTION);
	m_northSpotLight->SetAngleDeg(SPOTLIGHT_ANGLE);

	//ステージの南の穴を照らすスポットライト
	m_southSpotLight = NewGO<prefab::CSpotLight>(0);
	m_southSpotLight->SetPosition(SPOTLIGHT_SOUTH_POSITION);
	m_southSpotLight->SetColor(SPOTLIGHT_COLOR_WHITE);
	m_southSpotLight->SetRange(SPOTLIGHT_RANGE);
	m_southSpotLight->SetDirection(SPOTLIGHT_DIRECTION);
	m_southSpotLight->SetAngleDeg(SPOTLIGHT_ANGLE);
	
	//リスポーン地点の読み込み
	m_level.Init("Assets/modelData/Level_00.tkl", [&](prefab::LevelObjectData& objData) {
		if (strcmp(objData.name, "RespawnPoint") == 0) {//リスポーン地点

			//リスポーン地点の候補に追加していく。
			m_respawnPoints.push_back(objData.position);
			
			return true;
		}
		if (strcmp(objData.name, "DebrisBlock") == 0) {//ガレキブロック

			//ガレキブロックを作成
			DebrisBlock* debrisBlock = NewGO<DebrisBlock>(0, "debrisblock");
			debrisBlock->SetPosition(objData.position);

			return true;
		}
		if (strcmp(objData.name, "Repulsion") == 0) {//斥力床

			//斥力床を作成
			Repulsion* repulsion = NewGO<Repulsion>(0, "repulsion");
			repulsion->SetPosition(objData.position);
			repulsion->SetRotation(objData.rotation);

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

bool BackGround::isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint) const
{
	return m_skinModelRender->isLineHitModel(start, end, crossPoint);
}

Vector3 BackGround::CalcRespawnPoint(const Vector3& enemyPos) const
{
	//リスポーン地点
	Vector3 respawnpoint;

	//リスポーン地点から受け取った敵の座標への距離(0で初期化)
	float distance = RESPAWNPOINT_INIT_DISTANCE;

	//リスポーン地点の候補を走査
	for (auto ResPos : m_respawnPoints)
	{
		//リスポーン地点の候補から敵の座標へのベクトルを取得
		Vector3 diff = ResPos - enemyPos;

		//リスポーン地点の候補から敵の座標へのベクトルの長さが、現時点での長さより長いなら
		if (diff.Length() > distance)
		{
			//リスポーン地点を更新
			respawnpoint = ResPos;

			//リスポーン地点への距離を更新
			distance = diff.Length();
		}
	}

	return respawnpoint;
}