// HelpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "HelpDlg.h"
#include "afxdialogex.h"


// CHelpDlg �Ի���

IMPLEMENT_DYNAMIC(CHelpDlg, CDialog)

CHelpDlg::CHelpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_HELP_DIALOG, pParent)
{

}

CHelpDlg::~CHelpDlg()
{
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HELP_EDIT, m_help_edit);
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialog)
END_MESSAGE_MAP()


// CHelpDlg ��Ϣ�������


BOOL CHelpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//help_info.LoadString(IDS_HELP_INFO);
	GetHelpString();
	m_help_edit.SetWindowText(m_help_info);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CHelpDlg::GetHelpString()
{
	HRSRC hRes = FindResource(NULL,MAKEINTRESOURCE(IDR_TEXT1), _T("Text"));
	if (hRes == NULL)
	{
		FreeResource(hRes);
	}
	else
	{
		HGLOBAL hglobal = LoadResource(NULL, hRes);
		if (hglobal == NULL)
		{
			FreeResource(hglobal);
		}
		else
		{
			//get text
			m_help_info.Format(_T("%s"), (LPVOID)hglobal);
		}
	}
}