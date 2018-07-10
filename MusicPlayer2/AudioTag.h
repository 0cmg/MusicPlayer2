#pragma once
#include "AudioCommon.h"

class CAudioTag
{
public:
	CAudioTag(HSTREAM hStream, wstring file_dir, SongInfo & song_info);

	//��ȡ��Ƶ�ļ��ı�ǩ��Ϣ����������ڹ��캯�����ݽ�����SongInfo�ṹ�
	//id3v1_first���Ƿ����Ȼ�ȡID3V1��ǩ���������Ȼ�ȡID3V2��ǩ
	void GetAudioTag(bool id3v1_first);

	//��ȡ��Ƶ�ļ���ר�����棬�����浽��ʱĿ¼
	//image_type���������շ���ĸ�ʽ 0:jpg, 1:png, 2:gif
	//����ֵ��ר������ı���·��
	wstring GetAlbumCover(int& image_type);

	//��һ��MP3�ļ�д��ID3V1��ǩ
	//file_path��mp3�ļ���·��
	//text_cut_off�����д����ı����ȳ���ID3V1�����ɵĳ��ȣ���������ı����ᱻ�ضϣ�����text_cut_off��Ϊtrue
	//����ֵ���ɹ�����true�����򷵻�false
	static bool WriteMp3Tag(LPCTSTR file_path, const SongInfo& song_info, bool& text_cut_off);

	AudioType GetAudioType() const { return m_type; }

	~CAudioTag();

private:
	HSTREAM m_hStream;
	wstring m_file_dir;
	SongInfo& m_song_info;
	AudioType m_type;

	bool GetID3V1Tag();
	bool GetID3V2Tag();
	bool GetWmaTag();
	bool GetMp4Tag();
	bool GetOggTag();
	bool GetFlacTag();

	//��ȡFLAC��Ƶ�ı�ǩ���������
	static void GetFlacTagContents(wstring file_name, string& contents_buff);

	static wstring _GetAlbumCover(const string& tag_content, size_t cover_index, int& image_type);

};

