#pragma once
#include "afxwin.h"
#include "FolderBrowserDlg.h"

// CLyricSettingsDlg �Ի���

class CLyricSettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLyricSettingsDlg)

public:
	CLyricSettingsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLyricSettingsDlg();

	//ѡ�����õ�����
	LyricSettingData m_data;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LYRIC_SETTING_DIALOG };
#endif

protected:
	CButton m_karaoke_disp_check;
	CButton m_lyric_fuzzy_match_check;
	CButton m_lyric_double_line_chk;
	CButton m_show_album_cover_in_cortana_check;
	CButton m_cortana_icon_beat_check;
	CComboBox m_cortana_color_combo;

	CToolTipCtrl m_tool_tip;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	void SetCortanaControlEnable(bool enable);		//���û����Cortana�����ʾ��صĿؼ�

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStopWhenError();
	afx_msg void OnBnClickedKaraokeDisp();
	afx_msg void OnBnClickedExploreLyricButton();
	afx_msg void OnBnClickedShowTaskbarProgress();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBnClickedLyricFuzzyMatch();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_show_lyric_in_cortana_check;
	afx_msg void OnBnClickedShowLyricInCortana();
	//afx_msg void OnBnClickedSaveInOffsetTag();
	//afx_msg void OnBnClickedSaveInTimeTag();
	afx_msg void OnBnClickedLyricDoubleLineCheck();
	afx_msg void OnCbnSelchangeCortanaColorCombo();
	afx_msg void OnBnClickedShowAlbumCoverInCortana();
	afx_msg void OnBnClickedCortanaIconDeatCheck();
};
