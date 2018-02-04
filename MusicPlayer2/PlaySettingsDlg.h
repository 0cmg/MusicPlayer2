#pragma once
#include "afxwin.h"


// CPlaySettingsDlg �Ի���

class CPlaySettingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPlaySettingsDlg)

public:
	CPlaySettingsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPlaySettingsDlg();

	//ѡ�����õ����ݣ�����ͬCMusicPlayerDlg�������ͬ������
	bool m_stop_when_error;
	bool m_karaoke_disp;
	bool m_show_taskbar_progress;
	bool m_lyric_fuzzy_match;
	wstring m_lyric_path;
	bool m_show_lyric_in_cortana;
	bool m_save_lyric_in_offset;
	bool m_lyric_double_line;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLAY_SETTING_DIALOG };
#endif

protected:
	CButton m_stop_when_error_check;
	CButton m_karaoke_disp_check;
	CButton m_show_taskbar_progress_check;
	CButton m_lyric_fuzzy_match_check;
	CButton m_lyric_double_line_chk;

	CToolTipCtrl m_tool_tip;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

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
	afx_msg void OnBnClickedSaveInOffsetTag();
	afx_msg void OnBnClickedSaveInTimeTag();
	afx_msg void OnBnClickedLyricDoubleLineCheck();
};
