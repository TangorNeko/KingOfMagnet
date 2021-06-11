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


//TODO:定数　いろんな所で使えそうな定数は仮でstdafx.hに置く、場所は検討
const int PAD_PLAYER1 = 0;
const int PAD_PLAYER2 = 1;

const int TRANSITION_TIME = 50;

const int MAGNETSTATE_GRAVITY = -1;
const int MAGNETSTATE_REPULSION = 1;