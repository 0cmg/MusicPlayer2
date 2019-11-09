// ========================================================
// �ļ�:LyricsWindow.h
// ����:CLyricsWindow
// ����:�����ʾ����
// ����:ʹ��GDIPlus���Ƹ��
// ����:��ѧ��(�ᴳ����)
// ========================================================
// �ӿ�˵��
// Create				������ʴ��� 
// UpdateLyrics			���¸��(����ı�,�������Ȱٷֱ�)
// Draw					�ػ���ʴ���
// SetLyricsColor		���ø����ɫ
// SetLyricsBorder		���ø�ʱ߿�
// SetHighlightColor	���ø��������ɫ
// SetHighlightBorder	���ø�����ʱ߿�
// SetLyricsShadow		���ø����Ӱ
// SetLyricsFont		���ø������
// ========================================================

/*
˵���������ԭ���ߣ���ѧ�򣬵�ַ��https://blog.csdn.net/CometNet/article/details/37508613
��ԭ�еĻ��������޸�
*/

#pragma once

#include <gdiplus.h>
#pragma comment(lib,"GdiPlus.lib")
#include "DrawCommon.h"
#include "CommonData.h"

//��ʽ���ģʽ
enum LyricsGradientMode
{
	LyricsGradientMode_None		=0, //�޽���
	LyricsGradientMode_Two		=1, //��ɫ����
	LyricsGradientMode_Three	=2 //��ɫ����
};

// CLyricsWindow

class CLyricsWindow : public CWnd
{
	DECLARE_DYNAMIC(CLyricsWindow)

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
        BTN_SETTING,
        BTN_CLOSE
    };

public:
	CLyricsWindow();
	virtual ~CLyricsWindow();
public:
	//�������� 
	BOOL Create(int nHeight = -1);
	BOOL Create(LPCTSTR lpszClassName);
	BOOL Create(LPCTSTR lpszClassName,int nWidth,int nHeight);
public:
	//���¸��(����ı�,�������Ȱٷֱ�)
	void UpdateLyrics(LPCTSTR lpszLyrics,int nHighlight);
	//���¸�������(�������Ȱٷֱ�)
	void UpdateLyrics(int nHighlight);
	//���¸�ʷ����ı�
	void UpdateLyricTranslate(LPCTSTR lpszLyricTranslate);
	//�ػ���ʴ���
	void Draw();
	//���ø����ɫ
	void SetLyricsColor(Gdiplus::Color TextColor1);
	void SetLyricsColor(Gdiplus::Color TextColor1,Gdiplus::Color TextColor2,LyricsGradientMode TextGradientMode);
	//���ø�ʱ߿�
	void SetLyricsBorder(Gdiplus::Color BorderColor, Gdiplus::REAL BorderWidth = 1.0f);
	//���ø��������ɫ
	void SetHighlightColor(Gdiplus::Color TextColor1);
	void SetHighlightColor(Gdiplus::Color TextColor1,Gdiplus::Color TextColor2,LyricsGradientMode TextGradientMode);
	//���ø�����ʱ߿�
	void SetHighlightBorder(Gdiplus::Color BorderColor, Gdiplus::REAL BorderWidth = 1.0f);
	//���ø����Ӱ
	void SetLyricsShadow(Gdiplus::Color ShadowColor,int nShadowOffset=2);
	//���ø������
	void SetLyricsFont(const WCHAR * familyName, Gdiplus::REAL emSize,INT style= Gdiplus::FontStyleRegular, Gdiplus::Unit unit= Gdiplus::UnitPixel);
	//�����Ƿ�˫����ʾ
	void SetLyricDoubleLine(bool doubleLine);
	void SetNextLyric(LPCTSTR lpszNextLyric);
	void SetDrawBackground(bool drawBackground);
    void SetShowTranslate(bool showTranslate);
    //���ò�͸����
    void SetAlpha(int alpha);
    //��ȡ��ǰ����ı�
    const CString& GetLyricStr() const;
    void SetLyricChangeFlag(bool bFlag);
private:
	void DrawLyricText(Gdiplus::Graphics* pGraphics, LPCTSTR strText, Gdiplus::RectF rect, bool bDrawHighlight, bool bDrawTranslate = false);
	//���Ƹ��
	void DrawLyrics(Gdiplus::Graphics* pGraphics);
    void DrawLyricsDoubleLine(Gdiplus::Graphics* pGraphics);
	//���Ƹ������
	void DrawHighlightLyrics(Gdiplus::Graphics* pGraphics,Gdiplus::GraphicsPath* pPath, Gdiplus::RectF& dstRect);

    void DrawToolbar(Gdiplus::Graphics* pGraphics);
    void DrawToolIcon(Gdiplus::Graphics* pGraphics, IconRes icon, CRect rect, BtnKey btn, bool checked = false);
	//�������仭ˢ
	Gdiplus::Brush* CreateGradientBrush(LyricsGradientMode TextGradientMode,Gdiplus::Color& Color1,Gdiplus::Color& Color2, Gdiplus::RectF& dstRect);
	//ע�ᴰ����
	BOOL RegisterWndClass(LPCTSTR lpszClassName);

    void AddToolTips();
    void AddMouseToolTip(BtnKey btn, LPCTSTR str);		//Ϊһ����ť��������ʾ

protected:
	DECLARE_MESSAGE_MAP()
private:
	HDC m_hCacheDC;//����DC
	int m_nWidth;
	int m_nHeight;
	CString m_lpszLyrics;//Unicode��ʽ�ĸ��
	int m_nHighlight;//������ʵİٷֱ� 0--1000
	Gdiplus::Color m_TextColor1;//��ͨ�����ɫ,ARGB��ɫ
	Gdiplus::Color m_TextColor2;//��ͨ�����ɫ,ARGB��ɫ
	LyricsGradientMode m_TextGradientMode;//��ͨ��ʽ���ģʽ
	Gdiplus::Pen* m_pTextPen;//��ͨ��ʱ߿򻭱�
	Gdiplus::Color m_HighlightColor1;//���������ɫ,ARGB��ɫ
	Gdiplus::Color m_HighlightColor2;//���������ɫ,ARGB��ɫ
	LyricsGradientMode m_HighlightGradientMode;//������ʽ���ģʽ
	Gdiplus::Pen*m_pHighlightPen;//������ʱ߿򻭱�
	Gdiplus::Brush* m_pShadowBrush;//��Ӱ��ˢ,GDIPlus��ˢ 
	int m_nShadowOffset;//��Ӱƫ��
	Gdiplus::Font* m_pFont;//GDIPlus����
	int m_FontStyle;
	Gdiplus::REAL m_FontSize;
	Gdiplus::FontFamily* m_pFontFamily;
	Gdiplus::StringFormat* m_pTextFormat;
	bool m_bDoubleLine = false;		//���˫����ʾ
    bool m_bShowTranslate = false;  //��ʾ��ʷ���
	CString m_strTranslate;			//��ʷ���
	CString m_strNextLyric;			//��һ����
	bool m_bDrawBackground = false;	//�Ƿ����һ����͸���İ�ɫ����
    int m_alpha = 255;                    //��͸����
    bool m_bHover;
    bool m_lyricChangeFlag = false; //��ʷ����ı��־

    CMenu m_popupMenu;
    CToolTipCtrl m_tool_tip;
    std::map<BtnKey, UIButton> m_buttons;
    CSize m_frameSize{};
    bool m_first_draw = false;      //��һ�λ��ƹ�����ʱ����Ϊtrue

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
};


