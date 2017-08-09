#pragma once
//#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v2)))
//#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))

class CColorConvert
{
public:

	struct COLOR_RGB
	{
		int  red;              // [0,255]
		int  green;            // [0,255]
		int  blue;             // [0,255]
	};

	struct COLOR_HSL
	{
		float hue;              // [0,360]
		float saturation;       // [0,100]
		float luminance;        // [0,100]
	};

	CColorConvert();
	~CColorConvert();

	template<class T>
	static T min3v(T v1, T v2, T v3)
	{
		T min = v1;
		if (v2 < min)
			min = v2;
		if (v3 < min)
			min = v3;
		return min;
	}

	template<class T>
	static T max3v(T v1, T v2, T v3)
	{
		T max = v1;
		if (v2 > max)
			max = v2;
		if (v3 > max)
			max = v3;
		return max;
	}

	// Converts RGB to HSL
	static void RGBtoHSL(const COLOR_RGB *rgb, COLOR_HSL *hsl);

	// Converts HSL to RGB
	static void HSLtoRGB(const COLOR_HSL *hsl, COLOR_RGB *rgb);

	//�˺������ڸ�������ɫ���㲥���б��е�ǰ������Ŀ���ı���ɫ
	//�ڱ���ɫ��ͱ��ͶȲ��������¼�����ɫ������ֵ�����ȱ任Ϊһ�κ���y=0.5x+12
	static COLORREF ConvertToItemColor(COLORREF color);

	//�˺������ڸ�������ɫ���㲥���б��е�ǰ������Ŀ�ı�����ɫ
	//�ڱ���ɫ��ͱ��ͶȲ���������������ɫ������ֵ�����ȱ任Ϊһ�κ���y=0.1x+90
	static COLORREF ConvertToBackColor(COLORREF color);

	//�˺������ڸ�������ɫ��������ģʽ�п���OK��ʾʱ��ʵ���ɫ
	//�ڱ���ɫ��ͱ��ͶȲ���������������ɫ������ֵ�����ȱ任Ϊһ�κ���y=0.4x+50
	static COLORREF ConvertToLightColor(COLORREF color);
};

