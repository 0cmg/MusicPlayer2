// PlaySettingsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "LyricSettingsDlg.h"
#include "afxdialogex.h"


// CLyricSettingsDlg �Ի���

IMPLEMENT_DYNAMIC(CLyricSettingsDlg, CDialogEx)

CLyricSettingsDlg::CLyricSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LYRIC_SETTING_DIALOG, pParent)
{

}

CLyricSettingsDlg::~CLyricSettingsDlg()
{
}

void CLyricSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_KARAOKE_DISP, m_karaoke_disp_check);
	DDX_Control(pDX, IDC_LYRIC_FUZZY_MATCH, m_lyric_fuzzy_match_check);
	DDX_Control(pDX, IDC_SHOW_LYRIC_IN_CORTANA, m_show_lyric_in_cortana_check);
	DDX_Control(pDX, IDC_LYRIC_DOUBLE_LINE_CHECK, m_lyric_double_line_chk);
	DDX_Control(pDX, IDC_SHOW_ALBUM_COVER_IN_CORTANA, m_show_album_cover_in_cortana_check);
	DDX_Control(pDX, IDC_CORTANA_ICON_DEAT_CHECK, m_cortana_icon_beat_check);
	DDX_Control(pDX, IDC_CORTANA_COLOR_COMBO, m_cortana_color_combo);
}


BEGIN_MESSAGE_MAP(CLyricSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_STOP_WHEN_ERROR, &CLyricSettingsDlg::OnBnClickedStopWhenError)
	ON_BN_CLICKED(IDC_KARAOKE_DISP, &CLyricSettingsDlg::OnBnClickedKaraokeDisp)
	ON_BN_CLICKED(IDC_EXPLORE_LYRIC_BUTTON, &CLyricSettingsDlg::OnBnClickedExploreLyricButton)
	ON_BN_CLICKED(IDC_SHOW_TASKBAR_PROGRESS, &CLyricSettingsDlg::OnBnClickedShowTaskbarProgress)
	ON_BN_CLICKED(IDC_LYRIC_FUZZY_MATCH, &CLyricSettingsDlg::OnBnClickedLyricFuzzyMatch)
	ON_BN_CLICKED(IDC_SHOW_LYRIC_IN_CORTANA, &CLyricSettingsDlg::OnBnClickedShowLyricInCortana)
	//ON_BN_CLICKED(IDC_SAVE_IN_OFFSET_TAG, &CLyricSettingsDlg::OnBnClickedSaveInOffsetTag)
	//ON_BN_CLICKED(IDC_SAVE_IN_TIME_TAG, &CLyricSettingsDlg::OnBnClickedSaveInTimeTag)
	ON_BN_CLICKED(IDC_LYRIC_DOUBLE_LINE_CHECK, &CLyricSettingsDlg::OnBnClickedLyricDoubleLineCheck)
	ON_CBN_SELCHANGE(IDC_CORTANA_COLOR_COMBO, &CLyricSettingsDlg::OnCbnSelchangeCortanaColorCombo)
	ON_BN_CLICKED(IDC_SHOW_ALBUM_COVER_IN_CORTANA, &CLyricSettingsDlg::OnBnClickedShowAlbumCoverInCortana)
	ON_BN_CLICKED(IDC_CORTANA_ICON_DEAT_CHECK, &CLyricSettingsDlg::OnBnClickedCortanaIconDeatCheck)
END_MESSAGE_MAP()


// CLyricSettingsDlg ��Ϣ�������


BOOL CLyricSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetBackgroundColor(RGB(255, 255, 255));

	//��ʼ�����ؼ���״̬
	m_karaoke_disp_check.SetCheck(m_data.lyric_karaoke_disp);
	m_lyric_fuzzy_match_check.SetCheck(m_data.lyric_fuzzy_match);
	m_lyric_double_line_chk.SetCheck(m_data.cortana_lyric_double_line);
	m_show_album_cover_in_cortana_check.SetCheck(m_data.cortana_show_album_cover);
	m_cortana_icon_beat_check.SetCheck(m_data.cortana_icon_beat);
	//m_cortana_icon_beat_check.EnableWindow(!m_data.cortana_show_album_cover);
	if (theApp.m_win_version.IsWindows10OrLater())
	{
		m_show_lyric_in_cortana_check.SetCheck(m_data.show_lyric_in_cortana);
	}
	else
	{
		m_show_lyric_in_cortana_check.EnableWindow(FALSE);		//Win10����ϵͳ���ô˸�ѡ��ť
		m_data.show_lyric_in_cortana = false;
	}
//#ifdef COMPILE_IN_WIN_XP
//	m_show_taskbar_progress_check.SetCheck(FALSE);
//	m_show_taskbar_progress_check.EnableWindow(FALSE);
//#endif
	//if(m_data.save_lyric_in_offset)
	//	((CButton*)GetDlgItem(IDC_SAVE_IN_OFFSET_TAG))->SetCheck(TRUE);
	//else
	//	((CButton*)GetDlgItem(IDC_SAVE_IN_TIME_TAG))->SetCheck(TRUE);

	SetCortanaControlEnable(m_data.show_lyric_in_cortana);

	SetDlgItemText(IDC_LYRIC_PATH_EDIT, m_data.lyric_path.c_str());

	m_tool_tip.Create(this);
	m_tool_tip.SetMaxTipWidth(300);
	m_tool_tip.AddTool(&m_lyric_fuzzy_match_check, _T("���ȥ���˸�ѡ��Ĺ�ѡ����ֻ��ƥ��͸����ļ�����ȫһ���ĸ���ļ���\r\n���ѡ�д˸�ѡ�򣬵��Ҳ����ļ�����ȫһ���ĸ���ļ�ʱ����ƥ���ļ����а��������Һ͸�������ĸ���ļ���\r\n��������Ҫ�����������������Ч����"));
	//m_tool_tip.AddTool(GetDlgItem(IDC_SAVE_IN_OFFSET_TAG), _T("�����ƫ�Ʊ��浽offset��ǩ�У�ѡ������ʹ���޸ĵ�ʱ��ƫ�ƺ����׻ָ������ǲ������еĲ�������֧��offset��ǩ��"));
	//m_tool_tip.AddTool(GetDlgItem(IDC_SAVE_IN_TIME_TAG), _T("�����ƫ�Ʊ��浽ÿ��ʱ���ǩ�У�ѡ������ʹ���޸ĵ�ʱ��ƫ�Ʋ���ô���׻ָ������Ƕ������������ļ����Ժܺá�"));
	m_tool_tip.AddTool(GetDlgItem(IDC_LYRIC_PATH_EDIT), _T("˵���������������Ŀ¼���Ҳ���ƥ��ĸ���ļ����ͻ��ڴ��ļ�����Ѱ�Ҹ���ļ���"));
	m_tool_tip.AddTool(GetDlgItem(IDC_SHOW_LYRIC_IN_CORTANA), _T("��ѡ��󣬿����� Cortana ����������ʾ��ʡ��������ƺ�ר�����档�����˹��ܺ������Ҫ�����������������Ч��"));

	m_cortana_color_combo.AddString(_T("����ϵͳ"));
	m_cortana_color_combo.AddString(_T("��ɫ"));
	m_cortana_color_combo.AddString(_T("��ɫ"));
	m_cortana_color_combo.SetCurSel(m_data.cortana_color);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CLyricSettingsDlg::SetCortanaControlEnable(bool enable)
{
	m_lyric_double_line_chk.EnableWindow(enable);
	m_show_album_cover_in_cortana_check.EnableWindow(enable);
	m_cortana_color_combo.EnableWindow(enable);
	//if(m_data.cortana_show_album_cover)
	//	m_cortana_icon_beat_check.EnableWindow(FALSE);
	//else
	m_cortana_icon_beat_check.EnableWindow(enable);
}


void CLyricSettingsDlg::OnBnClickedStopWhenError()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CLyricSettingsDlg::OnBnClickedKaraokeDisp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_data.lyric_karaoke_disp = (m_karaoke_disp_check.GetCheck() != 0);
}


void CLyricSettingsDlg::OnBnClickedExploreLyricButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
#ifdef COMPILE_IN_WIN_XP
	CFolderBrowserDlg folderPickerDlg(this->GetSafeHwnd());
	folderPickerDlg.SetInfo(_T("��ѡ�����ļ��С�"));
#else
	CFolderPickerDialog folderPickerDlg(m_data.lyric_path.c_str());
	folderPickerDlg.m_ofn.lpstrTitle = _T("ѡ�����ļ���");		//���öԻ������
#endif // COMPILE_IN_WIN_XP
	if (folderPickerDlg.DoModal() == IDOK)
	{
		m_data.lyric_path = folderPickerDlg.GetPathName();
		if (m_data.lyric_path.back() != L'\\') m_data.lyric_path.push_back(L'\\');	//ȷ��·��ĩβ�з�б��
		SetDlgItemText(IDC_LYRIC_PATH_EDIT, m_data.lyric_path.c_str());
	}
}


void CLyricSettingsDlg::OnBnClickedShowTaskbarProgress()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CLyricSettingsDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnCancel();
}


void CLyricSettingsDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}


void CLyricSettingsDlg::OnBnClickedLyricFuzzyMatch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_data.lyric_fuzzy_match = (m_lyric_fuzzy_match_check.GetCheck() != 0);
}


BOOL CLyricSettingsDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_MOUSEMOVE)
		m_tool_tip.RelayEvent(pMsg);

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CLyricSettingsDlg::OnBnClickedShowLyricInCortana()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_data.show_lyric_in_cortana = (m_show_lyric_in_cortana_check.GetCheck() != 0);
	SetCortanaControlEnable(m_data.show_lyric_in_cortana);
}


//void CLyricSettingsDlg::OnBnClickedSaveInOffsetTag()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	m_data.save_lyric_in_offset = true;
//}


//void CLyricSettingsDlg::OnBnClickedSaveInTimeTag()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	m_data.save_lyric_in_offset = false;
//}


void CLyricSettingsDlg::OnBnClickedLyricDoubleLineCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_data.cortana_lyric_double_line = (m_lyric_double_line_chk.GetCheck() != 0);
}


void CLyricSettingsDlg::OnCbnSelchangeCortanaColorCombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_data.cortana_color = m_cortana_color_combo.GetCurSel();
}


void CLyricSettingsDlg::OnBnClickedShowAlbumCoverInCortana()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_data.cortana_show_album_cover = (m_show_album_cover_in_cortana_check.GetCheck() != 0);
	//m_cortana_icon_beat_check.EnableWindow(!m_data.cortana_show_album_cover);
}


void CLyricSettingsDlg::OnBnClickedCortanaIconDeatCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_data.cortana_icon_beat = (m_cortana_icon_beat_check.GetCheck() != 0);
}
