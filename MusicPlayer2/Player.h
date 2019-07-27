#pragma once
#include"Common.h"
#include"Lyric.h"
#include"SetPathDlg.h"
#include"AudioTag.h"
#include "FilePathHelper.h"
#include "BASSMidiLibrary.h"
//#include"MusicPlayerDlg.h"
#include "GaussBlur.h"
#include "PlaylistMgr.h"
#include "IPlayerCore.h"
#include "BassCore.h"

#define WM_PLAYLIST_INI_START (WM_USER+104)			//�����б�ʼ����ʱ����Ϣ
#define WM_PLAYLIST_INI_COMPLATE (WM_USER+105)		//�����б���������Ϣ
#define WM_SET_TITLE (WM_USER+106)					//���ô��ڱ������Ϣ
#define WM_CONNOT_PLAY_WARNING (WM_USER+108)		//�޷������ļ�ʱ����������ʾ�����Ϣ
#define WM_MUSIC_STREAM_OPENED (WM_USER+109)		//����Ƶ�ļ���ʱ����Ϣ


class CPlayer
{
public:
	//�������ʼ�������б�����Ϣ�Ľṹ��
	struct ThreadInfo
	{
		bool refresh_info;
		bool sort;		//ָʾ�����겥���б���Ƿ�Ҫ����
        bool play;      //�����겥���б���Ƿ���������
		int process_percent;
	};
	//��ʼ�������б�Ĺ����̺߳���
	static UINT IniPlaylistThreadFunc(LPVOID lpParam);
	ThreadInfo m_thread_info;


private:
	CWinThread* m_pThread{};		//��ʼ�������б���߳�

	//
    IPlayerCore* m_pCore{};


	vector<SongInfo> m_playlist;		//�����б�����ÿ�������ļ��ĸ�����Ϣ
	wstring m_path;		            //�ļ���ģʽ�£���ǰ�����ļ���Ŀ¼
    wstring m_playlist_path;        //��ǰ�����б��ļ���·��
	wstring m_current_file_name_tmp;	//�򿪵�����Ƶʱ������ʱ�����ļ���
	wstring m_current_file_type;
	deque<PathInfo> m_recent_path;		//����򿪹���·��
    CPlaylistMgr m_recent_playlist;

	Time m_song_length;		//���ڲ��ŵ��ļ��ĳ���
	Time m_current_position;		//��ǰ���ŵ���λ��
	//int m_song_length_int;		//���ڲ��ŵ��ļ��ĳ��ȣ�int���ͣ�
	//int m_current_position_int;		//��ǰ���ŵ���λ�ã�int���ͣ�
	int m_total_time;		//�����б���������Ŀ��ʱ�䣨���룩

	int m_index{ 0 };	//��ǰ�������ֵ����
	int m_index_tmp{ 0 };
	//int m_song_num{ 0 };	//�����б��еĸ�������
	int m_error_code{ 0 };	//����������
	int m_playing{ 0 };		//���ڲ��ű�־��0����ֹͣ��1������ͣ��2�����ڲ��ţ�
	RepeatMode m_repeat_mode;		//ѭ��ģʽ��0��˳�򲥷ţ�1��������ţ�2���б�ѭ����3������ѭ����
	int m_volume{ 100 };		//�������ٷֱȣ�

	float m_fft[FFT_SAMPLE];		//����Ƶ�׷���������
	float m_spectral_data[SPECTRUM_ROW]{};	//������ʾ��ÿ��Ƶ�����εĸ߶�
	float m_last_spectral_data[SPECTRUM_ROW]{};	//��һ�ε�Ƶ������
	float m_spectral_peak[SPECTRUM_ROW]{};		//Ƶ�׶��˵ĸ߶�

	int m_equalizer_gain[EQU_CH_NUM]{};		//���ڱ������úõ�ÿ��ͨ��������
	bool m_equ_enable{ false };		//ָʾ�Ƿ����������

	int m_equ_style{};
	int m_reverb_mix{};		//����ǿ�ȣ�0~100��
	int m_reverb_time{1};	//����ʱ�䣨��λ10ms��1~300��
	bool m_reverb_enable{ false };		//ָʾ�Ƿ��������

	CImage m_album_cover;			//ר������
	CImage m_album_cover_blur;		//��˹ģ�����ר������
	wstring m_album_cover_path;		//ר�������ļ���·��
	int m_album_cover_type;			//ר������ĸ�ʽ
	bool m_inner_cover{ false };			//���ר����������ǶͼƬ����Ϊtrue

	//bool m_no_ape_plugin{ false };		//����޷�����bass_ape.dll����Ϊtrue

	SongInfo m_no_use;
public:
	CLyrics m_Lyrics;		//���
	//wstring m_lyric_path;	//����ļ��е�·��

	//bool lyric_karaoke_disp{ true };	//�����Ƿ��Կ���OK��ʽ��ʾ
	//bool lyric_fuzzy_match{ true };	//���ģ��ƥ��

	SortMode m_sort_mode;		//����ʽ

	bool m_loading{ false };		//����������벥���б���Ϊtrue

	//bool m_is_ous_folder{ false };	//����򿪵���OUS��SongsĿ¼����Ϊtrue

private:
	vector<wstring> m_current_path_lyrics;	//���浱ǰ·���µĸ���ļ����ļ��������ڸ��ģ��ƥ��ʱ������
	vector<wstring> m_lyric_path_lyrics;		//�������ļ����µĸ���ļ����ļ��������ڸ��ģ��ƥ��ʱ������

	vector<int> m_shuffle_list;			//����������Ź�����Ŀ���

    bool m_playlist_mode{ false };       //��������б��е���Ŀ���Բ����б��ļ��������Ǵ�һ��·�����������ģ���Ϊtrue

private:
	void IniPlayerCore();			//��ʼ��BASS��Ƶ��
	void UnInitPlayerCore();
	void IniPlayList(bool cmd_para = false, bool refresh_info = false, bool play = false);	//��ʼ�������б�(�������cmd_paraΪtrue����ʾ��������ֱ�ӻ�ȡ�����ļ��������Ǵ�ָ��·����������
																		//���refresh_infoΪtrue���򲻹�theApp.m_song_data���Ƿ��е�ǰ��������Ϣ�������ļ����»�ȡ��Ϣ)

	void ChangePath(const wstring& path, int track = 0);		//�ı䵱ǰ·��

	void ApplyEqualizer(int channel, int gain);		//Ӧ��һ��������ͨ��������

	void LoadRecentPath();		//���ļ��������·���б�
    void LoadRecentPlaylist();
public:
    void SaveCurrentPlaylist();
	void EmplaceCurrentPathToRecent();		//����ǰ·�����뵽���·����
    void EmplaceCurrentPlaylistToRecent();
	void SaveRecentPath() const;		//�����·���б��浽�ļ�
	void OnExit();		//�˳�ʱ�Ĵ���
	void IniPlaylistComplate();		//�����б�������ʱ�Ĵ���

	void SetEqualizer(int channel, int gain);		//���þ�������channelΪͨ����ȡֵΪ0~9��gainΪ���棬ȡֵΪ-15~15��
	int GeEqualizer(int channel);		//��ȡָ��������ͨ��������
	void SetAllEqualizer();			//������õ�ÿ��ͨ�������棨m_equalizer_gain�����õ�������
	void ClearAllEqulizer();		//��ÿ��������ͨ�������渴λ
	void EnableEqualizer(bool enable);			//����������
	bool GetEqualizerEnable() const { return m_equ_enable; }

	int GetReverbMix() const { return m_reverb_mix; }
	int GetReverbTime() const { return m_reverb_time; }
	void EnableReverb(bool enable);		//���쿪��
	bool GetReverbEnable() const { return m_reverb_enable; }

private:
	CPlayer();

private:
	static CPlayer m_instance;		//CPlayer��Ψһ�Ķ���
	 
public:
	static CPlayer& GetInstance();		//��ȡCPlayer���Ψһ�Ķ���
	~CPlayer();
	void Create();		//��ʼ��CPlayer��
	void Create(const vector<wstring>& files);	//��ʼ��CPlayer��
	void Create(const wstring & path);
	void MusicControl(Command command, int volume_step = 2);		//�������ֲ���
	bool SongIsOver() const;			//�жϵ�ǰ�����Ƿ񲥷����
	void GetPlayerCoreCurrentPosition();		//��BASS��Ƶ���ȡ��ǰ���ŵ���λ��
	void GetPlayerCoreSongLength();			//��BASS��Ƶ���ȡ���ڲ����ļ��ĳ���

	void CalculateSpectralData();		//Ƶ�׷���
	int GetCurrentSecond();		//��ȡ��ǰ���ŵ���λ�õ�����
	bool IsPlaying() const;			//�жϵ�ǰ�Ƿ����ڲ���

	bool PlayTrack(int song_track);		//����ָ����ŵĸ���

	void SetVolume();		//��m_volume��ֵ��������
	void SetPath(const wstring& path, int track, int position, SortMode sort_mode);		//����·��
    void SetPlaylist(const wstring& playlist_path, int track, int position, bool init = false);
	void OpenFolder(wstring path);	//ͨ�������ļ��С�������·���Ĵ���
	void OpenFiles(const vector<wstring>& files, bool play = true);	//�򿪶���ļ���play���������Ƿ���������
	void OpenAFile(wstring file);	//��һ����Ƶ�ļ�������Ϊ�ļ��ľ���·��
    void AddFiles(const vector<wstring>& files);
	void SetRepeatMode();		//����ѭ��ģʽ
	void SetRepeatMode(RepeatMode repeat_mode);	//����ѭ��ģʽ
	RepeatMode GetRepeatMode() const;

	bool GetPlayerCoreError();		//��ȡBASS��Ƶ��Ĵ���
	bool IsError() const;				//�д���ʱ����ture�����򷵻�false

	void SetTitle() const;		//���ô��ڱ���

	void SaveConfig() const;		//�������õ�ini�ļ�
	void LoadConfig();		//��ini�ļ���ȡ����

	void SearchLyrics(/*bool refresh = false*/);		//��������ļ�(������refreshΪtrue���򲻹�theApp.m_song_data���Ƿ��е�ǰ�������ļ�·���������ļ����¼������)
	void IniLyrics();		//��ʼ�����
	void IniLyrics(const wstring& lyric_path);

	void ExplorePath(int track = -1) const;	//����Դ�������򿪵�ǰ·����ѡ��ָ��ָ���ļ�����trackС��0ʱѡ�е�ǰ���ڲ��ŵ��ļ���
	void ExploreLyric() const;		//����Դ�������򿪸���ļ����ڵ�λ��

	vector<SongInfo>& GetPlayList() { return m_playlist; }	//��ȡ�����б������
	Time GetAllSongLength(int track) const;				//��ȡָ����ŵĸ����ĳ���
	int GetSongNum() const;			//��ȡ��������
    wstring GetCurrentDir() const;   	//��ȡ��ǰĿ¼
    wstring GetCurrentFolderOrPlaylistName() const;
    wstring GetCurrentFilePath() const;		//��ȡ���ڲ����ļ���·��
	int GetIndex() const { return m_index; }		//��ȡ��ǰ���ŵ���Ŀ���
	wstring GetFileName() const;
	wstring GetLyricName() const { return m_Lyrics.GetPathName(); }
	int GetVolume() const { return m_volume; }
	CImage& GetAlbumCover() { return m_album_cover; }
	CImage& GetAlbumCoverBlur() { return m_album_cover_blur; }
	bool AlbumCoverExist() { return !m_album_cover.IsNull(); }
	wstring GetAlbumCoverPath() const { return m_album_cover_path; }
	int GetAlbumCoverType() const { return m_album_cover_type; }
	void DeleteAlbumCover();
	
	void ReloadPlaylist();		//�������벥���б�

	void RemoveSong(int index);		//�Ӳ����б���ɾ��ָ������Ŀ
	void RemoveSongs(vector<int> indexes);	//�Ӳ����б���ɾ�����ָ������Ŀ
    int RemoveSameSongs();         //�Ӳ����б����Ƴ���ͬ����Ŀ���������Ƴ�����Ŀ����
    int RemoveInvalidSongs();       //�Ӳ����б����Ƴ���Ч����Ŀ���������Ƴ�����Ŀ����
	void ClearPlaylist();			//��ղ����б�
    bool MoveUp(int first, int last);        //��ָ����Χ�ڵ���Ŀ����
    bool MoveDown(int first, int last);      //��ָ����Χ�ڵ���Ŀ����
    int MoveItems(std::vector<int> indexes, int dest);      //�ƶ������Ŀ��dest��λ�ã������ƶ����1����Ŀ������

	void SeekTo(int position);		//��λ��ָ��λ��
	void SeekTo(double position);	//��λ��ָ��λ��(��Χ0~1)
	//static void SeekTo(HSTREAM hStream, int position);
	void ClearLyric();		//�����ǰ�ļ��ĸ�ʹ���

	int GetCurrentPosition() const { return m_current_position.toInt(); }		//���ص�ǰ���ŵ���λ��
	int GetSongLength() const { return m_song_length.toInt(); }				//�������ڲ����ļ��ĳ���
	wstring GetTimeString() const;				//���ص�ǰ����ʱ����ַ�����ʽ
	const float* GetSpectralData() const { return m_spectral_data; }	//����Ƶ�׷���ÿ�����εĸ߶ȵ�����
	const float* GetSpectralPeakData() const { return m_spectral_peak; }
	const float* GetFFTData() const { return m_fft; }			//����Ƶ�׷�����ԭʼ����
	deque<PathInfo>& GetRecentPath() { return m_recent_path; }	//�����������·���б������
    CPlaylistMgr& GetRecentPlaylist() { return m_recent_playlist; }
	wstring GetPlayingState() const;		//��ȡ����״̬���ַ���
	int GetPlayingState2() const { return m_playing; }	//��ȡ���ڲ���״̬��0����ֹͣ��1������ͣ��2�����ڲ��ţ�
	const SongInfo& GetCurrentSongInfo() const;
	void SetRelatedSongID(wstring song_id);		//Ϊ��ǰ�������ù��������������ָ���ID
	void SetRelatedSongID(int index, wstring song_id);		//Ϊ��index�׸������ù��������������ָ���ID
    void SetFavourite(bool favourite);          //Ϊ��ǰ�������á���ϲ�������
    bool IsFavourite();
	bool IsInnerCover() const { return m_inner_cover; }		//�жϵ�ǰר�������Ƿ�����ǶͼƬ

	void AddListenTime(int sec);		//Ϊ��ǰ���������ۼ��Ѳ���ʱ��

	bool IsMidi() const { return m_pCore->IsMidi(); }
	MidiInfo GetMidiInfo() const { return m_pCore->GetMidiInfo(); }
	wstring GetMidiLyric() const { return m_pCore->GetMidiInnerLyric(); }
	bool MidiNoLyric() const { return m_pCore->MidiNoLyric(); }
	wstring GetSoundFontName() const { return m_pCore->GetSoundFontName(); }

    int GetChannels();
    int GetFreq();

	void ReIniPlayerCore(bool replay = false);		//���³�ʼ��BASS����replayΪtrueʱ�����ԭ�����ڲ��ţ������³�ʼ�����������

	void SortPlaylist(bool change_index = true);	//�����б���m_sort_mode���򣨵�change_indexΪtrueʱ����������²������ڲ��ŵĸ�����

private:
	void ConnotPlayWarning() const;		//���޷�����ʱ������ʾ��Ϣ
	void SearchAlbumCover();		//��ȡר������
	wstring GetCurrentFileName() const;

public:
    void SearchOutAlbumCover();		//����ƥ����ⲿר�����棬������ר������
	void AlbumCoverGaussBlur();		//ר�������˹ģ��
	static wstring GetRelatedAlbumCover(const wstring& file_path, const SongInfo& song_info);		//��ȡ�������ⲿר������ͼƬ�������ļ�·��
	wstring GetCurrentFileType() { return m_current_file_type; }
    bool IsOsuFile() const;
    bool IsPlaylistMode() const { return m_playlist_mode; }

    void SetPlaylistPath(const wstring& playlist_path);
    wstring GetPlaylistPath() const;
    IPlayerCore* GetPlayerCore() { return m_pCore; }
};

