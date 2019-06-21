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


class CPlaylistMgr
{
public:
    CPlaylistMgr();
    ~CPlaylistMgr();

    void EmplacePlaylist(const wstring& path, int track, int pos, int track_num, int total_time);
    void AddNewPlaylist(const wstring& path);

    void SavePlaylistData();
    void LoadPlaylistData();

public:
    PlaylistInfo m_default_playlist;
    std::deque<PlaylistInfo> m_recent_playlists;
    bool m_use_default_playlist{ true };
};

