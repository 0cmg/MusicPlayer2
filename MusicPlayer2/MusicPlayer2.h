
// MusicPlayer2.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "Player.h"
#include "AudioCommon.h"
#include "ColorConvert.h"


// CMusicPlayerApp: 
// �йش����ʵ�֣������ MusicPlayer2.cpp
//

class CMusicPlayerApp : public CWinApp
{
public:
	CMusicPlayerApp();

	int m_dpi;
	bool m_is_windows10;		//��ǰϵͳ�Ƿ���Windows10
	CPlayer m_player;

	wstring m_config_path;		//�����ļ���·��
	wstring m_song_data_path;	//�������и�����Ϣ�����ļ���·��
	wstring m_recent_path_dat_path;	//"recent_path.dat"�ļ���·��
	wstring m_desktop_path;		//�����·��

	map<wstring, SongInfo> m_song_data;		//�������и�����Ϣ���ݵ�ӳ������������ÿһ����Ƶ�ļ��ľ���·����������ÿһ����Ƶ�ļ�����Ϣ
	//COLORREF m_theme_color;		//������ɫ
	ColorTable m_theme_color;		//������ɫ
	int m_sprctrum_height{ 100 };		//Ƶ�׷����߶ȱ�����%��

	volatile bool m_lyric_download_dialog_exit{ false };		//����ָʾ������ضԻ����Ѿ��˳�
	volatile bool m_batch_download_dialog_exit{ false };		//����ָʾ����������ضԻ����Ѿ��˳�

	//CMutex m_mutex{0,_T("playlist")};		//�������Ķ���

	void SaveSongData() const;		//�����и�����Ϣ�����л��ķ�ʽ���浽�ļ�

private:
	void LoadSongData();			//���ļ��������л��ķ�ʽ��ȡ���и�����Ϣ

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	afx_msg void OnHelp();
};

extern CMusicPlayerApp theApp;