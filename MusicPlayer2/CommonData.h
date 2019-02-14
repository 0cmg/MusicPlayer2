#pragma once
#include "stdafx.h"
#include "ColorConvert.h"
#include "DrawCommon.h"
#include "Common.h"
#include <functional>

namespace CONSTVAL
{
	const COLORREF BACKGROUND_COLOR = GRAY(255);		//���Ĵ���ɫ��ֵ�����޸������ڱ���ɫ
	const double FULL_SCREEN_ZOOM_FACTOR = 1.5;
}

struct DeviceInfo	//�����豸����Ϣ
{
	int index;		//�豸������
	wstring name;	//�豸������
	wstring driver;	//�豸����������
	DWORD flags;	//�豸��״̬
};

//ѡ����������
struct LyricSettingData
{
	bool lyric_karaoke_disp{ true };			//�����Ƿ��Կ���OK��ʽ��ʾ
	bool lyric_fuzzy_match{ true };				//���ģ��ƥ��
	bool save_lyric_in_offset{};				//�Ƿ񽫸�ʱ�����offset��ǩ�У����Ǳ�����ÿ��ʱ���ǩ��
	wstring lyric_path;							//����ļ��е�·��
	bool show_lyric_in_cortana{};				//�Ƿ���Cortana������������ʾ���
	bool cortana_lyric_double_line{ true };		//�Ƿ���Cortana��������˫����ʾ���
	int cortana_color{ 0 };						//Cortana������ı�����ɫ��0������ϵͳ��1����ɫ��2����ɫ��
	bool cortana_show_album_cover{ true };		//�Ƿ���Cortana��������ʾר������
	bool cortana_icon_beat{ true };				//Cortanaͼ�������ֽ�������
	bool cortana_lyric_compatible_mode{ false };	//Cortana����������ʾʹ�ü���ģʽ
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
	bool use_inner_image_first{ true };			//����ʹ����Ƕר������
	vector<wstring> default_album_name;			//Ĭ�ϵ�ר�������ļ���
	bool background_gauss_blur{ true };			//������˹ģ��
	int gauss_blur_radius{ 60 };				//��˹ģ���뾶*10
	bool lyric_background{ true };				//��ʽ��汳��
	bool dark_mode{ false };					//��ɫģʽ
};

struct GeneralSettingData
{
	bool id3v2_first{ false };					//���Ȼ�ȡID3V2��ǩ
	bool auto_download_lyric{ false };			//�Ƿ��Զ����ظ��
	bool auto_download_album_cover{ true };		//�Ƿ��Զ�����ר������
	bool auto_download_only_tag_full{ true };	//���ڸ�����Ϣ����ʱ�Զ�����
	bool check_update_when_start{ true };		//�Ƿ��ڳ�������ʱ������
	wstring sf2_path;							//MIDI��ɫ��·��
	bool midi_use_inner_lyric{ false };			//����MIDI����ʱ��ʾ��Ƕ���
	bool minimize_to_notify_icon{ false };		//�Ƿ���С��֪ͨ��ͼ��

	Language language;
};

struct PlaySettingData
{
	bool stop_when_error{ true };				//���ִ���ʱֹͣ����
	bool auto_play_when_start{ false };			//��������ʱ�Զ�����
	bool show_taskbar_progress{ false };		//����������ť����ʾ���Ž���
	bool show_playstate_icon{ true };			//����������ť����ʾ����״̬�ĽǱ�
	wstring output_device;						//�����豸������
	int device_selected{};
};

struct GlobalHotKeySettingData
{
	bool hot_key_enable = true;
	bool global_multimedia_key_enable{ true };	//�Ƿ���ȫ�ַ�Χ�����ö�ý���
};

struct NonCategorizedSettingData
{
	int volum_step{ 3 };			//����������е���������ʱһ�ε����Ĳ���
	int mouse_volum_step{ 2 };		//ͨ�������ֵ�������ʱ�Ĳ���
	int volume_map{ 100 };			//����ӳ�䣨���磺�������ֵ��100��Ϊ60������������Ϊ���100%��ʱ��������СΪԭ����60%��
	bool show_cover_tip{ true };	//�Ƿ���ʾר�������ϵ������ʾ
	//wstring default_back_image_path{};	//û��ר������ʱ��Ĭ�ϱ�����·��
	bool no_sf2_warning{ true };	//�Ƿ���û��MIDI��ɫ��ʱ������ʾ��Ϣ
};

struct IconRes
{
private:
	HICON hIcon;
	HICON hIconDark;
	HICON hIconLarge;
	HICON hIconDarkLarge;
	CSize iconSize;
	CSize iconSizeLarge;

public:
	const HICON& GetIcon(bool dark = false, bool large = false) const
	{
		if(large)
			return (dark && hIconDarkLarge != NULL ? hIconDarkLarge : hIconLarge);
		else
			return (dark && hIconDark != NULL ? hIconDark : hIcon);
	}

	void Load(UINT id, UINT id_dark, int size)
	{
		int size_large = static_cast<int>(size * CONSTVAL::FULL_SCREEN_ZOOM_FACTOR);

		if (id != 0)
		{
			hIcon = CDrawCommon::LoadIconResource(id, size, size);
			hIconLarge = CDrawCommon::LoadIconResource(id, size_large, size_large);
		}
		if (id_dark != 0)
		{
			hIconDark = CDrawCommon::LoadIconResource(id_dark, size, size);
			hIconDarkLarge = CDrawCommon::LoadIconResource(id_dark, size_large, size_large);
		}
		iconSize.cx = iconSize.cy = size;
		iconSizeLarge.cx = iconSizeLarge.cy = size_large;
	}

	const CSize& GetSize(bool large = false) const
	{
		return (large? iconSizeLarge : iconSize);
	}
};

struct IconSet
{
	IconRes default_cover;
	IconRes skin;
	IconRes eq;
	IconRes setting;
	IconRes mini;
	IconRes play_oder;
	IconRes play_shuffle;
	IconRes loop_playlist;
	IconRes loop_track;
	IconRes previous;
	IconRes play;
	IconRes pause;
	IconRes next;
	IconRes info;
	IconRes select_folder;
	IconRes show_playlist;
	IconRes find_songs;
	IconRes full_screen;

	IconRes stop_l;
	IconRes previous_l;
	IconRes play_l;
	IconRes pause_l;
	IconRes next_l;
};


struct UIData
{
	bool show_translate{ true };		//����Ƿ���ʾ����
	bool narrow_mode;					//խ����ģʽ
	bool show_playlist{ true };
	bool show_menu_bar{ true };
	bool full_screen{ false };

	int client_width;					//���ڿͻ������
	int client_height;					//���ڿͻ����߶�
	CImage default_background;			//Ĭ�ϵı���
	DisplayFormat display_format{};		//�����б�����Ŀ����ʾ��ʽ
};

struct UIFont
{
private:
	CFont font;
	CFont font_l;

public:
	void Load(int font_size)
	{
		if (font_size < 5)
			font_size = 5;
		font.CreatePointFont(font_size * 10, CCommon::LoadText(IDS_DEFAULT_FONT));
		font_l.CreatePointFont(static_cast<int>(font_size * 10 * CONSTVAL::FULL_SCREEN_ZOOM_FACTOR), CCommon::LoadText(IDS_DEFAULT_FONT));
	}

	CFont& GetFont(bool large = false)
	{
		return (large ? font_l : font);
	}

	void SetFont(int font_size, LPCTSTR name)
	{
		if (font_size < 5)
			font_size = 5;

		if (font.m_hObject)
			font.DeleteObject();
		font.CreatePointFont(font_size * 10, CCommon::LoadText(IDS_DEFAULT_FONT));

		if (font_l.m_hObject)
			font_l.DeleteObject();
		font_l.CreatePointFont(static_cast<int>(font_size * 10 * CONSTVAL::FULL_SCREEN_ZOOM_FACTOR), CCommon::LoadText(IDS_DEFAULT_FONT));
	}
};

struct FontSet
{
	UIFont normal;				//��ͨ������
	UIFont time;				//��ʾ����ʱ��
	UIFont title;				//����2�ĸ�������

	UIFont lyric;				//�������
	UIFont lyric_translate;		//��ʷ��������

	void Init()
	{
		normal.Load(9);
		time.Load(8);
		title.Load(10);
	}
};

