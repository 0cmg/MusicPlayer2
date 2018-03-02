// LyricBatchDownloadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "LyricBatchDownloadDlg.h"
#include "afxdialogex.h"


// CLyricBatchDownloadDlg �Ի���

IMPLEMENT_DYNAMIC(CLyricBatchDownloadDlg, CDialog)

CLyricBatchDownloadDlg::CLyricBatchDownloadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_LYRIC_BATCH_DOWN_DIALOG, pParent)
{

}

CLyricBatchDownloadDlg::~CLyricBatchDownloadDlg()
{
}

void CLyricBatchDownloadDlg::SaveConfig() const
{
	CCommon::WritePrivateProfileIntW(L"lyric_batch_download", L"save_as_utf8", static_cast<int>(m_save_code), theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"lyric_batch_download", L"download_translate", m_download_translate, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"lyric_batch_download", L"save_to_song_folder", m_save_to_song_folder, theApp.m_config_path.c_str());
}

void CLyricBatchDownloadDlg::LoadConfig()
{
	m_save_code = static_cast<CodeType>(GetPrivateProfileInt(_T("lyric_batch_download"), _T("save_as_utf8"), 1, theApp.m_config_path.c_str()));
	m_download_translate = (GetPrivateProfileInt(_T("lyric_batch_download"), _T("download_translate"), 0, theApp.m_config_path.c_str()) != 0);
	m_save_to_song_folder = (GetPrivateProfileInt(_T("lyric_batch_download"), _T("save_to_song_folder"), 1, theApp.m_config_path.c_str()) != 0);
}

void CLyricBatchDownloadDlg::EnableControls(bool enable)
{
	m_skip_exist_check.EnableWindow(enable);
	m_save_code_combo.EnableWindow(enable);
	m_download_translate_chk.EnableWindow(enable);
	GetDlgItem(IDC_SAVE_TO_SONG_FOLDER)->EnableWindow(enable);
	if (m_lyric_path_not_exit)
		GetDlgItem(IDC_SAVE_TO_LYRIC_FOLDER)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_SAVE_TO_LYRIC_FOLDER)->EnableWindow(enable);
	GetDlgItem(IDC_START_DOWNLOAD)->EnableWindow(enable);
}

bool CLyricBatchDownloadDlg::SaveLyric(const wchar_t * path, const wstring& lyric_wcs, CodeType code_type)
{
	bool char_cannot_convert;
	string lyric_str = CCommon::UnicodeToStr(lyric_wcs, code_type, &char_cannot_convert);
	ofstream out_put{ path, std::ios::binary };
	out_put << lyric_str;
	return char_cannot_convert;
}

void CLyricBatchDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SKIP_EXIST_CHECK, m_skip_exist_check);
	DDX_Control(pDX, IDC_COMBO1, m_save_code_combo);
	DDX_Control(pDX, IDC_SONG_LIST1, m_song_list_ctrl);
	DDX_Control(pDX, IDC_DOWNLOAD_TRASNLATE_CHECK2, m_download_translate_chk);
	DDX_Control(pDX, IDC_INFO_STATIC, m_info_static);
}


BEGIN_MESSAGE_MAP(CLyricBatchDownloadDlg, CDialog)
	ON_BN_CLICKED(IDC_START_DOWNLOAD, &CLyricBatchDownloadDlg::OnBnClickedStartDownload)
	ON_BN_CLICKED(IDC_SKIP_EXIST_CHECK, &CLyricBatchDownloadDlg::OnBnClickedSkipExistCheck)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CLyricBatchDownloadDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_DOWNLOAD_TRASNLATE_CHECK2, &CLyricBatchDownloadDlg::OnBnClickedDownloadTrasnlateCheck2)
	ON_MESSAGE(WM_BATCH_DOWNLOAD_COMPLATE, &CLyricBatchDownloadDlg::OnBatchDownloadComplate)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SAVE_TO_SONG_FOLDER, &CLyricBatchDownloadDlg::OnBnClickedSaveToSongFolder)
	ON_BN_CLICKED(IDC_SAVE_TO_LYRIC_FOLDER, &CLyricBatchDownloadDlg::OnBnClickedSaveToLyricFolder)
END_MESSAGE_MAP()


// CLyricBatchDownloadDlg ��Ϣ�������


BOOL CLyricBatchDownloadDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	LoadConfig();

	//�����б�ؼ�������ɫ
	m_song_list_ctrl.SetColor(theApp.m_app_setting_data.m_theme_color);

	//��ʼ���ؼ���״̬
	m_skip_exist_check.SetCheck(m_skip_exist);
	m_save_code_combo.AddString(_T("ANSI"));
	m_save_code_combo.AddString(_T("UTF-8"));
	m_save_code_combo.SetCurSel(static_cast<int>(m_save_code));
	m_download_translate_chk.SetCheck(m_download_translate);
	if (m_save_to_song_folder)
		((CButton*)GetDlgItem(IDC_SAVE_TO_SONG_FOLDER))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_SAVE_TO_LYRIC_FOLDER))->SetCheck(TRUE);
	//�жϸ���ļ����Ƿ����
	bool lyric_path_exist = CCommon::FolderExist(theApp.m_play_setting_data.m_lyric_path);
	if (!lyric_path_exist)		//�������ļ������ڣ�����á����浽����ļ��С���ѡ��ť����ǿ��ѡ�С����浽��������Ŀ¼��
	{
		((CButton*)GetDlgItem(IDC_SAVE_TO_LYRIC_FOLDER))->EnableWindow(FALSE);
		((CButton*)GetDlgItem(IDC_SAVE_TO_LYRIC_FOLDER))->SetCheck(FALSE);
		((CButton*)GetDlgItem(IDC_SAVE_TO_SONG_FOLDER))->SetCheck(TRUE);
		m_save_to_song_folder = true;
		m_lyric_path_not_exit = true;
	}

	//��ʼ�������б�ؼ�
	//���ø��еĿ��
	CRect rect;
	m_song_list_ctrl.GetClientRect(rect);
	int width0, width1, width2, width3, width4;
	width0 = rect.Width() / 10;
	width1 = rect.Width() * 2 / 10;
	width2 = rect.Width() * 2 / 10;
	width3 = rect.Width() * 3 / 10;
	width4 = rect.Width() - width0 - width1 - width2 - width3 - DPI(21);
	//������
	m_song_list_ctrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP);
	m_song_list_ctrl.InsertColumn(0, _T("���"), LVCFMT_LEFT, width0);		//�����1��
	m_song_list_ctrl.InsertColumn(1, _T("����"), LVCFMT_LEFT, width1);		//�����2��
	m_song_list_ctrl.InsertColumn(2, _T("������"), LVCFMT_LEFT, width2);		//�����3��
	m_song_list_ctrl.InsertColumn(3, _T("�ļ���"), LVCFMT_LEFT, width3);		//�����3��
	m_song_list_ctrl.InsertColumn(4, _T("״̬"), LVCFMT_LEFT, width4);		//�����4��
	//������Ŀ
	for (size_t i{}; i<m_playlist.size(); i++)
	{
		CString tmp;
		tmp.Format(_T("%d"), i + 1);
		m_song_list_ctrl.InsertItem(i, tmp);
		m_song_list_ctrl.SetItemText(i, 1, m_playlist[i].title.c_str());
		m_song_list_ctrl.SetItemText(i, 2, m_playlist[i].artist.c_str());
		m_song_list_ctrl.SetItemText(i, 3, m_playlist[i].file_name.c_str());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CLyricBatchDownloadDlg::OnBnClickedStartDownload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//�������״̬��һ�е�����
	for (size_t i{}; i < m_playlist.size(); i++)
	{
		m_song_list_ctrl.SetItemText(i, 4, _T(""));
	}

	EnableControls(false);		//���ÿؼ�

	//����Ҫ�������ع����̴߳��ݵ�����
	m_thread_info.hwnd = GetSafeHwnd();
	m_thread_info.download_translate = m_download_translate;
	m_thread_info.save_to_song_folder = m_save_to_song_folder;
	m_thread_info.skip_exist = m_skip_exist;
	m_thread_info.save_code = m_save_code;
	m_thread_info.list_ctrl = &m_song_list_ctrl;
	m_thread_info.static_ctrl = &m_info_static;
	m_thread_info.playlist = &m_playlist;
	theApp.m_batch_download_dialog_exit = false;

	//��������������صĹ����߳�
	m_pThread = AfxBeginThread(ThreadFunc, &m_thread_info);
}


void CLyricBatchDownloadDlg::OnBnClickedSkipExistCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_skip_exist = (m_skip_exist_check.GetCheck() != 0);
}


void CLyricBatchDownloadDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	SaveConfig();

}


void CLyricBatchDownloadDlg::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ��Ͽ���ѡ�еı����ʽ
	switch (m_save_code_combo.GetCurSel())
	{
	case 1: m_save_code = CodeType::UTF8; break;
	default: m_save_code = CodeType::ANSI; break;
	}
}


void CLyricBatchDownloadDlg::OnBnClickedDownloadTrasnlateCheck2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_download_translate = (m_download_translate_chk.GetCheck() != 0);
}

//�����̺߳���
UINT CLyricBatchDownloadDlg::ThreadFunc(LPVOID lpParam)
{
	ThreadInfo* pInfo = (ThreadInfo*)lpParam;

	//���������б���ÿһ�׸����ĸ��
	for (size_t i{}; i<pInfo->playlist->size(); i++)
	{
		if (theApp.m_batch_download_dialog_exit)
			return 0;
		CString info;
		info.Format(_T("�������ظ�ʣ������ %d%%�����Ժ򡭡�"), i * 100 / pInfo->playlist->size());
		pInfo->static_ctrl->SetWindowText(info);
		//SetDlgItemText(IDC_INFO_STATIC, info);

		pInfo->list_ctrl->SetItemText(i, 4, _T("�������ء�"));
		pInfo->list_ctrl->EnsureVisible(i, FALSE);

		//����Ҫ����ĸ�ʵ�·��
		wstring lyric_path;
		if (pInfo->save_to_song_folder)
			lyric_path = theApp.m_player.GetCurrentPath() + pInfo->playlist->at(i).file_name;
		else
			lyric_path = theApp.m_play_setting_data.m_lyric_path + pInfo->playlist->at(i).file_name;
		size_t index = lyric_path.rfind(L'.');		//�����ļ������һ����
		lyric_path = lyric_path.substr(0, index + 1) + L"lrc";	//���ļ�������չ����Ϊlrc

		//�жϸ���Ƿ��Ѿ�����
		bool lyric_exist = CCommon::FileExist(lyric_path) || (!pInfo->playlist->at(i).lyric_file.empty());
		if (pInfo->skip_exist && lyric_exist)		//��������������Ѵ��ڸ�ʵ���Ŀ�����Ҹ���Ѿ����ڣ���������
		{
			pInfo->list_ctrl->SetItemText(i, 4, _T("������"));
			continue;
		}

		//���������ؼ���
		wstring search_result;		//���Ҹ������صĽ��
		wstring lyric_str;			//���غõĸ��
		wstring keyword;		//���ҵĹؼ���
		if (pInfo->playlist->at(i).title == DEFAULT_TITLE)		//���û�б�����Ϣ���Ͱ��ļ�����Ϊ�����ؼ���
		{
			keyword = pInfo->playlist->at(i).file_name;
			size_t index = keyword.rfind(L'.');		//�������һ����
			keyword = keyword.substr(0, index);		//ȥ����չ��
		}
		else if (pInfo->playlist->at(i).artist == DEFAULT_ARTIST)	//����б�����Ϣ����û����������Ϣ���Ͱѱ�����Ϊ�����ؼ���
		{
			keyword = pInfo->playlist->at(i).title;
		}
		else		//���򽫡������� ���⡱��Ϊ�����ؼ���
		{
			keyword = pInfo->playlist->at(i).artist + L' ' + pInfo->playlist->at(i).title;
		}

		//��������
		wstring keyword_url = CLyricDownloadCommon::URLEncode(keyword);		//�������ؼ���ת����URL����
		wchar_t buff[1024];
		swprintf_s(buff, L"http://music.163.com/api/search/get/?s=%s&limit=20&type=1&offset=0", keyword_url.c_str());
		int rtn = CLyricDownloadCommon::HttpPost(buff, search_result);		//�����������ֵĸ�������API����http��POST����
		if (theApp.m_batch_download_dialog_exit)		//����CLyricDownloadCommon::HttpPost����ִ�е�ʱ��Ƚϳ�������������ִ���ж��Ƿ��˳��̵߳Ĵ���
			return 0;
		if (rtn != 0)
		{
			pInfo->list_ctrl->SetItemText(i, 4, _T("��������ʧ�ܻ�ʱ"));
			continue;
		}

		//�����ؽ��
		vector<CLyricDownloadCommon::ItemInfo> down_list;
		CLyricDownloadCommon::DisposeSearchResult(down_list, search_result);		//�����صĲ��ҽ�����������������down_list������
		if (down_list.empty())
		{
			pInfo->list_ctrl->SetItemText(i, 4, _T("�Ҳ����˸���"));
			continue;
		}

		//�������ѡ����
		wstring title = pInfo->playlist->at(i).title;
		wstring artist = pInfo->playlist->at(i).artist;
		wstring album = pInfo->playlist->at(i).album;
		if (title == DEFAULT_TITLE) title.clear();
		if (artist == DEFAULT_ARTIST) artist.clear();
		if (album == DEFAULT_ALBUM) album.clear();
		int best_matched = CLyricDownloadCommon::SelectMatchedItem(down_list, title, artist, album, pInfo->playlist->at(i).file_name, true);
		if (best_matched < 0)
		{
			pInfo->list_ctrl->SetItemText(i, 4, _T("û��ƥ��ĸ��"));
			continue;
		}

		//���ظ��
		CLyricDownloadCommon::DownloadLyric(down_list[best_matched].id, lyric_str, pInfo->download_translate);
		if (lyric_str.empty())
		{
			pInfo->list_ctrl->SetItemText(i, 4, _T("�������ʧ��"));
			continue;
		}

		//�������ı�
		if (!CLyricDownloadCommon::DisposeLryic(lyric_str))
		{
			pInfo->list_ctrl->SetItemText(i, 4, _T("�ø���û�и��"));
			continue;
		}

		//�ڸ��ǰ����ӱ�ǩ
		CLyricDownloadCommon::AddLyricTag(lyric_str, down_list[best_matched].id, down_list[best_matched].title, down_list[best_matched].artist, down_list[best_matched].album);

		//������
		if (CLyricBatchDownloadDlg::SaveLyric(lyric_path.c_str(), lyric_str, pInfo->save_code))
			pInfo->list_ctrl->SetItemText(i, 4, _T("�ɹ������Ǹ�������޷�ת����Unicode�ַ������鱣��ΪUTF-8��ʽ"));		//�����������true����˵�����޷�ת����Unicode�ַ�
		else
			pInfo->list_ctrl->SetItemText(i, 4, _T("�ɹ�"));
		if (pInfo->download_translate)
		{
			CLyrics lyrics{ lyric_path };		//�򿪱�����ĸ��
			lyrics.DeleteRedundantLyric();		//ɾ������ĸ��
			lyrics.CombineSameTimeLyric();		//����ʷ����ԭʼ��ʺϲ���һ��
			lyrics.SaveLyric2();
		}

	}
	::PostMessage(pInfo->hwnd, WM_BATCH_DOWNLOAD_COMPLATE, 0, 0);
	return 0;
}


afx_msg LRESULT CLyricBatchDownloadDlg::OnBatchDownloadComplate(WPARAM wParam, LPARAM lParam)
{
	SetDlgItemText(IDC_INFO_STATIC, _T("������ɡ�"));
	//������ɺ�����������
	theApp.m_player.SearchLyrics();
	theApp.m_player.IniLyrics();
	EnableControls(true);		//���ÿؼ�
	return 0;
}


void CLyricBatchDownloadDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialog::OnClose();
}


void CLyricBatchDownloadDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
	//�Ի���Ҫ�ر�ʱ�����˳���־��Ϊtrue
	theApp.m_batch_download_dialog_exit = true;
	int rtn;
	if (m_pThread != nullptr)
	{
		rtn = WaitForSingleObject(m_pThread->m_hThread, 2000);	//�ȴ��߳��˳�
#ifdef DEBUG
		CString info;
		info.Format(_T("WaitForSingleObject�����ķ���ֵΪ%x��"), rtn);
		MessageBox(info, NULL, MB_ICONINFORMATION);
#endif // DEBUG

	}
	DestroyWindow();
	//CDialog::OnCancel();
}


void CLyricBatchDownloadDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���
	//�Ի���Ҫ�ر�ʱ�����˳���־��Ϊtrue
	theApp.m_batch_download_dialog_exit = true;
	if (m_pThread != nullptr)
		WaitForSingleObject(m_pThread->m_hThread, 2000);	//�ȴ��߳��˳�

	CDialog::OnOK();
}


void CLyricBatchDownloadDlg::OnBnClickedSaveToSongFolder()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_save_to_song_folder = true;
}


void CLyricBatchDownloadDlg::OnBnClickedSaveToLyricFolder()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_save_to_song_folder = false;
}
