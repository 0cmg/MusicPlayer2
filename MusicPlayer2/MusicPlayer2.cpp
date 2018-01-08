
// MusicPlayer2.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "MusicPlayerDlg.h"
#include "HelpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMusicPlayerApp

BEGIN_MESSAGE_MAP(CMusicPlayerApp, CWinApp)
	//ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
	ON_COMMAND(ID_HELP, &CMusicPlayerApp::OnHelp)
END_MESSAGE_MAP()


// CMusicPlayerApp ����

CMusicPlayerApp::CMusicPlayerApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CMusicPlayerApp ����

CMusicPlayerApp theApp;


// CMusicPlayerApp ��ʼ��

BOOL CMusicPlayerApp::InitInstance()
{
	//�滻���Ի�������Ĭ������
	WNDCLASS wc;
	::GetClassInfo(AfxGetInstanceHandle(), _T("#32770"), &wc);	//MFCĬ�ϵ����жԻ��������Ϊ#32770
	wc.lpszClassName = _T("MusicPlayer_l3gwYT");	//���Ի���������޸�Ϊ������
	AfxRegisterClass(&wc);

	//������Windows��������ʱ��ֱ���˳�����
	wstring cmd_line{ m_lpCmdLine };
	if (cmd_line.find(L"RestartByRestartManager") != wstring::npos)
	{
		//�������в���д����־�ļ�
		wchar_t buff[256];
		swprintf_s(buff, L"�����ѱ�Windows��RestartManager����������������%s���������˳���", cmd_line.c_str());
		CCommon::WriteLog((CCommon::GetExePath() + L"error.log").c_str(), wstring{ buff });
		return FALSE;
	}

	//����Ƿ�����ʵ���������У�Debugʱ����飩
#ifndef _DEBUG
	HANDLE hMutex = ::CreateMutex(NULL, TRUE, _T("bXS1E7joK0Kh"));		//ʹ��һ��������ַ�������һ��������
	if (hMutex != NULL)
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS)		//����������ʧ�ܣ�˵���Ѿ���һ�������ʵ����������
		{
			//AfxMessageBox(_T("�Ѿ���һ�������������С�"));
			HWND handle = FindWindow(_T("MusicPlayer_l3gwYT"), NULL);		//������������������ʵ�����ڵľ��
			if (handle != NULL)
			{
				HWND minidlg_handle = FindWindow(NULL, _T("MiniDlg_ByH87M"));	//���ݴ��ڱ�����ҡ�����ģʽ�����ڵľ��
				if (minidlg_handle == NULL)			//û���ҵ�������ģʽ�����ڣ��򼤻�������
				{
					ShowWindow(handle, SW_SHOWNORMAL);		//�����ʾ����
					SetForegroundWindow(handle);		//����������Ϊ����
				}
				else				//�ҵ��ˡ�����ģʽ�����ڣ��򼤻����ģʽ������
				{
					ShowWindow(minidlg_handle, SW_SHOWNORMAL);
					SetForegroundWindow(minidlg_handle);
				}
				return FALSE;		//�˳���ǰ����
			}
		}
	}
#endif

	m_config_path = CCommon::GetExePath() + L"config.ini";
	m_song_data_path = CCommon::GetExePath() + L"song_data.dat";
	m_recent_path_dat_path = CCommon::GetExePath() + L"recent_path.dat";
	m_desktop_path = CCommon::GetDesktopPath();
	LoadSongData();

	//�жϵ�ǰ����ϵͳ�Ƿ���Windows10�����������CMusicPlayerApp���еĳ�Ա�����У��Ա���ȫ�ַ�Χ�ڿɷ���
	m_is_windows10 = CCommon::IsWindows10OrLater();

	CColorConvert::Initialize();

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CMusicPlayerDlg dlg(cmd_line);
	//CMusicPlayerDlg dlg(L"D:\\����2\\Test\\1.wma \"D:\\����2\\Test\\Sweety - ӣ����.mp3\"");
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	SaveSongData();

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}



void CMusicPlayerApp::OnHelp()
{
	// TODO: �ڴ���������������
	//AfxMessageBox(_T("���˰���"), NULL, MB_ICONINFORMATION);
	CHelpDlg helpDlg;
	helpDlg.DoModal();
}

void CMusicPlayerApp::SaveSongData() const
{
	// �򿪻����½��ļ�
	CFile file;
	BOOL bRet = file.Open(m_song_data_path.c_str(),
		CFile::modeCreate | CFile::modeWrite);
	if (!bRet)		//���ļ�ʧ��
	{
		return;
	}
	// ����CArchive����
	CArchive ar(&file, CArchive::store);
	// д����
	ar << m_song_data.size();		//д��ӳ�������Ĵ�С
	for (auto& song_data : m_song_data)
	{
		ar << CString(song_data.first.c_str())		//����ӳ�������ļ����������ľ���·��
			//<< CString(song_data.second.lyric_file.c_str())
			<< song_data.second.lengh.time2int()
			<< song_data.second.bitrate
			<< CString(song_data.second.title.c_str())
			<< CString(song_data.second.artist.c_str())
			<< CString(song_data.second.album.c_str())
			<< CString(song_data.second.year.c_str())
			<< CString(song_data.second.comment.c_str())
			<< CString(song_data.second.genre.c_str())
			<< song_data.second.track
			<< song_data.second.tag_type
			<< song_data.second.info_acquired;
	}
	// �ر�CArchive����
	ar.Close();
	// �ر��ļ�
	file.Close();

}

void CMusicPlayerApp::LoadSongData()
{
	// ���ļ�
	CFile file;
	BOOL bRet = file.Open(m_song_data_path.c_str(), CFile::modeRead);
	if (!bRet) return;
	// ����CArchive����
	CArchive ar(&file, CArchive::load);
	// ������
	size_t size{};
	SongInfo song_info;
	CString song_path;
	CString temp;
	int song_length;
	try
	{
		ar >> size;		//��ȡӳ�������ĳ���
		for (size_t i{}; i < size; i++)
		{
			ar >> song_path;
			//ar >> temp;
			//song_info.lyric_file = temp;
			ar >> song_length;
			song_info.lengh.int2time(song_length);
			ar >> song_info.bitrate;
			ar >> temp;
			song_info.title = temp;
			ar >> temp;
			song_info.artist = temp;
			ar >> temp;
			song_info.album = temp;
			ar >> temp;
			song_info.year = temp;
			ar >> temp;
			song_info.comment = temp;
			ar >> temp;
			song_info.genre = temp;
			ar >> song_info.track;
			ar >> song_info.tag_type;
			m_song_data[wstring{ song_path }] = song_info;		//����ȡ����һ�׸�����Ϣ��ӵ�ӳ��������
			ar >> song_info.info_acquired;
		}
	}
	catch(CArchiveException* exception)
	{
		CString info;
		info.Format(_T("��ȡ�����ļ�ʱ���������л��쳣���쳣���ͣ�CArchiveException��m_cause = %d��������song_data.dat�ļ����𻵻�汾��ƥ����ɵġ�"), exception->m_cause);
//		info.Format(_T("���棺��ȡ�����ļ�ʱ���������л��쳣���쳣���ͣ�CArchiveException��m_cause = %d��\
//������song_data.dat�ļ����𻵻�汾��ƥ����ɵģ�����Ժ��������Ϣ����Ϊsong_data.dat���������ɡ�"), exception->m_cause);
//		AfxMessageBox(info, MB_ICONWARNING);
		CCommon::WriteLog((CCommon::GetExePath() + L"error.log").c_str(), wstring{ info });
	}
	// �رն���
	ar.Close();
	// �ر��ļ�
	file.Close();
}

