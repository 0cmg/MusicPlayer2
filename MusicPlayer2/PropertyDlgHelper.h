#pragma once
#include "SongInfo.h"
#include <functional>

#define FORMULAR_TITLE  L"<%title%>"
#define FORMULAR_ARTIST L"<%artist%>"
#define FORMULAR_ALBUM  L"<%album%>"
#define FORMULAR_TRACK  L"<%track%>"
#define FORMULAR_YEAR   L"<%year%>"
#define FORMULAR_GENRE  L"<%genre%>"

class CPropertyDlgHelper
{
public:
    CPropertyDlgHelper(const vector<SongInfo>& songs);
    ~CPropertyDlgHelper();

    wstring GetMultiFileName();
    wstring GetMultiFilePath();
    wstring GetMultiType();
    wstring GetMultiLength();
    wstring GetMultiSize();
    wstring GetMultiBitrate();

    wstring GetMultiTitle();
    wstring GetMultiArtist();
    wstring GetMultiAlbum();
    wstring GetMultiTrack();
    wstring GetMultiYear();
    wstring GetMultiGenre();
    wstring GetMultiComment();

    bool IsMultiWritable();
    bool IsMultiCoverWritable();

private:
    //��ȡm_song_info��һ���ֶε�ֵ��������ֶ�ȫ����ͬ���򷵻ظ�ֵ�������һ����ͬ���򷵻ء������ֵ��
    wstring GetMultiValue(std::function<wstring(const SongInfo&)> fun_get_value);

private:
    const vector<SongInfo>& m_song_info;

public:

    //���ļ����²��ǩ
    //file_name: �ļ�����������չ����
    //formular: �²�ķ��������硰<%artist%> - <%title%>��
    //song_info: ���ղ²���
    static void GetTagFromFileName(const wstring& file_name, const wstring& formular, SongInfo& song_info);
};

