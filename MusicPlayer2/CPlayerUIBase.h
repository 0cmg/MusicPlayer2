#pragma once
#include "MusicPlayer2.h"
#include "IPlayerUI.h"

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


class CPlayerUIBase : public IPlayerUI
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

	struct UIData
	{
		CFont lyric_font;					//�������
		CFont lyric_translate_font;			//��ʷ��������
		bool show_translate{ true };		//����Ƿ���ʾ����
		bool m_narrow_mode;		//խ����ģʽ

		int client_width;					//���ڿͻ������
		int client_height;					//���ڿͻ����߶�
		CImage default_background;			//Ĭ�ϵı���
	};

public:
	CPlayerUIBase(UIData& ui_data);
	~CPlayerUIBase();

	void SetToolTip(CToolTipCtrl* pToolTip);

public:
	virtual void Init(CDC* pDC) override = 0;
	virtual void DrawInfo(bool reset = false) override;

	virtual void RButtonUp(CPoint point) override;
	virtual void MouseMove(CPoint point) override;
	virtual void LButtonUp(CPoint point) override;
	virtual void OnSizeRedraw(int cx, int cy) override = 0;

	virtual CRect GetThumbnailClipArea() override = 0;
	void UpdateRepeatModeToolTip();

protected:
	enum BtnKey		//��ʶ��ť������
	{
		BTN_REPETEMODE,			//��ѭ��ģʽ����ť
		BTN_VOLUME,				//������ť
		BTN_TRANSLATE,			//��ʷ��밴ť
		BTN_SKIN,				//�л����水ť
		BTN_EQ,					//��Ч�趨��ť
		BTN_SETTING,			//���ð�ť
		BTN_MINI				//����ģʽ��ť
	};

protected:
	void PreDrawInfo();
	void DrawLyricTextMultiLine(CRect rect, bool midi_lyric);
	void DrawLyricTextSingleLine(CRect rect, bool midi_lyric);
	void DrawSongInfo(CRect rect, bool reset = false);
	void DrawControlBar(bool draw_background, CRect rect, bool draw_translate_button, UIData* pUIData = nullptr);
	void DrawVolumnAdjBtn(bool draw_background);

	virtual void AddMouseToolTip(BtnKey btn, LPCTSTR str) = 0;		//Ϊһ����ť��������ʾ
	virtual void UpdateMouseToolTip(BtnKey btn, LPCTSTR str) = 0;
	virtual void UpdateToolTipPosition() = 0;

	void AddToolTips();			//Ϊÿһ����ť��������ʾ�����ڰ�ť�ľ�������ֻ���ڵ�һ�λ�ͼ֮�����ȷ�������Դ˺��������ڵ�һ�λ�ͼ֮����ã�

	static CRect DrawAreaToClient(CRect rect, CRect draw_area);
	static CRect ClientAreaToDraw(CRect rect, CRect draw_area);

private:
	void DrawLyricDoubleLine(CRect rect, LPCTSTR lyric, LPCTSTR next_lyric, int progress);
	void DrawUIButton(CRect rect, UIButton& btn, HICON icon, bool draw_background);
	void SetRepeatModeToolTipText();

protected:
	CDC* m_pDC;
	UIColors m_colors;
	CDrawCommon m_draw;		//���ڻ����ı��Ķ���

	CMenu m_popup_menu;			//����Ҽ��˵�
	CMenu m_main_popup_menu;

	CToolTipCtrl* m_tool_tip = nullptr;

	CString m_repeat_mode_tip;

	int m_lyric_text_height;
	UIData& m_ui_data;

	//UI ����
	CRect m_draw_rect;						//��ͼ����
	CRect m_volume_up_rect, m_volume_down_rect;	//�������������Ӻͼ��������ľ�������
	bool m_show_volume_adj{ false };		//��ʾ����������ť

	std::map<BtnKey, UIButton> m_buttons;

private:
	bool m_first_draw{ true };

};

