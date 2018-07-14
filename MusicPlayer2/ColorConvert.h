#pragma once
//#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v2)))
//#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))

struct ColorTable
{
	//ԭʼ����ɫ
	COLORREF original_color;
	//�任���Ⱥ����ɫ
	COLORREF dark1;
	COLORREF dark2;
	COLORREF dark3;
	COLORREF light1;
	COLORREF light2;
	COLORREF light3;
	COLORREF light4;
	//�̶�����ɫ
	const static COLORREF WHITE{ RGB(255,255,255) };
	const static COLORREF BLACK{ RGB(0,0,0) };
};

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

	static ColorTable m_gray_color;

	static void Initialize();

	// Converts RGB to HSL
	static void RGBtoHSL(const COLOR_RGB *rgb, COLOR_HSL *hsl);

	// Converts HSL to RGB
	static void HSLtoRGB(const COLOR_HSL *hsl, COLOR_RGB *rgb);

	//����ColorTable�е�original_color��ȡ������ɫ
	static void ConvertColor(ColorTable& color_table);

	//ȥɫ������ɫ�ı��ͶȽ�Ϊ0��
	static void Desaturate(COLORREF& color);
};

