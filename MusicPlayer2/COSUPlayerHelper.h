#pragma once
#include "SongInfo.h"
class COSUPlayerHelper
{
public:
    COSUPlayerHelper();
    ~COSUPlayerHelper();

    static bool IsOsuFolder(const std::wstring& strPath);
    static void GetOSUAudioFiles(wstring path, vector<SongInfo>& files);
    static void GetOSUAudioTitleArtist(SongInfo& song_info);
    static wstring GetAlbumCover(wstring file_path);

private:
    static void GetOSUFile(wstring folder_path);
};


//����osu�ļ�����
class COSUFile
{
public:
    COSUFile(const wchar_t* file_path);
    wstring GetAudioFile();
    wstring GetArtist();
    wstring GetTitle();
    wstring GetAlbum();
    wstring GetBeatampSetId();

private:
    void GetTag(const string& tag, string& tag_content);
    wstring GetTagItem(const string& tag, const string& tag_content);

private:
    string m_data;
    string m_general_seg;
    string m_metadata_seg;
};