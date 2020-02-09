#pragma once
class Time
{
public:
	int min{};
	int sec{};
	int msec{};

	Time()
	{}

	Time(int _min, int _sec, int _msec) : min{_min}, sec{_sec}, msec{_msec}
	{}

	Time(int time)
	{
		fromInt(time);
	}

	~Time()
	{}

	//��int���͵�ʱ�䣨��������ת����Time�ṹ
	void fromInt(int time)
	{
		msec = time % 1000;
		sec = time / 1000 % 60;
		min = time / 1000 / 60;
	}

	//��Time�ṹת����int���ͣ���������
	int toInt() const
	{
		return msec + sec * 1000 + min * 60000;
	}

	bool operator>(const Time& time) const
	{
		if (min != time.min)
			return (min > time.min);
		else if (sec != time.sec)
			return(sec > time.sec);
		else if (msec != time.msec)
			return(msec > time.msec);
		else return false;
	}

    bool operator<(const Time& time) const
    {
        return time > *this;
    }

	bool operator==(const Time& time) const
	{
		return (min == time.min && sec == time.sec && msec == time.msec);
	}
	
	bool operator!=(const Time& time) const
	{
		return !(*this == time);
	}

	bool operator>=(const Time& time) const
	{
		if (min != time.min)
			return (min > time.min);
		else if (sec != time.sec)
			return(sec > time.sec);
		else if (msec != time.msec)
			return(msec > time.msec);
		else return true;
	}

	//��������������ڼ�������Time�����ʱ������int���ͣ���λΪ����
	int operator-(const Time& time) const
	{
		return (min - time.min) * 60000 + (sec - time.sec) * 1000 + (msec - time.msec);
	}

	//�ӷ���ֵ������������ڵ�ǰʱ���ϼ���һ��int���͵ĺ�����
	Time operator+=(int time)
	{
		int added = this->toInt();
		added += time;
		this->fromInt(added);
		return *this;
	}

    Time operator-=(int time)
    {
        return operator+=(-time);
    }

	//�ӷ�������������ڵ�ǰʱ���ϼ���һ��int���͵ĺ�����������Time����
	Time operator+(int time) const
	{
		int added = this->toInt();
		added += time;
		return Time{ added };
	}

	//��ʱ��ת�����ַ�������ʽ����:�룩
	wstring toString(bool no_zero = true) const
	{
		wchar_t buff[16];
		if (no_zero && *this == Time{ 0,0,0 })
			wcscpy_s(buff, L"-:--");
		else
			swprintf_s(buff, L"%d:%.2d", min, sec);
		return wstring(buff);
	}

	//��ʱ��ת�����ַ�������ʽ����:��.���룩
	wstring toString2(bool no_zero = true) const
	{
		wchar_t buff[16];
		if (no_zero && *this == Time{ 0,0,0 })
			wcscpy_s(buff, L"-:--");
		else
			swprintf_s(buff, L"%d:%.2d.%.3d", min, sec, msec);
		return wstring(buff);
	}

	//��ʱ��ת�����ַ�������ʽ��ʱ:��:�룩
	wstring toString3(bool no_zero = true) const
	{
		int hour, min1;
		hour = min / 60;
		min1 = min % 60;
		wchar_t buff[16];
		if (no_zero && *this == Time{ 0,0,0 })
			wcscpy_s(buff, L"-:--:--");
		else
			swprintf_s(buff, L"%d:%.2d:%.2d", hour, min1, sec);
		return wstring(buff);
	}

	//�ж�ʱ���Ƿ�Ϊ0
	bool isZero() const
	{
		return (min == 0 && sec == 0 && msec == 0);
	}
};

