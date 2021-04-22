#pragma once

class GameScene;

class PopRandItem:public IGameObject
{
	~PopRandItem();
	bool Start();
	void Update();
	//�A�C�e�����o��Ԋu
	int m_PopInterval=200;
	//�A�C�e�����o��m���A�番��
	int m_MagInversion_probability = 250;
	int m_MagAcceleration_probability = 250;
	int m_Grenade_probability = 250;
	int m_GravityGrenade = 100;
	int m_Gun_probability = 250;
public:
	GameScene* m_gameScene = nullptr;	
	int m_Loop = 0;


	enum EnItem {
		MagInversion,		//���͔��]
		MagAcceleration,	//���͑J�ڂ̉���
		Gun,				//�}�V���K��
		ExplosionGrenade,	//���j�O���l�[�h
		FlashGrenade,		//�M���O���l�[�h
		GravityGrenade,		//���͒e
		MoveActStockRecover,	//���[�u�A�N�V�����̃X�g�b�N����
		ItemNum
	};
	EnItem enitem = Gun;
	
	int m_ItemCount = ItemNum;//�A�C�e����
	int m_gatya=0;
	int m_positionX = 0;
	int m_positionZ = 0;
	
	Vector3 ItemSetPosition();
	void ItemSelect();
	void ItemPop();
};

