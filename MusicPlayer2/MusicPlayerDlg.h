
// MusicPlayerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
//#include"Player.h"
#include"PlayListCtrl.h"
#include"FindDlg.h"
#include"OptionsDlg.h"
#include"PropertyDlg.h"
#include"ProgressStatic.h"
#include"ColorConvert.h"
//#include<VersionHelpers.h>
#include"MiniModeDlg.h"
#include"LyricEditDlg.h"
#include "LyricDownloadDlg.h"
#include "LyricBatchDownloadDlg.h"
#include "DrawCommon.h"
#include "StaticEx.h"
//#include "EqualizerDlg.h"
#include "SoundEffectDlg.h"
#include "CortanaLyric.h"

// CMusicPlayerDlg �Ի���
class CMusicPlayerDlg : public CDialog
{
// ����
public:
	CMusicPlayerDlg(wstring cmdLine = wstring() ,CWnd* pParent = NULL);	// ��׼���캯��
	~CMusicPlayerDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MUSICPLAYER2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CToolTipCtrl m_Mytip;
	CPlayListCtrl m_playlist_list{ theApp.m_player.GetPlayList() };		//�����б�ؼ�(��ʼ��ʱͨ�����캯�����ݸ�����Ϣ�����ã�
																	//����֧�����ָ���б��е���Ŀʱ��ʾ������Ϣ)
	CStatic m_path_static;
	CEdit m_path_edit;
	CMenu m_list_popup_menu;		//�����б��Ҽ��˵�
	CMenu m_popup_menu;			//����Ҽ��˵�
	CMenu m_main_popup_menu;
	CMFCButton m_play_pause_button;
	CMFCButton m_stop_button;
	CMFCButton m_previous_button;
	CMFCButton m_next_button;
	CProgressStatic m_progress_bar;		//�������ؼ�
	CStaticEx m_time_static;				//��ʾ����ʱ��Ŀؼ�
	CButton m_set_path_button;

	ITaskbarList3* m_pTaskbar;          //����֧����������ʾ���Ž���
	THUMBBUTTON m_thumbButton[3]{};
	//ͼ�꣨��������������ͼ��ť��
	HICON m_hPlayPauseIcon;		//����/��ͣͼ�꣨δʹ�ã�
	HICON m_hPreviousIcon;		//��һ��ͼ��
	HICON m_hNextIcon;			//��һ��ͼ��
	HICON m_hPlayIcon;			//����ͼ��
	HICON m_hPauseIcon;			//��ͣͼ��
	//Сͼ�꣨���������ڰ�ť��
	HICON m_hPreviousIcon_s;		//��һ��ͼ��
	HICON m_hNextIcon_s;			//��һ��ͼ��
	HICON m_hPlayIcon_s;			//����ͼ��
	HICON m_hPauseIcon_s;			//��ͣͼ��
	HICON m_hStopIcon_s;			//ֹͣͼ��

	CFindDlg m_findDlg{ theApp.m_player.GetPlayList() };		//���ҶԻ���

	wstring m_cmdLine;	//�����в���

	CDC* m_pDC;				//��ǰ���ڵ�DC
	CDrawCommon m_draw;		//���ڻ����ı��Ķ���

	bool m_first_start{ true };		//��ʼʱΪtrue���ڶ�ʱ����һ����������Ϊflase

	int sec_current, sec_temp;		//���ڿ��ƽ�����Ϣˢ��Ƶ�ʣ���ǰ��������ÿ��1��ˢ��

	int m_window_width;		//���ڵĿ��
	int m_window_height;	//���ڵĸ߶�

	int m_margin;		//��Ե������
	int m_width_threshold{};		//�������ͨ����ģʽ�л���խ����ģʽʱ�����ȵ���ֵ
	int m_info_height;				//խ����ģʽʱ��ʾ��Ϣ����ĸ߶�
	int m_info_height2;				//��ͨ����ģʽʱ��ʾ��Ϣ����ĸ߶�
	int m_control_bar_height;		//�����Ϸ��Ĳ��ſ��ư�ť���ֵĸ߶�
	int m_path_edit_height;			//��ʾ��ǰ·���ؼ�����ĸ߶�
	int m_progress_bar_height;		//(խ����ģʽʱ)����������ĸ߶�
	int m_progress_bar_left_pos;	//(��ͨ����ģʽʱ)����������λ��
	CSize m_spectral_size{};			//Ƶ�׷�������Ĵ�С
	int m_min_width;			//������С���
	int m_min_height;			//������С�߶�
	int m_time_width;			//��ʾ����ʱ��ؼ��Ŀ��
	int m_time_height;			//��ʾ����ʱ��ؼ��ĸ߶�
	int m_client_width;			//���ڿͻ������
	int m_client_height;		//���ڿͻ����߶�

	int m_lyric_line_space{ 2 };	//��ʵ��м��
	wstring m_lyric_font_name;		//�����������
	int m_lyric_font_size;		//��������С
	CFont m_lyric_font;			//�������

	bool m_narrow_mode;		//խ����ģʽ

	unsigned int m_timer_count{};

	bool m_stop_when_error{ true };		//���ִ���ʱֹͣ����
	bool m_show_taskbar_progress{ false };	//����������ť����ʾ���Ž���

	int m_transparency{ 100 };			//����͸����
	bool m_theme_color_follow_system{ true };		//������ɫ����ϵͳ����Win8����֧�֣�
	bool m_show_album_cover;			//��ʾר������
	CDrawCommon::StretchMode m_album_cover_fit{ CDrawCommon::StretchMode::FILL };		//ר���������϶ȣ�����ģʽ��
	bool m_album_cover_as_background{ false };		//��ר��������Ϊ����
	bool m_show_spectrum{ true };		//��ʾƵ�׷���
	bool m_cortana_show_album_cover{ true };

	CRect m_draw_rect;		//��ͼ����
	CRect m_repetemode_rect;	//��ʾ��ѭ��ģʽ���ľ�������
	bool m_repetemode_hover{ false };	//���ָ���ˡ�ѭ��ģʽ���ľ�������
	CRect m_volume_rect;		//��ʾ�����ľ�������
	bool m_volume_hover{ false };	//���ָ���������ľ�������
	CRect m_volume_up_rect, m_volume_down_rect;	//�������������Ӻͼ��������ľ�������
	bool m_show_volume_adj{ false };	//��ʾ����������ť

	int m_item_selected{};		//�����б������ѡ�е���Ŀ
	int m_tab_selected{};		//ѡ��������ѡ��ı�ǩ

	static unsigned int m_WM_TASKBARCREATED;	//������������Ϣ

	DisplayFormat m_display_format{};		//�����б�����Ŀ����ʾ��ʽ

	CMiniModeDlg m_miniModeDlg{ m_item_selected, m_list_popup_menu };		//����ģʽ�Ի���

	bool m_show_lyric_in_cortana{};	//�Ƿ���Cortana������������ʾ���
	CCortanaLyric m_cortana_lyric;		//������ʾCortana���
	bool m_cortana_lyric_double_line{true};		//�Ƿ���Cortana��������˫����ʾ���

	bool m_save_lyric_in_offset{};	//�Ƿ񽫸�ʱ�����offset��ǩ�У����Ǳ�����ÿ��ʱ���ǩ��
	CLyricEditDlg* m_pLyricEdit;		//��ʱ༭�Ի��򣨷�ģ̬�Ի���
	CLyricBatchDownloadDlg* m_pLyricBatchDownDlg;	//����������ضԻ��򣨷�ģ̬�Ի���

	CWinThread* m_pThread;		//ִ�����߲鿴���߳�
	static UINT ViewOnlineThreadFunc(LPVOID lpParam);	//ִ�����߲鿴���̺߳���

	//˽�еĺ���
	void SaveConfig();		//�������õ�ini�ļ�
	void LoadConfig();		//��ini�ļ���ȡ����
	void SetTransparency();			//����m_transparency��ֵ���ô���͸����
	void DrawInfo(bool reset = false);		//������Ϣ
	void DrawLyricsSingleLine(CRect lyric_rect);			//���Ƹ�ʣ�խ����ģʽ�µ�����ʾ��������Ϊ��ʾ��ʵľ�������
	void DrawLyricsMulityLine(CRect lyric_rect, CDC* pDC);			//���Ƹ�ʣ���ͨģʽ�¶�����ʾ��������Ϊ��ʾ��ʵľ�������
	void SetPlaylistSize(int cx, int cy);		//���ò����б�Ĵ�С
	void SetPorgressBarSize(int cx, int cy);		//���ý������ڴ����е�λ��
	void SetPorgressBarSize();
public:
	void ShowPlayList();
protected:
	void SetPlayListColor();
	void SwitchTrack();		//���л����ڲ��ŵĸ���ʱ�Ĵ���
	void ShowTime();		//��ʾ����ʱ��

	void UpdateProgress();			//���½������Ľ���
	void UpdateTaskBarProgress();	//������������ť�ϵĽ���
	void UpdatePlayPauseButton();		//���ݵ�ǰ����״̬���¡�����/��ͣ����ť�ϵ����ֺ�ͼ��
	void SetThumbnailClipArea();		//��������������ͼ������
	void EnablePlaylist(bool enable);		//�������û���ò����б�ؼ�

	void CreateDesktopShortcut();		//��������ʾ�û����������ݷ�ʽ

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPlayPause();
	afx_msg void OnStop();
	afx_msg void OnPrevious();
	afx_msg void OnNext();
	afx_msg void OnRew();
	afx_msg void OnFF();
	afx_msg void OnSetPath();
	afx_msg void OnFind();
	afx_msg void OnExplorePath();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnFileOpenFolder();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnInitMenu(CMenu* pMenu);
	afx_msg void OnPlayOrder();
	afx_msg void OnPlayShuffle();
	afx_msg void OnLoopPlaylist();
	afx_msg void OnLoopTrack();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//afx_msg void OnBnClickedVolumeUp();
	//afx_msg void OnBnClickedVolumeDown();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnNMDblclkPlaylistList(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnRefreshPlaylist();
	afx_msg void OnOptionSettings();
	afx_msg void OnReloadPlaylist();
	afx_msg void OnNMRClickPlaylistList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPlayItem();
	afx_msg void OnItemProperty();
	afx_msg void OnRemoveFromPlaylist();
//	afx_msg void OnClearPlaylist();
	afx_msg void OnExploreTrack();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnStnClickedProgressStatic();
	afx_msg void OnReIniBass();
	afx_msg void OnSortByFile();
	afx_msg void OnSortByTitle();
	afx_msg void OnSortByArtist();
	afx_msg void OnSortByAlbum();
	afx_msg void OnSortByTrack();
public:
	afx_msg void OnDeleteFromDisk();
protected:
	afx_msg LRESULT OnTaskbarcreated(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDispFileName();
	afx_msg void OnDispTitle();
	afx_msg void OnDispArtistTitle();
	afx_msg void OnDispTitleArtist();
	afx_msg void OnMiniMode();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedPrevious();
	afx_msg void OnBnClickedPlayPause();
	afx_msg void OnBnClickedNext();
//	afx_msg void OnMove(int x, int y);
	afx_msg void OnReloadLyric();
	afx_msg void OnSongInfo();
	afx_msg void OnCopyCurrentLyric();
	afx_msg void OnCopyAllLyric();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLyricForward();
	afx_msg void OnLyricDelay();
	afx_msg void OnSaveModifiedLyric();
	afx_msg void OnEditLyric();
	afx_msg void OnDownloadLyric();
	afx_msg void OnLyricBatchDownload();
	afx_msg void OnDeleteLyric();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	afx_msg LRESULT OnPlaylistIniComplate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetTitle(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnEqualizer();
	afx_msg void OnExploreOnline();
protected:
	afx_msg LRESULT OnPlaylistIniStart(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBrowseLyric();
	afx_msg void OnTranslateToSimplifiedChinese();
	afx_msg void OnTranslateToTranditionalChinese();
};
