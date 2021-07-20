#include "stdafx.h"
#include "OptionValue.h"

void OptionValue::Init(float changeRate, float minValue, float maxValue, valueChangeMode mode)
{
	if (m_isInited == true)
	{
		return;
	}

	//各項目をセット
	m_valueChangeRate = changeRate;
	m_minValue = minValue;
	m_maxValue = maxValue;
	m_valueChangeMode = mode;

	//初期化完了
	m_isInited = true;
}

void OptionValue::SetValue(float value)
{
	//初期化されていない場合エラー
	if (m_isInited == false)
	{
		MessageBoxA(nullptr, "値の設定は初期化してから使用してください。\n", "エラー", MB_OK);
		std::abort();
	}

	//値をセット
	m_value = value;

	//範囲チェック
	RangeCheck();
}

void OptionValue::IncreaseValue()
{
	//増減値分増加
	m_value += m_valueChangeRate;

	//範囲チェック
	RangeCheck();
}

void OptionValue::DecreaseValue()
{
	//増減値分減少
	m_value -= m_valueChangeRate;

	//範囲チェック
	RangeCheck();
}

void OptionValue::RangeCheck()
{
	if (m_value < m_minValue)
	{
		//最小値より小さければ最小値に
		m_value = m_minValue;
	}
	else if (m_maxValue < m_value)
	{
		//最大値より大きければ最大値に
		m_value = m_maxValue;
	}
}