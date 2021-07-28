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


//様々な場所で使えそうな定数
const int PAD_PLAYER1 = 0;						//プレイヤー1のパッド番号
const int PAD_PLAYER2 = 1;						//プレイヤー2のパッド番号

const int NUMBER_PLAYER1 = 0;					//プレイヤー1の番号
const int NUMBER_PLAYER2 = 1;					//プレイヤー2の番号

const int TRANSITION_TIME_NORMAL = 50;			//通常のトランジション時間

const int MAGNETSTATE_GRAVITY = -1;				//磁力の状態:引力
const int MAGNETSTATE_REPULSION = 1;			//磁力の状態:斥力

const float BURST_AFFECT_RANGE_MIN = 50.0f;		//バーストの最小距離
const float BURST_AFFECT_RANGE_MAX = 500.0f;	//バーストの最大距離

const float SOUND_SE_VOICE_VOLUME = 3.0f;		//ボイスSEのボリューム