#pragma once
#include "Character_base.h"
class DropItem_base :public IGameObject

{
	
protected:
	public:	

	prefab::CSkinModelRender* m_skinModelRender = nullptr;//�L�����N�^�[�̃��f��
	MyCapsuleCollider m_collider;
	Vector3 m_position;
	Vector3 m_scale = { 1.0f,1.0f,1.0f};
	Quaternion m_qrot;
	float angle = 1.0f;//�A�C�e���̉�]�X�s�[�h
	
	int m_ExistenceMax;//�}�b�v�ɑ��݂��鐔�̏��
	int m_ItemCount=0;//�A�C�e���̐�

	bool m_Upmotion=true;
	void ItemMotion();//�A�C�e���̓���

	float m_startpositionY;
};

