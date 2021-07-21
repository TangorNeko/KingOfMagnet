#pragma once

class Player;
class GameScene;

class OpeningCamera : public IGameObject
{	
	bool Start() override;
	void Update() override;
public:
	/**
	 * @brief �I�[�v�j���O�J����
	*/
	void CameraMove();

	void SetPlayerNum(int num) { m_playerNum = num; }
	void SetCameraFront(Vector3 front) { m_cameraFront = front; }
	void SetPlayerPos(Vector3 pos) { m_playerPos = pos; }
private:
	//�I�[�v�j���O�J����
	int m_openingCameraCount = 0;		
	Vector3 m_sequenceCameraPos = Vector3::Zero;
	Vector3 m_targetPos = Vector3::Zero;	
	float m_openingCameraRotationRad = 0;
	float m_openingCamerZoomSpeed = 10;//�J�����ƃ^�[�Q�b�g�Ƃ̐ڋߑ��x	
	GameScene* m_gameScene = nullptr;
	//�v���C���[��������
	int m_playerNum = 0;
	Vector3 m_cameraFront = Vector3::Zero;
	Vector3 m_playerPos = Vector3::Zero;

	
};

