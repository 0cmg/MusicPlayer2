// PlaySettingsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "PlaySettingsDlg.h"
#include "afxdialogex.h"


// CPlaySettingsDlg �Ի���

IMPLEMENT_DYNAMIC(CPlaySettingsDlg, CDialogEx)

CPlaySettingsDlg::CPlaySettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PLAY_SETTING_DIALOG, pParent)
{

}

CPlaySettingsDlg::~CPlaySettingsDlg()
{
}

void CPlaySettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STOP_WHEN_ERROR, m_stop_when_error_check);
	DDX_Control(pDX, IDC_KARAOKE_DISP, m_karaoke_disp_check);
	DDX_Control(pDX, IDC_SHOW_TASKBAR_PROGRESS, m_show_taskbar_progress_check);
	DDX_Control(pDX, IDC_LYRIC_FUZZY_MATCH, m_lyric_fuzzy_match_check);
	DDX_Control(pDX, IDC_SHOW_LYRIC_IN_CORTANA, m_show_lyric_in_cortana_check);
}


BEGIN_MESSAGE_MAP(CPlaySettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_STOP_WHEN_ERROR, &CPlaySettingsDlg::OnBnClickedStopWhenError)
	ON_BN_CLICKED(IDC_KARAOKE_DISP, &CPlaySettingsDlg::OnBnClickedKaraokeDisp)
	ON_BN_CLICKED(IDC_EXPLORE_LYRIC_BUTTON, &CPlaySettingsDlg::OnBnClickedExploreLyricButton)
	ON_BN_CLICKED(IDC_SHOW_TASKBAR_PROGRESS, &CPlaySettingsDlg::OnBnClickedShowTaskbarProgress)
	ON_BN_CLICKED(IDC_LYRIC_FUZZY_MATCH, &CPlaySettingsDlg::OnBnClickedLyricFuzzyMatch)
	ON_BN_CLICKED(IDC_SHOW_LYRIC_IN_CORTANA, &CPlaySettingsDlg::OnBnClickedShowLyricInCortana)
	ON_BN_CLICKED(IDC_SAVE_IN_OFFSET_TAG, &CPlaySettingsDlg::OnBnClickedSaveInOffsetTag)
	ON_BN_CLICKED(IDC_SAVE_IN_TIME_TAG, &CPlaySettingsDlg::OnBnClickedSaveInTimeTag)
END_MESSAGE_MAP()


// CPlaySettingsDlg ��Ϣ�������


BOOL CPlaySettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetBackgroundColor(RGB(255, 255, 255));

	//��ʼ�����ؼ���״̬
	m_stop_when_error_check.SetCheck(m_stop_when_error);
	m_karaoke_disp_check.SetCheck(m_karaoke_disp);
	m_show_taskbar_progress_check.SetCheck(m_show_taskbar_progress);
	m_lyric_fuzzy_match_check.SetCheck(m_lyric_fuzzy_match);
	if (theApp.m_is_windows10)
	{
		m_show_lyric_in_cortana_check.SetCheck(m_show_lyric_in_cortana);
	}
	else
	{
		m_show_lyric_in_cortana_check.EnableWindow(FALSE);		//Win10����ϵͳ���ô˸�ѡ��ť
		m_show_lyric_in_cortana = false;
	}

	if(m_save_lyric_in_offset)
		((CButton*)GetDlgItem(IDC_SAVE_IN_OFFSET_TAG))->SetCheck(TRUE);
	else
		((CButton*)GetDlgItem(IDC_SAVE_IN_TIME_TAG))->SetCheck(TRUE);

	SetDlgItemText(IDC_LYRIC_PATH_EDIT, m_lyric_path.c_str());

	m_tool_tip.Create(this);
	m_tool_tip.SetMaxTipWidth(300);
	m_tool_tip.AddTool(&m_lyric_fuzzy_match_check, _T("���ȥ���˸�ѡ��Ĺ�ѡ����ֻ��ƥ��͸����ļ�����ȫһ���ĸ���ļ���\r\n���ѡ�д˸�ѡ�򣬵��Ҳ����ļ�����ȫһ���ĸ���ļ�ʱ����ƥ���ļ����а��������Һ͸�������ĸ���ļ���\r\n��������Ҫ�����������������Ч����"));
	m_tool_tip.AddTool(GetDlgItem(IDC_SAVE_IN_OFFSET_TAG), _T("�����ƫ�Ʊ��浽offset��ǩ�У�ѡ������ʹ���޸ĵ�ʱ��ƫ�ƺ����׻ָ������ǲ������еĲ�������֧��offset��ǩ��"));
	m_tool_tip.AddTool(GetDlgItem(IDC_SAVE_IN_TIME_TAG), _T("�����ƫ�Ʊ��浽ÿ��ʱ���ǩ�У�ѡ������ʹ���޸ĵ�ʱ��ƫ�Ʋ���ô���׻ָ������Ƕ������������ļ����Ժܺá�"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CPlaySettingsDlg::OnBnClickedStopWhenError()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_stop_when_error = (m_stop_when_error_check.GetCheck() != 0);
}


void CPlaySettingsDlg::OnBnClickedKaraokeDisp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_karaoke_disp = (m_karaoke_disp_check.GetCheck() != 0);
}


void CPlaySettingsDlg::OnBnClickedExploreLyricButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//TCHAR szPath[MAX_PATH];		//���ѡ���Ŀ¼·��
	//CString str;

	//BROWSEINFO bi;
	//bi.hwndOwner = m_hWnd;
	//bi.pidlRoot = NULL;
	//bi.pszDisplayName = szPath;
	//bi.lpszTitle = _T("��ѡ���Ÿ���ļ����ļ��С�");
	//bi.ulFlags = 0;
	//bi.lpfn = NULL;
	//bi.lParam = 0;
	//bi.iImage = 0;
	////����ѡ��Ŀ¼�Ի���
	//LPITEMIDLIST lp = SHBrowseForFolder(&bi);

	//if (lp)
	//{
	//	if (SHGetPathFromIDList(lp, szPath))
	//	{
	//		m_lyric_path = szPath;
	//		if (m_lyric_path.back() != L'\\') m_lyric_path.push_back(L'\\');	//ȷ��·��ĩβ�з�б��
	//		SetDlgItemText(IDC_LYRIC_PATH_EDIT, szPath);
	//	}
	//	else
	//		AfxMessageBox(_T("��Ч��Ŀ¼��������ѡ��"));
	//}

	CFolderPickerDialog folderPickerDlg(m_lyric_path.c_str());
	folderPickerDlg.m_ofn.lpstrTitle = _T("ѡ�����ļ���");		//���öԻ������
	if (folderPickerDlg.DoModal() == IDOK)
	{
		m_lyric_path = folderPickerDlg.GetPathName();
		if (m_lyric_path.back() != L'\\') m_lyric_path.push_back(L'\\');	//ȷ��·��ĩβ�з�б��
		SetDlgItemText(IDC_LYRIC_PATH_EDIT, m_lyric_path.c_str());
	}
}


void CPlaySettingsDlg::OnBnClickedShowTaskbarProgress()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_show_taskbar_progress = (m_show_taskbar_progress_check.GetCheck() != 0);
}


void CPlaySettingsDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnCancel();
}


void CPlaySettingsDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}


void CPlaySettingsDlg::OnBnClickedLyricFuzzyMatch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_lyric_fuzzy_match = (m_lyric_fuzzy_match_check.GetCheck() != 0);
}


BOOL CPlaySettingsDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_MOUSEMOVE)
		m_tool_tip.RelayEvent(pMsg);

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CPlaySettingsDlg::OnBnClickedShowLyricInCortana()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_show_lyric_in_cortana = (m_show_lyric_in_cortana_check.GetCheck() != 0);
}


void CPlaySettingsDlg::OnBnClickedSaveInOffsetTag()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_save_lyric_in_offset = true;
}


void CPlaySettingsDlg::OnBnClickedSaveInTimeTag()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_save_lyric_in_offset = false;
}
