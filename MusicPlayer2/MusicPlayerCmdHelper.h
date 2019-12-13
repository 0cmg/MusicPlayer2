#pragma once
#include "SongInfo.h"
#include "FormatConvertDlg.h"
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
    template<class Func>
    bool OnAddToNewPlaylist(Func get_song_list, std::wstring& playlist_path, const std::wstring& default_name = L"");

private:
    CWnd* GetOwner();

private:
    CWnd* m_pOwner{};
};


template<class Func>
bool CMusicPlayerCmdHelper::OnAddToNewPlaylist(Func get_song_list, std::wstring& playlist_path, const std::wstring& default_name /*= L""*/)
{
    CInputDlg imput_dlg;
    imput_dlg.SetTitle(CCommon::LoadText(IDS_NEW_PLAYLIST));
    imput_dlg.SetInfoText(CCommon::LoadText(IDS_INPUT_PLAYLIST_NAME));
    imput_dlg.SetEditText(default_name.c_str());
    if (imput_dlg.DoModal() == IDOK)
    {
        CString playlist_name = imput_dlg.GetEditText();
        if (!CCommon::IsFileNameValid(wstring(playlist_name.GetString())))
        {
            GetOwner()->MessageBox(CCommon::LoadText(IDS_FILE_NAME_INVALID_WARNING), NULL, MB_ICONWARNING | MB_OK);
            return false;
        }
        playlist_path = theApp.m_playlist_dir + playlist_name.GetString() + PLAYLIST_EXTENSION;
        if (CCommon::FileExist(playlist_path))
        {
            GetOwner()->MessageBox(CCommon::LoadTextFormat(IDS_PLAYLIST_EXIST_WARNING, { playlist_name }), NULL, MB_ICONWARNING | MB_OK);
            return false;
        }
        //��ӿյĲ����б�
        CPlayer::GetInstance().GetRecentPlaylist().AddNewPlaylist(playlist_path);

        //��ȡѡ�е���Ŀ��·��
        std::vector<SongInfo> selected_item_path;
        get_song_list(selected_item_path);

        CPlaylist playlist;
        playlist.LoadFromFile(playlist_path);
        playlist.AddFiles(selected_item_path);
        playlist.SaveToFile(playlist_path);
        theApp.m_pMainWnd->SendMessage(WM_INIT_ADD_TO_MENU);
        return true;
    }
    return false;
}

