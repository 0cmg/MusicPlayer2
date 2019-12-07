#pragma once

//����·����Ϣ
struct PlaylistInfo
{
    wstring path;		    //·��
    int track{};		    //��󲥷ŵ�����Ŀ���
    int position{};		    //��󲥷ŵ���λ��
    int track_num{};		//·������Ƶ�ļ�������
    int total_time{};		//·������Ƶ�ļ�����ʱ��
};

enum PlaylistType
{
    PT_USER,
    PT_DEFAULT,
    PT_FAVOURITE,
    PT_TEMP
};

class CPlaylistMgr
{
public:
    CPlaylistMgr();
    ~CPlaylistMgr();

    void EmplacePlaylist(const wstring& path, int track, int pos, int track_num, int total_time);
    void AddNewPlaylist(const wstring& path);
    bool DeletePlaylist(const wstring& path);
    void UpdateCurrentPlaylist(int track, int pos, int track_num, int total_time);

    void SavePlaylistData();
    void LoadPlaylistData();

public:
    PlaylistInfo m_default_playlist;
    PlaylistInfo m_favourite_playlist;
    PlaylistInfo m_temp_playlist;
    std::deque<PlaylistInfo> m_recent_playlists;
    PlaylistType m_cur_playlist_type{ PT_DEFAULT };
};

