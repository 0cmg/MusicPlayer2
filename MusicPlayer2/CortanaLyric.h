#pragma once
#include "DrawCommon.h"
#include "ColorConvert.h"
class CCortanaLyric
{
public:
	CCortanaLyric();
	~CCortanaLyric();

	void Init();	//��ʼ������ȡCortana��������������
	void SetEnable(bool enable);
	void SetColors(ColorTable colors);

	/* ��Cortana�������ϻ��ƹ�����ʾ���ı�
	������
		str:	Ҫ���Ƶ��ַ���
		reset:	���Ϊtrue�����ù���λ��
		scroll_pixel:	�ı�����һ���ƶ�������ֵ�����ֵԽ�������Խ�죩
	*/
	void DrawCortanaText(LPCTSTR str, bool reset, int scroll_pixel);
	
	/* ��Cortana�������ϻ��ƶ�̬��ʾ��ʵ��ı�
	������
		str:	Ҫ���Ƶ��ַ���
		progress:	��ǰ��ʵĽ��ȣ���ΧΪ0~1000��
	*/
	void DrawCortanaText(LPCTSTR str, int progress);
	void ResetCortanaText();		//��Cortana��������ı��ָ�ΪĬ��
	void CheckDarkMode();			//���Cortana�������Ƿ�Ϊ��ɫģʽ

private:
	bool m_enable;
	HWND m_cortana_hwnd{};		//Cortana�ľ��
	HWND m_hCortanaStatic;
	wstring m_cortana_default_text;	//Cortana��������ԭ�����ı�
	CDrawCommon m_cortana_draw;		//������Cortana�������л�ͼ�Ķ���
	CWnd* m_cortana_wnd{};		//Cortana�������ָ��
	CFont m_cortana_font;		//��Cortana����������ʾ��ʵ�����
	CRect m_cortana_rect;		//Cortana�������ľ�������
	int m_cortana_left_space;		//Cortana����������ʾ�ı������������ľ���
	CDC* m_cortana_pDC{};				//��Cortana�������л�ͼ��DC

	bool m_dark_mode{ true };			//Cortana�������Ƿ�����ɫģʽ
	ColorTable m_colors;

	CPoint m_lefttop_point{};			//Cortana���������Ͻǵ��λ�ã������ж�Cortana���������ɫ����ɫ���ǰ�ɫ

};

