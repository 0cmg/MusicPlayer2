//�������ڶ�����Ƶ��Ϣ��ص�ȫ�ֺ���
#pragma once
#include "Time.h"

//��Ƶ�ļ�����
enum AudioType
{
	AU_MP3,
	AU_WMA,
	AU_OGG,
	AU_MP4,
	AU_APE,
	AU_FLAC,
	AU_CUE,
	AU_OTHER
};

//����ʽ
enum SortMode
{
	SM_FILE,
	SM_TITLE,
	SM_ARTIST,
	SM_ALBUM,
	SM_TRACK
};

//һ�׸�������Ϣ
struct SongInfo
{
	wstring file_name{};	//�������ļ���
	wstring lyric_file{};	//ƥ��ĸ���ļ���·��
	Time lengh;			//�����ĳ���
	int bitrate{};		//������
	wstring title{ DEFAULT_TITLE };		//����
	wstring artist{ DEFAULT_ARTIST };	//������
	wstring album{ DEFAULT_ALBUM };		//��Ƭ��
	wstring year{ DEFAULT_YEAR };		//���
	wstring comment;	//ע��
	wstring genre{ DEFAULT_GENRE };		//����
	BYTE track{};		//�������
	int tag_type{};		//��ǩ�����ͣ�0��������1��ID3v1��2��ID3v2��
	bool info_acquired{ false };		//����Ѿ���ȡ������Ϣ����Ϊture
	Time start_pos{};		//��Ƶ����ʼλ�ã�����cue�ֹ�
	Time end_pos{};
	bool is_cue{ false };		//�����Ŀ��cue�ֹ죬��Ϊtrue

	//�����ļ����ıȽϺ������������ļ�������
	static bool ByFileName(const SongInfo& a, const SongInfo& b) { return a.file_name < b.file_name; }
	//���ݱ���ıȽϺ����������Ա�������
	static bool ByTitle(const SongInfo& a, const SongInfo& b) { return a.title < b.title; }
	//���������ҵıȽϺ���������������������
	static bool ByArtist(const SongInfo& a, const SongInfo& b) { return a.artist < b.artist; }
	//���ݳ�Ƭ���ıȽϺ����������Գ�Ƭ������
	static bool ByAlbum(const SongInfo& a, const SongInfo& b) { return a.album < b.album; }
	//����������ŵıȽϺ����������������������
	static bool ByTrack(const SongInfo& a, const SongInfo& b) { return a.track < b.track; }
};


// ID3v1 tag structure
struct TAG_ID3V1
{
	char id[3];
	char title[30];
	char artist[30];
	char album[30];
	char year[4];
	char comment[28];
	BYTE track[2];
	BYTE genre;
};

//����·����Ϣ
struct PathInfo
{
	wstring path;		//·��
	int track{};			//��󲥷ŵ�����Ŀ��
	int position{};		//��󲥷ŵ���λ��
	SortMode sort_mode{};	//·�����ļ�������ʽ
	int track_num{};		//·������Ƶ�ļ�������
	int total_time{};		//·������Ƶ�ļ�����ʱ��

	//PathInfo(wstring _path, int _track, int _position, SortMode _sort_mode) :
	//	path{ _path }, track{ _track }, position{ _position }, sort_mode{ _sort_mode }
	//{}
};

//ѭ��ģʽ
enum RepeatMode
{
	RM_PLAY_ORDER,		//˳�򲥷�
	RM_PLAY_SHUFFLE,	//�������
	RM_LOOP_PLAYLIST,	//�б�ѭ��
	RM_LOOP_TRACK		//����ѭ��
};

//�����б�����Ŀ����ʾ��ʽ
enum DisplayFormat
{
	DF_FILE_NAME,		//�ļ���
	DF_TITLE,			//����
	DF_ARTIST_TITLE,	//������ - ����
	DF_TITLE_ARTIST		//���� - ������
};

class CAudioCommon
{
public:
	CAudioCommon();
	~CAudioCommon();

	//�ж��ļ������Ƿ�Ϊ��Ƶ�ļ�
	static bool FileIsAudio(const wstring& file_name);

	//�����ļ����ж��ļ�������
	static AudioType GetAudioType(const wstring& file_name);

	//����pathĿ¼�µ�������Ƶ�ļ��������ļ������浽files�����У����޶�����ļ���Ϊmax_file
	static void GetAudioFiles(wstring path, vector<SongInfo>& files, size_t max_file);

	//����pathĿ¼�µ����и���ļ��������ļ������浽files������
	static void GetLyricFiles(wstring path, vector<wstring>& files);

	//����files�����е�cue�ļ�������ÿ�ηֹ���Ϊһ����Ŀ��ӵ�files�����У�pathΪ�ļ���·��
	static void GetCueTracks(vector<SongInfo>& files, wstring path);

	//ͨ��BASS��Ƶ���ȡ��Ƶ�ļ�����Ϣ����������song_info��
	static void GetAudioTags(HSTREAM hStream, AudioType type, SongInfo& song_info);

	//��ñ�׼������Ϣ
	static wstring GetGenre(BYTE genre);

	////ɾ���ַ���ĩβ�Ŀո�
	//static void DeleteEndSpace(wstring& str);
};

