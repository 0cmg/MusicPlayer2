#pragma once
#include "afxwin.h"

// CHelpDlg �Ի���

class CHelpDlg : public CDialog
{
	DECLARE_DYNAMIC(CHelpDlg)

public:
	CHelpDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHelpDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HELP_DIALOG };
#endif

protected:
	CEdit m_help_edit;
	CSize m_min_size;		//���ڵ���С��С

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	static CString GetHelpString();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};
