// PropertyDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "PropertyDlg.h"
#include "afxdialogex.h"


// CPropertyDlg �Ի���

IMPLEMENT_DYNAMIC(CPropertyDlg, CDialog)

CPropertyDlg::CPropertyDlg(const vector<SongInfo>& all_song_info, CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PROPERTY_DIALOG, pParent), m_all_song_info{ all_song_info }
{

}

CPropertyDlg::~CPropertyDlg()
{
}

void CPropertyDlg::ShowInfo()
{
	//��ʾ�ļ���
	m_file_name_edit.SetWindowText(m_all_song_info[m_index].file_name.c_str());

	//��ʾ�ļ�·��
	m_file_path_edit.SetWindowText((m_path + m_all_song_info[m_index].file_name).c_str());

	//��ʾ�ļ�����
	_tstring file_type;
	size_t index;
	index = m_all_song_info[m_index].file_name.find_last_of(L'.');
	if (index != string::npos)
		file_type = m_all_song_info[m_index].file_name.substr(index + 1);		//��ȡ�ļ�����չ��
	std::transform(file_type.begin(), file_type.end(), file_type.begin(), tolower);		//����չ��ת����Сд
	if (file_type == _T("mp3"))
		m_file_type_edit.SetWindowText(_T("MP3��Ƶ�ļ�"));
	else if (file_type == _T("wma"))
		m_file_type_edit.SetWindowText(_T("Windows Media ��Ƶ�ļ�"));
	else if (file_type == _T("wav"))
		m_file_type_edit.SetWindowText(_T("WAV��Ƶ�ļ�"));
	else if (file_type == _T("mid"))
		m_file_type_edit.SetWindowText(_T("MIDI����"));
	else if (file_type == _T("ogg"))
		m_file_type_edit.SetWindowText(_T("OGG��Ƶ�ļ�"));
	else if (file_type == _T("m4a"))
		m_file_type_edit.SetWindowText(_T("MPEG-4 ��Ƶ�ļ�"));
	else
		m_file_type_edit.SetWindowText((file_type + _T("�ļ�")).c_str());

	//��ʾ�ļ�����
	wstring song_length;
	if (m_all_song_info[m_index].lengh.isZero())
		song_length = L"��δ��ȡ���޷���ȡ��Ƶ�ĳ���";
	else
		song_length = m_all_song_info[m_index].lengh.time2str2();
	m_song_length_edit.SetWindowText(song_length.c_str());

	//��ʾ�ļ���С
	size_t file_size;
	file_size = CCommon::GetFileSize(m_path + m_all_song_info[m_index].file_name);
	CString size_info;
	if (file_size < 1024)
		size_info.Format(_T("%u B"), file_size);
	else if (file_size < 1024 * 1024)
		size_info.Format(_T("%.2f KB"), file_size / 1024.0f);
	else if (file_size < 1024 * 1024 * 1024)
		size_info.Format(_T("%.2f MB"), file_size / 1024.0f / 1024.0f);
	else
		size_info.Format(_T("%.2f GB"), file_size / 1024.0f / 1024.0f / 1024.0f);
	m_file_size_edit.SetWindowText(size_info);

	//��ʾ������
	CString info;
	if (file_size == 0 || m_all_song_info[m_index].lengh.isZero() || file_type == L"mid")		//�ļ���СΪ0���ļ�����Ϊ0���ļ�Ϊmidi����ʱ����ʾ������
	{
		info = _T("-");
	}
	else
	{
		info.Format(_T("%d Kbps"), m_all_song_info[m_index].bitrate);
	}
	m_bit_rate_edit.SetWindowText(info);

	//��ʾ���·��
	if(!m_all_song_info[m_index].lyric_file.empty())
		m_lyric_file_edit.SetWindowText(m_all_song_info[m_index].lyric_file.c_str());
	else
		m_lyric_file_edit.SetWindowText(_T("û��ƥ��ĸ��"));

	//��ʾ��Ƶ��Ϣ
	//CString info;
	m_title_edit.SetWindowText(m_all_song_info[m_index].title.c_str());
	m_artist_edit.SetWindowText(m_all_song_info[m_index].artist.c_str());
	m_album_edit.SetWindowText(m_all_song_info[m_index].album.c_str());
	m_year_edit.SetWindowText(m_all_song_info[m_index].year.c_str());
	if (m_all_song_info[m_index].track != 0)
		info.Format(_T("%u"), m_all_song_info[m_index].track);
	else
		info = _T("");
	m_track_edit.SetWindowText(info);
	m_genre_edit.SetWindowText(m_all_song_info[m_index].genre.c_str());
	m_comment_edit.SetWindowText(m_all_song_info[m_index].comment.c_str());

	//��ʾ��ǩ����
	if (file_type == _T("mp3"))
	{
		CString tag_type_str{ _T("MP3��ǩ���ͣ�") };
		switch (m_all_song_info[m_index].tag_type)
		{
		case 0: tag_type_str += _T("δ֪"); break;
		case 1: tag_type_str += _T("ID3v1"); break;
		case 2: tag_type_str += _T("ID3v2"); break;
		}
		SetDlgItemText(IDC_TAG_TYPE_STATIC, tag_type_str);
	}
	else
	{
		SetDlgItemText(IDC_TAG_TYPE_STATIC, _T(""));
	}


	//��ʾҳ��
	CString item_info;
	item_info.Format(_T("%d/%d"), m_index + 1, m_song_num);
	SetDlgItemText(IDC_ITEM_STATIC, item_info);
}

void CPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_NAME_EDIT, m_file_name_edit);
	DDX_Control(pDX, IDC_FILE_PATH_EDIT, m_file_path_edit);
	DDX_Control(pDX, IDC_FILE_TYPE_EDIT, m_file_type_edit);
	DDX_Control(pDX, IDC_SONG_LENGTH_EDIT, m_song_length_edit);
	DDX_Control(pDX, IDC_FILE_SIZE_EDIT, m_file_size_edit);
	DDX_Control(pDX, IDC_BIT_RATE_EDIT, m_bit_rate_edit);
	DDX_Control(pDX, IDC_TITEL_EDIT, m_title_edit);
	DDX_Control(pDX, IDC_ARTIST_EDIT, m_artist_edit);
	DDX_Control(pDX, IDC_ALBUM_EDIT, m_album_edit);
	DDX_Control(pDX, IDC_TRACK_EDIT, m_track_edit);
	DDX_Control(pDX, IDC_YEAR_EDIT, m_year_edit);
	DDX_Control(pDX, IDC_GENRE_EDIT, m_genre_edit);
	DDX_Control(pDX, IDC_COMMENT_EDIT, m_comment_edit);
	DDX_Control(pDX, IDC_LYRIC_FILE_EDIT, m_lyric_file_edit);
}


BEGIN_MESSAGE_MAP(CPropertyDlg, CDialog)
	ON_BN_CLICKED(IDC_PREVIOUS_BUTTON, &CPropertyDlg::OnBnClickedPreviousButton)
	ON_BN_CLICKED(IDC_NEXT_BUTTON, &CPropertyDlg::OnBnClickedNextButton)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


// CPropertyDlg ��Ϣ�������


BOOL CPropertyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ShowInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CPropertyDlg::OnBnClickedPreviousButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_index--;
	if (m_index < 0) m_index = m_song_num - 1;
	if (m_index < 0) m_index = 0;
	ShowInfo();
}


void CPropertyDlg::OnBnClickedNextButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_index++;
	if (m_index >= m_song_num) m_index = 0;
	ShowInfo();
}


BOOL CPropertyDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_UP)
		{
			OnBnClickedPreviousButton();
			return TRUE;
		}
		if (pMsg->wParam == VK_DOWN)
		{
			OnBnClickedNextButton();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


BOOL CPropertyDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//ͨ�������ַ�ҳ
	if (zDelta > 0)
	{
		OnBnClickedPreviousButton();
	}
	if (zDelta < 0)
	{
		OnBnClickedNextButton();
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
