#pragma once

namespace prefab
{
	class CSkinModelRender :public IGameObject
	{
	public:
		//���f���̏��
		enum EModel {
			eModel_View1,	//���1�ɕ`�悳��郂�f��
			eModel_View2,	//���2�ɕ`�悳��郂�f��
			eModel_Num,		//���f���̏�Ԃ̐�
		};
	private:

		/**
		 * @brief ���f���̏����X�V����
		*/
		void UpdateModel();
	public:
		/**
		 * @brief ���f���̕`��
		 * @param rc �����_�[�R���e�L�X�g
		 * @param camera �`�悷��J����
		*/
		void Render(RenderContext& rc,Camera* camera) override;
		
		/**
		 * @brief �e���f���̕`��
		 * @param rc �����_�[�R���e�L�X�g
		 * @param viewMatrix �r���[�s��
		 * @param projMatrix �v���W�F�N�V�����s��
		 * @param screenNo ��ʔԍ�
		 * @param areaNo �e�̃G���A�ԍ�
		*/
		void ShadowRender(RenderContext& rc, const Matrix& viewMatrix, const Matrix& projMatrix, int screenNo, int areaNo) override;

		/**
		 * @brief ���f���̏������֐��@�A�j���[�V������
		 * @param modelPath ���f���t�@�C���̃p�X(.tkm)
		 * @param skeletonPath �X�P���g���t�@�C���̃p�X(.tks)
		 * @param animClips �A�j���[�V�����N���b�v�̔z��
		 * @param animClipNum �A�j���[�V�����N���b�v�̐�
		*/
		void Init(const char* modelPath, const char* skeletonPath, AnimationClip* animClips, int animClipNum);
		
		/**
		 * @brief ���f���̏������֐�
		 * @param modelPath ���f���t�@�C���̃p�X(.tkm)
		 * @param skeletonPath �X�P���g���t�@�C���̃p�X(.tks)
		*/
		void Init(const char* modelPath,const char* skeletonPath);
		
		/**
		 * @brief ���f���̏������֐��@���f���p�X������
		 * @param modelPath ���f���t�@�C���̃p�X(.tkm)
		*/
		void Init(const char* modelPath);
		
		/**
		 * @brief �A�j���[�V�����̍Đ�
		 * @param animationNo �A�j���[�V�����ԍ�
		 * @param interpolateTime �A�j���[�V�����̕⊮����
		*/
		void PlayAnimation(int animationNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animationNo, interpolateTime);
			m_animation.Progress(m_animationSpeed /60.0f);
			UpdateModel();
		}

		/**
		 * @brief �A�j���[�V�����̍Đ����x��ݒ肷��
		 * @param animationSpeed �Đ����x
		*/
		void SetAnimationSpeed(float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}
		
		/**
		 * @brief �A�j���[�V�������Đ����Ă��邩?
		 * @return �Đ����Ă���:true �Đ����Ă��Ȃ�:false
		*/
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}
		
		/**
		 * @brief ���f���̍��W��ݒ肷��
		 * @param pos ���W
		*/
		void SetPosition(Vector3 pos);
		
		/**
		 * @brief ���f���̉�]��ݒ肷��
		 * @param qRot ��]
		*/
		void SetRotation(Quaternion qRot);
		
		/**
		 * @brief ���f���̊g�嗦��ݒ肷��
		 * @param scale �g�嗦
		*/
		void SetScale(Vector3 scale);
		
		/**
		 * @brief ���f���̃��[���h�s��𒼐ڃZ�b�g����
		 * @param world ���[���h�s��
		*/
		void SetMatrix(Matrix world)
		{
			for (auto& model : m_model) {
				model.UpdateWorldMatrix(world);
				m_skeleton.Update(world);
			}
		}

		/**
		 * @brief ���f�����e����邩�ǂ�����ݒ肷��
		 * @param isShadowCaster ���:true ���Ȃ�:false
		*/
		void SetShadowCasterFlag(bool isShadowCaster)
		{
			m_isShadowCaster = isShadowCaster;
		}
		
		/**
		 * @brief ���f���Ɛ������������Ă���?
		 * @param start �����̎n�_
		 * @param end �����̏I�_
		 * @param crossPoint �����̌����_(�߂�l�A�Q�Ǝ󂯎��)
		 * @return �������Ă���:true �������Ă��Ȃ�:false
		*/
		bool isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint);
		
		/**
		 * @brief ���f���̍��W���擾
		 * @return ���W
		*/
		const Vector3& GetPosition() const
		{
			return m_position;
		}
		
		/**
		 * @brief ���f���̉�]���擾
		 * @return ��]
		*/
		const Quaternion& GetRotation() const
		{
			return m_qRot;
		}
		
		/**
		 * @brief ���f���̊g�嗦���擾
		 * @return �g�嗦
		*/
		const Vector3& GetScale() const
		{
			return m_scale;
		}
		
		/**
		 * @brief ���f�����擾
		 * @param model �r���[�|�[�g�ԍ�(�J�����ԍ�)
		 * @return �ԍ�����擾�������f���ւ̎Q��
		*/
		Model& GetModel(EModel model = eModel_View1)
		{
			return m_model[model];
		}
		
		/**
		 * @brief �{�[���̖��O���炻�̃{�[���̃��[���h�s����擾����
		 * @param boneName �{�[���̖��O
		 * @return �{�[���̃��[���h�s��
		*/
		Matrix GetWorldMatrixFromBoneName(const wchar_t* boneName);

		/**
		 * @brief ���O�Ƀ��f�������[�h���Ă���
		 * @param tkmFilePath ���[�h���������f���̃t�@�C���p�X
		*/
		static void PreLoadModel(const char* tkmFilePath);

	private:
		enum shadowMapArea
		{
			enShort = 0,					//�ߋ���
			enMedium = 1,					//������
			enLong = 2,						//������
			enShadowMapAreaNum = 3			//�V���h�E�}�b�v�̃G���A�̐�
		};
		Skeleton m_skeleton;																	//�X�P���g��
		Model m_model[eModel_Num];																//���f��
		Model m_shadowModel[eModel_Num][enShadowMapAreaNum];		//�e�p���f��
		Vector3 m_position = Vector3::Zero;														//���W
		Quaternion m_qRot = Quaternion::Identity;												//��]
		Vector3 m_scale = Vector3::One;															//�g�嗦
		AnimationClip* m_animationClips = nullptr;												//�A�j���[�V�����N���b�v
		int m_animationClipNum = 0;																//�A�j���[�V�����N���b�v�̐�
		Animation m_animation;																	//�A�j���[�V�����B
		bool m_isShadowCaster = false;															//���̃��f���͉e����邩?
		float m_animation_speed = 1.0;															//�A�j���[�V�������x
		float m_animationSpeed = 1.0f;															//�A�j���[�V�������x
	};
}

