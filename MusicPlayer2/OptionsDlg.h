#pragma once
#include "afxcmn.h"
#include "PlaySettingsDlg.h"
#include "AppearanceSettingDlg.h"
#include "DataSettingsDlg.h"

// COptionsDlg �Ի���
#define TIMER_ID2 1122

class COptionsDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionsDlg)

public:
	COptionsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COptionsDlg();

	CPlaySettingsDlg m_tab1_dlg;	//������ѡ��ӶԻ���
	CAppearanceSettingDlg m_tab2_dlg;	//��������á��ӶԻ���
	CDataSettingsDlg m_tab3_dlg;		//�������ļ����ӶԻ���

	int m_tab_selected{};

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPTIONS_DIALOG };
#endif

protected:

	CTabCtrl m_tab;		//ѡ��ؼ�

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeOptionsTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
