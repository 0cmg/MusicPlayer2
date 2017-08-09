#pragma once
#include "afxcmn.h"
#include "AudioCommon.h"
#include "afxwin.h"
//#include "ListCtrlEx.h"
#include "Common.h"


// CFindDlg �Ի���

class CFindDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFindDlg)

public:
	CFindDlg(const vector<SongInfo>& playlist, vector<int>& find_result, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFindDlg();

	//wstring m_config_path;

	int GetSelectedTrack() const;
	void SaveConfig();
	void LoadConfig();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIND_DIALOG };
#endif

protected:

	const vector<SongInfo>& m_playlist;		//�����б�
	vector<int>& m_find_result;			//����Ĳ��ҽ������Ŀ��ţ�
	wstring m_key_word;				//���ҵ��ַ���

	CListCtrl m_find_result_list;		//���ҽ���ؼ�
	int m_item_selected{ -1 };		//���ѡ�е���Ŀ���

	//int m_dpi;

	CButton m_find_file_check;
	CButton m_find_title_check;
	CButton m_find_artist_check;
	CButton m_find_album_check;

	bool m_find_file{ true };
	bool m_find_title{ true };
	bool m_find_artist{ true };
	bool m_find_album{ true };

	int m_find_option_data{};		//�������ѡ������ݣ���ÿһ��bitλ��ʾÿ������ѡ���Ƿ�ѡ��

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void ShowFindResult();

	DECLARE_MESSAGE_MAP()

	afx_msg void OnEnChangeFindEdit();
	afx_msg void OnNMClickFindList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedFindButton();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkFindList(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg void OnBnClickedFindFileCheck();
	afx_msg void OnBnClickedFindTitleCheck();
	afx_msg void OnBnClickedFindArtistCheck();
	afx_msg void OnBnClickedFindAlbumCheck();
};
