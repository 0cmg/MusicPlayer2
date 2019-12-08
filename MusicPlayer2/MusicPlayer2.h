
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
#include "IniHelper.h"
#include "WinVersionHelper.h"
#include "CHotkeyManager.h"
#include "CommonData.h"
#include "MediaLibHelper.h"


// CMusicPlayerApp:
// �йش����ʵ�֣������ MusicPlayer2.cpp
//

class CMusicPlayerApp : public CWinApp
{
public:
    CMusicPlayerApp();

    wstring m_module_dir;		//��ǰ����exe�ļ�����Ŀ¼
    wstring m_local_dir;		//��ǰĿ¼��debugģʽ��Ϊ.\��releaseģʽ��Ϊexe�ļ�����Ŀ¼��
    wstring m_playlist_dir;     //�����б�Ŀ¼
    wstring m_config_path;		//�����ļ���·��
    wstring m_song_data_path;	//�������и�����Ϣ�����ļ���·��
    wstring m_recent_path_dat_path;	//"recent_path.dat"�ļ���·��
    wstring m_recent_playlist_data_path;
    wstring m_desktop_path;		//�����·��
    //wstring m_temp_path;		//��ʱ�ļ��е�·��

    std::unordered_map<wstring, SongInfo> m_song_data;		//�������и�����Ϣ���ݵ�ӳ������������ÿһ����Ƶ�ļ��ľ���·����������ÿһ����Ƶ�ļ�����Ϣ
    vector<DeviceInfo> m_output_devices;	//�����豸����Ϣ

    CMediaClassifier m_artist_classifer{ CMediaClassifier::CT_ARTIST, true };     //�����и�����Ϣ�������ҷ���
    CMediaClassifier m_album_classifer{ CMediaClassifier::CT_ALBUM, true };       //�����и�����Ϣ����Ƭ������
    CMediaClassifier m_genre_classifer{ CMediaClassifier::CT_GENRE, true };       //�����и�����Ϣ�����ɷ���

    LyricSettingData m_lyric_setting_data;			//��ѡ�����á��Ի����С�������á��е�����
    ApperanceSettingData m_app_setting_data;		//��ѡ�����á��Ի����С�������á��е�����
    GeneralSettingData m_general_setting_data;		//��ѡ�����á��Ի����С��������á��е�����
    PlaySettingData m_play_setting_data;			//��ѡ�����á��Ի����С��������á��е�����
    NonCategorizedSettingData m_nc_setting_data;	//δ�������������
    GlobalHotKeySettingData m_hot_key_setting_data;	//��ȫ�ֿ�ݼ�������
    CHotkeyManager m_hot_key;

    UIData m_ui_data;
    IconSet m_icon_set;			//ͼ����Դ
    FontSet m_font_set;			//������Դ
    MenuSet m_menu_set;			//�˵���Դ

    volatile bool m_lyric_download_dialog_exit{ true };		//����ָʾ������ضԻ����Ѿ��˳�
    volatile bool m_batch_download_dialog_exit{ true };		//����ָʾ����������ضԻ����Ѿ��˳�
    volatile bool m_cover_download_dialog_exit{ true };		//����ָʾ������ضԻ����Ѿ��˳�
    volatile bool m_format_convert_dialog_exit{ true };		//����ָʾ��ʽ�Ի����Ѿ��˳�

    void SaveSongData();		//�����и�����Ϣ�����л��ķ�ʽ���浽�ļ�

    static void CheckUpdate(bool message);

    void SaveConfig();
    void LoadConfig();

    void SaveSongInfo(const SongInfo& song_info);       //��һ��������Ϣ���浽m_song_data��

    void LoadIconResource();
    void InitMenuResourse();

    int DPI(int pixel);		//��һ������ֵ����DPI�任
    int DPI(double pixel);
    //����DPI�任���������봦��
    //round��roundΪ0.5ʱ�������룬roundΪ0.4ʱΪ��������
    int DPIRound(double pixel, double round = 0.5);		//�Խ�������������봦��
    void GetDPIFromWindow(CWnd* pWnd);

    int GetDPI()
    {
        return m_dpi;
    }

    WORD GetCurrentLanguage() const;
    bool IsGlobalMultimediaKeyEnabled() const;

    //��ȡ�����ı�
    CString GetHelpString();

    //��ȡϵͳ��Ϣ�ı�
    CString GetSystemInfoString();

    void SetSongDataModified();
    bool IsSongDataModified() const;

    void WriteErrorLog(const wstring& log_str);

    void StartClassifySongData();

private:
    void LoadSongData();			//���ļ��������л��ķ�ʽ��ȡ���и�����Ϣ

    static LRESULT CALLBACK MultiMediaKeyHookProc(int nCode, WPARAM wParam, LPARAM lParam);
    static UINT CheckUpdateThreadFunc(LPVOID lpParam);	//����ʱ�������̺߳���
    static UINT ClassifySongDataThreadFunc(LPVOID lpParam);	//������������̺߳���

private:
    HHOOK m_multimedia_key_hook = NULL;

    int m_dpi{};
    bool m_song_data_modified{ false };

	ULONG_PTR m_gdiplusToken;

// ��д
public:
    virtual BOOL InitInstance();

// ʵ��

    DECLARE_MESSAGE_MAP()
    afx_msg void OnHelp();
	virtual int ExitInstance();
};

extern CMusicPlayerApp theApp;