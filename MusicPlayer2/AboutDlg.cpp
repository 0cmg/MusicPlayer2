#include "stdafx.h"
#include "MusicPlayer2.h"
#include "AboutDlg.h"

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CAboutDlg::OnNMClickSyslink1)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK2, &CAboutDlg::OnNMClickSyslink2)
	ON_NOTIFY(NM_CLICK, IDC_GITHUB_SYSLINK, &CAboutDlg::OnNMClickGithubSyslink)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	CString version_info;
	GetDlgItemText(IDC_STATIC_VERSION, version_info);
	version_info.Replace(_T("<version>"), VERSION);
#ifdef COMPILE_IN_WIN_XP
	version_info += _T(" (For WinXP)");
#endif // COMPILE_FOR_WINXP
	SetDlgItemText(IDC_STATIC_VERSION, version_info);

	//��������������
	CString temp_str;
	GetDlgItemText(IDC_STATIC_COPYRIGHT, temp_str);
	temp_str.Replace(_T("<compile_date>"), COMPILE_DATE);
	SetDlgItemText(IDC_STATIC_COPYRIGHT, temp_str);

	m_tool_tip.Create(this);
	m_tool_tip.AddTool(GetDlgItem(IDC_SYSLINK1), CCommon::LoadText(IDS_SEND_EMAIL_TO_ATHOUR, _T("\r\nmailto:zhongyang219@hotmail.com")));
	m_tool_tip.AddTool(GetDlgItem(IDC_GITHUB_SYSLINK), CCommon::LoadText(IDS_GOTO_GITHUB, _T("\r\nhttps://github.com/zhongyang219/MusicPlayer2")));
	m_tool_tip.SetDelayTime(300);	//�����ӳ�
	m_tool_tip.SetMaxTipWidth(theApp.DPI(400));

	//if (theApp.m_is_windows10)
	//	SetDlgItemText(IDC_DEBUG_INFO_STATIC, _T("Windows10"));
	//else
	//	SetDlgItemText(IDC_DEBUG_INFO_STATIC, _T("Not Windows10"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CAboutDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����ˡ���ϵ���ߡ�
	ShellExecute(NULL, _T("open"), _T("mailto:zhongyang219@hotmail.com"), NULL, NULL, SW_SHOW);	//�򿪳�����
	*pResult = 0;
}

void CAboutDlg::OnNMClickSyslink2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����ˡ������¡�
	theApp.CheckUpdate(true);
	*pResult = 0;
}

void CAboutDlg::OnNMClickGithubSyslink(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShellExecute(NULL, _T("open"), _T("https://github.com/zhongyang219/MusicPlayer2/"), NULL, NULL, SW_SHOW);	//�򿪳�����
	*pResult = 0;
}

BOOL CAboutDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_MOUSEMOVE)
		m_tool_tip.RelayEvent(pMsg);


	if (pMsg->message == WM_KEYDOWN)
	{
		//���ڽ��水��Ctrl + shift + 1��ʾ������Ϣ
		if ((GetKeyState(VK_CONTROL) & 0x80) && (GetKeyState(VK_SHIFT) & 0x8000) && pMsg->wParam == '1')
		{
			CString info{ _T("Debug Info:\r\n") };
			
			CString strTmp;
			strTmp.Format(_T("Windows Version: %d.%d build %d\r\n"), theApp.m_win_version.GetMajorVersion(), 
				theApp.m_win_version.GetMinorVersion(), theApp.m_win_version.GetBuildNumber());
			info += strTmp;
			
			strTmp.Format(_T("DPI: %d"), theApp.m_dpi);
			info += strTmp;

			MessageBox(info, NULL, MB_ICONINFORMATION | MB_OK);
			return TRUE;
		}
	}


	return CDialog::PreTranslateMessage(pMsg);
}

