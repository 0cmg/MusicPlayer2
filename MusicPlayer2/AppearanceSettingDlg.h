#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Common.h"

// CAppearanceSettingDlg �Ի���

class CAppearanceSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAppearanceSettingDlg)

public:
	wstring m_font;
	int m_font_size;
	int m_line_space;
	int m_transparency;
	HWND m_hMainWnd;		//�����ڵľ��������ʵʱ���Ĵ��ڲ�͸����
	COLORREF m_theme_color;
	bool m_theme_color_follow_system;

	CAppearanceSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAppearanceSettingDlg();

	void DrawColor();

#define MAX_LINE_SPACE 40		//����м���趨�����ֵ

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_APPEREANCE_SETTING_DLG };
#endif

protected:
	bool m_font_changed{ false };
	CSliderCtrl m_transparency_slid;
	CStatic m_color_static;
	CStatic m_color_static1;
	CStatic m_color_static2;
	CStatic m_color_static3;
	CStatic m_color_static4;
	CStatic m_color_static5;
	CStatic m_color_static6;

	const COLORREF m_color1{ RGB(123,189,255) };		//����ɫ
	const COLORREF m_color2{ RGB(115,210,45) };			//��ɫ
	const COLORREF m_color3{ RGB(255,167,87) };			//��ɫ
	const COLORREF m_color4{ RGB(33,147,167) };			//����ɫ
	const COLORREF m_color5{ RGB(255,162,208) };		//ǳ��ɫ
	const COLORREF m_color6{ RGB(168,152,222) };		//����ɫ

	CToolTipCtrl m_toolTip;

	CButton m_follow_system_color_check;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	void SetTransparency();
	void ClickColor();

	DECLARE_MESSAGE_MAP()
public:
	bool FontChanged()const { return m_font_changed; }

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSetFontButton();
//	afx_msg void OnNMReleasedcaptureTransparentSlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedSetThemeButton();
	afx_msg void OnStnClickedColorStatic2();
	afx_msg void OnStnClickedColorStatic3();
	afx_msg void OnStnClickedColorStatic4();
	afx_msg void OnStnClickedColorStatic5();
	afx_msg void OnStnClickedColorStatic6();
	afx_msg void OnStnClickedColorStatic7();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedFollowSystemColorCheck();
	virtual void OnCancel();
	virtual void OnOK();
//	afx_msg void OnEnChangeFontNameEdit();
	afx_msg void OnEnChangeLineSpaceEdit();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
