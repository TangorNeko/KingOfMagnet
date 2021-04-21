#pragma once
class GameScene;

class Repulsion : public IGameObject
{
	~Repulsion();
	bool Start();
	void Update();
public:
	prefab::CSkinModelRender* m_skinModelRender = nullptr;

	Vector3 m_position;
	Vector3 m_scale;
	Vector3 diff0;
	Vector3 diff1;
	Quaternion m_rot;
	bool m_CharacterOn=false;//ƒLƒƒƒ‰‚ªã‚Éæ‚Á‚Ä‚¢‚é‚©
	
	int m_loop0=0;
	int m_jumploop0 = 0;
	
	int m_loop1 = 0;
	int m_jumploop1 = 0;

	int m_objNum;

	GameScene* m_gameScene = nullptr;
};

