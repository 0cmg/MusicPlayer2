
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
#include "DrawCommon.h"

//ѡ����������
struct PlaySettingData
{
	bool m_stop_when_error{ true };		//���ִ���ʱֹͣ����
	bool m_show_taskbar_progress{ false };	//����������ť����ʾ���Ž���
	bool m_lyric_karaoke_disp{ true };	//�����Ƿ��Կ���OK��ʽ��ʾ
	bool m_lyric_fuzzy_match{ true };	//���ģ��ƥ��
	bool m_save_lyric_in_offset{};	//�Ƿ񽫸�ʱ�����offset��ǩ�У����Ǳ�����ÿ��ʱ���ǩ��
	wstring m_lyric_path;	//����ļ��е�·��
	bool m_show_lyric_in_cortana{};	//�Ƿ���Cortana������������ʾ���
	bool m_cortana_lyric_double_line{ true };		//�Ƿ���Cortana��������˫����ʾ���
};

struct ApperanceSettingData
{
	wstring m_lyric_font_name;		//�����������
	int m_lyric_font_size;		//��������С
	int m_lyric_line_space{ 2 };	//��ʵ��м��
	int m_transparency{ 100 };			//����͸����
	ColorTable m_theme_color;		//������ɫ
	bool m_theme_color_follow_system{ true };		//������ɫ����ϵͳ����Win8����֧�֣�
	bool m_show_album_cover;			//��ʾר������
	CDrawCommon::StretchMode m_album_cover_fit{ CDrawCommon::StretchMode::FILL };		//ר���������϶ȣ�����ģʽ��
	bool m_album_cover_as_background{ false };		//��ר��������Ϊ����
	bool m_show_spectrum{ true };		//��ʾƵ�׷���
	int m_sprctrum_height{ 100 };		//Ƶ�׷����߶ȱ�����%��
	int m_background_transparency{ 80 };		//������͸����
	bool m_use_out_image{ true };			//ʹ���ⲿͼƬ��Ϊר������
};

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
	//wstring m_temp_path;		//��ʱ�ļ��е�·��

	map<wstring, SongInfo> m_song_data;		//�������и�����Ϣ���ݵ�ӳ������������ÿһ����Ƶ�ļ��ľ���·����������ÿһ����Ƶ�ļ�����Ϣ

	PlaySettingData m_play_setting_data;
	ApperanceSettingData m_app_setting_data;

	volatile bool m_lyric_download_dialog_exit{ false };		//����ָʾ������ضԻ����Ѿ��˳�
	volatile bool m_batch_download_dialog_exit{ false };		//����ָʾ����������ضԻ����Ѿ��˳�

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