#include "stdafx.h"
#include "OptionValue.h"

void OptionValue::Init(float changeRate, float minValue, float maxValue, valueChangeMode mode)
{
	if (m_isInited == true)
	{
		return;
	}

	//�e���ڂ��Z�b�g
	m_valueChangeRate = changeRate;
	m_minValue = minValue;
	m_maxValue = maxValue;
	m_valueChangeMode = mode;

	//����������
	m_isInited = true;
}

void OptionValue::SetValue(float value)
{
	//����������Ă��Ȃ��ꍇ�G���[
	if (m_isInited == false)
	{
		MessageBoxA(nullptr, "�l�̐ݒ�͏��������Ă���g�p���Ă��������B\n", "�G���[", MB_OK);
		std::abort();
	}

	//�l���Z�b�g
	m_value = value;

	//�͈̓`�F�b�N
	RangeCheck();
}

void OptionValue::IncreaseValue()
{
	//�����l������
	m_value += m_valueChangeRate;

	//�͈̓`�F�b�N
	RangeCheck();
}

void OptionValue::DecreaseValue()
{
	//�����l������
	m_value -= m_valueChangeRate;

	//�͈̓`�F�b�N
	RangeCheck();
}

void OptionValue::RangeCheck()
{
	if (m_value < m_minValue)
	{
		//�ŏ��l��菬������΍ŏ��l��
		m_value = m_minValue;
	}
	else if (m_maxValue < m_value)
	{
		//�ő�l���傫����΍ő�l��
		m_value = m_maxValue;
	}
}