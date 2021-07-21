#pragma once

class Player;
class GameScene;

class OpeningCamera : public IGameObject
{	
	bool Start() override;
	void Update() override;
public:
	/**
	 * @brief オープニングカメラ
	*/
	void CameraMove();

	void SetPlayerNum(int num) { m_playerNum = num; }
	void SetCameraFront(Vector3 front) { m_cameraFront = front; }
	void SetPlayerPos(Vector3 pos) { m_playerPos = pos; }
private:
	//オープニングカメラ
	int m_openingCameraCount = 0;		
	Vector3 m_sequenceCameraPos = Vector3::Zero;
	Vector3 m_targetPos = Vector3::Zero;	
	float m_openingCameraRotationRad = 0;
	float m_openingCamerZoomSpeed = 10;//カメラとターゲットとの接近速度	
	GameScene* m_gameScene = nullptr;
	//プレイヤーから入れる
	int m_playerNum = 0;
	Vector3 m_cameraFront = Vector3::Zero;
	Vector3 m_playerPos = Vector3::Zero;

	
};

