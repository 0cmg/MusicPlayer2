// DataSettingsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "DataSettingsDlg.h"
#include "afxdialogex.h"


// CDataSettingsDlg �Ի���

IMPLEMENT_DYNAMIC(CDataSettingsDlg, CDialogEx)

CDataSettingsDlg::CDataSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DATA_SETTINGS_DIALOG, pParent)
{

}

CDataSettingsDlg::~CDataSettingsDlg()
{
}

void CDataSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDataSettingsDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CLEAN_DATA_FILE_BUTTON, &CDataSettingsDlg::OnBnClickedCleanDataFileButton)
END_MESSAGE_MAP()


// CDataSettingsDlg ��Ϣ�������


BOOL CDataSettingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetBackgroundColor(RGB(255, 255, 255));

	m_data_size = CCommon::GetFileSize(theApp.m_song_data_path);
	ShowDataSizeInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CDataSettingsDlg::ShowDataSizeInfo()
{
	CString info;
	if (m_data_size < 1024)
		info.Format(_T("��ǰ�����ļ���С��%d���ֽ�"), m_data_size);
	else if (m_data_size < 1024 * 1024)
		info.Format(_T("��ǰ�����ļ���С��%.2fKB (%d���ֽ�)"), static_cast<float>(m_data_size) / 1024.0f, m_data_size);
	else
		info.Format(_T("��ǰ�����ļ���С��%.2fMB (%d���ֽ�)"), static_cast<float>(m_data_size) / 1024.0f / 1024.0f, m_data_size);		//ע���˴��������ڡ�%.2fMB��©���ˡ�f�����³�����һ������Ի�������ֹͣ�������������⡣
	SetDlgItemText(IDC_SIZE_STATIC, info);
}



void CDataSettingsDlg::OnBnClickedCleanDataFileButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	int clear_cnt{};		//ͳ��ɾ������Ŀ������
	//����ӳ��������ɾ������Ҫ����Ŀ��
	for (auto iter{ theApp.m_song_data.begin() }; iter != theApp.m_song_data.end();)
	{
		//������Ŀ��Ӧ���ļ����ڵ�·���Ƿ��ڡ��������·�����б���
		bool path_exist{ false };	//���iterָ�����Ŀ���ļ�·���ڡ��������·�����б�(theApp.m_player.GetRecentPath())���Ϊtrue
		wstring item_path;
		size_t index = iter->first.rfind(L'\\');
		item_path = iter->first.substr(0, index + 1);		//��ȡiterָ����Ŀ���ļ�Ŀ¼
		for (int i{}; i < theApp.m_player.GetRecentPath().size(); i++)
		{
			if (item_path == theApp.m_player.GetRecentPath()[i].path)
			{
				path_exist = true;
				break;
			}
		}
		//�������Ŀ��Ӧ���ļ����ڵ�·�����ڡ��������·�����б�������Ŀ��Ӧ���ļ������ڣ���ɾ������Ŀ
		if (!path_exist || !CCommon::FileExist(iter->first))
		{
			iter = theApp.m_song_data.erase(iter);		//ɾ����Ŀ֮�󽫵�����ָ��ɾ����Ŀ��ǰһ����Ŀ
			clear_cnt++;
		}
		else
		{
			iter++;
		}
	}
	theApp.SaveSongData();		//���������д���ļ�

	size_t data_size = CCommon::GetFileSize(theApp.m_song_data_path);	 //����������ļ��Ĵ�С
	int size_reduced = m_data_size - data_size;		//����������ļ����ٵ��ֽ���
	if (size_reduced < 0) size_reduced = 0;
	CString info;
	info.Format(_T("������ɣ���ɾ��%d����Ŀ�������ļ���С��С%d���ֽڡ�"), clear_cnt, size_reduced);
	MessageBox(info, NULL, MB_ICONINFORMATION);
	m_data_size = data_size;
	ShowDataSizeInfo();
}
