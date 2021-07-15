#pragma once
class OptionValue
{
public:
	OptionValue() {}
	OptionValue(const OptionValue& opt) = delete;
	void operator= (const OptionValue& opt) = delete;

	//値の増減モード
	enum valueChangeMode
	{
		enPress,	//押してる間増減
		enTrigger	//押したフレームだけ増減
	};

	/**
	 * @brief 初期化
	 * @param changeRate 値の増減量
	 * @param maxValue 値の最大量
	 * @param minValue 値の最小量
	 * @param mode 増減モード
	*/
	void Init(float changeRate, float minValue, float maxValue, valueChangeMode mode);

	/**
	 * @brief 設定されている数値を取得
	 * @return 数値
	*/
	float GetValue() { return m_value; }
	
	/**
	 * @brief 数値の設定
	 * @param value 数値
	*/
	void SetValue(float value);

	/**
	 * @brief 値を増加させる
	*/
	void IncreaseValue();

	/**
	 * @brief 値を減少させる
	*/
	void DecreaseValue();

	/**
	 * @brief 増減モードを取得
	 * @return 増減モード
	*/
	valueChangeMode GetValueChangeMode() { return m_valueChangeMode; }

private:

	/**
	 * @brief 値の範囲チェック
	*/
	void RangeCheck();

private:
	float m_value = 0.0f;							//項目の数値
	float m_valueChangeRate = 0.0f;					//数値の増減量
	float m_minValue = 0.0f;						//数値の最小値
	float m_maxValue = 1.0f;						//数値の最大値
	valueChangeMode m_valueChangeMode = enPress;	//増減モード
	bool m_isInited = false;						//初期化された?
};

