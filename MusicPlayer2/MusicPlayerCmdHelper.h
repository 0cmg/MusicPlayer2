#pragma once
#include "SongInfo.h"
#include "FormatConvertDlg.h"
#include <functional>
class CMusicPlayerCmdHelper
{
public:
    CMusicPlayerCmdHelper(CWnd* pOwner = nullptr);
    ~CMusicPlayerCmdHelper();

    void VeiwOnline(SongInfo& song);
    void FormatConvert(const std::vector<SongInfo>& songs);

    //ִ����ӵ��½������б�����ɹ�����true
    //get_song_list: ��ȡҪ��ӵ��ļ��б�Ļص�����������ԭ��Ϊ void Func(std::vector<SongInfo>&)
    //playlist_path: �����²����б��·��
    //default_name���²����б��Ĭ������
    bool OnAddToNewPlaylist(std::function<void(std::vector<SongInfo>&)> get_song_list, std::wstring& playlist_path, const std::wstring& default_name = L"");

    void OnAddToPlaylistCommand(std::function<void(std::vector<SongInfo>&)> get_song_list, DWORD command);

private:
    CWnd* GetOwner();

private:
    CWnd* m_pOwner{};
};

