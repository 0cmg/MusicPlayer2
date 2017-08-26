#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CEqualizerDlg �Ի���

class CEqualizerDlg : public CDialog
{
	DECLARE_DYNAMIC(CEqualizerDlg)

public:
	CEqualizerDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEqualizerDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EQUALIZER_DIALOG };
#endif

protected:
	int m_user_defined_gain[FX_CH_NUM]{};		//�û��Զ���ľ���������

	CSliderCtrl m_sliders[FX_CH_NUM];
	CListBox m_equ_style_list;
	CButton m_enable_equ_check;
	CToolTipCtrl m_Mytip;		//�����ʾ
	int m_equ_style_selected{};		//ѡ�еľ�����Ԥ��

	void EnableControls(bool enable);		//���û���ÿؼ�
	void SaveConfig() const;
	void LoadConfig();
	void UpdateChannelTip(int channel, int gain);		//����ͨ���������������ʾ

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedEnableEquCheck();
	afx_msg void OnLbnSelchangeEquStylesList();
	afx_msg void OnDestroy();
};
