#pragma once
#include "DrawCommon.h"
#include "ColorConvert.h"
#include "MusicPlayer2.h"

class CCortanaLyric
{
public:
	CCortanaLyric();
	~CCortanaLyric();

	void Init();	//��ʼ������ȡCortana��������������
	void SetEnable(bool enable);
	void SetColors(ColorTable colors);
	void SetCortanaColor(int color);		//����Cortana������ı�����ɫ��0������ϵͳ��1����ɫ��2����ɫ��

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

	//��Cortana��������˫����ʾ���
	void DrawLyricDoubleLine(LPCTSTR lyric, LPCTSTR next_lyric, int progress);

	void DrawAlbumCover(const CImage& album_cover);

	void ResetCortanaText();		//��Cortana��������ı��ָ�ΪĬ��
	void CheckDarkMode();			//���Cortana�������Ƿ�Ϊ��ɫģʽ
	void AlbumCoverEnable(bool enable);
	void SetSpectrum(int spectrum);

private:
	bool m_enable;
	HWND m_cortana_hwnd{};		//Cortana�ľ��
	HWND m_hCortanaStatic;
	wstring m_cortana_default_text;	//Cortana��������ԭ�����ı�
	CDrawCommon m_cortana_draw;		//������Cortana�������л�ͼ�Ķ���
	CWnd* m_cortana_wnd{};		//Cortana�������ָ��
	CFont m_cortana_font;		//��Cortana����������ʾ��ʵ�����
	CFont m_font_double_line;		//˫����ʾʱ��ʵ�����
	CRect m_cortana_rect;		//Cortana�������ľ�������
	CRect m_icon_rect;			//Cortanaͼ�괦�ľ�������
	int m_cortana_left_space;		//Cortana����������ʾ�ı������������ľ���
	CDC* m_cortana_pDC{};				//��Cortana�������л�ͼ��DC

	bool m_dark_mode{ true };			//Cortana�������Ƿ�����ɫģʽ
	ColorTable m_colors;

	CPoint m_check_dark_point{};			//�����ж�Cortana�������Ƿ�Ϊ��ɫģʽ�ĵ��λ��
	COLORREF m_back_color;
	const COLORREF m_border_color{ GRAY(180) };		//ǳɫģʽʱ�߿����ɫ

	bool m_show_album_cover{ false };			//�Ƿ���Cortanaͼ�괦��ʾר������
	int m_cortana_color;		//Cortana������ı�����ɫ��0������ϵͳ��1����ɫ��2����ɫ��

	int m_spectrum{0};		//Ƶ�׵ķ�ֵ��ȡֵΪ0~1000

public:
	bool m_cortana_disabled;
};

