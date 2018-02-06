#pragma once
#include"Player.h"
#include"StaticEx.h"
#include "afxwin.h"
#include "PropertyDlg.h"
#include "afxcmn.h"
#include "PlayListCtrl.h"
#include "ColorConvert.h"
#include"ProgressStatic.h"
#include "DrawCommon.h"

// CMiniModeDlg �Ի���

class CMiniModeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMiniModeDlg)

public:
	CMiniModeDlg(int& item_selected, CMenu& popup_menu, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMiniModeDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MINI_DIALOG };
#endif

	DisplayFormat m_display_format{};		//�����б�����Ŀ����ʾ��ʽ

	void UpdatePlayPauseButton();
	void ShowPlaylist();
	void SetPlayListColor();
	void RePaint();			//���»��ƽ����Ԫ��

protected:
	//CPlayer& m_player;

	//int m_dpi{ 96 };
	HICON m_hPreviousIcon_s;
	HICON m_hNextIcon_s;
	HICON m_hPlayIcon_s;
	HICON m_hPauseIcon_s;
	HBITMAP m_back_img;

	ColorTable m_theme_color{};

	int m_position_x;
	int m_position_y;

	int m_screen_width;
	int m_screen_height;

	CRect m_rect;		//��ʾ�����б�ʱ�Ľ����С
	CRect m_rect_s;		//����ʾ�����б�ʱ�Ľ����С

	bool m_show_playlist{ false };		//�Ƿ���ʾ�����б�
	bool m_dark_mode{ false };
	bool m_follow_main_color{ true };
	bool m_first_start{ true };

	int& m_item_selected;		//�����б������ѡ�е���Ŀ������MusicPlayerDlg���е�ͬ�������������㴰���в����б�ѡ�е���Ŀ�仯ʱ��ͬ������������ѡ�е���Ŀ

	CToolTipCtrl m_Mytip;
	CMenu m_menu;
	CMenu& m_popup_menu;		//�����б��Ҽ��˵�

	CString m_song_tip_info;

	//
#define ROW 4				//Ƶ�����ε�����
	CRect m_spectral_client_rect;		//Ƶ�׵ľ�������
	CRect m_spectral_rect[ROW];		//ÿ�����εľ�������
	CDC* m_spectrum_pDC;		//Ƶ�׷�������static�ؼ���CDC
	CDrawCommon m_draw;
	CRect m_album_rect;			//ר������ľ�������

	CStaticEx m_lyric_static;
	CStaticEx m_time_static;
	CMFCButton m_previous_button;
	CMFCButton m_play_pause_button;
	CMFCButton m_next_button;
	CMFCButton m_show_list_button;
	CMFCButton m_return_button;
	CMFCButton m_exit_button;
	CStatic m_spectral_static;
	CPlayListCtrl m_playlist_ctrl{ theApp.m_player.GetPlayList() };
public:
	CProgressStatic m_progress_bar;

protected:
	int m_last_lyric_index{};		//��һ���ʵ���ţ������жϸ���Ƿ�ı�
	int m_last_index{};			//��һ��������Ŀ����ţ������жϲ�����Ŀ�Ƿ�ı�
	bool m_show_volume{ false };	//����ָʾ�Ƿ�����ʾʱ��Ŀؼ���ʾ������������������ʱ��1.5���ڣ��˱�����ֵΪtrue


	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	void SaveConfig() const;
	void LoadConfig();

	void CheckWindowPos();
	void UpdateSongTipInfo();

	void DrawSpectral();		//����Ƶ�׷���
	void DrawBorder();		//���Ʊ߿�
	void ShowVolume(int volume);
	void ShowInfo(bool force_refresh);

	void ColorChanged();		//��������ɫ�ı�ʱ����
	void SetVolume(bool up);	//

	DECLARE_MESSAGE_MAP()


protected:

	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMiniModeExit();
//	afx_msg void OnSongInfo();
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnSetTextColor();
	afx_msg void OnBnClickedMPlayPause();
	afx_msg void OnBnClickedMPrevious();
	afx_msg void OnBnClickedMNext();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedShowListButton();
	afx_msg void OnPaint();
	afx_msg void OnDarkMode();
	afx_msg void OnFollowMainColor();
public:
	afx_msg void OnStnClickedMiniProgressStatic();
};
