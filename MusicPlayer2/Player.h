#pragma once
#include"Common.h"
#include"Lyric.h"
#include"SetPathDlg.h"
#include"AudioCommon.h"
//#include"MusicPlayerDlg.h"

#define WM_PLAYLIST_INI_COMPLATE (WM_USER+104)		//�����б���������Ϣ
#define WM_SET_TITLE (WM_USER+105)					//���ô��ڱ������Ϣ

class CPlayer
{
public:
	//�������ʼ�������б�����Ϣ�Ľṹ��
	struct ThreadInfo
	{
		//��������������
		vector<SongInfo>* playlist;
		//�������룺
		wstring path;
		bool refresh_info;
		bool sort;		//ָʾ�����겥���б���Ƿ�Ҫ����
		//HSTREAM music_stream;
		//�������
		int process_percent;
	};
	//��ʼ�������б�Ĺ����̺߳���
	static UINT IniPlaylistThreadFunc(LPVOID lpParam);
	ThreadInfo m_thread_info;

private:
	CWinThread* m_pThread{};		//��ʼ�������б���߳�

	HSTREAM m_musicStream{};		//��ǰ����Ƶ���

	vector<SongInfo> m_playlist;		//�����б�����ÿ�������ļ��ĸ�����Ϣ
	wstring m_path;		//��ǰ�����ļ���·��
	wstring m_current_file_name;		//���ڲ��ŵ��ļ���
	deque<PathInfo> m_recent_path;		//����򿪹���·��

	Time m_song_length;		//���ڲ��ŵ��ļ��ĳ���
	Time m_current_position;		//��ǰ���ŵ���λ��
	int m_song_length_int;		//���ڲ��ŵ��ļ��ĳ��ȣ�int���ͣ�
	int m_current_position_int;		//��ǰ���ŵ���λ�ã�int���ͣ�
	int m_total_time;		//�����б���������Ŀ��ʱ�䣨���룩

	int m_index{ 0 };	//��ǰ�������ֵ����
	int m_index_tmp{ 0 };
	int m_song_num{ 0 };	//�����б��еĸ�������
	int m_error_code{ 0 };	//����������
	int m_playing{ 0 };		//���ڲ��ű�־��0����ֹͣ��1������ͣ��2�����ڲ��ţ�
	RepeatMode m_repeat_mode;		//ѭ��ģʽ��0��˳�򲥷ţ�1��������ţ�2���б�ѭ����3������ѭ����
	int m_volume{ 100 };		//�������ٷֱȣ�

	float m_fft[FFT_SAMPLE];		//����Ƶ�׷���������
	float m_spectral_data[FFT_NUM]{};	//������ʾ��ÿ��Ƶ�����εĸ߶�

	int m_fx_handle[FX_CH_NUM]{};		//������ͨ���ľ��
	const float FREQ_TABLE[FX_CH_NUM]{ 80, 125, 250, 500, 1000, 1500, 2000, 4000, 8000, 1600};		//ÿ��������ͨ��������Ƶ��
	int m_equalizer_gain[FX_CH_NUM]{};		//���ڱ������úõ�ÿ��ͨ��������
	bool m_equ_enable{ true };		//ָʾ�Ƿ����������
	int m_equ_style{};

public:
	CLyrics m_Lyrics;		//���
	wstring m_lyric_path;	//����ļ��е�·��

	bool m_lyric_karaoke_disp{ true };	//�����Ƿ��Կ���OK��ʽ��ʾ
	bool m_lyric_fuzzy_match{ true };	//���ģ��ƥ��

	SortMode m_sort_mode;		//����ʽ

	bool m_loading{ false };		//����������벥���б���Ϊtrue

private:
	vector<int> m_find_result;		//������ҽ���ĸ������

	vector<wstring> m_current_path_lyrics;	//���浱ǰ·���µĸ���ļ����ļ��������ڸ��ģ��ƥ��ʱ������
	vector<wstring> m_lyric_path_lyrics;		//�������ļ����µĸ���ļ����ļ��������ڸ��ģ��ƥ��ʱ������

	void IniBASS();			//��ʼ��BASS��Ƶ��
	void IniPlayList(bool cmd_para = false, bool refresh_info = false);	//��ʼ�������б�(�������cmd_paraΪtrue����ʾ��������ֱ�ӻ�ȡ�����ļ��������Ǵ�ָ��·����������
																		//���refresh_infoΪtrue���򲻹�theApp.m_song_data���Ƿ��е�ǰ��������Ϣ�������ļ����»�ȡ��Ϣ)

	void ChangePath(const wstring& path, int track = 0);		//�ı䵱ǰ·��

	void SetFXHandle();		//���þ�����ͨ���ľ��
	void RemoveFXHandle();		//�Ƴ�������ͨ���ľ��
	void ApplyEqualizer(int channel, int gain);		//Ӧ��һ��������ͨ��������

	void EmplaceCurrentPathToRecent();		//����ǰ·�����뵽���·����
	void LoadRecentPath();		//���ļ��������·���б�
public:
	void SaveRecentPath() const;		//�����·���б��浽�ļ�

	void SetEqualizer(int channel, int gain);		//���þ�������channelΪͨ����ȡֵΪ0~9��gainΪ���棬ȡֵΪ-15~15��
	int GeEqualizer(int channel);		//��ȡָ��������ͨ��������
	void SetAllEqualizer();			//������õ�ÿ��ͨ�������棨m_equalizer_gain�����õ�������
	void ClearAllEqulizer();		//��ÿ��������ͨ�������渴λ
	void EnableEqualizer(bool enable);			//����������
	bool GetEqualizerEnable() const { return m_equ_enable; }

public:
	CPlayer();
	~CPlayer();
	void Create();		//��ʼ��CPlayer��
	void Create(const vector<wstring>& files);	//��ʼ��CPlayer��
	void Create(const wstring & path);
	void MusicControl(Command command, int volume_step = 2);		//�������ֲ���
	void IniPlaylistComplate(bool sort);		//�����б�������ʱ�Ĵ���
	bool SongIsOver() const;			//�жϵ�ǰ�����Ƿ񲥷����
	void GetBASSCurrentPosition();		//��BASS��Ƶ���ȡ��ǰ���ŵ���λ��
	void GetBASSSongLength();			//��BASS��Ƶ���ȡ���ڲ����ļ��ĳ���
	static Time GetBASSSongLength(HSTREAM hStream);

	void GetBASSSpectral();		//Ƶ�׷���
	int GetCurrentSecond();		//��ȡ��ǰ���ŵ���λ�õ�����
	bool IsPlaying() const;			//�жϵ�ǰ�Ƿ����ڲ���

	bool PlayTrack(int song_track);		//����ָ����ŵĸ���

	void SetVolume();		//��m_volume��ֵ��������
	void SetPath(const wstring& path, int track, int position, SortMode sort_mode);		//����·��
	void OpenFolder(wstring path);	//ͨ�������ļ��С�������·���Ĵ���
	void OpenFiles(const vector<wstring>& files, bool play = true);	//��һ�������ļ���play���������Ƿ���������
	void SetRepeatMode();		//����ѭ��ģʽ
	void SetRepeatMode(RepeatMode repeat_mode);	//����ѭ��ģʽ
	RepeatMode GetRepeatMode() const;

	bool GetBASSError();		//��ȡBASS��Ƶ��Ĵ���
	bool IsError() const;				//�д���ʱ����ture�����򷵻�false

	void SetTitle() const;		//���ô��ڱ���

	void SaveConfig() const;		//�������õ�ini�ļ�
	void LoadConfig();		//��ini�ļ���ȡ����

	void SearchLyrics(/*bool refresh = false*/);		//��������ļ�(������refreshΪtrue���򲻹�theApp.m_song_data���Ƿ��е�ǰ�������ļ�·���������ļ����¼������)
	void IniLyrics();		//��ʼ�����
	void IniLyrics(const wstring& lyric_path);

	void ExplorePath(int track = -1) const;	//����Դ�������򿪵�ǰ·����ѡ��ָ��ָ���ļ�����trackС��0ʱѡ�е�ǰ���ڲ��ŵ��ļ���

	const vector<SongInfo>& GetPlayList() const { return m_playlist; }	//��ȡ�����б������
	Time GetAllSongLength(int track) const;				//��ȡָ����ŵĸ����ĳ���
	int GetSongNum() const { return m_song_num; }		//��ȡ��������
	const wstring& GetCurrentPath() const { return m_path; }	//��ȡ��ǰ·��
	int GetIndex() const { return m_index; }		//��ȡ��ǰ���ŵ���Ŀ���
	wstring GetFileName() const { return m_current_file_name; }
	wstring GetLyricName() const { return m_Lyrics.GetPathName(); }
	int GetVolume() const { return m_volume; }
	
	void ReloadPlaylist();		//�������벥���б�

	void RemoveSong(int index);		//�Ӳ����б���ɾ��ָ������Ŀ
	void ClearPlaylist();			//��ղ����б�
	void SeekTo(int position);		//��λ��ָ��λ��
	void ClearLyric();		//�����ǰ�ļ��ĸ�ʹ���

	int GetCurrentPosition() const { return m_current_position_int; }		//���ص�ǰ���ŵ���λ��
	int GetSongLength() const { return m_song_length_int; }				//�������ڲ����ļ��ĳ���
	wstring GetTimeString() const;				//���ص�ǰ����ʱ����ַ�����ʽ
	const float* GetSpectralData() const { return m_spectral_data; }	//����Ƶ�׷���ÿ�����εĸ߶ȵ�����
	deque<PathInfo>& GetRecentPath() { return m_recent_path; }	//�����������·���б������
	wstring GetPlayingState() const;		//��ȡ����״̬���ַ���
	const SongInfo& GetCurrentSongInfo() const { return m_playlist[m_index]; }

	void ReIniBASS();		//���³�ʼ��BASS

	void SortPlaylist(bool change_index = true);	//�����б���m_sort_mode���򣨵�change_indexΪtrueʱ����������²������ڲ��ŵĸ�����
};

