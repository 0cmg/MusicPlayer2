#pragma once
#include "MusicPlayer2.h"
#include "IPlayerUI.h"
#include "CPlayerUIHelper.h"

struct SLayoutData
{
	const int margin = theApp.DPI(4);							//��Ե������
	const int width_threshold = theApp.DPI(600);				//�������ͨ����ģʽ�л���խ����ģʽʱ�����ȵ���ֵ
	const int info_height = theApp.DPI(206);					//խ����ģʽʱ��ʾ��Ϣ����ĸ߶�
	const int info_height2 = theApp.DPI(143);					//��ͨ����ģʽʱ��ʾ��Ϣ����ĸ߶�
	const int path_edit_height = theApp.DPI(32);				//ǰ·��Edit�ؼ�����ĸ߶�
	const int search_edit_height = theApp.DPI(26);				//����������Edit�ؼ�����ĸ߶�
	const CSize spectral_size{ theApp.DPI(120), theApp.DPI(90) };	//Ƶ�׷�������Ĵ�С
};


class CPlayerUIBase : public IPlayerUI
{
public:

	struct UIData
	{
		CFont lyric_font;					//�������
		CFont lyric_translate_font;			//��ʷ��������
		bool show_translate{ true };		//����Ƿ���ʾ����
		bool m_narrow_mode;					//խ����ģʽ
		bool show_playlist{true};

		int client_width;					//���ڿͻ������
		int client_height;					//���ڿͻ����߶�
		CImage default_background;			//Ĭ�ϵı���
	};

public:
	CPlayerUIBase(UIData& ui_data);
	~CPlayerUIBase();

	void SetToolTip(CToolTipCtrl* pToolTip);

public:
	virtual void Init(CDC* pDC) override;
	virtual void DrawInfo(bool reset = false) override;
	virtual void ClearInfo() override;

	virtual void LButtonDown(CPoint point) override;
	virtual void RButtonUp(CPoint point) override;
	virtual void MouseMove(CPoint point) override;
	virtual void LButtonUp(CPoint point) override;
	virtual void OnSizeRedraw(int cx, int cy) override;

	virtual CRect GetThumbnailClipArea() override;
	void UpdateRepeatModeToolTip();
	void UpdateSongInfoToolTip();
	void UpdatePlayPauseButtonTip() override;

	virtual bool SetCursor() override;

protected:
	enum BtnKey		//��ʶ��ť������
	{
		BTN_REPETEMODE,			//��ѭ��ģʽ����ť
		BTN_VOLUME,				//������ť
		BTN_TRANSLATE,			//��ʷ��밴ť
		BTN_SKIN,				//�л����水ť
		BTN_EQ,					//��Ч�趨��ť
		BTN_SETTING,			//���ð�ť
		BTN_MINI,				//����ģʽ��ť
		BTN_INFO,				//��Ŀ��Ϣ��ť
		BTN_STOP,
		BTN_PREVIOUS,
		BTN_PLAY_PAUSE,
		BTN_NEXT,
		BTN_SHOW_PLAYLIST,
		BTN_SELECT_FOLDER,
		BTN_PROGRESS
	};

	struct DrawData
	{
		CRect cover_rect;
		CRect lyric_rect;
		CRect thumbnail_rect;
	};

protected:
	void PreDrawInfo();
	void SetDrawRect();
	void DrawLyricTextMultiLine(CRect rect, bool midi_lyric);
	void DrawLyricTextSingleLine(CRect rect, bool midi_lyric);
	void DrawSongInfo(CRect rect, bool reset = false);
	void DrawToolBar(bool draw_background, CRect rect, bool draw_translate_button, UIData* pUIData = nullptr);
	void DrawVolumnAdjBtn(bool draw_background);
	void DrawControlBar(CRect rect, bool draw_background);
	void DrawProgressBar(CRect rect, bool draw_background);

	virtual void AddMouseToolTip(BtnKey btn, LPCTSTR str) = 0;		//Ϊһ����ť��������ʾ
	virtual void UpdateMouseToolTip(BtnKey btn, LPCTSTR str) = 0;
	virtual void UpdateToolTipPosition() = 0;

	void AddToolTips();			//Ϊÿһ����ť��������ʾ�����ڰ�ť�ľ�������ֻ���ڵ�һ�λ�ͼ֮�����ȷ�������Դ˺��������ڵ�һ�λ�ͼ֮����ã�

	static CRect DrawAreaToClient(CRect rect, CRect draw_area);
	static CRect ClientAreaToDraw(CRect rect, CRect draw_area);

	bool DrawNarrowMode();			//�Ƿ�ʹ��խ����ģʽ��ͼ

private:
	void DrawLyricDoubleLine(CRect rect, LPCTSTR lyric, LPCTSTR next_lyric, int progress);
	void DrawUIButton(CRect rect, UIButton& btn, const IconRes& icon, bool draw_background);
	void DrawControlButton(CRect rect, UIButton& btn, const IconRes& icon, bool draw_background);
	void SetRepeatModeToolTipText();
	void SetSongInfoToolTipText();

protected:
	CDC* m_pDC;
	UIColors m_colors;
	CDrawCommon m_draw;		//���ڻ����ı��Ķ���
	SLayoutData m_layout;
	CFont m_font_time;
	DrawData m_draw_data;

	CMenu m_popup_menu;			//����Ҽ��˵�
	CMenu m_main_popup_menu;

	CToolTipCtrl* m_tool_tip = nullptr;

	CString m_repeat_mode_tip;
	CString m_info_tip;

	int m_lyric_text_height;
	UIData& m_ui_data;

	//UI ����
	CRect m_draw_rect;						//��ͼ����
	CRect m_volume_up_rect, m_volume_down_rect;	//�������������Ӻͼ��������ľ�������
	bool m_show_volume_adj{ false };		//��ʾ����������ť

	std::map<BtnKey, UIButton> m_buttons;

	const int m_progress_on_top_threshold = theApp.DPI(350);		//���������Ŀ��С�ڴ�ֵ������������ʾ�ڲ��ſ��ư�ť���Ϸ�

private:
	bool m_first_draw{ true };

};

