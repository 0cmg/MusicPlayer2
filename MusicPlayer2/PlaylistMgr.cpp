#include "stdafx.h"
#include "PlaylistMgr.h"
#include "MusicPlayer2.h"
#include <set>


CPlaylistMgr::CPlaylistMgr()
{
    m_default_playlist.path = theApp.m_playlist_dir + DEFAULT_PLAYLIST_NAME;
}


CPlaylistMgr::~CPlaylistMgr()
{
}

void CPlaylistMgr::EmplacePlaylist(const wstring& path, int track, int pos, int track_num, int total_time)
{
    if (path == m_default_playlist.path)
        return;

    for (size_t i{ 0 }; i < m_recent_playlists.size(); i++)
    {
        if (path == m_recent_playlists[i].path)
            m_recent_playlists.erase(m_recent_playlists.begin() + i);		//�����ǰ·���Ѿ������·���У��Ͱ������·����ɾ��
    }
    PlaylistInfo playlist_info;
    playlist_info.path = path;
    playlist_info.track = track;
    playlist_info.position = pos;
    playlist_info.track_num = track_num;
    playlist_info.total_time = total_time;
    m_recent_playlists.push_front(playlist_info);		//��ǰ·�����뵽m_recent_playlists��ǰ��
}

void CPlaylistMgr::AddNewPlaylist(const wstring& path)
{
    PlaylistInfo playlist_info{};
    playlist_info.path = path;
    m_recent_playlists.push_back(playlist_info);
}

bool CPlaylistMgr::DeletePlaylist(const wstring & path)
{
    auto iter = std::find_if(m_recent_playlists.begin(), m_recent_playlists.end(), [path](const PlaylistInfo& item) {
        return item.path == path;
    });

    if (iter != m_recent_playlists.end())
    {
        m_recent_playlists.erase(iter);
        return true;
    }
    else
    {
        return false;
    }
}

void CPlaylistMgr::SavePlaylistData()
{
    // �򿪻����½��ļ�
    CFile file;
    BOOL bRet = file.Open(theApp.m_recent_playlist_data_path.c_str(),
        CFile::modeCreate | CFile::modeWrite);
    if (!bRet)		//���ļ�ʧ��
    {
        return;
    }
    // ����CArchive����
    CArchive ar(&file, CArchive::store);
    // д����
    ar << 1;        //д�������ļ��汾

    ar << static_cast<int>(m_use_default_playlist);
    //д��Ĭ�ϲ����б���Ϣ
    ar << m_default_playlist.track
        << m_default_playlist.position
        << m_default_playlist.track_num
        << m_default_playlist.total_time;

    ar << static_cast<unsigned int>(m_recent_playlists.size());		//д��m_recent_playlists�Ĵ�С
    for (auto& path_info : m_recent_playlists)
    {
        CFilePathHelper path_helper{ path_info.path };
        ar << CString(path_helper.GetFileNameWithoutExtension().c_str())
            << path_info.track
            << path_info.position
            << path_info.track_num
            << path_info.total_time;
    }
    // �ر�CArchive����
    ar.Close();
    // �ر��ļ�
    file.Close();

}

void CPlaylistMgr::LoadPlaylistData()
{
    m_recent_playlists.clear();
    std::vector<PlaylistInfo> playlist_info_vect;       //�����ݻ�ȡ���Ĳ����б���Ϣ

    // ���ļ�
    CFile file;
    BOOL bRet = file.Open(theApp.m_recent_playlist_data_path.c_str(), CFile::modeRead);
    if (!bRet)		//�ļ�������
    {
        return;
    }
    // ����CArchive����
    CArchive ar(&file, CArchive::load);
    // ������
    unsigned int size{};
    CString temp;
    try
    {
        int version{};
        ar >> version;

        int temp;
        ar >> temp;
        m_use_default_playlist = (temp != 0);
        ar >> m_default_playlist.track;
        ar >> m_default_playlist.position;
        ar >> m_default_playlist.track_num;
        ar >> m_default_playlist.total_time;;

        ar >> size;		//��ȡӳ�������ĳ���
        for (unsigned int i{}; i < size; i++)
        {
            PlaylistInfo path_info;
            CString strTmp;
            ar >> strTmp;
            path_info.path = strTmp;
            ar >> path_info.track;
            ar >> path_info.position;
            ar >> path_info.track_num;
            ar >> path_info.total_time;

            playlist_info_vect.push_back(path_info);
        }
    }
    catch (CArchiveException* exception)
    {
        //�������л�ʱ���ֵ��쳣
        CString info;
        info = CCommon::LoadTextFormat(IDS_SERIALIZE_ERROR, { theApp.m_recent_playlist_data_path, exception->m_cause });
        theApp.WriteErrorLog(wstring{ info });
    }
    // �رն���
    ar.Close();
    // �ر��ļ�
    file.Close();

    std::set<wstring> files_in_playlist_info;       //��������ӵĲ����б��ļ�·��
    for (const auto& playlist_info : playlist_info_vect)
    {
        wstring file_path{ theApp.m_playlist_dir + playlist_info.path + L".playlist" };
        if (CCommon::FileExist(file_path))
        {
            CFilePathHelper path_helper{ file_path };
            wstring file_name = path_helper.GetFileName();
            if (file_name != DEFAULT_PLAYLIST_NAME)
            {
                m_recent_playlists.push_back(playlist_info);
                files_in_playlist_info.insert(file_path);
            }
        }
    }

    //��ȡplaylistĿ¼�µĲ����б��ļ�
    vector<wstring> file_list;
    CCommon::GetFiles(theApp.m_playlist_dir + L"*.playlist", file_list);

    for (const auto& file : file_list)
    {
        CFilePathHelper path_helper{ theApp.m_playlist_dir + file };
        wstring file_name = path_helper.GetFileName();
        if(file_name == DEFAULT_PLAYLIST_NAME || files_in_playlist_info.find(path_helper.GetFilePath())!= files_in_playlist_info.end())
            continue;

        PlaylistInfo path_info;
        path_info.path = path_helper.GetFilePath();
        m_recent_playlists.push_back(path_info);
    }
    int a = 0;
}
