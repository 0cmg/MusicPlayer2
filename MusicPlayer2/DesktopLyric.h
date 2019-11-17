#pragma once
#include "LyricsWindow.h"
#include "CommonData.h"

struct LyricStyle
{
    COLORREF color1 = 0;
    COLORREF color2 = 0;
    int gradient_mode = 0;
};

struct LyricStyleDefaultData        //������Ԥ������
{
    LyricStyle normal_style;
    LyricStyle highlight_style;
};
const int LYRIC_DEFAULT_STYLE_NUM = 3;

class CDesktopLyric : public CLyricsWindow
{
public:
    struct UIButton		//�����л��Ƶİ�ť
    {
        CRect rect;				//��ť�ľ�������
        bool hover{ false };	//����Ƿ�ָ��ť
        bool pressed{ false };	//��ť�Ƿ���
        bool enable{ true };	//��ť�Ƿ�����
    };

    enum BtnKey		//��ʶ��ť������
    {
        BTN_APP,
        BTN_STOP,				//ֹͣ
        BTN_PREVIOUS,			//��һ��
        BTN_PLAY_PAUSE,			//����/��ͣ
        BTN_NEXT,				//��һ��
        BTN_LOCK,
        BTN_DOUBLE_LINE,
        BTN_BACKGROUND_PENETRATE,
        BTN_SETTING,
        BTN_DEFAULT_STYLE,
        BTN_CLOSE,
        MAX             //����ָʾö�ٵ�������
    };

public:
	CDesktopLyric();
	~CDesktopLyric();

	void Create();
	void ShowLyric();
	void ClearLyric();
	void ApplySettings(const DesktopLyricSettingData& data);
	void SetLyricWindowVisible(bool visible);
	void SetLyricWindowLock(bool locked);
    void SetLyricOpacity(int opacity);
    void SetLyricBackgroundPenetrate(bool penetrate);
    LyricStyleDefaultData GetDefaultStyle(int index);

    static void LyricStyleDefaultDataToLyricSettingData(const LyricStyleDefaultData& style_data, DesktopLyricSettingData& setting_data);

    //���ƹ�����
    void DrawToolbar(Gdiplus::Graphics* pGraphics);
    //���ƹ������ϵ�ͼ��
    void DrawToolIcon(Gdiplus::Graphics* pGraphics, IconRes icon, CRect rect, BtnKey btn, bool checked = false);

    //��������ʾ
    void AddToolTips();
    //Ϊһ����ť��������ʾ
    void AddMouseToolTip(BtnKey btn, LPCTSTR str);
    void UpdateMouseToolTip(BtnKey btn, LPCTSTR str);
    void UpdateToolTipPosition();

protected:
    virtual void PreDrawLyric(Gdiplus::Graphics* pGraphics) override;
    virtual void AfterDrawLyric(Gdiplus::Graphics* pGraphics) override;

private:
    void SetWindowStyle();

private:
	//CLyricsWindow m_lyric_window;
    LyricStyleDefaultData m_default_style[LYRIC_DEFAULT_STYLE_NUM];

    CMenu m_popupMenu;
    CToolTipCtrl m_tool_tip;
    std::map<BtnKey, UIButton> m_buttons;
    bool m_first_draw = true;       //��һ�λ��ƹ�����ʱ����Ϊtrue
    bool m_bHover = false;                  //����Ƿ���ָ�򴰿�
    bool m_bMouseInWindowRect = false;      //����Ƿ��ڵ�ǰ����������
    bool m_bMenuPopedUp = false;
    bool m_bLocked = false;     	//�Ƿ�������ʴ���
    bool m_lyricBackgroundPenetrate = false;

public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseHover(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
protected:
    afx_msg LRESULT OnInitmenu(WPARAM wParam, LPARAM lParam);
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnTimer(UINT_PTR nIDEvent);

    DECLARE_MESSAGE_MAP()
    afx_msg void OnLyricDefaultStyle1();
    afx_msg void OnLyricDefaultStyle2();
    afx_msg void OnLyricDefaultStyle3();
};

