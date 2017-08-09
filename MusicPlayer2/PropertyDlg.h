#pragma once
#include "Time.h"
#include "Common.h"
#include "Player.h"
#include "EditEx.h"
#include "afxwin.h"

// CPropertyDlg �Ի���

class CPropertyDlg : public CDialog
{
	DECLARE_DYNAMIC(CPropertyDlg)

public:
	CPropertyDlg(const vector<SongInfo>& all_song_info, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPropertyDlg();

	wstring m_path;
	int m_index{};		//��ǰ��ʾ��Ŀ����Ŀ���
	//int m_playing_index{};	//���ڲ��ŵ���Ŀ���
	int m_song_num;
	//wstring m_lyric_name;

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROPERTY_DIALOG };
#endif

protected:
	//const vector<wstring>& m_all_file_name;
	const vector<SongInfo>& m_all_song_info;

	//�ؼ�����
	CEditEx m_file_name_edit;
	CEditEx m_file_path_edit;
	CEditEx m_file_type_edit;
	CEditEx m_song_length_edit;
	CEditEx m_file_size_edit;
	CEditEx m_bit_rate_edit;
	CEditEx m_title_edit;
	CEditEx m_artist_edit;
	CEditEx m_album_edit;
	CEditEx m_track_edit;
	CEditEx m_year_edit;
	CEditEx m_genre_edit;
	CEditEx m_comment_edit;
	CEditEx m_lyric_file_edit;

	void ShowInfo();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedPreviousButton();
	afx_msg void OnBnClickedNextButton();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
