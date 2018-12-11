#pragma once
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


class CPlayerUIBase
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
		CFont lyric_font;					//�������
		CFont lyric_translate_font;			//��ʷ��������
		bool show_translate{ true };		//����Ƿ���ʾ����

		int client_width;					//���ڿͻ������
		int client_height;					//���ڿͻ����߶�
		CImage default_background;			//Ĭ�ϵı���
	};

public:
	CPlayerUIBase();
	~CPlayerUIBase();

	void SetToolTip(CToolTipCtrl* pToolTip);

public:
	virtual void Init(CDC* pDC) = 0;
	virtual void DrawInfo(bool narrow_mode, bool reset = false);

	virtual void RButtonUp(CPoint point, bool narrow_mode);
	virtual void MouseMove(CPoint point);
	virtual void LButtonUp(CPoint point);
	virtual void OnSizeRedraw(int cx, int cy, bool narrow_mode) = 0;

	virtual CRect GetThumbnailClipArea(bool narrow_mode) = 0;

protected:
	void DrawLyricTextMultiLine(CRect rect, CFont* font, CFont* translate_font, bool show_translate, bool midi_lyric);
	void DrawLyricTextSingleLine(CRect rect, CFont* font, CFont* translate_font, bool show_translate, bool midi_lyric);
	//void DrawControlBar(bool draw_background, CRect rect, bool draw_translate_button);

	void AddMouseToolTip(const UIButton& btn, LPCTSTR str, bool* static_bool);		//Ϊһ����ť��������ʾ��ֻ������Ӧ��WM_MOUSEMOVE��ʱ����

	static CRect DrawAreaToClient(CRect rect, CRect draw_area);

private:
	void DrawLyricDoubleLine(CRect rect, CFont* font, LPCTSTR lyric, LPCTSTR next_lyric, int progress);

protected:
	CDC* m_pDC;
	UIColors m_colors;
	CDrawCommon m_draw;		//���ڻ����ı��Ķ���

	CMenu m_popup_menu;			//����Ҽ��˵�
	CMenu m_main_popup_menu;

	CToolTipCtrl* m_tool_tip = nullptr;

	//UI ����
	UIButton repetemode_btn;				//��ѭ��ģʽ����ť
	UIButton volume_btn;				//������ť
	CRect volume_up_rect, volume_down_rect;	//�������������Ӻͼ��������ľ�������
	bool show_volume_adj{ false };		//��ʾ����������ť
	UIButton translate_btn;				//��ʷ��밴ť
	UIButton skin_btn;

};

