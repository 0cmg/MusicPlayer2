// FindDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "FindDlg.h"
#include "afxdialogex.h"


// CFindDlg �Ի���

IMPLEMENT_DYNAMIC(CFindDlg, CDialogEx)

CFindDlg::CFindDlg(const vector<SongInfo>& playlist, vector<int>& find_result, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FIND_DIALOG, pParent), m_playlist{ playlist }, m_find_result{ find_result }
{

}

CFindDlg::~CFindDlg()
{
}

void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
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
	for (int i{}; i<m_find_result.size(); i++)
	{
		str.Format(_T("%u"), m_find_result[i] + 1);
		m_find_result_list.InsertItem(i, str);
		m_find_result_list.SetItemText(i, 1, m_playlist[m_find_result[i]].file_name.c_str());
		m_find_result_list.SetItemText(i, 2, m_playlist[m_find_result[i]].title.c_str());
		m_find_result_list.SetItemText(i, 3, m_playlist[m_find_result[i]].artist.c_str());
		m_find_result_list.SetItemText(i, 4, m_playlist[m_find_result[i]].album.c_str());
	}
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
	CCommon::WritePrivateProfileIntW(L"config", L"find_option_data", m_find_option_data, theApp.m_config_path.c_str());
}

void CFindDlg::LoadConfig()
{
	m_find_option_data = GetPrivateProfileInt(_T("config"), _T("find_option_data"), 15, theApp.m_config_path.c_str());
	m_find_file = (m_find_option_data % 2 != 0);
	m_find_title = ((m_find_option_data >> 1) % 2 != 0);
	m_find_artist = ((m_find_option_data >> 2) % 2 != 0);
	m_find_album = ((m_find_option_data >> 3) % 2 != 0);
}

int CFindDlg::GetSelectedTrack() const
{
	if (m_item_selected >= 0 && m_item_selected < m_find_result.size())
		return m_find_result[m_item_selected];
	else
		return -1;
}


BEGIN_MESSAGE_MAP(CFindDlg, CDialogEx)
	ON_EN_CHANGE(IDC_FIND_EDIT, &CFindDlg::OnEnChangeFindEdit)
	ON_NOTIFY(NM_CLICK, IDC_FIND_LIST, &CFindDlg::OnNMClickFindList)
	ON_BN_CLICKED(IDC_FIND_BUTTON, &CFindDlg::OnBnClickedFindButton)
	ON_NOTIFY(NM_DBLCLK, IDC_FIND_LIST, &CFindDlg::OnNMDblclkFindList)
	ON_BN_CLICKED(IDC_FIND_FILE_CHECK, &CFindDlg::OnBnClickedFindFileCheck)
	ON_BN_CLICKED(IDC_FIND_TITLE_CHECK, &CFindDlg::OnBnClickedFindTitleCheck)
	ON_BN_CLICKED(IDC_FIND_ARTIST_CHECK, &CFindDlg::OnBnClickedFindArtistCheck)
	ON_BN_CLICKED(IDC_FIND_ALBUM_CHECK, &CFindDlg::OnBnClickedFindAlbumCheck)
END_MESSAGE_MAP()


// CFindDlg ��Ϣ�������

void CFindDlg::OnEnChangeFindEdit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
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
	*pResult = 0;
}


void CFindDlg::OnBnClickedFindButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_key_word.empty())
	{
		m_find_result.clear();
		int index;
		bool find_flag;
		for (int i{ 0 }; i < m_playlist.size(); i++)
		{
			find_flag = false;
			if (m_find_file)
			{
				index = m_playlist[i].file_name.find(m_key_word);
				if (index != string::npos) find_flag = true;
			}
			if (m_find_title)
			{
				index = m_playlist[i].title.find(m_key_word);
				if (index != string::npos) find_flag = true;
			}
			if (m_find_artist)
			{
				index = m_playlist[i].artist.find(m_key_word);
				if (index != string::npos) find_flag = true;
			}
			if (m_find_album)
			{
				index = m_playlist[i].album.find(m_key_word);
				if (index != string::npos) find_flag = true;
			}

			if (find_flag)
				m_find_result.push_back(i);		//����ҵ��ˣ����沥���б��е���Ŀ���
		}
		ShowFindResult();
		if (!m_find_result.empty())
			SetDlgItemText(IDC_FIND_RESULT_STATIC, _T("���ҽ����"));
		else
			SetDlgItemText(IDC_FIND_RESULT_STATIC, _T("û���ҵ������"));
		m_item_selected = -1;
	}
}


BOOL CFindDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	////��ȡdpi����
	//CWindowDC dc(this);
	//HDC hDC = dc.GetSafeHdc();
	//m_dpi = GetDeviceCaps(hDC, LOGPIXELSY);

	//���ò���ѡ�ѡ��ť��״̬
	m_find_file_check.SetCheck(m_find_file);
	m_find_title_check.SetCheck(m_find_title);
	m_find_artist_check.SetCheck(m_find_artist);
	m_find_album_check.SetCheck(m_find_album);

	//��ʼ���б�ؼ�
	CRect rect;
	m_find_result_list.GetClientRect(rect);
	m_find_result_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP);
	int list_width{ rect.Width() - DPI(21) };		//�б�ؼ���ȼ�ȥ������ֱ�������Ŀ������
	int width0, width1, width2;
	width0 = DPI(40);
	width2 = (list_width - width0) / 4;
	width1 = list_width - width0 - width2 * 3;
	m_find_result_list.InsertColumn(0, _T("���"), LVCFMT_LEFT, width0);		//�����0��
	m_find_result_list.InsertColumn(1, _T("�ļ���"), LVCFMT_LEFT, width1);		//�����1��
	m_find_result_list.InsertColumn(2, _T("����"), LVCFMT_LEFT, width2);		//�����2��
	m_find_result_list.InsertColumn(3, _T("������"), LVCFMT_LEFT, width2);		//�����3��
	m_find_result_list.InsertColumn(4, _T("��Ƭ��"), LVCFMT_LEFT, width2);		//�����4��

	ShowFindResult();	//��ʾ����һ�εģ����ҽ��

	GetDlgItem(IDC_FIND_EDIT)->SetFocus();		//���������õ�������

	//�����б�ؼ�����ʾ�����ö������ڽ����������˴��ڵĸ����ھ����ö�����ʱ����ʾ��Ϣ�ڴ������������
	m_find_result_list.GetToolTips()->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);


	return FALSE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CFindDlg::OnNMDblclkFindList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_item_selected = pNMItemActivate->iItem;
	if (m_item_selected >= 0 && m_item_selected < m_find_result.size())
		OnOK();		//˫���б���Ŀ��رնԻ��򲢲���ѡ����Ŀ
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

	return CDialogEx::PreTranslateMessage(pMsg);
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

