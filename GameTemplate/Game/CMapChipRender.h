#pragma once

namespace prefab
{
	struct LevelObjectData;
	class CMapChipRender : public IGameObject
	{
	public:
		CMapChipRender();
		~CMapChipRender();

		void Init();

		void QueryRenderObjDatas(std::function<void(const LevelObjectData& objData)> queryFunc)
		{
			for (const auto& renderObjectData : m_renderObjectDatas)
			{
				queryFunc(*renderObjectData);
			}
		}

		void AddRenderObject(const LevelObjectData& objData)
		{
			m_renderObjectDatas.push_back(&objData);
		}

	private:
		//�I�u�W�F�N�g�̃f�[�^�̉ϒ��z��
		std::vector<const LevelObjectData*> m_renderObjectDatas;

		//�I�u�W�F�N�g�̃f�[�^�̌�����SkinModelRender
		std::vector<prefab::CSkinModelRender*> m_modelRenders;

		//�I�u�W�F�N�g�̃f�[�^�̌�����PhysicsStaticObject
		std::vector<PhysicsStaticObject*> m_physicsStaticObjects;

	};

}

