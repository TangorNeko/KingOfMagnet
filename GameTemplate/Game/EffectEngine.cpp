#include "stdafx.h"

#include "EffectEngine.h"

EffectEngine* EffectEngine::m_instance = nullptr;	//�B��̃C���X�^���X�B

EffectEngine::EffectEngine()
{
	MY_ASSERT(
		m_instance == nullptr,
		"EffectEngine�̃C���X�^���X�𕡐���邱�Ƃ͂ł��܂���B"
	);
	auto format = DXGI_FORMAT_R8G8B8A8_UNORM;
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	auto commandQueue = g_graphicsEngine->GetCommandQueue();
	// �����_���[���쐬�B
	m_renderer[0] = ::EffekseerRendererDX12::Create(
		d3dDevice,
		commandQueue,
		3,
		&format,
		1,
		DXGI_FORMAT_D32_FLOAT,
		false,
		8000
	);

	m_renderer[1] = ::EffekseerRendererDX12::Create(
		d3dDevice,
		commandQueue,
		3,
		&format,
		1,
		DXGI_FORMAT_D32_FLOAT,
		false,
		8000
	);
	//�������v�[���̍쐬�B
	m_memoryPool[0] = EffekseerRenderer::CreateSingleFrameMemoryPool(m_renderer[0]->GetGraphicsDevice());

	m_memoryPool[1] = EffekseerRenderer::CreateSingleFrameMemoryPool(m_renderer[1]->GetGraphicsDevice());
	// �R�}���h���X�g�̍쐬
	m_commandList[0] = EffekseerRenderer::CreateCommandList(m_renderer[0]->GetGraphicsDevice(), m_memoryPool[0]);

	m_commandList[1] = EffekseerRenderer::CreateCommandList(m_renderer[1]->GetGraphicsDevice(), m_memoryPool[1]);
	// �G�t�F�N�g�}�l�[�W���[�̍쐬�B
	m_manager[0] = ::Effekseer::Manager::Create(8000);

	m_manager[1] = ::Effekseer::Manager::Create(8000);

	// �`�惂�W���[���̐ݒ�B
	m_manager[0]->SetSpriteRenderer(m_renderer[0]->CreateSpriteRenderer());
	m_manager[0]->SetRibbonRenderer(m_renderer[0]->CreateRibbonRenderer());
	m_manager[0]->SetRingRenderer(m_renderer[0]->CreateRingRenderer());
	m_manager[0]->SetTrackRenderer(m_renderer[0]->CreateTrackRenderer());
	m_manager[0]->SetModelRenderer(m_renderer[0]->CreateModelRenderer());

	m_manager[1]->SetSpriteRenderer(m_renderer[1]->CreateSpriteRenderer());
	m_manager[1]->SetRibbonRenderer(m_renderer[1]->CreateRibbonRenderer());
	m_manager[1]->SetRingRenderer(m_renderer[1]->CreateRingRenderer());
	m_manager[1]->SetTrackRenderer(m_renderer[1]->CreateTrackRenderer());
	m_manager[1]->SetModelRenderer(m_renderer[1]->CreateModelRenderer());

	// ���[�_�[�̐ݒ�B
	m_manager[0]->SetTextureLoader(m_renderer[0]->CreateTextureLoader());
	m_manager[0]->SetModelLoader(m_renderer[0]->CreateModelLoader());
	m_manager[0]->SetMaterialLoader(m_renderer[0]->CreateMaterialLoader());
	m_manager[0]->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

	m_manager[1]->SetTextureLoader(m_renderer[1]->CreateTextureLoader());
	m_manager[1]->SetModelLoader(m_renderer[1]->CreateModelLoader());
	m_manager[1]->SetMaterialLoader(m_renderer[1]->CreateMaterialLoader());
	m_manager[1]->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}
Effekseer::EffectRef EffectEngine::LoadEffect(const char16_t* filePath)
{
	std::u16string u16FilePath = filePath;
	Effekseer::EffectRef effect;
	auto it = m_effectMap.find(u16FilePath);
	if (it != m_effectMap.end()) {
		//���[�h�ς݁B
		effect = it->second;
		m_effectMap.insert({ u16FilePath, effect });
	}
	else {
		//�V�K

		//NOTE:�����G�t�F�N�g��ǂݍ���ł���̂�m_manager[0]��1�񕪂����ł���?
		effect = Effekseer::Effect::Create(m_manager[0], filePath);
	}

	return effect;
}
int EffectEngine::Play(Effekseer::EffectRef effect)
{
	//NOTE:handle�͍��ꂽ�G�t�F�N�g�����Ԗڂ��Ƃ��������������B
	//�ʁX��Play���Ă΂�邱�Ƃ͂Ȃ��̂œ���handle��Ԃ��Ă������炭���v?
	auto handle = m_manager[0]->Play(effect, 0, 0, 0);
	m_manager[1]->Play(effect, 0, 0, 0);
	return handle;
}
void EffectEngine::Stop(int effectHandle)
{
	m_manager[0]->StopEffect(effectHandle);
	m_manager[1]->StopEffect(effectHandle);
}


void EffectEngine::Update(float deltaTime, int cameraNum)
{
	//NOTE:�J�����̔ԍ����󂯎�蕪��B

	m_memoryPool[cameraNum]->NewFrame();

	// Begin a command list
	// �R�}���h���X�g���J�n����B
	EffekseerRendererDX12::BeginCommandList(m_commandList[cameraNum], g_graphicsEngine->GetCommandList());
	m_renderer[cameraNum]->SetCommandList(m_commandList[cameraNum]);

	m_manager[cameraNum]->Update();

	//�����_���[�ɃJ�����s���ݒ�B
	m_renderer[cameraNum]->SetCameraMatrix(*(const Effekseer::Matrix44*)&g_camera3D[cameraNum]->GetViewMatrix());
	//�����_���[�Ƀv���W�F�N�V�����s���ݒ�B
	m_renderer[cameraNum]->SetProjectionMatrix(*(const Effekseer::Matrix44*)&g_camera3D[cameraNum]->GetProjectionMatrix());

	m_renderer[cameraNum]->SetTime(deltaTime);
}

void EffectEngine::Draw(int cameraNum)
{
	//NOTE:�J�����̔ԍ����󂯎�蕪��B

	// Begin to rendering effects
	// �G�t�F�N�g�̕`��J�n�������s���B
	m_renderer[cameraNum]->BeginRendering();

	// Render effects
	// �G�t�F�N�g�̕`����s���B
	m_manager[cameraNum]->Draw();

	// Finish to rendering effects
	// �G�t�F�N�g�̕`��I���������s���B
	m_renderer[cameraNum]->EndRendering();

	// Finish a command list
	// �R�}���h���X�g���I������B
	m_renderer[cameraNum]->SetCommandList(nullptr);
	EffekseerRendererDX12::EndCommandList(m_commandList[cameraNum]);
}