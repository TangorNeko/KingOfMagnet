#pragma once

namespace prefab
{
	struct LevelObjectData
	{
		Vector3 position = Vector3::Zero;
		Quaternion rotation = Quaternion::Identity;
		Vector3 scale = Vector3::One;
		const char* name = nullptr;
	};
	class CMapChipRender : public IGameObject
	{
	public:
		CMapChipRender();
		~CMapChipRender();

		void Init();

		/*
		void QueryRenderObjDatas(std::function<void(const LevelObjectData& objData)> queryFunc)
		{
			queryFunc(m_renderObjectData);
		}
		*/

		void AddRenderObject(LevelObjectData objData)
		{
			m_renderObjectData = objData;
		}

	private:
		//�I�u�W�F�N�g�̃f�[�^�̉ϒ��z��
		LevelObjectData m_renderObjectData;// = nullptr;

		//�I�u�W�F�N�g�̃f�[�^�̌�����SkinModelRender
		prefab::CSkinModelRender* m_modelRender = nullptr;

		//�I�u�W�F�N�g�̃f�[�^�̌�����PhysicsStaticObject
		PhysicsStaticObject m_physicsStaticObject;

	};

}

