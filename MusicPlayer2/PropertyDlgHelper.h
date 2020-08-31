#pragma once
#include "SongInfo.h"
#include <functional>
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
};

