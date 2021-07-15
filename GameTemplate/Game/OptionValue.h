#pragma once
class OptionValue
{
public:
	OptionValue() {}
	OptionValue(const OptionValue& opt) = delete;
	void operator= (const OptionValue& opt) = delete;

	//�l�̑������[�h
	enum valueChangeMode
	{
		enPress,	//�����Ă�ԑ���
		enTrigger	//�������t���[����������
	};

	/**
	 * @brief ������
	 * @param changeRate �l�̑�����
	 * @param maxValue �l�̍ő��
	 * @param minValue �l�̍ŏ���
	 * @param mode �������[�h
	*/
	void Init(float changeRate, float minValue, float maxValue, valueChangeMode mode);

	/**
	 * @brief �ݒ肳��Ă��鐔�l���擾
	 * @return ���l
	*/
	float GetValue() { return m_value; }
	
	/**
	 * @brief ���l�̐ݒ�
	 * @param value ���l
	*/
	void SetValue(float value);

	/**
	 * @brief �l�𑝉�������
	*/
	void IncreaseValue();

	/**
	 * @brief �l������������
	*/
	void DecreaseValue();

	/**
	 * @brief �������[�h���擾
	 * @return �������[�h
	*/
	valueChangeMode GetValueChangeMode() { return m_valueChangeMode; }

private:

	/**
	 * @brief �l�͈̔̓`�F�b�N
	*/
	void RangeCheck();

private:
	float m_value = 0.0f;							//���ڂ̐��l
	float m_valueChangeRate = 0.0f;					//���l�̑�����
	float m_minValue = 0.0f;						//���l�̍ŏ��l
	float m_maxValue = 1.0f;						//���l�̍ő�l
	valueChangeMode m_valueChangeMode = enPress;	//�������[�h
	bool m_isInited = false;						//���������ꂽ?
};

