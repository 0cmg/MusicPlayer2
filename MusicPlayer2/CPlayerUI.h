#pragma once
#include "DrawCommon.h"
#include "MusicPlayer2.h"

struct SLayoutData
{
	const int control_bar_height = theApp.DPI(30);				//�����Ϸ��Ĳ��ſ��ư�ť���ֵĸ߶�
	const int margin = theApp.DPI(4);							//��Ե������
	const int width_threshold = theApp.DPI(600);				//�������ͨ����ģʽ�л���խ����ģʽʱ�����ȵ���ֵ
	const int info_height = theApp.DPI(166);					//խ����ģʽʱ��ʾ��Ϣ����ĸ߶�
	const int info_height2 = theApp.DPI(143);					//��ͨ����ģʽʱ��ʾ��Ϣ����ĸ߶�
	const int path_edit_height = theApp.DPI(32);				//ǰ·��Edit�ؼ�����ĸ߶�
	const int search_edit_height = theApp.DPI(26);				//����������Edit�ؼ�����ĸ߶�
	const int progress_bar_height = theApp.DPI(20);				//(խ����ģʽʱ)����������ĸ߶�
	const CSize spectral_size{ theApp.DPI(120), theApp.DPI(90) };	//Ƶ�׷�������Ĵ�С
};

class CPlayerUI
{
public:
	struct UIColors		//������ɫ
	{
		COLORREF color_text;				//�ı���ɫ
		COLORREF color_text_lable;			//��ǩ�ı�����ɫ
		COLORREF color_text_2;				//���δ�����ı�����ɫ
		COLORREF color_text_heighlight;		//���ָ��ʱ�ı�����ɫ
		COLORREF color_back;				//������ɫ
		COLORREF color_lyric_back;			//��ʽ��汳����ɫ
		COLORREF color_control_bar_back;	//������������ɫ
		COLORREF color_spectrum;			//Ƶ�׷������ε���ɫ
		COLORREF color_spectrum_cover;		//��ר������ʱ��Ƶ�׷������ε���ɫ
		COLORREF color_spectrum_back;		//Ƶ�׷����ı�����ɫ
		COLORREF color_button_back;			//��ʷ��밴ť�ı���ɫ
		int background_transparency;		//������͸����0~100
	};

	struct UIButton		//�����л��Ƶİ�ť
	{
		CRect rect;				//��ť�ľ�������
		bool hover{ false };	//����Ƿ�ָ��ť
		bool enable{ true };	//��ť�Ƿ�����
	};

	struct UIData
	{
		CRect draw_rect;					//��ͼ����

		CRect cover_rect;					//��ʾר������ľ��������Ի�ͼ�������Ͻ�Ϊԭ�㣩
		CRect repetemode_rect;				//��ʾ��ѭ��ģʽ���ľ�������
		bool repetemode_hover{ false };		//���ָ���ˡ�ѭ��ģʽ���ľ�������
		UIButton volume_btn;				//������ť
		CRect volume_up_rect, volume_down_rect;	//�������������Ӻͼ��������ľ�������
		bool show_volume_adj{ false };		//��ʾ����������ť
		UIButton translate_btn;				//��ʷ��밴ť

		CFont lyric_font;					//�������
		CFont lyric_translate_font;			//��ʷ��������

		bool show_translate{ true };		//����Ƿ���ʾ����

		int client_width;					//���ڿͻ������
		int client_height;					//���ڿͻ����߶�
		CImage default_background;			//Ĭ�ϵı���

	};

public:
	CPlayerUI(UIData& ui_data);
	~CPlayerUI();

	void Init(CDC* pDC);
	void DrawInfo(bool narrow_mode, bool reset = false);		//������Ϣ

protected:
	void DrawLyricsSingleLine(CRect lyric_rect);			//���Ƹ�ʣ�խ����ģʽ�µ�����ʾ��������Ϊ��ʾ��ʵľ�������
	void DrawLyricsMulityLine(CRect lyric_rect, CDC* pDC);			//���Ƹ�ʣ���ͨģʽ�¶�����ʾ��������Ϊ��ʾ��ʵľ�������


protected:
	CDC* m_pDC;
	UIData& m_ui_data;
	UIColors m_colors;

	CDrawCommon m_draw;		//���ڻ����ı��Ķ���

	std::shared_ptr<SLayoutData> m_pLayout{ nullptr };
};

