// DataSettingsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "DataSettingsDlg.h"
#include "afxdialogex.h"


// CDataSettingsDlg �Ի���

IMPLEMENT_DYNAMIC(CDataSettingsDlg, CTabDlg)

CDataSettingsDlg::CDataSettingsDlg(CWnd* pParent /*=NULL*/)
	: CTabDlg(IDD_DATA_SETTINGS_DIALOG, pParent)
{

}

CDataSettingsDlg::~CDataSettingsDlg()
{
}

void CDataSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CTabDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SF2_PATH_EDIT, m_sf2_path_edit);
	DDX_Control(pDX, IDC_COMBO1, m_language_combo);
}


BEGIN_MESSAGE_MAP(CDataSettingsDlg, CTabDlg)
	ON_BN_CLICKED(IDC_ID3V2_FIRST_CHECK, &CDataSettingsDlg::OnBnClickedId3v2FirstCheck)
	ON_BN_CLICKED(IDC_COVER_AUTO_DOWNLOAD_CHECK, &CDataSettingsDlg::OnBnClickedCoverAutoDownloadCheck)
	ON_BN_CLICKED(IDC_LYRIC_AUTO_DOWNLOAD_CHECK, &CDataSettingsDlg::OnBnClickedLyricAutoDownloadCheck)
	ON_BN_CLICKED(IDC_CHECK_UPDATE_CHECK, &CDataSettingsDlg::OnBnClickedCheckUpdateCheck)
	//ON_BN_CLICKED(IDC_BROWSE_BUTTON, &CDataSettingsDlg::OnBnClickedBrowseButton)
	ON_BN_CLICKED(IDC_MIDI_USE_INNER_LYRIC_CHECK, &CDataSettingsDlg::OnBnClickedMidiUseInnerLyricCheck)
	ON_BN_CLICKED(IDC_DOWNLOAD_WHEN_TAG_FULL_CHECK, &CDataSettingsDlg::OnBnClickedDownloadWhenTagFullCheck)
	ON_EN_CHANGE(IDC_SF2_PATH_EDIT, &CDataSettingsDlg::OnEnChangeSf2PathEdit)
    ON_MESSAGE(WM_EDIT_BROWSE_CHANGED, &CDataSettingsDlg::OnEditBrowseChanged)
END_MESSAGE_MAP()


// CDataSettingsDlg ��Ϣ�������


BOOL CDataSettingsDlg::OnInitDialog()
{
	CTabDlg::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//SetBackgroundColor(RGB(255, 255, 255));

	m_language_combo.AddString(CCommon::LoadText(IDS_FOLLOWING_SYSTEM));
	m_language_combo.AddString(_T("English"));
	m_language_combo.AddString(_T("��������"));
	m_language_combo.SetCurSel(static_cast<int>(m_data.language));


	((CButton*)GetDlgItem(IDC_ID3V2_FIRST_CHECK))->SetCheck(m_data.id3v2_first);
	((CButton*)GetDlgItem(IDC_COVER_AUTO_DOWNLOAD_CHECK))->SetCheck(m_data.auto_download_album_cover);
	((CButton*)GetDlgItem(IDC_LYRIC_AUTO_DOWNLOAD_CHECK))->SetCheck(m_data.auto_download_lyric);
	((CButton*)GetDlgItem(IDC_DOWNLOAD_WHEN_TAG_FULL_CHECK))->SetCheck(m_data.auto_download_only_tag_full);
	((CButton*)GetDlgItem(IDC_CHECK_UPDATE_CHECK))->SetCheck(m_data.check_update_when_start);
    m_sf2_path_edit.SetWindowText(m_data.sf2_path.c_str());
    CString szFilter = CCommon::LoadText(IDS_SOUND_FONT_FILTER);
    m_sf2_path_edit.EnableFileBrowseButton(_T("SF2"), szFilter);
	((CButton*)GetDlgItem(IDC_MIDI_USE_INNER_LYRIC_CHECK))->SetCheck(m_data.midi_use_inner_lyric);
	if (m_data.minimize_to_notify_icon)
		((CButton*)GetDlgItem(IDC_MINIMIZE_TO_NOTIFY_RADIO))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_EXIT_PROGRAM_RADIO))->SetCheck(TRUE);

    if(m_data.save_lyric_to_song_folder)
        ((CButton*)GetDlgItem(IDC_SAVE_TO_SONG_FOLDER))->SetCheck(TRUE);
    else
        ((CButton*)GetDlgItem(IDC_SAVE_TO_LYRIC_FOLDER))->SetCheck(TRUE);

    //�жϸ���ļ����Ƿ����
    bool lyric_path_exist = CCommon::FolderExist(theApp.m_lyric_setting_data.lyric_path);
    if (!lyric_path_exist)		//�������ļ������ڣ�����á����浽����ļ��С���ѡ��ť����ǿ��ѡ�С����浽��������Ŀ¼��
    {
        ((CButton*)GetDlgItem(IDC_SAVE_TO_LYRIC_FOLDER))->EnableWindow(FALSE);
        ((CButton*)GetDlgItem(IDC_SAVE_TO_LYRIC_FOLDER))->SetCheck(FALSE);
        ((CButton*)GetDlgItem(IDC_SAVE_TO_SONG_FOLDER))->SetCheck(TRUE);
        m_data.save_lyric_to_song_folder = true;
    }

	m_toolTip.Create(this);
	m_toolTip.SetMaxTipWidth(theApp.DPI(300));
	m_toolTip.AddTool(GetDlgItem(IDC_DOWNLOAD_WHEN_TAG_FULL_CHECK), CCommon::LoadText(IDS_AUTO_DOWNLOAD_LYRIC_TIP_INFO));
	//m_toolTip.AddTool(GetDlgItem(IDC_SF2_PATH_EDIT), _T("��Ҫ�������ɫ����ܲ��� MIDI ���֡�"));
	m_toolTip.AddTool(GetDlgItem(IDC_MIDI_USE_INNER_LYRIC_CHECK), CCommon::LoadText(IDS_MIDI_INNER_LYRIC_TIP_INFO));

	m_toolTip.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

    EnableControl();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CDataSettingsDlg::EnableControl()
{
    bool enable = !CPlayer::GetInstance().IsMciCore();
    m_sf2_path_edit.EnableWindow(enable && theApp.m_format_convert_dialog_exit);		//���ڽ��и�ʽת��ʱ�����������ɫ��
    CWnd* pWnd = GetDlgItem(IDC_BROWSE_BUTTON);
    if(pWnd!=nullptr)
        pWnd->EnableWindow(enable && theApp.m_format_convert_dialog_exit);
    pWnd = GetDlgItem(IDC_MIDI_USE_INNER_LYRIC_CHECK);
    if (pWnd != nullptr)
        pWnd->EnableWindow(enable);

    ((CButton*)GetDlgItem(IDC_SAVE_TO_SONG_FOLDER))->EnableWindow(m_data.auto_download_lyric);
    ((CButton*)GetDlgItem(IDC_SAVE_TO_LYRIC_FOLDER))->EnableWindow(m_data.auto_download_lyric && CCommon::FolderExist(theApp.m_lyric_setting_data.lyric_path));

}


void CDataSettingsDlg::OnBnClickedId3v2FirstCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_data.id3v2_first = (((CButton*)GetDlgItem(IDC_ID3V2_FIRST_CHECK))->GetCheck() != 0);
}


void CDataSettingsDlg::OnBnClickedCoverAutoDownloadCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_data.auto_download_album_cover = (((CButton*)GetDlgItem(IDC_COVER_AUTO_DOWNLOAD_CHECK))->GetCheck() != 0);
}


void CDataSettingsDlg::OnBnClickedLyricAutoDownloadCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_data.auto_download_lyric = (((CButton*)GetDlgItem(IDC_LYRIC_AUTO_DOWNLOAD_CHECK))->GetCheck() != 0);
    EnableControl();
}


void CDataSettingsDlg::OnBnClickedCheckUpdateCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_data.check_update_when_start = (((CButton*)GetDlgItem(IDC_CHECK_UPDATE_CHECK))->GetCheck() != 0);
}


BOOL CDataSettingsDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_MOUSEMOVE)
		m_toolTip.RelayEvent(pMsg);

	return CTabDlg::PreTranslateMessage(pMsg);
}


void CDataSettingsDlg::OnBnClickedMidiUseInnerLyricCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_data.midi_use_inner_lyric = (((CButton*)GetDlgItem(IDC_MIDI_USE_INNER_LYRIC_CHECK))->GetCheck() != 0);
}


void CDataSettingsDlg::OnBnClickedDownloadWhenTagFullCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_data.auto_download_only_tag_full = (((CButton*)GetDlgItem(IDC_DOWNLOAD_WHEN_TAG_FULL_CHECK))->GetCheck() != 0);
}


void CDataSettingsDlg::OnEnChangeSf2PathEdit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CTabDlg::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	if (m_sf2_path_edit.GetModify())
	{
		CString str;
		m_sf2_path_edit.GetWindowText(str);
		m_data.sf2_path = str;
	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CDataSettingsDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	m_data.minimize_to_notify_icon = (((CButton*)GetDlgItem(IDC_MINIMIZE_TO_NOTIFY_RADIO))->GetCheck() != 0);
    m_data.save_lyric_to_song_folder = (((CButton*)GetDlgItem(IDC_SAVE_TO_SONG_FOLDER))->GetCheck() != 0);

	//��ȡ���Ե�����
	m_data.language = static_cast<Language>(m_language_combo.GetCurSel());
	if (m_data.language != theApp.m_general_setting_data.language)
	{
		MessageBox(CCommon::LoadText(IDS_LANGUAGE_CHANGE_INFO), NULL, MB_ICONINFORMATION | MB_OK);
	}

	CTabDlg::OnOK();
}


afx_msg LRESULT CDataSettingsDlg::OnEditBrowseChanged(WPARAM wParam, LPARAM lParam)
{
    CString str;
    m_sf2_path_edit.GetWindowText(str);
    m_data.sf2_path = str;
    return 0;
}
