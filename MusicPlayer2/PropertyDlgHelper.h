#pragma once
#include "SongInfo.h"
#include <functional>

#define FORMULAR_TITLE  L"%(Title)"
#define FORMULAR_ARTIST L"%(Artist)"
#define FORMULAR_ALBUM  L"%(Album)"
#define FORMULAR_TRACK  L"%(Track)"
#define FORMULAR_YEAR   L"%(Year)"
#define FORMULAR_GENRE  L"%(Genre)"

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

    bool IsTitleModified(const vector<SongInfo>& list_ori);
    bool IsArtistModified(const vector<SongInfo>& list_ori);
    bool IsAlbumModified(const vector<SongInfo>& list_ori);
    bool IsTrackModified(const vector<SongInfo>& list_ori);
    bool IsYearModified(const vector<SongInfo>& list_ori);
    bool IsGenreModified(const vector<SongInfo>& list_ori);
    bool IsCommentModified(const vector<SongInfo>& list_ori);

private:
    //��ȡm_song_info��һ���ֶε�ֵ��������ֶ�ȫ����ͬ���򷵻ظ�ֵ�������һ����ͬ���򷵻ء������ֵ��
    static wstring GetMultiValue(std::function<wstring(const SongInfo&)> fun_get_value, const vector<SongInfo>& song_list);

    //�ж�m_song_info�е�һ���ֶκ�list_ori�Ƿ��в�ͬ��������򷵻�true
    bool IsValueModified(std::function<wstring(const SongInfo&)> fun_get_value, const vector<SongInfo>& list_ori);

private:
    const vector<SongInfo>& m_song_info;

public:

    //���ļ����²��ǩ
    //file_name: �ļ�����������չ����
    //formular: �²�ķ��������硰<%artist%> - <%title%>��
    //song_info: ���ղ²���
    static void GetTagFromFileName(const wstring& file_name, const wstring& formular, SongInfo& song_info);
};

