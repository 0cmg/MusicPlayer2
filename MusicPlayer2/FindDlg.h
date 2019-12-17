#pragma once
#include "afxcmn.h"
#include "AudioCommon.h"
#include "afxwin.h"
#include "ListCtrlEx.h"
#include "Common.h"


// CFindDlg �Ի���

class CFindDlg : public CDialog
{
	DECLARE_DYNAMIC(CFindDlg)

public:
	CFindDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFindDlg();

	//wstring m_config_path;

	//int GetSelectedTrack() const;
	bool IsFindCurrentPlaylist() const;
	void GetSongsSelected(vector<wstring>& files) const;
	void GetSongsSelected(vector<SongInfo>& files) const;
	void SaveConfig();
	void LoadConfig();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FIND_DIALOG };
#endif

protected:
    enum ColumeIndex
    {
        COL_INDEX = 0,
        COL_FILE_NAME,
        COL_TITLE,
        COL_ARTIST,
        COL_ALBUM,
        COL_PATH,
    };

	//const vector<SongInfo>& m_playlist;		//�����б�
	//vector<int> m_find_result;			//���浱ǰ�����б�Ĳ��ҽ������Ŀ��ţ�
	wstring m_key_word;				//���ҵ��ַ���
	vector<SongInfo> m_find_result;		//�������в����б�Ĳ��ҽ��

	CListCtrlEx m_find_result_list;		//���ҽ���ؼ�
	int m_item_selected{ -1 };		//���ѡ�е���Ŀ���
    vector<int> m_items_selected;
	//CMenu m_menu;

	//int m_dpi;

	CButton m_find_file_check;
	CButton m_find_title_check;
	CButton m_find_artist_check;
	CButton m_find_album_check;

	bool m_find_current_playlist{ true };		//������ҷ�ΧΪ��ǰ�����б���Ϊtrue����������в����б���Ϊfalse
    bool m_result_in_current_playlist{ true };  //���ҽ���Ƿ�Ϊ��ǰ�����б�
	bool m_find_file{ true };
	bool m_find_title{ true };
	bool m_find_artist{ true };
	bool m_find_album{ true };

	int m_find_option_data{};		//�������ѡ������ݣ���ÿһ��bitλ��ʾÿ������ѡ���Ƿ�ѡ��

	int m_min_width{};
	int m_min_height{};

	CString m_selected_string;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void ShowFindResult();
	void ShowFindInfo();
    bool _OnAddToNewPlaylist(std::wstring& playlist_path);       //ִ����ӵ��½������б�����ɹ�����true��playlist_path���ڽ����²����б��·��
    static UINT ViewOnlineThreadFunc(LPVOID lpParam);	//ִ�����߲鿴���̺߳���
    void GetCurrentSongList(std::vector<SongInfo>& song_list);

public: 
	void ClearFindResult();

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
	afx_msg void OnBnClickedFindCurrentPlaylistRadio();
	afx_msg void OnBnClickedFindAllPlaylistRadio();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPlayItem();
	afx_msg void OnExploreTrack();
	afx_msg void OnNMRClickFindList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCopyText();
    afx_msg void OnPlayItemInFolderMode();
    afx_msg void OnAddToNewPlaylist();
    afx_msg void OnExploreOnline();
    afx_msg void OnFormatConvert();
    afx_msg void OnItemProperty();
    virtual void OnOK();
    afx_msg void OnAddToNewPalylistAndPlay();
private:
public:
    afx_msg void OnInitMenu(CMenu* pMenu);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
