#pragma once
#include"Player.h"

// CMaskDlg �Ի���

class CMaskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMaskDlg)

public:
	CMaskDlg(/*CPlayer& player, */CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMaskDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MASK_DIALOG };
#endif

protected:
	//CPlayer& m_player;
	CMenu m_menu;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
//	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
