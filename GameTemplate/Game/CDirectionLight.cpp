#include "stdafx.h"
#include "CDirectionLight.h"

namespace prefab
{
	bool CDirectionLight::Start()
	{
		/*
		//���C�g�̃f�[�^�̍\���̂̃T�C�Y���m�ۂ���
		cb.Init(sizeof(LigData),nullptr);
		//�萔�o�b�t�@��o�^
		//TODO:�������C�g�������Ă������ƃ��W�X�^b1�ōs����̂�?�v�ڍ�
		ds.RegistConstantBuffer(1, cb);
		//�o�^���m��
		ds.Commit();

		ligData.ligDir = { 1.0f,0.0f,0.0f };
		ligData.ligColor = { 1.0f,1.0f,1.0f };
		*/

		return true;
	}

	void CDirectionLight::Update()
	{
		/*
		//�萔�o�b�t�@���쐬
		ConstantBuffer cb;

		//���C�g�̃f�[�^�̍\���̂̃T�C�Y���m�ۂ���
		cb.Init(sizeof(LigData));

		//�f�B�X�N���v�^�q�[�v���쐬
		DescriptorHeap ds;
		//�萔�o�b�t�@��o�^
		//TODO:�������C�g�������Ă������ƃ��W�X�^b1�ōs����̂�?�v�ڍ�
		ds.RegistConstantBuffer(1, cb);
		//�o�^���m��
		ds.Commit();
		*/


		/*
		//���ۂ̃��C�g�̃f�[�^��]��
		cb.CopyToVRAM(ligData);

		auto& RenderContext = g_graphicsEngine->GetRenderContext();
		RenderContext.SetDescriptorHeap(ds);
		*/
		ligData[0].eyePos = g_camera3D->GetPosition();
		ligData[1].eyePos = g_camera3D->GetPosition();
	}
}