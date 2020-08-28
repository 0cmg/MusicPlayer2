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

    //д��ǩ��Ϣ
    static bool WriteAudioTag(SongInfo& song_info);

    //����һ���ļ���չ���жϴ˸�ʽ�Ƿ���֧��д���ǩ
    static bool IsFileTypeTagWriteSupport(const wstring& ext);

private:
    static bool WriteMpegTag(SongInfo& song_info);
    static bool WriteFlacTag(SongInfo& song_info);
};

