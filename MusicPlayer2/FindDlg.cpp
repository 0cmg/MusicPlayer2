// FindDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "FindDlg.h"
#include "afxdialogex.h"
#include "MusicPlayerCmdHelper.h"
#include "PropertyDlg.h"
#include "Playlist.h"
#include "AddToPlaylistDlg.h"


// CFindDlg �Ի���

IMPLEMENT_DYNAMIC(CFindDlg, CDialog)

CFindDlg::CFindDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_FIND_DIALOG, pParent)
{

}

CFindDlg::~CFindDlg()
{
}

void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FIND_LIST, m_find_result_list);
	DDX_Control(pDX, IDC_FIND_FILE_CHECK, m_find_file_check);
	DDX_Control(pDX, IDC_FIND_TITLE_CHECK, m_find_title_check);
	DDX_Control(pDX, IDC_FIND_ARTIST_CHECK, m_find_artist_check);
	DDX_Control(pDX, IDC_FIND_ALBUM_CHECK, m_find_album_check);
}

void CFindDlg::ShowFindResult()
{
	m_find_result_list.DeleteAllItems();
	CString str;
    for (size_t i{}; i < m_find_result.size(); i++)
    {
        str.Format(_T("%u"), i + 1);
        m_find_result_list.InsertItem(i, str);
        m_find_result_list.SetItemText(i, COL_FILE_NAME, m_find_result[i].GetFileName().c_str());
        m_find_result_list.SetItemText(i, COL_TITLE, m_find_result[i].GetTitle().c_str());
        m_find_result_list.SetItemText(i, COL_ARTIST, m_find_result[i].GetArtist().c_str());
        m_find_result_list.SetItemText(i, COL_ALBUM, m_find_result[i].GetAlbum().c_str());
        m_find_result_list.SetItemText(i, COL_PATH, m_find_result[i].file_path.c_str());
    }
}

void CFindDlg::ShowFindInfo()
{
	CString str;
	int result_mun;
	if (m_find_current_playlist)
		result_mun = m_find_result.size();
	else
		result_mun = m_find_result.size();
	str = CCommon::LoadTextFormat(IDS_FIND_DLG_INFO, { m_key_word, result_mun });
	SetDlgItemText(IDC_FIND_INFO_STATIC, str);
}

bool CFindDlg::_OnAddToNewPlaylist(std::wstring& playlist_path)
{
    auto getSongList = [&](std::vector<SongInfo>& song_list)
    {
        GetSongsSelected(song_list);
    };
    CMusicPlayerCmdHelper cmd_helper(this);
    return cmd_helper.OnAddToNewPlaylist(getSongList, playlist_path);
}

UINT CFindDlg::ViewOnlineThreadFunc(LPVOID lpParam)
{
    CFindDlg* pThis = (CFindDlg*)(lpParam);
    if (pThis == nullptr)
        return 0;
    CCommon::SetThreadLanguage(theApp.m_general_setting_data.language);
    //������������ת��������Ӧ�����������ֵ�����ҳ��
    if (pThis->m_item_selected >= 0)
    {
        wstring file_path = pThis->m_find_result_list.GetItemText(pThis->m_item_selected, COL_PATH).GetString();
        if (CCommon::FileExist(file_path))
        {
            SongInfo song{ theApp.m_song_data[file_path] };
            song.file_path = file_path;
            CMusicPlayerCmdHelper cmd_helper(pThis);
            cmd_helper.VeiwOnline(song);
        }
    }
    return 0;
}

void CFindDlg::ClearFindResult()
{
	m_find_result.clear();
}

void CFindDlg::SaveConfig()
{
	m_find_option_data = 0;
	if (m_find_file)
		m_find_option_data |= 0x01;
	if (m_find_title)
		m_find_option_data |= 0x02;
	if (m_find_artist)
		m_find_option_data |= 0x04;
	if (m_find_album)
		m_find_option_data |= 0x08;
	if(m_find_current_playlist)
		m_find_option_data |= 0x10;

	CIniHelper ini(theApp.m_config_path);
	ini.WriteInt(L"config", L"find_option_data", m_find_option_data);
	ini.Save();
}

void CFindDlg::LoadConfig()
{
	CIniHelper ini(theApp.m_config_path);
	m_find_option_data = ini.GetInt(L"config", L"find_option_data", 0xff);

	m_find_file = (m_find_option_data % 2 != 0);
	m_find_title = ((m_find_option_data >> 1) % 2 != 0);
	m_find_artist = ((m_find_option_data >> 2) % 2 != 0);
	m_find_album = ((m_find_option_data >> 3) % 2 != 0);
	m_find_current_playlist = ((m_find_option_data >> 4) % 2 != 0);
}

bool CFindDlg::IsFindCurrentPlaylist() const
{
    return m_result_in_current_playlist;
}


void CFindDlg::GetSongsSelected(vector<wstring>& files) const
{
    files.clear();
    for (int index : m_items_selected)
    {
        if (index >= 0 && index < m_find_result_list.GetItemCount())
        {
            wstring file_path = m_find_result_list.GetItemText(index, COL_PATH).GetString();
            files.push_back(file_path);
        }
    }
}

void CFindDlg::GetSongsSelected(vector<SongInfo>& songs) const
{
    songs.clear();
    vector<wstring> files;
    GetSongsSelected(files);
    for (const auto& file : files)
    {
        SongInfo song;
        auto iter = theApp.m_song_data.find(file);
        if (iter != theApp.m_song_data.end())
        {
            song = iter->second;
        }
        song.file_path = file;
        songs.push_back(song);
    }
}

void CFindDlg::GetCurrentSongList(std::vector<SongInfo>& song_list)
{
    for (int index = 0; index < m_find_result_list.GetItemCount(); index++)
    {
        std::wstring file = m_find_result_list.GetItemText(index, COL_PATH);
        SongInfo song;
        auto iter = theApp.m_song_data.find(file);
        if (iter != theApp.m_song_data.end())
        {
            song = iter->second;
        }
        song.file_path = file;
        song_list.push_back(song);
    }
}

BEGIN_MESSAGE_MAP(CFindDlg, CDialog)
	ON_EN_CHANGE(IDC_FIND_EDIT, &CFindDlg::OnEnChangeFindEdit)
	ON_NOTIFY(NM_CLICK, IDC_FIND_LIST, &CFindDlg::OnNMClickFindList)
	ON_BN_CLICKED(IDC_FIND_BUTTON, &CFindDlg::OnBnClickedFindButton)
	ON_NOTIFY(NM_DBLCLK, IDC_FIND_LIST, &CFindDlg::OnNMDblclkFindList)
	ON_BN_CLICKED(IDC_FIND_FILE_CHECK, &CFindDlg::OnBnClickedFindFileCheck)
	ON_BN_CLICKED(IDC_FIND_TITLE_CHECK, &CFindDlg::OnBnClickedFindTitleCheck)
	ON_BN_CLICKED(IDC_FIND_ARTIST_CHECK, &CFindDlg::OnBnClickedFindArtistCheck)
	ON_BN_CLICKED(IDC_FIND_ALBUM_CHECK, &CFindDlg::OnBnClickedFindAlbumCheck)
	ON_BN_CLICKED(IDC_FIND_CURRENT_PLAYLIST_RADIO, &CFindDlg::OnBnClickedFindCurrentPlaylistRadio)
	ON_BN_CLICKED(IDC_FIND_ALL_PLAYLIST_RADIO, &CFindDlg::OnBnClickedFindAllPlaylistRadio)
	ON_WM_GETMINMAXINFO()
	ON_WM_SIZE()
	ON_COMMAND(ID_PLAY_ITEM, &CFindDlg::OnPlayItem)
	ON_COMMAND(ID_EXPLORE_TRACK, &CFindDlg::OnExploreTrack)
	ON_NOTIFY(NM_RCLICK, IDC_FIND_LIST, &CFindDlg::OnNMRClickFindList)
	ON_COMMAND(ID_COPY_TEXT, &CFindDlg::OnCopyText)
    ON_COMMAND(ID_PLAY_ITEM_IN_FOLDER_MODE, &CFindDlg::OnPlayItemInFolderMode)
    ON_COMMAND(ID_ADD_TO_NEW_PLAYLIST, &CFindDlg::OnAddToNewPlaylist)
    ON_COMMAND(ID_EXPLORE_ONLINE, &CFindDlg::OnExploreOnline)
    ON_COMMAND(ID_FORMAT_CONVERT, &CFindDlg::OnFormatConvert)
    ON_COMMAND(ID_ITEM_PROPERTY, &CFindDlg::OnItemProperty)
    ON_COMMAND(ID_ADD_TO_NEW_PALYLIST_AND_PLAY, &CFindDlg::OnAddToNewPalylistAndPlay)
    ON_WM_INITMENU()
END_MESSAGE_MAP()


// CFindDlg ��Ϣ�������

void CFindDlg::OnEnChangeFindEdit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString find_string;
	GetDlgItemText(IDC_FIND_EDIT, find_string);
	m_key_word = find_string;
}


void CFindDlg::OnNMClickFindList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_item_selected = pNMItemActivate->iItem;		//�������ҽ���б�ʱ����ѡ�е���Ŀ���
    m_find_result_list.GetItemSelected(m_items_selected);
	GetDlgItem(IDOK)->EnableWindow(m_item_selected != -1);
	*pResult = 0;
}


void CFindDlg::OnBnClickedFindButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_key_word.empty())
	{
		if (m_find_current_playlist)		//���ҵ�ǰ�����б�ʱ����m_playlist�����в���
		{
			m_find_result.clear();
			int index;
			bool find_flag;
			for (size_t i{ 0 }; i < CPlayer::GetInstance().GetSongNum(); i++)
			{
				find_flag = false;
				if (m_find_file && !find_flag)
				{
					index = CCommon::StringFindNoCase(CPlayer::GetInstance().GetPlayList()[i].GetFileName(), m_key_word);
					if (index != string::npos) find_flag = true;
				}
				if (m_find_title && !find_flag)
				{
					index = CCommon::StringFindNoCase(CPlayer::GetInstance().GetPlayList()[i].title, m_key_word);
					if (index != string::npos) find_flag = true;
				}
				if (m_find_artist && !find_flag)
				{
					index = CCommon::StringFindNoCase(CPlayer::GetInstance().GetPlayList()[i].artist, m_key_word);
					if (index != string::npos) find_flag = true;
				}
				if (m_find_album && !find_flag)
				{
					index = CCommon::StringFindNoCase(CPlayer::GetInstance().GetPlayList()[i].album, m_key_word);
					if (index != string::npos) find_flag = true;
				}

				if (find_flag)
                {
                    m_find_result.push_back(CPlayer::GetInstance().GetPlayList()[i]);
                }
			}
			ShowFindResult();
			if (!m_find_result.empty())
				SetDlgItemText(IDC_FIND_RESULT_STATIC, CCommon::LoadText(IDS_FIND_RESULT, _T(": ")));
			else
				SetDlgItemText(IDC_FIND_RESULT_STATIC, CCommon::LoadText(IDS_NO_RESULT));
		}
		else			//�������в����б�ʱ����theApp.m_song_data�����в���
		{
			m_find_result.clear();
			wstring a_result;
			int index;
			bool find_flag;
			for (const auto& item : theApp.m_song_data)
			{
				find_flag = false;
				if (m_find_file && !find_flag)
				{
                    index = CCommon::StringFindNoCase(item.second.GetFileName(), m_key_word);
                    if (index != string::npos) find_flag = true;
				}
				if (m_find_title && !find_flag)
				{
					index = CCommon::StringFindNoCase(item.second.title, m_key_word);
					if (index != string::npos) find_flag = true;
				}
				if (m_find_artist && !find_flag)
				{
                    index = CCommon::StringFindNoCase(item.second.artist, m_key_word);
					if (index != string::npos) find_flag = true;
				}
				if (m_find_album && !find_flag)
				{
					index = CCommon::StringFindNoCase(item.second.album, m_key_word);
					if (index != string::npos) find_flag = true;
				}
				if (find_flag)
                {
                    SongInfo song = item.second;
                    song.file_path = item.first;
                    m_find_result.push_back(song);		//����ҵ��ˣ��ͱ�����
                }
			}
			ShowFindResult();
			if (!m_find_result.empty())
				SetDlgItemText(IDC_FIND_RESULT_STATIC, CCommon::LoadText(IDS_FIND_RESULT, _T(": ")));
			else
				SetDlgItemText(IDC_FIND_RESULT_STATIC, CCommon::LoadText(IDS_NO_RESULT));
		}
		m_item_selected = -1;
        m_items_selected.clear();
		ShowFindInfo();
        m_result_in_current_playlist = m_find_current_playlist;
	}
}


BOOL CFindDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), FALSE);		// ����Сͼ��

	//���ò���ѡ�ѡ��ť��״̬
	m_find_file_check.SetCheck(m_find_file);
	m_find_title_check.SetCheck(m_find_title);
	m_find_artist_check.SetCheck(m_find_artist);
	m_find_album_check.SetCheck(m_find_album);

	if (m_find_current_playlist)
		((CButton*)GetDlgItem(IDC_FIND_CURRENT_PLAYLIST_RADIO))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_FIND_ALL_PLAYLIST_RADIO))->SetCheck(TRUE);

	//��ʼ���б�ؼ�
	CRect rect;
	m_find_result_list.GetClientRect(rect);
    m_find_result_list.SetExtendedStyle(m_find_result_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP);
	int list_width{ rect.Width() - theApp.DPI(20) - 1 };		//�б�ؼ���ȼ�ȥ������ֱ�������Ŀ������
	int width0, width1, width2;
	width0 = theApp.DPI(40);
	width2 = (list_width - width0) / 5;
	width1 = list_width - width0 - width2 * 4;
	m_find_result_list.InsertColumn(0, CCommon::LoadText(IDS_NUMBER), LVCFMT_LEFT, width0);		//�����0��
	m_find_result_list.InsertColumn(1, CCommon::LoadText(IDS_FILE_NAME), LVCFMT_LEFT, width1);		//�����1��
	m_find_result_list.InsertColumn(2, CCommon::LoadText(IDS_TITLE), LVCFMT_LEFT, width2);		//�����2��
	m_find_result_list.InsertColumn(3, CCommon::LoadText(IDS_ARTIST), LVCFMT_LEFT, width2);		//�����3��
	m_find_result_list.InsertColumn(4, CCommon::LoadText(IDS_ALBUM), LVCFMT_LEFT, width2);		//�����4��
	m_find_result_list.InsertColumn(5, CCommon::LoadText(IDS_FILE_PATH), LVCFMT_LEFT, width2);		//�����5��

	ShowFindResult();	//��ʾ����һ�εģ����ҽ��
	ShowFindInfo();

	m_key_word.clear();

	GetDlgItem(IDC_FIND_EDIT)->SetFocus();		//���������õ�������

	//�����б�ؼ�����ʾ�����ö������ڽ����������˴��ڵĸ����ھ����ö�����ʱ����ʾ��Ϣ�ڴ������������
	m_find_result_list.GetToolTips()->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	GetWindowRect(rect);
	m_min_width = rect.Width();
	m_min_height = rect.Height();

	//��ʼ���Ҽ��˵�
	//if (m_menu.m_hMenu == NULL)
	//	m_menu.LoadMenu(IDR_FIND_POPUP_MENU);
	//m_menu.GetSubMenu(0)->SetDefaultItem(ID_FD_PLAY);

	GetDlgItem(IDOK)->EnableWindow(FALSE);	//���á�����ѡ����Ŀ����ť������ѡ����һ����Ŀ

	return FALSE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CFindDlg::OnNMDblclkFindList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_item_selected = pNMItemActivate->iItem;
    m_find_result_list.GetItemSelected(m_items_selected);
    GetDlgItem(IDOK)->EnableWindow(m_item_selected != -1);

	//˫���б���Ŀ��رնԻ��򲢲���ѡ����Ŀ
	OnPlayItem();
	*pResult = 0;
}


BOOL CFindDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYUP && pMsg->wParam == VK_RETURN)		//���س���ִ����������
	{
		OnBnClickedFindButton();
		return TRUE;
	}
	if (pMsg->wParam == VK_RETURN)		//���ΰ��س����˳�
	{
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CFindDlg::OnBnClickedFindFileCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_find_file = (m_find_file_check.GetCheck() != 0);
}


void CFindDlg::OnBnClickedFindTitleCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_find_title = (m_find_title_check.GetCheck() != 0);
}


void CFindDlg::OnBnClickedFindArtistCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_find_artist = (m_find_artist_check.GetCheck() != 0);
}


void CFindDlg::OnBnClickedFindAlbumCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_find_album = (m_find_album_check.GetCheck() != 0);
}



void CFindDlg::OnBnClickedFindCurrentPlaylistRadio()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_find_current_playlist = true;
}


void CFindDlg::OnBnClickedFindAllPlaylistRadio()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_find_current_playlist = false;
}


void CFindDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//���ƴ�����С��С
	lpMMI->ptMinTrackSize.x = m_min_width;		//������С���
	lpMMI->ptMinTrackSize.y = m_min_height;		//������С�߶�

	CDialog::OnGetMinMaxInfo(lpMMI);
}


void CFindDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (m_find_result_list.m_hWnd != NULL&&nType != SIZE_MINIMIZED)
	{
		//�����б�����Ŀ�Ŀ��
		CRect rect;
		m_find_result_list.GetWindowRect(rect);
		int list_width{ rect.Width() - theApp.DPI(20) - 1 };		//�б�ؼ���ȼ�ȥ������ֱ�������Ŀ������
		int width0, width1, width2;
		width0 = theApp.DPI(40);
		width2 = (list_width - width0) / 5;
		width1 = list_width - width0 - width2 * 4;
		m_find_result_list.SetColumnWidth(1, width1);
		m_find_result_list.SetColumnWidth(2, width2);
		m_find_result_list.SetColumnWidth(3, width2);
		m_find_result_list.SetColumnWidth(4, width2);
		m_find_result_list.SetColumnWidth(5, width2);
	}
}


void CFindDlg::OnPlayItem()
{
	// TODO: �ڴ���������������
    OnOK();
}


void CFindDlg::OnExploreTrack()
{
	// TODO: �ڴ���������������
	wstring file;
	if (m_item_selected >= 0 && m_item_selected < static_cast<int>(m_find_result.size()))
    {
        file = m_find_result[m_item_selected].file_path;
	    CString str;
	    str.Format(_T("/select,\"%s\""), file.c_str());
	    ShellExecute(NULL, _T("open"), _T("explorer"), str, NULL, SW_SHOWNORMAL);
    }
}


void CFindDlg::OnNMRClickFindList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_item_selected = pNMItemActivate->iItem;
    m_find_result_list.GetItemSelected(m_items_selected);
    GetDlgItem(IDOK)->EnableWindow(m_item_selected != -1);

	if (m_find_current_playlist && (m_item_selected >= 0 && m_item_selected < static_cast<int>(m_find_result.size()))
		|| !m_find_current_playlist && (m_item_selected >= 0 && m_item_selected < static_cast<int>(m_find_result.size())))
	{
		//��ȡ����������ı�
		int sub_item;
		sub_item = pNMItemActivate->iSubItem;
		m_selected_string = m_find_result_list.GetItemText(m_item_selected, sub_item);
		//�����Ҽ��˵�
		CMenu* pContextMenu = theApp.m_menu_set.m_media_lib_popup_menu.GetSubMenu(1);
        m_find_result_list.ShowPopupMenu(pContextMenu, pNMItemActivate->iItem, this);
    }

	*pResult = 0;
}


void CFindDlg::OnCopyText()
{
	// TODO: �ڴ���������������
	if (!CCommon::CopyStringToClipboard(wstring(m_selected_string)))
		MessageBox(CCommon::LoadText(IDS_COPY_CLIPBOARD_FAILED), NULL, MB_ICONWARNING);
}


void CFindDlg::OnPlayItemInFolderMode()
{
    // TODO: �ڴ���������������
    if (m_item_selected >= 0)
    {
        std::wstring file_path = m_find_result_list.GetItemText(m_item_selected, COL_PATH);
        CPlayer::GetInstance().OpenAFile(file_path, true);

        OnCancel();
    }

}


void CFindDlg::OnAddToNewPlaylist()
{
    // TODO: �ڴ���������������
    wstring playlist_path;
    _OnAddToNewPlaylist(playlist_path);
}


void CFindDlg::OnExploreOnline()
{
    // TODO: �ڴ���������������
    AfxBeginThread(ViewOnlineThreadFunc, (void*)this);
}


void CFindDlg::OnFormatConvert()
{
    // TODO: �ڴ���������������
    std::vector<SongInfo> songs;
    GetSongsSelected(songs);
    CMusicPlayerCmdHelper cmd_helper(this);
    cmd_helper.FormatConvert(songs);
}


void CFindDlg::OnItemProperty()
{
    // TODO: �ڴ���������������
    if (m_item_selected < 0)
        return;
    std::vector<SongInfo> songs;
    GetCurrentSongList(songs);
    CPropertyDlg propertyDlg(songs, this, true);
    propertyDlg.m_index = m_item_selected;
    propertyDlg.DoModal();
}


void CFindDlg::OnOK()
{
    // TODO: �ڴ����ר�ô����/����û���

    SongInfo song_selected;
    if (m_item_selected < 0 || m_item_selected >= static_cast<int>(m_find_result.size()))
        return;
    song_selected = m_find_result[m_item_selected];
    auto iter = std::find_if(CPlayer::GetInstance().GetPlayList().begin(), CPlayer::GetInstance().GetPlayList().end(), [&](const SongInfo& song)
    {
        return song.IsSameSong(song_selected);
    });
    if (iter != CPlayer::GetInstance().GetPlayList().end())      //������ҽ���ǵ�ǰ�����б��е���Ŀ�����ڵ�ǰ�����б��в���ѡ�е���Ŀ��������
    {
        int selected_track = iter - CPlayer::GetInstance().GetPlayList().begin();
        CPlayer::GetInstance().PlayTrack(selected_track);
        m_result_in_current_playlist = true;
    }
    else
    {
        vector<wstring> files;
        GetSongsSelected(files);
        if (!files.empty())
        {
            if (files.size() == 1)
                CPlayer::GetInstance().OpenFiles(files);
            else
                CPlayer::GetInstance().OpenFilesInTempPlaylist(files);
        }
        m_result_in_current_playlist = false;
    }

    CDialog::OnOK();
}


void CFindDlg::OnAddToNewPalylistAndPlay()
{
    // TODO: �ڴ���������������
    wstring playlist_path;
    if (_OnAddToNewPlaylist(playlist_path))
    {
        CPlayer::GetInstance().SetPlaylist(playlist_path, 0, 0, false, true);
        CPlayer::GetInstance().SaveRecentPath();
        OnCancel();
    }
}


void CFindDlg::OnInitMenu(CMenu* pMenu)
{
    CDialog::OnInitMenu(pMenu);

    // TODO: �ڴ˴������Ϣ����������
    pMenu->SetDefaultItem(ID_PLAY_ITEM);
}


BOOL CFindDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // TODO: �ڴ����ר�ô����/����û���
    WORD command = LOWORD(wParam);
    auto getSelectedItems = [&](std::vector<SongInfo>& item_list)
    {
        GetSongsSelected(item_list);
    };
    //��Ӧ�����б��Ҽ��˵��еġ���ӵ������б�
    CMusicPlayerCmdHelper cmd_helper;
    cmd_helper.OnAddToPlaylistCommand(getSelectedItems, command);

    return CDialog::OnCommand(wParam, lParam);
}
