
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
	bool stop_when_error{ true };				//���ִ���ʱֹͣ����
	bool show_taskbar_progress{ false };		//����������ť����ʾ���Ž���
	bool show_playstate_icon{ true };			//����������ť����ʾ����״̬�ĽǱ�
	bool lyric_karaoke_disp{ true };			//�����Ƿ��Կ���OK��ʽ��ʾ
	bool lyric_fuzzy_match{ true };				//���ģ��ƥ��
	bool save_lyric_in_offset{};				//�Ƿ񽫸�ʱ�����offset��ǩ�У����Ǳ�����ÿ��ʱ���ǩ��
	wstring lyric_path;							//����ļ��е�·��
	bool show_lyric_in_cortana{};				//�Ƿ���Cortana������������ʾ���
	bool cortana_lyric_double_line{ true };		//�Ƿ���Cortana��������˫����ʾ���
	int cortana_color{ 0 };						//Cortana������ı�����ɫ��0������ϵͳ��1����ɫ��2����ɫ��
	bool cortana_show_album_cover{ true };		//�Ƿ���Cortana��������ʾר������
	bool cortana_icon_beat{ true };				//Cortanaͼ�������ֽ�������
};

struct ApperanceSettingData
{
	wstring lyric_font_name;					//�����������
	int lyric_font_size;						//��������С
	int lyric_line_space{ 2 };					//��ʵ��м��
	int window_transparency{ 100 };				//����͸����
	ColorTable theme_color;						//������ɫ
	bool theme_color_follow_system{ true };		//������ɫ����ϵͳ����Win8����֧�֣�
	bool show_album_cover;						//��ʾר������
	CDrawCommon::StretchMode album_cover_fit{ CDrawCommon::StretchMode::FILL };		//ר���������϶ȣ�����ģʽ��
	bool album_cover_as_background{ false };	//��ר��������Ϊ����
	bool show_spectrum{ true };					//��ʾƵ�׷���
	int sprctrum_height{ 100 };					//Ƶ�׷����߶ȱ�����%��
	int background_transparency{ 80 };			//������͸����
	bool use_out_image{ true };					//ʹ���ⲿͼƬ��Ϊר������
	vector<wstring> default_album_name;				//Ĭ�ϵ�ר�������ļ���
};

struct OtherSettingData
{
	bool id3v2_first{ false };		//���Ȼ�ȡID3V2��ǩ
};

struct NonCategorizedSettingData
{
	int volum_step{ 3 };			//����������е���������ʱһ�ε����Ĳ���
	int mouse_volum_step{ 2 };		//ͨ�������ֵ�������ʱ�Ĳ���
	int volume_map{ 100 };			//����ӳ�䣨���磺�������ֵ��100��Ϊ60������������Ϊ���100%��ʱ��������СΪԭ����60%��
	bool show_cover_tip{ true };	//�Ƿ���ʾר�������ϵ������ʾ
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

	PlaySettingData m_play_setting_data;		//��ѡ�����á��Ի����С����������á��е�����
	ApperanceSettingData m_app_setting_data;		//��ѡ�����á��Ի����С��������������á��е�����
	OtherSettingData m_other_setting_data;
	NonCategorizedSettingData m_nc_setting_data;	//δ�������������

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