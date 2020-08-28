#pragma once
#include "SongInfo.h"

//
class CTagLabHelper
{
public:
    CTagLabHelper();
    ~CTagLabHelper();

    //��ר������
    static string GetM4aAlbumCover(const wstring& file_path, int& type);
    static string GetFlacAlbumCover(const wstring& file_path, int& type);

    //����ǩ��Ϣ
    static void GetFlacTagInfo(SongInfo& song_info);
    static void GetM4aTagInfo(SongInfo& song_info);

    //д��ǩ��Ϣ
    static bool WriteAudioTag(SongInfo& song_info);

    //дר������
    static bool WriteMp3AlbumCover(const wstring& file_path, const wstring& album_cover_path);

    //����һ���ļ���չ���жϴ˸�ʽ�Ƿ���֧��д���ǩ
    static bool IsFileTypeTagWriteSupport(const wstring& ext);

private:
    static bool WriteMpegTag(SongInfo& song_info);
    static bool WriteFlacTag(SongInfo& song_info);
    static bool WriteM4aTag(SongInfo& song_info);
};

