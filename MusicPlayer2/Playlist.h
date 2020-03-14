#pragma once
#include "SongInfo.h"
class CPlaylistFile
{
public:
	enum Type			//�����б��ʽ
	{
		PL_PLAYLIST,	//MusicPlayer2�����б�
		PL_M3U,			//m3u�����б�
		PL_M3U8			//m3u8�����б�
	};

    CPlaylistFile();
    ~CPlaylistFile();
    void LoadFromFile(const wstring& file_path);
    void SaveToFile(const wstring& file_path, Type type = PL_PLAYLIST) const;
    vector<SongInfo> GetPlaylist() const;
    void AddFiles(const vector<wstring>& files);
    void AddFiles(const vector<SongInfo>& files);
    void FromSongList(const vector<SongInfo>& song_list);
    void ToSongList(vector<SongInfo>& song_list);
    bool IsFileInPlaylist(const SongInfo& file);
    void RemoveFile(const wstring& file);

	static bool IsPlaylistFile(const wstring& file_path);

public:
    const static vector<wstring> m_surpported_playlist;		//֧�ֵĲ����б��ļ�����չ���б�

private:
	void DisposePlaylistFileLine(const string& str_current_line, bool utf8);

private:
    vector<SongInfo> m_playlist;
};

