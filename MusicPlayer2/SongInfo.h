#pragma once
#include "stdafx.h"
#include "Time.h"
#include "Common.h"
#include "Resource.h"


//һ�׸�������Ϣ
struct SongInfo
{
    //SongInfo(bool ini = true)
    //{
    //    if (ini)
    //    {
    //        title = CCommon::LoadText(IDS_DEFAULT_TITLE);
    //        artist = CCommon::LoadText(IDS_DEFAULT_ARTIST);
    //        album = CCommon::LoadText(IDS_DEFAULT_ALBUM);
    //        year = CCommon::LoadText(IDS_DEFAULT_YEAR);
    //        genre = CCommon::LoadText(IDS_DEFAULT_GENRE);
    //    }
    //}

    //wstring file_name{};	//�������ļ���
    wstring file_path{};    //������·��
    wstring lyric_file{};	//ƥ��ĸ���ļ���·��
    Time lengh{};			//�����ĳ���
    int bitrate{};		//������
    wstring title;		//����
    wstring artist;		//������
    wstring album;		//��Ƭ��
    wstring year;		//���
    wstring comment;	//ע��
    wstring genre;		//����
    BYTE genre_idx{ 255 };		//���ֽڱ�ʾ�����ɺ�
    int track{};		//�������
    int tag_type{};		//��ǩ�����ͣ�0��������1��ID3v1��2��ID3v2��
    bool info_acquired{ false };		//����Ѿ���ȡ������Ϣ����Ϊture
    Time start_pos{};		//��Ƶ����ʼλ�ã�����cue�ֹ�
    Time end_pos{};
    bool is_cue{ false };		//�����Ŀ��cue�ֹ죬��Ϊtrue
    wstring song_id{};			//������Ӧ�������������еĸ���ID
    int listen_time{};			//�����ۼ�����ʱ�䣨��λΪ�룩
    bool no_online_lyric{ false };         //���Ϊtrue�����������ظ��
    bool no_online_album_cover{ false };   //���Ϊtrue������������ר������

    bool is_favourite{ false };

	__int64 last_played_time{};		//�ϴβ��ŵ�ʱ��

    //�����ļ����ıȽϺ������������ļ�������
    static bool ByFileName(const SongInfo& a, const SongInfo& b)
    {
        return a.GetFileName() < b.GetFileName();
    }
    //���ݱ���ıȽϺ����������Ա�������
    static bool ByTitle(const SongInfo& a, const SongInfo& b)
    {
        return a.title < b.title;
    }
    //���������ҵıȽϺ���������������������
    static bool ByArtist(const SongInfo& a, const SongInfo& b)
    {
        return a.artist < b.artist;
    }
    //���ݳ�Ƭ���ıȽϺ����������Գ�Ƭ������
    static bool ByAlbum(const SongInfo& a, const SongInfo& b)
    {
        return a.album < b.album;
    }
    //����������ŵıȽϺ����������������������
    static bool ByTrack(const SongInfo& a, const SongInfo& b)
    {
        return a.track < b.track;
    }

    //����һ��SongInfo�����Ʊ�ǩ��Ϣ
    void CopyAudioTag(const SongInfo& song_info)
    {
        title = song_info.title;
        artist = song_info.artist;
        album = song_info.album;
        year = song_info.year;
        comment = song_info.comment;
        genre = song_info.genre;
        genre_idx = song_info.genre_idx;
        track = song_info.track;
        tag_type = song_info.tag_type;
    }

    void CopySongInfo(const SongInfo& song_info)
    {
        CopyAudioTag(song_info);
        lengh = song_info.lengh;
        bitrate = song_info.bitrate;
        listen_time = song_info.listen_time;
        song_id = song_info.song_id;
        is_favourite = song_info.is_favourite;
    }

    bool IsTitleEmpty() const
    {
        return title.empty() || title == wstring(CCommon::LoadText(IDS_DEFAULT_TITLE));
    }

    bool IsArtistEmpty() const
    {
        return artist.empty() || artist == wstring(CCommon::LoadText(IDS_DEFAULT_ARTIST));
    }

    bool IsAlbumEmpty() const
    {
        return album.empty() || album == wstring(CCommon::LoadText(IDS_DEFAULT_ALBUM));
    }

    bool IsYearEmpty() const
    {
        return year.empty() || year == wstring(CCommon::LoadText(IDS_DEFAULT_YEAR));
    }

    bool IsGenreEmpty() const
    {
        return genre.empty() || genre == wstring(CCommon::LoadText(IDS_DEFAULT_GENRE));
    }

    wstring GetTitle() const
    {
        return title.empty() ? wstring(CCommon::LoadText(IDS_DEFAULT_TITLE)) : title;
    }

    wstring GetArtist() const
    {
        return artist.empty() ? wstring(CCommon::LoadText(IDS_DEFAULT_ARTIST)) : artist;
    }

    wstring GetAlbum() const
    {
        return album.empty() ? wstring(CCommon::LoadText(IDS_DEFAULT_ALBUM)) : album;
    }

    wstring GetYear() const
    {
        return year.empty() ? wstring(CCommon::LoadText(IDS_DEFAULT_YEAR)) : year;
    }

    wstring GetGenre() const
    {
        return genre.empty() ? wstring(CCommon::LoadText(IDS_DEFAULT_GENRE)) : genre;
    }

    wstring GetFileName() const
    {
        wstring file_name;
        size_t index = file_path.rfind(L'\\');
        if (index == wstring::npos)
            index = file_path.rfind(L'/');
        if (index == wstring::npos)
            return file_path;

        file_name = file_path.substr(index + 1);
        return file_name;
    }

    bool IsSameSong(const SongInfo& song) const
    {
        if (!is_cue && !song.is_cue)
            return file_path == song.file_path;
        else
            return file_path == song.file_path && track == song.track;
    }
};

