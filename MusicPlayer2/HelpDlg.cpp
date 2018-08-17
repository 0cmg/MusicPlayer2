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
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CHelpDlg ��Ϣ�������


BOOL CHelpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//help_info.LoadString(IDS_HELP_INFO);

	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), FALSE);		// ����Сͼ��

	//��ȡ��ʼʱ���ڵĴ�С
	CRect rect;
	GetWindowRect(rect);
	m_min_size.cx = rect.Width();
	m_min_size.cy = rect.Height();

	GetHelpString();
	wstring help_info{ m_help_info.GetString() };

	//�ڰ����ĵ������֧�ֵ���Ƶ��ʽ
	wstring str_support_format;
	str_support_format += L"��֧�ֵ���Ƶ��ʽ��\r\n";
	str_support_format += L"������Ƶ��ʽ������BASS��������λ��\".\\Plugins\"Ŀ¼�£���\r\n";
	for (const auto support_format : CAudioCommon::m_surpported_format)
	{
		str_support_format += support_format.description;
		str_support_format += L": ";
		str_support_format += support_format.extensions_list;
		str_support_format += L"\r\n";
	}
	str_support_format += L"\r\n";
	size_t index = help_info.find(L"������*˵����");		//����ڡ�����˵����֮ǰ
	if (index != wstring::npos)
	{
		help_info.insert(index, str_support_format);
	}
	else
	{
		help_info += L"\r\n";
		help_info += str_support_format;
	}

	m_help_edit.SetWindowText(help_info.c_str());
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CHelpDlg::GetHelpString()
{
	HRSRC hRes = FindResource(NULL,MAKEINTRESOURCE(IDR_TEXT1), _T("Text"));
	if (hRes != NULL)
	{
		HGLOBAL hglobal = LoadResource(NULL, hRes);
		if (hglobal != NULL)
			m_help_info.Format(_T("%s"), (LPVOID)hglobal);
	}
}

void CHelpDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//���ƴ�����С��С
	lpMMI->ptMinTrackSize.x = m_min_size.cx;		//������С���
	lpMMI->ptMinTrackSize.y = m_min_size.cy;		//������С�߶�

	CDialog::OnGetMinMaxInfo(lpMMI);
}
