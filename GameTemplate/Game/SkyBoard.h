#pragma once
class SkyBoard : public IGameObject
{
	bool Start()override;
	void Update()override;
	void Render(RenderContext& rc, Camera* camera)override;

private:
	//��̃f�[�^(�V�F�[�_�[�ɓn�����)
	struct SkyData
	{
		float textureSize;//�e�N�X�`���̃T�C�Y(�����`��z�肵�Ă���̂�float�B)
		float scrollingValue = 0;//�X�N���[����
	};

	Model m_skyModel[2];	//��̃��f���A2��ʗp��2�p�ӂ���B
	SkyData m_skyData;		//�V�F�[�_�[�ɓn������̃f�[�^
};

