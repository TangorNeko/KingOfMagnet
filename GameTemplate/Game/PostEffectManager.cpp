#include "stdafx.h"
#include "PostEffectManager.h"
//#include "GaussianBlur.h"

PostEffectManager* PostEffectManager::m_instance = nullptr;

void PostEffectManager::Init(bool bloomMode,bool shadowMode)
{
	m_bloomMode = bloomMode;
	m_shadowMode = shadowMode;

	if (m_bloomMode == true)
	{
		//�ŏI�I�ɉ�ʂɏo�͂���p�̃����_�[�^�[�Q�b�g
		m_mainRenderTarget.Create(
			1280,
			720,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		//�P�x�𒊏o�������̂��i�[����郌���_�[�^�[�Q�b�g
		m_luminnceRenderTarget.Create(
			1280,
			720,
			1,
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			DXGI_FORMAT_D32_FLOAT
		);

		//�s�N�Z���V�F�[�_�[PSSamplingLuminance�ŁA���C�������_�[�^�[�Q�b�g����P�x�𒊏o����p�̃X�v���C�g�B
		SpriteInitData luminanceSpriteInitData;
		luminanceSpriteInitData.m_fxFilePath = "Assets/shader/postEffect.fx";
		luminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		luminanceSpriteInitData.m_psEntryPoinFunc = "PSSamplingLuminance";
		luminanceSpriteInitData.m_width = 1280;
		luminanceSpriteInitData.m_height = 720;
		luminanceSpriteInitData.m_textures[0] =	&m_mainRenderTarget.GetRenderTargetTexture();

		m_luminanceSprite.Init(luminanceSpriteInitData);
		
		//�P�x���o�����X�v���C�g�����ƂɃu���[������������悤�ɃZ�b�g�B
		m_gaussianBlur.Init(&m_luminnceRenderTarget.GetRenderTargetTexture());

		//�P�x���o���ꂽ���̂Ƀu���[���������X�v���C�g�B
		SpriteInitData bokeLuminanceSpriteInitData;
		bokeLuminanceSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		bokeLuminanceSpriteInitData.m_vsEntryPointFunc = "VSMain";
		bokeLuminanceSpriteInitData.m_width = 1280;
		bokeLuminanceSpriteInitData.m_height = 720;
		bokeLuminanceSpriteInitData.m_alphaBlendMode = AlphaBlendMode_Add;
		bokeLuminanceSpriteInitData.m_textures[0] = &m_gaussianBlur.GetBokeTexture();

		m_bokeLuminanceSprite.Init(bokeLuminanceSpriteInitData);

		//�ŏI�I��mainRenderTarget�ɂ͌��̉�ʂɃu���[�������������̂��o�͂����̂ŁA�������ʂɏo�͂���p�̃X�v���C�g�B
		SpriteInitData copyToBufferSpriteInitData;
		copyToBufferSpriteInitData.m_textures[0] = &m_mainRenderTarget.GetRenderTargetTexture();
		copyToBufferSpriteInitData.m_width = 1280;
		copyToBufferSpriteInitData.m_height = 720;
		copyToBufferSpriteInitData.m_fxFilePath = "Assets/shader/sprite.fx";
		m_copyToFrameBufferSprite.Init(copyToBufferSpriteInitData);
	}

	if (m_shadowMode)
	{
		float clearColor[4] = { 1.0f,1.0f,1.0f,1.0f };
		m_shadowMap.Create(
			1024,
			1024,
			1,
			1,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_FORMAT_D32_FLOAT,
			clearColor
		);

		testLightCamera.SetPosition({ 0.0f,1000.0f,0.0f });
		testLightCamera.SetTarget(0.0f, 0.0f, 0.0f);
		testLightCamera.SetUp({ 1.0f,0.0f,0.0f });
		testLightCamera.SetViewAngle(Math::DegToRad(20.0f));
		testLightCamera.Update();
	}
}

void PostEffectManager::ShadowRender(RenderContext& rc)
{
	rc.WaitUntilToPossibleSetRenderTarget(m_shadowMap);
	rc.SetRenderTargetAndViewport(m_shadowMap);
	rc.ClearRenderTargetView(m_shadowMap.GetRTVCpuDescriptorHandle(), m_shadowMap.GetRTVClearColor());
	rc.ClearDepthStencilView(m_shadowMap.GetDSVCpuDescriptorHandle(), m_shadowMap.GetDSVClearValue());
}

void PostEffectManager::EndShadowRender(RenderContext& rc)
{
	rc.WaitUntilFinishDrawingToRenderTarget(m_shadowMap);
}

void PostEffectManager::BeforeRender(RenderContext& rc)
{
	if (m_bloomMode == true)
	{
		//���C�������_�[�^�[�Q�b�g��`���ɃZ�b�g����B
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		rc.ClearRenderTargetView(m_mainRenderTarget.GetRTVCpuDescriptorHandle(), m_mainRenderTarget.GetRTVClearColor());
		rc.ClearDepthStencilView(m_mainRenderTarget.GetDSVCpuDescriptorHandle(), m_mainRenderTarget.GetDSVClearValue());
		//���̌�A�e���f���̃h���[�R�[�����Ă΂��(�͂�)�B
	}
}

void PostEffectManager::AfterRender(RenderContext& rc)
{
	if (m_bloomMode == true)
	{
		//���C�������_�[�^�[�Q�b�g���`����I����܂ő҂B
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		//�P�x���o�p�̃����_�[�^�[�Q�b�g�ɕύX����B
		rc.WaitUntilToPossibleSetRenderTarget(m_luminnceRenderTarget);
		rc.SetRenderTargetAndViewport(m_luminnceRenderTarget);
		rc.ClearRenderTargetView(m_luminnceRenderTarget.GetRTVCpuDescriptorHandle(), m_luminnceRenderTarget.GetRTVClearColor());
		//�P�x���o���s��
		m_luminanceSprite.Draw(rc);
		//�P�x���o�p�̃����_�[�^�[�Q�b�g���`����I����܂ő҂B
		rc.WaitUntilFinishDrawingToRenderTarget(m_luminnceRenderTarget);

		//�K�E�V�A���u���[�����s����
		m_gaussianBlur.ExecuteOnGPU(rc, 20);

		//���C�������_�[�^�[�Q�b�g���g����悤�ɂȂ�܂ő҂�
		rc.WaitUntilToPossibleSetRenderTarget(m_mainRenderTarget);
		rc.SetRenderTargetAndViewport(m_mainRenderTarget);
		//�P�x���o�������̂��{�J�����������C�������_�[�^�[�Q�b�g�ɉ��Z����
		//���̎��_�Ń��C�������_�[�^�[�Q�b�g�ɂ͌��̉摜�ƃu���[�̉摜�����킳�������̂ɂȂ�B
		m_bokeLuminanceSprite.Draw(rc);
		//���C�������_�[�^�[�Q�b�g���`����I����܂ő҂B
		rc.WaitUntilFinishDrawingToRenderTarget(m_mainRenderTarget);

		//�`�����t���[���o�b�t�@�ɂ���B
		rc.SetRenderTarget(g_graphicsEngine->GetCurrentFrameBuffuerRTV(), g_graphicsEngine->GetCurrentFrameBuffuerDSV());

		//���C�������_�[�^�[�Q�b�g�ɕ`����Ă����ŏI�C���[�W����ʏ�ɕ`���B
		m_copyToFrameBufferSprite.Draw(rc);
	}
}