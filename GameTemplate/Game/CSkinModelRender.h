#pragma once

namespace prefab
{
	class CSkinModelRender :public IGameObject
	{
	public:
		enum EModel {
			eModel_View1,
			eModel_View2,
			eModel_Shadow,
			eModel_Num,
		};
		//�A�j���[�V�������x
		float m_animation_speed = 1.0;
	private:
		Skeleton m_skeleton; //�X�P���g��
		Model m_model[eModel_Num]; //���f��

		Vector3 m_position; //���W
		Quaternion m_qRot; //��]
		Vector3 m_scale; //�g�嗦

		AnimationClip* m_animationClips;
		int m_animationClipNum;
		Animation m_animation;

		bool m_isShadowCaster = false;

		/// <summary>
		/// ���f���̏����X�V����B
		/// </summary>
		void UpdateModel();
	public:
		CSkinModelRender() :m_position(Vector3::Zero), m_qRot(g_quatIdentity), m_scale(Vector3::One) {}

		void Render(RenderContext& rc,Camera* camera) override;

		/// <summary>
		/// ���f���̏������֐��B�A�j���[�V�������B
		/// </summary>
		/// <param name="modelPath">���f���t�@�C���̃p�X(.tkm)</param>
		/// <param name="skeletonPath">�X�P���g���t�@�C���̃p�X(.tks)</param>
		/// <param name="animationClips">�A�j���[�V�����N���b�v�̔z��</param>
		/// <param name="animationClipNum">�A�j���[�V�����N���b�v�̐�</param>
		void Init(const char* modelPath, const char* skeletonPath, AnimationClip* animClips, int animClipNum);

		/// <summary>
		/// ���f���̏������֐��B
		/// </summary>
		/// <param name="modelPath">���f���t�@�C���̃p�X(.tkm)</param>
		/// <param name="skeletonPath">�X�P���g���t�@�C���̃p�X(.tks)</param>
		void Init(const char* modelPath,const char* skeletonPath);

		/// <summary>
		/// ���f���̏������֐��B���f���p�X�����ŁB
		/// </summary>
		/// <param name="modelPath">���f���t�@�C���̃p�X(.tkm)</param>
		void Init(const char* modelPath);

		/// <summary>
		/// �A�j���[�V�����̍Đ�
		/// </summary>
		/// <param name="animationNo">�A�j���[�V�����ԍ�</param>
		/// <param name="interpolateTime">�A�j���[�V�����̕⊮����</param>
		void PlayAnimation(int animationNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animationNo, interpolateTime);
			m_animation.Progress(m_animation_speed /60.0f);
			UpdateModel();
		}

		/// <summary>
		/// �A�j���[�V�������Đ����Ă��邩?
		/// </summary>
		/// <param name="pos"></param>
		/// <returns>�Đ����Ă���:true �Đ����Ă��Ȃ�:false</returns>
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}

		/// <summary>
		/// ���f���̍��W��ݒ肷��B
		/// </summary>
		/// <param name="pos">���W(Vector3)</param>
		void SetPosition(Vector3 pos);

		/// <summary>
		/// ���f���̉�]��ݒ肷��B
		/// </summary>
		/// <param name="qRot">��](Quaternion)</param>
		void SetRotation(Quaternion qRot);

		/// <summary>
		/// ���f���̊g�嗦��ݒ肷��B
		/// </summary>
		/// <param name="scale">�g�嗦(Vector3)</param>
		void SetScale(Vector3 scale);

		/// <summary>
		/// ���f���̃��[���h�s��𒼐ڃZ�b�g����B
		/// </summary>
		/// <param name="world">���[���h�s��(Matrix)</param>
		void SetMatrix(Matrix world)
		{
			for (auto& model : m_model) {
				model.UpdateWorldMatrix(world);
				m_skeleton.Update(world);
			}
		}

		void SetShadowCasterFlag(bool isShadowCaster)
		{
			m_isShadowCaster = isShadowCaster;
		}

		/// <summary>
		/// �A�j���[�V�����̍Đ��t���O���擾
		/// </summary>
		/// <returns>�Đ����Ă���:true �Đ����Ă��Ȃ�:false</returns>
		bool GetAnimFlag()
		{ 
			return m_animation.IsPlaying(); 
		}

		/// <summary>
		/// ���f���Ɛ������������Ă���?
		/// </summary>
		/// <param name="start">�����̎n�_</param>
		/// <param name="end">�����̏I�_</param>
		/// <param name="crossPoint">�����̌����_(�߂�l�A�Q�Ǝ󂯎��)</param>
		/// <returns>�������Ă��邩(���Ă���:true ���Ă��Ȃ�:false)</returns>
		bool isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint);

		/// <summary>
		/// ���f���̍��W���擾
		/// </summary>
		/// <returns>���W(Vector3)</returns>
		Vector3 GetPosition()
		{
			return m_position;
		}

		/// <summary>
		/// ���f���̉�]���擾
		/// </summary>
		/// <returns>��]��(Quaternion)</returns>
		Quaternion GetRotation()
		{
			return m_qRot;
		}

		/// <summary>
		/// ���f���̊g�嗦���擾
		/// </summary>
		/// <returns>�g�嗦(Vector3)</returns>
		Vector3 GetScale()
		{
			return m_scale;
		}

		/// <summary>
		/// ���f�����擾
		/// </summary>
		/// <param name="model">�r���[�|�[�g�ԍ�(�J�����ԍ�)</param>
		/// <returns>�ԍ�����擾�������f���ւ̎Q��</returns>
		Model& GetModel(EModel model = eModel_View1)
		{
			return m_model[model];
		}

		/// <summary>
		/// �{�[���̖��O���炻�̃{�[���̃��[���h�s����擾����B
		/// </summary>
		/// <param name="boneName">�{�[���̖��O</param>
		/// <returns></returns>
		Matrix GetWorldMatrixFromBoneName(const wchar_t* boneName);

		//���O�Ƀ��f�������[�h���Ă����B
		void PreLoadModel(const char* tkmFilePath);
	};
}

