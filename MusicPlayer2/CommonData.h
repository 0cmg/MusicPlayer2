#pragma once
#include "stdafx.h"
#include "ColorConvert.h"
#include "DrawCommon.h"
#include "Common.h"

namespace CONSTVAL
{
	const COLORREF BACKGROUND_COLOR = GRAY(255);		//���Ĵ���ɫ��ֵ�����޸������ڱ���ɫ
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
	CSize iconSize;

public:
	const HICON& GetIcon(bool dark = false) const
	{
		return (dark ? hIconDark : hIcon);
	}

	void Load(UINT id, UINT id_dark, int size)
	{
		if (id != 0)
			hIcon = CDrawCommon::LoadIconResource(id, size, size);
		if (id_dark != 0)
			hIconDark = CDrawCommon::LoadIconResource(id_dark, size, size);
		iconSize.cx = iconSize.cy = size;
	}

	const CSize& GetSize() const
	{
		return iconSize;
	}
};


struct UIData
{
	CFont lyric_font;					//�������
	CFont lyric_translate_font;			//��ʷ��������
	bool show_translate{ true };		//����Ƿ���ʾ����
	bool narrow_mode;					//խ����ģʽ
	bool show_playlist{ true };
	bool show_menu_bar{ true };

	int client_width;					//���ڿͻ������
	int client_height;					//���ڿͻ����߶�
	CImage default_background;			//Ĭ�ϵı���
	DisplayFormat display_format{};		//�����б�����Ŀ����ʾ��ʽ
};
