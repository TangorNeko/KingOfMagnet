#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "MiniEngine.h"
#include "ExEngine.h"

#include <xaudio2.h>
#include <x3daudio.h>
#include <xaudio2fx.h>

#include "EffectEngine.h"
#include "SoundEngine.h"
#include "prefab.h"
#include "CLightManager.h"
#include "ResourceBankManager.h"
#include "PostEffectManager.h"

#include "time/Stopwatch.h"
#include "time/GameTime.h"
#include "NameKey.h"
#include "TransitionGenerator.h"


//�l�X�ȏꏊ�Ŏg�������Ȓ萔
const int PAD_PLAYER1 = 0;						//�v���C���[1�̃p�b�h�ԍ�
const int PAD_PLAYER2 = 1;						//�v���C���[2�̃p�b�h�ԍ�

const int NUMBER_PLAYER1 = 0;					//�v���C���[1�̔ԍ�
const int NUMBER_PLAYER2 = 1;					//�v���C���[2�̔ԍ�

const int TRANSITION_TIME_NORMAL = 50;			//�ʏ�̃g�����W�V��������

const int MAGNETSTATE_GRAVITY = -1;				//���͂̏��:����
const int MAGNETSTATE_REPULSION = 1;			//���͂̏��:�˗�

const float BURST_AFFECT_RANGE_MIN = 50.0f;		//�o�[�X�g�̍ŏ�����
const float BURST_AFFECT_RANGE_MAX = 500.0f;	//�o�[�X�g�̍ő勗��

const float SOUND_SE_VOICE_VOLUME = 3.0f;		//�{�C�XSE�̃{�����[��