#include "stdafx.h"
#include "Player.h"
#include "MusicPlayer2.h"
#include "COSUPlayerHelper.h"
#include "Playlist.h"
#include "BassCore.h"

CPlayer CPlayer::m_instance;

CPlayer::CPlayer()
{
    m_pCore = new CBassCore();
}

CPlayer & CPlayer::GetInstance()
{
    return m_instance;
}

CPlayer::~CPlayer()
{
    UnInitPlayerCore();
    delete m_pCore;
}

void CPlayer::IniPlayerCore()
{
    m_pCore->InitCore();
}

void CPlayer::UnInitPlayerCore()
{
    m_pCore->UnInitCore();
}

void CPlayer::Create()
{
    IniPlayerCore();
    LoadConfig();
    LoadRecentPath();
    LoadRecentPlaylist();
    if(!m_from_playlist)
    {
        IniPlayList();	//��ʼ�������б�
    }
    else
    {
        PlaylistInfo playlist_info;
        if (m_recent_playlist.m_use_default_playlist || m_recent_playlist.m_recent_playlists.empty())
            playlist_info = m_recent_playlist.m_default_playlist;
        else
            playlist_info = m_recent_playlist.m_recent_playlists.front();
        SetPlaylist(playlist_info.path, playlist_info.track, playlist_info.position, true);
    }
    SetTitle();		//�õ�ǰ���ڲ��ŵĸ�������Ϊ���ڱ���
}

void CPlayer::Create(const vector<wstring>& files)
{
    IniPlayerCore();
    LoadConfig();
    LoadRecentPath();
    LoadRecentPlaylist();
    OpenFiles(files);
}

void CPlayer::Create(const wstring& path)
{
    IniPlayerCore();
    LoadConfig();
    LoadRecentPath();
    LoadRecentPlaylist();
    OpenFolder(path);
    SetTitle();		//�õ�ǰ���ڲ��ŵĸ�������Ϊ���ڱ���
}

void CPlayer::IniPlayList(bool cmd_para, bool refresh_info, bool play)
{
    if (!m_loading)
    {
        m_from_playlist = cmd_para;
        m_is_ous_folder = !cmd_para && COSUPlayerHelper::IsOsuFolder(m_path);
        if (!cmd_para)
        {
            if (m_is_ous_folder)
                COSUPlayerHelper::GetOSUAudioFiles(m_path, m_playlist);
            else
                CAudioCommon::GetAudioFiles(m_path, m_playlist, MAX_SONG_NUM);
        }
        //m_index = 0;
        //m_song_num = m_playlist.size();
        m_index_tmp = m_index;		//����������
        if (m_index < 0 || m_index >= GetSongNum()) m_index = 0;		//ȷ����ǰ������Ų��ᳬ����������

        m_song_length = { 0, 0, 0 };
        if (GetSongNum() == 0)
        {
            m_playlist.push_back(SongInfo{});		//û�и���ʱ�򲥷��б����һ���յ�SongInfo����
        }

        m_loading = true;
        //m_thread_info.playlist = &m_playlist;
        m_thread_info.refresh_info = refresh_info;
        m_thread_info.sort = !cmd_para;
        m_thread_info.play = play;
        //m_thread_info.path = m_path;
        //������ʼ�������б�Ĺ����߳�
        m_pThread = AfxBeginThread(IniPlaylistThreadFunc, &m_thread_info);
    }
}

UINT CPlayer::IniPlaylistThreadFunc(LPVOID lpParam)
{
    CCommon::SetThreadLanguage(theApp.m_general_setting_data.language);
    SendMessage(theApp.m_pMainWnd->GetSafeHwnd(), WM_PLAYLIST_INI_START, 0, 0);
    ThreadInfo* pInfo = (ThreadInfo*)lpParam;
    //��ȡ�����б���ÿһ�׸�������Ϣ
    //���ֻ��ȡMAX_NUM_LENGTH�׸�ĳ��ȣ�����MAX_NUM_LENGTH�����ĸ����ĳ����ڴ�ʱ��á���ֹ�ļ�������Ƶ�ļ����ർ�µȴ�ʱ�����
    int song_num = GetInstance().m_playlist.size();
    int song_count = min(song_num, MAX_NUM_LENGTH);
    for (int i{}, count{}; count < song_count && i < song_num; i++)
    {
        pInfo->process_percent = i * 100 / song_count + 1;

        if (!pInfo->refresh_info)
        {
            wstring file_name{ GetInstance().m_playlist[i].file_name };
            auto iter = theApp.m_song_data.find(GetInstance().m_playlist[i].file_path);
            if (iter != theApp.m_song_data.end())		//���������Ϣ�������Ѿ������ø���������Ҫ�ٻ�ȡ������Ϣ
            {
                GetInstance().m_playlist[i].CopySongInfo(iter->second);
                //GetInstance().m_playlist[i] = iter->second;
                //GetInstance().m_playlist[i].file_name = file_name;
                //GetInstance().m_playlist[i].file_path = iter->first;
                continue;
            }
        }
        wstring file_path{ GetInstance().m_playlist[i].file_path };
        HSTREAM hStream;
        hStream = BASS_StreamCreateFile(FALSE, file_path.c_str(), 0, 0, BASS_SAMPLE_FLOAT);
        GetInstance().AcquireSongInfo(hStream, file_path, GetInstance().m_playlist[i], GetInstance().m_is_ous_folder);
        BASS_StreamFree(hStream);
        count++;
    }
    GetInstance().m_loading = false;
    GetInstance().IniPlaylistComplate();
    //GetInstance().IniLyrics();
    PostMessage(theApp.m_pMainWnd->GetSafeHwnd(), WM_PLAYLIST_INI_COMPLATE, 0, 0);
    return 0;
}

void CPlayer::IniPlaylistComplate()
{
    CAudioCommon::CheckCueFiles(m_playlist, m_path);
    CAudioCommon::GetCueTracks(m_playlist, m_path);
    //m_song_num = m_playlist.size();
    m_index = m_index_tmp;
    if (m_index < 0 || m_index >= GetSongNum()) m_index = 0;		//ȷ����ǰ������Ų��ᳬ����������
    //ͳ���б���ʱ��
    m_total_time = 0;
    for (const auto& somg : m_playlist)
    {
        m_total_time += somg.lengh.time2int();
    }

    if(!IsPlaying())
    {
        //�����ʺ�ר������
        m_album_cover.Destroy();
        m_album_cover_blur.Destroy();
        m_Lyrics = CLyrics();
    }

    //�Բ����б�����
    wstring current_file_name = GetCurrentFileName();		//����ǰ���浱ǰ�����ļ���
    if (m_thread_info.sort && m_playlist.size() > 1)
        SortPlaylist(false);

    SearchLyrics();

    if (GetSongNum() > 0)
    {
        if (m_playing == 0)		//�����б��ʼ����ɣ���������ɺ������ʱû���ڲ��ţ����������ò��ŵ��ļ�
        {
            if (!m_current_file_name_tmp.empty())		//���ִ����ReloadPlaylist��m_current_file_name_tmp��Ϊ�գ������m_current_file_name_tmp�������Ŀ������
            {
                //�������벥���б�󣬲������ڲ�����Ŀ�����
                MusicControl(Command::CLOSE);
                for (int i{}; i < GetSongNum(); i++)
                {
                    if (m_current_file_name_tmp == m_playlist[i].file_name)
                    {
                        m_index = i;
                        //m_current_file_name = m_current_file_name_tmp;
                        break;
                    }
                }
                m_current_file_name_tmp.clear();
                MusicControl(Command::OPEN);
                MusicControl(Command::SEEK);
                if(m_thread_info.play)
                    MusicControl(Command::PLAY);
            }
            else		//����ֱ�Ӵ򿪲��ŵ�index����Ŀ
            {
                MusicControl(Command::CLOSE);
                //m_current_file_name = m_playlist[m_index].file_name;
                MusicControl(Command::OPEN);
                MusicControl(Command::SEEK);
                if (theApp.m_play_setting_data.auto_play_when_start || m_thread_info.play)
                    MusicControl(Command::PLAY);
            }
        }
        else		//����û��ڲ��ų�ʼ���Ĺ����н����˲��ţ���������ڲ��ŵ��ļ������²������ڲ��ŵ����
        {
            for (int i{}; i < GetSongNum(); i++)
            {
                if (current_file_name == m_playlist[i].file_name)
                {
                    m_index = i;
                    break;
                }
            }
        }
    }
    //if(!sort)		//����ļ���ͨ�������в����򿪵ģ���sort��Ϊfalse����ʱ�򿪺�ֱ�Ӳ���
    //    MusicControl(Command::PLAY);

    EmplaceCurrentPathToRecent();
    EmplaceCurrentPlaylistToRecent();
    SetTitle();
    m_shuffle_list.clear();
    if (m_repeat_mode == RM_PLAY_SHUFFLE)
        m_shuffle_list.push_back(m_index);
}

void CPlayer::SearchLyrics(/*bool refresh*/)
{
    //��������ļ�
    //���������ģ��ƥ�䣬�Ƚ����еĸ���ļ����ļ������浽�������Թ�ģ��ƥ��ʱ����
    if (theApp.m_lyric_setting_data.lyric_fuzzy_match)
    {
        m_current_path_lyrics.clear();
        m_lyric_path_lyrics.clear();
        CAudioCommon::GetLyricFiles(m_path, m_current_path_lyrics);
        CAudioCommon::GetLyricFiles(theApp.m_lyric_setting_data.lyric_path, m_lyric_path_lyrics);
    }

    //���������б���ÿһ�׸����ĸ���ļ�����������ļ�·�����浽�б���
    for (auto& song : m_playlist)
    {
        if (song.file_name.size() < 3) continue;
        song.lyric_file.clear();		//�������ǰ�����֮ǰ�Ѿ��������ĸ��
        //if (!song.lyric_file.empty() && CCommon::FileExist(song.lyric_file))		//���������Ϣ���и���ļ����Ҹ���ļ����ڣ�����Ҫ�ٻ�ȡ���
        //	continue;
        CFilePathHelper lyric_path{ song.file_path };		//�õ�·��+�ļ������ַ���
        lyric_path.ReplaceFileExtension(L"lrc");		//���ļ���չ�滻��lrc
        CFilePathHelper lyric_path2{ theApp.m_lyric_setting_data.lyric_path + song.file_name };
        lyric_path2.ReplaceFileExtension(L"lrc");
        //���Ҹ���ļ����͸����ļ�����ȫƥ��ĸ��
        if (CCommon::FileExist(lyric_path.GetFilePath()))
        {
            song.lyric_file = lyric_path.GetFilePath();
        }
        else if (CCommon::FileExist(lyric_path2.GetFilePath()))		//��ǰĿ¼��û�ж�Ӧ�ĸ���ļ�ʱ������theApp.m_lyric_setting_data.m_lyric_pathĿ¼��Ѱ�Ҹ���ļ�
        {
            song.lyric_file = lyric_path2.GetFilePath();
        }
        else if (theApp.m_lyric_setting_data.lyric_fuzzy_match)
        {
            wstring matched_lyric;		//ƥ��ĸ�ʵ�·��
            //��Ѱ�Ҹ���ļ���ͬʱ������������������ҵĸ���ļ�
            for (const auto& str : m_current_path_lyrics)	//�ڵ�ǰĿ¼��Ѱ��
            {
                //if (str.find(song.artist) != string::npos && str.find(song.title) != string::npos)
                if (CCommon::StringNatchWholeWord(str, song.artist) != -1 && CCommon::StringNatchWholeWord(str, song.title) != -1)
                {
                    matched_lyric = m_path + str;
                    break;
                }
            }

            if (matched_lyric.empty())		//�����ǰĿ¼��û�ҵ�
            {
                for (const auto& str : m_lyric_path_lyrics)	//�ڸ��Ŀ¼��Ѱ��
                {
                    //if (str.find(song.artist) != string::npos && str.find(song.title) != string::npos)
                    if (CCommon::StringNatchWholeWord(str, song.artist) != -1 && CCommon::StringNatchWholeWord(str, song.title) != -1)
                    {
                        matched_lyric = theApp.m_lyric_setting_data.lyric_path + str;
                        break;
                    }
                }
            }

            //û���ҵ��Ļ���Ѱ�Ҹ���ļ���ֻ������������ĸ���ļ�
            if (matched_lyric.empty())
            {
                for (const auto& str : m_current_path_lyrics)	//�ڵ�ǰĿ¼��Ѱ��
                {
                    //if (str.find(song.title) != string::npos)
                    if (CCommon::StringNatchWholeWord(str, song.title) != -1)
                    {
                        matched_lyric = m_path + str;
                        break;
                    }
                }
            }

            if (matched_lyric.empty())
            {
                for (const auto& str : m_lyric_path_lyrics)	//�ڸ��Ŀ¼��Ѱ��
                {
                    //if (str.find(song.title) != string::npos)
                    if (CCommon::StringNatchWholeWord(str, song.title) != -1)
                    {
                        matched_lyric = theApp.m_lyric_setting_data.lyric_path + str;
                        break;
                    }
                }
            }

            if (!matched_lyric.empty())
                song.lyric_file = matched_lyric;
        }
        ////����Ѿ���ȡ���˸�ʣ��򽫸��·�����浽���и�����Ϣ������
        //auto iter = theApp.m_song_data.find(m_path + song.file_name);
        //if (iter != theApp.m_song_data.end())
        //	iter->second.lyric_file = song.lyric_file;
    }
}

void CPlayer::IniLyrics()
{
    if (!m_playlist.empty() && !m_playlist[m_index].lyric_file.empty())
        m_Lyrics = CLyrics{ m_playlist[m_index].lyric_file };
    else
        m_Lyrics = CLyrics{};
}

void CPlayer::IniLyrics(const wstring& lyric_path)
{
    m_Lyrics = CLyrics{ lyric_path };
    m_playlist[m_index].lyric_file = lyric_path;
}


void CPlayer::MusicControl(Command command, int volume_step)
{
    if (!CCommon::FileExist(GetCurrentFilePath()))
        return;

    switch (command)
    {
    case Command::OPEN:
        m_error_code = 0;
        m_pCore->Open(GetCurrentFilePath().c_str());
        //��ȡ��Ƶ����
        m_current_file_type = m_pCore->GetAudioType();		//����ͨ����Ϣ��ȡ��ǰ��Ƶ�ļ�������
        if (m_current_file_type.empty())		//�����ȡ������Ƶ�ļ������ͣ������ļ���չ����Ϊ�ļ�����
        {
            CFilePathHelper file_path{ GetCurrentFileName() };
            m_current_file_type = file_path.GetFileExtension(true);
        }
        if (GetSongNum() > 0)
        {
            if (!m_playlist[m_index].info_acquired)	//�����ǰ�򿪵��ļ�û���ڳ�ʼ�������б�ʱ�����Ϣ�����ʱ���»�ȡ
                AcquireSongInfo(m_pCore->GetHandle(), GetCurrentFilePath(), m_playlist[m_index], m_is_ous_folder);
            m_song_length = m_playlist[m_index].lengh;
            m_song_length_int = m_song_length.time2int();
            //��ʱ��ȡר������
            SearchAlbumCover();
            //��ʼ�����
            IniLyrics();
        }
        if (m_playlist[m_index].is_cue)
        {
            //SeekTo(0);
            m_song_length = GetCurrentSongInfo().lengh;
            m_song_length_int = m_song_length.time2int();
        }
        SetVolume();
        memset(m_spectral_data, 0, sizeof(m_spectral_data));		//���ļ�ʱ���Ƶ�׷���������
        //SetFXHandle();
        if (m_equ_enable)
            SetAllEqualizer();
        if (m_reverb_enable)
            m_pCore->SetReverb(m_reverb_mix, m_reverb_time);
        else
            m_pCore->ClearReverb();
        PostMessage(theApp.m_pMainWnd->m_hWnd, WM_MUSIC_STREAM_OPENED, 0, 0);
        break;
    case Command::PLAY:
        ConnotPlayWarning();
        m_pCore->Play();
        m_playing = 2;
        break;
    case Command::CLOSE:
        //RemoveFXHandle();
        m_pCore->Close();
        m_playing = 0;
        break;
    case Command::PAUSE:
        m_pCore->Pause();
        m_playing = 1;
        break;
    case Command::STOP:
        m_pCore->Stop();
        m_playing = 0;
        SeekTo(0);
        memset(m_spectral_data, 0, sizeof(m_spectral_data));		//ֹͣʱ���Ƶ�׷���������
        //GetBASSCurrentPosition();
        break;
    case Command::FF:		//���
        GetBASSCurrentPosition();		//��ȡ��ǰλ�ã����룩
        m_current_position_int += 5000;		//ÿ�ο��5000����
        if (m_current_position_int > m_song_length_int) m_current_position_int -= 5000;
        SeekTo(m_current_position_int);
        break;
    case Command::REW:		//����
        GetBASSCurrentPosition();		//��ȡ��ǰλ�ã����룩
        m_current_position_int -= 5000;		//ÿ�ο���5000����
        if (m_current_position_int < 0) m_current_position_int = 0;		//��ֹ���˵�����λ��
        SeekTo(m_current_position_int);
        break;
    case Command::PLAY_PAUSE:
        if (m_playing == 2)
        {
            m_pCore->Pause();
            m_playing = 1;
        }
        else
        {
            ConnotPlayWarning();
            m_pCore->Play();
            m_playing = 2;
        }
        break;
    case Command::VOLUME_UP:
        if (m_volume < 100)
        {
            m_volume += volume_step;
            if (m_volume > 100) m_volume = 100;
            SetVolume();
            SaveConfig();
        }
        break;
    case Command::VOLUME_DOWN:
        if (m_volume > 0)
        {
            m_volume -= volume_step;
            if (m_volume < 0) m_volume = 0;
            SetVolume();
            SaveConfig();
        }
        break;
    case Command::SEEK:		//��λ��m_current_position��λ��
        if (m_current_position_int > m_song_length_int)
        {
            m_current_position_int = 0;
            m_current_position = Time{ 0, 0, 0 };
        }
        SeekTo(m_current_position_int);
        break;
    default:
        break;
    }
    GetBASSError();
}

bool CPlayer::SongIsOver() const
{
    if (GetCurrentSongInfo().is_cue)
    {
        return (m_playing == 2 && m_current_position_int >= m_song_length_int);
    }
    else
    {
        bool song_is_over;
        static int last_pos;
        if ((m_playing == 2 && m_current_position_int == last_pos && m_current_position_int != 0	//������ڲ����ҵ�ǰ���ŵ�λ��û�з����仯�ҵ�ǰ����λ�ò�Ϊ0��
                && m_current_position_int > m_song_length_int - 2000)		//�Ҳ��Ž��ȵ������2��
                || m_error_code == BASS_ERROR_ENDED)	//���߳���BASS_ERROR_ENDED�������жϵ�ǰ������������
            //��ʱ������ʶ��ĸ������ȳ���ʵ�ʸ������ȵ����⣬�����ᵼ�¸������Ž��ȳ���ʵ�ʸ�����βʱ�����BASS_ERROR_ENDED����
            //��⵽�������ʱֱ���жϸ����Ѿ��������ˡ�
            song_is_over = true;
        else
            song_is_over = false;
        last_pos = m_current_position_int;
        return song_is_over;
        //���ﱾ��ֱ��ʹ��return m_current_position_int>=m_song_length_int���жϸ����������ˣ�
        //����BASS��Ƶ���ڲ���ʱ���ܻ���ֵ�ǰ����λ��һֱ�޷������������λ�õ����⣬
        //���������ͻ�һֱ����false��
    }
}

void CPlayer::GetBASSSongLength()
{
    m_song_length_int = m_pCore->GetSongLength();
    m_song_length.int2time(m_song_length_int);		//������ת����Time�ṹ
}

void CPlayer::GetBASSCurrentPosition()
{
    m_current_position_int = m_pCore->GetCurPosition();
    if (m_playlist[m_index].is_cue)
    {
        m_current_position_int -= m_playlist[m_index].start_pos.time2int();
    }
    m_current_position.int2time(m_current_position_int);
}


void CPlayer::SetVolume()
{
    m_pCore->SetVolume(m_volume);
    GetBASSError();
}


void CPlayer::CalculateSpectralData()
{
    if (m_pCore->GetHandle() && m_playing != 0 && m_current_position_int < m_song_length_int - 500)	//ȷ����Ƶ�����Ϊ�գ����Ҹ������500���벻��ʾƵ�ף��Է�ֹ��������ĩβ�޷���ȡƵ�׵Ĵ���
    {
        //BASS_ChannelGetData(m_pCore->GetHandle(), m_fft, BASS_DATA_FFT256);
        m_pCore->GetFFTData(m_fft);
        memset(m_spectral_data, 0, sizeof(m_spectral_data));
        for (int i{}; i < FFT_SAMPLE; i++)
        {
            m_spectral_data[i / (FFT_SAMPLE / SPECTRUM_ROW)] += m_fft[i];
        }

        for (int i{}; i < SPECTRUM_ROW; i++)
        {
            m_spectral_data[i] /= (FFT_SAMPLE / SPECTRUM_ROW);
            m_spectral_data[i] = std::sqrtf(m_spectral_data[i]);		//��ÿ��Ƶ�����ε�ֵȡƽ�������Լ��ٲ�ͬƵ��Ƶ��ֵ�Ĳ���
            m_spectral_data[i] *= 60;			//��������ĳ������Ե���Ƶ�׷�������ͼ����ĸ߶�
        }
    }
    else
    {
        memset(m_spectral_data, 0, sizeof(m_spectral_data));
    }
    //����Ƶ�׶��˵ĸ߶�
    if (m_playing != 1)
    {
        static int fall_count;
        for (int i{}; i < SPECTRUM_ROW; i++)
        {
            if (m_spectral_data[i] > m_last_spectral_data[i])
            {
                m_spectral_peak[i] = m_spectral_data[i];		//�����ǰ��Ƶ�ױ���һ�ε�Ƶ�׸ߣ���Ƶ�׶��˸߶���Ϊ��ǰƵ�׵ĸ߶�
                fall_count = 0;
            }
            else
            {
                fall_count++;
                m_spectral_peak[i] -= (fall_count * 0.2f);		//�����ǰƵ�ױ���һ�ε�Ƶ�����ͣ���Ƶ�׶��˵ĸ߶����½�
            }
        }
    }

    memcpy_s(m_last_spectral_data, sizeof(m_last_spectral_data), m_spectral_data, sizeof(m_spectral_data));
}


int CPlayer::GetCurrentSecond()
{
    return m_current_position.sec;
}

bool CPlayer::IsPlaying() const
{
    return m_playing == 2;
}

bool CPlayer::PlayTrack(int song_track)
{
    switch (m_repeat_mode)
    {
    case RM_PLAY_ORDER:		//˳�򲥷�
        if (song_track == NEXT)		//������һ��
            song_track = m_index + 1;
        if (song_track == PREVIOUS)		//������һ��
            song_track = m_index - 1;
        break;
    case RM_PLAY_SHUFFLE:		//�������
        if (song_track == NEXT)
        {
            SYSTEMTIME current_time;
            GetLocalTime(&current_time);			//��ȡ��ǰʱ��
            srand(current_time.wMilliseconds);		//�õ�ǰʱ��ĺ��������ò��������������
            song_track = rand() % GetSongNum();
            m_shuffle_list.push_back(song_track);	//����������Ź�����Ŀ
        }
        else if (song_track == PREVIOUS)		//������һ�����������Ŀ
        {
            if (m_shuffle_list.size() >= 2)
            {
                if (m_index == m_shuffle_list.back())
                    m_shuffle_list.pop_back();
                song_track = m_shuffle_list.back();
            }
            else
            {
                MusicControl(Command::STOP);	//�޷�����ʱֹͣ����
                return true;
            }
        }
        //else if (song_track >= 0 && song_track < m_song_num)
        //{
        //	m_shuffle_list.push_back(song_track);	//����������Ź�����Ŀ
        //}
        break;
    case RM_LOOP_PLAYLIST:		//�б�ѭ��
        if (song_track == NEXT)		//������һ��
        {
            song_track = m_index + 1;
            if (song_track >= GetSongNum()) song_track = 0;
            if (song_track < 0) song_track = GetSongNum() - 1;
        }
        if (song_track == PREVIOUS)		//������һ��
        {
            song_track = m_index - 1;
            if (song_track >= GetSongNum()) song_track = 0;
            if (song_track < 0) song_track = GetSongNum() - 1;
        }
        break;
    case RM_LOOP_TRACK:		//����ѭ��
        if (song_track == NEXT || song_track == PREVIOUS)
            song_track = m_index;
    }

    if (song_track >= 0 && song_track < GetSongNum())
    {
        MusicControl(Command::CLOSE);
        m_index = song_track;
        //m_current_file_name = m_playlist[m_index].file_name;
        MusicControl(Command::OPEN);
        //IniLyrics();
        if (m_playlist[m_index].is_cue)
            SeekTo(0);
        MusicControl(Command::PLAY);
        GetBASSCurrentPosition();
        SetTitle();
        SaveConfig();
        EmplaceCurrentPathToRecent();
        SaveRecentPath();
        return true;
    }
    else
    {
        MusicControl(Command::CLOSE);
        m_index = 0;
        //m_current_file_name = m_playlist[m_index].file_name;
        MusicControl(Command::OPEN);
        //IniLyrics();
        GetBASSCurrentPosition();
        SetTitle();
        SaveConfig();
        EmplaceCurrentPathToRecent();
        SaveRecentPath();
    }
    return false;
}

void CPlayer::ChangePath(const wstring& path, int track)
{
    if (m_loading) return;
    MusicControl(Command::CLOSE);
    m_path = path;
    if (m_path.empty() || (m_path.back() != L'/' && m_path.back() != L'\\'))		//����������·��Ϊ�ջ�ĩβû��б�ܣ�����ĩβ����һ��
        m_path.append(1, L'\\');
    m_playlist.clear();		//��ղ����б�
    m_index = track;
    //��ʼ�������б�
    IniPlayList();		//������·�����³�ʼ�������б�
    m_current_position_int = 0;
    m_current_position = { 0, 0, 0 };
    SaveConfig();
    SetTitle();
    //MusicControl(Command::OPEN);
    //IniLyrics();
}

void CPlayer::SetPath(const wstring& path, int track, int position, SortMode sort_mode)
{
    //if (m_song_num>0 && !m_playlist[0].file_name.empty())		//�����ǰ·���и������ͱ��浱ǰ·�������·��
    if (m_from_playlist)
        EmplaceCurrentPlaylistToRecent();
    else
        EmplaceCurrentPathToRecent();
    m_sort_mode = sort_mode;
    ChangePath(path, track);
    m_current_position_int = position;
    m_current_position.int2time(m_current_position_int);
    //MusicControl(Command::SEEK);
    EmplaceCurrentPathToRecent();		//�����µ�·�������·��

}

void CPlayer::SetPlaylist(const wstring& playlist_path, int track, int position, bool init)
{
    m_recent_playlist.m_use_default_playlist = (playlist_path == m_recent_playlist.m_default_playlist.path);

    if(!init)
    {
        SaveCurrentPlaylist();
        if (m_from_playlist)
            EmplaceCurrentPlaylistToRecent();
        else
            EmplaceCurrentPathToRecent();
        MusicControl(Command::STOP);
        MusicControl(Command::CLOSE);
    }
    m_playlist.clear();
    CPlaylist playlist;
    playlist.LoadFromFile(playlist_path);

    SongInfo song_info;
    for (const auto& file : playlist.GetPlaylist())
    {
        CFilePathHelper file_path{ file };
        song_info.file_name = file_path.GetFileName();
        song_info.file_path = file;
        m_playlist.push_back(song_info);	//���ļ������浽�����б�
    }

    m_index = track;
    m_current_position_int = position;
    m_current_position.int2time(m_current_position_int);
    SetTitle();
    m_playlist_path = playlist_path;
    EmplaceCurrentPlaylistToRecent();

    IniPlayList(true);
}

void CPlayer::OpenFolder(wstring path)
{
    if (m_loading) return;
    if (path.empty() || (path.back() != L'/' && path.back() != L'\\'))		//����򿪵���·��Ϊ�ջ�ĩβû��б�ܣ�����ĩβ����һ��
        path.append(1, L'\\');
    bool path_exist{ false };
    int track;
    int position;
    if (GetSongNum() > 0) EmplaceCurrentPathToRecent();		//�����ǰ·���и������ͱ��浱ǰ·�������·��
    //���򿪵�·���Ƿ��Ѿ����������·����
    for (const auto& a_path_info : m_recent_path)
    {
        if (path == a_path_info.path)
        {
            path_exist = true;
            track = a_path_info.track;
            position = a_path_info.position;
            m_sort_mode = a_path_info.sort_mode;
            break;
        }
    }
    if (path_exist)			//����򿪵�·���Ѿ����������·����
    {
        ChangePath(path, track);
        m_current_position_int = position;
        m_current_position.int2time(m_current_position_int);
        MusicControl(Command::SEEK);
        EmplaceCurrentPathToRecent();		//����򿪵�·�������·��
        SaveRecentPath();
    }
    else		//����򿪵�·�����µ�·��
    {
        m_sort_mode = SM_FILE;
        ChangePath(path);
        EmplaceCurrentPathToRecent();		//�����µ�·�������·��
        SaveRecentPath();
    }
}

void CPlayer::OpenFiles(const vector<wstring>& files, bool play)
{
    //���ļ�ʱʼ����ӵ�Ĭ�ϲ����б���

    if (files.empty()) return;
    if (m_loading) return;

    if(m_pCore->GetHandle() != 0)
        MusicControl(Command::CLOSE);
    if (GetSongNum() > 0)
    {
        if (!m_from_playlist || !m_recent_playlist.m_use_default_playlist)
            SaveCurrentPlaylist();
        EmplaceCurrentPlaylistToRecent();
    }

    m_recent_playlist.m_use_default_playlist = true;
    m_from_playlist = true;
    m_playlist_path = m_recent_playlist.m_default_playlist.path;

    //����Ĭ�ϲ����б�
    m_playlist.clear();
    CPlaylist playlist;
    playlist.LoadFromFile(m_recent_playlist.m_default_playlist.path);
    playlist.ToSongList(m_playlist);

    //�����ŵ��ļ���ӵ�Ĭ�ϲ����б�
    int play_index = GetSongNum();        //���ŵ����
    for (const auto& file : files)
    {
        SongInfo song_info;
        CFilePathHelper file_path{ file };
        song_info.file_name = file_path.GetFileName();
        song_info.file_path = file;

        auto iter = std::find_if(m_playlist.begin(), m_playlist.end(), [file](const SongInfo& song)
        {
            return song.file_path == file;
        });

        if (iter == m_playlist.end())     //���Ҫ�򿪵��ļ����ڲ����б�������
            m_playlist.push_back(song_info);
        else
            play_index = iter - m_playlist.begin();
    }
    m_index = play_index;

    m_current_position_int = 0;
    m_current_position = Time();

    SaveCurrentPlaylist();
    SetTitle();		//�õ�ǰ���ڲ��ŵĸ�������Ϊ���ڱ���

    IniPlayList(true, false, play);
}

void CPlayer::OpenAFile(wstring file)
{
    if (file.empty()) return;
    if (m_loading) return;
    MusicControl(Command::CLOSE);
    if (GetSongNum() > 0) EmplaceCurrentPathToRecent();		//�ȱ��浱ǰ·���Ͳ��Ž��ȵ����·��

    CFilePathHelper file_path(file);
    m_path = file_path.GetDir();
    m_playlist.clear();
    m_current_position_int = 0;
    m_current_position = { 0, 0, 0 };
    m_index = 0;
    //m_current_file_name = file.substr(index + 1);
    //m_song_num = 1;

    //��ȡ��·��������ʽ
    m_sort_mode = SortMode::SM_FILE;
    for (const auto& path_info : m_recent_path)
    {
        if (m_path == path_info.path)
            m_sort_mode = path_info.sort_mode;
    }

    //��ʼ�������б�
    m_current_file_name_tmp = file_path.GetFileName();
    IniPlayList(false, false);		//������·�����³�ʼ�������б�
}

void CPlayer::AddFiles(const vector<wstring>& files)
{
    if (m_playlist.size() == 1 && m_playlist[0].file_path.empty() && m_playlist[0].file_name.empty())
        m_playlist.clear();     //ɾ�������б��е�ռλ��

    SongInfo song_info;
    for (const auto& file : files)
    {
        CFilePathHelper file_path{ file };
        song_info.file_name = file_path.GetFileName();
        song_info.file_path = file;
        m_playlist.push_back(song_info);
    }
    SaveCurrentPlaylist();
    IniPlayList(true);
}

void CPlayer::SetRepeatMode()
{
    int repeat_mode{ static_cast<int>(m_repeat_mode) };
    repeat_mode++;
    if (repeat_mode > 3)
        repeat_mode = 0;
    m_repeat_mode = static_cast<RepeatMode>(repeat_mode);
    SaveConfig();
}

void CPlayer::SetRepeatMode(RepeatMode repeat_mode)
{
    m_repeat_mode = repeat_mode;
    SaveConfig();
}

RepeatMode CPlayer::GetRepeatMode() const
{
    return m_repeat_mode;
}

bool CPlayer::GetBASSError()
{
    if (m_loading)
        return false;
    int error_code_tmp = BASS_ErrorGetCode();
    if (error_code_tmp && error_code_tmp != m_error_code)
    {
        CString info = CCommon::LoadTextFormat(IDS_BASS_ERROR_LOG_INFO, {error_code_tmp, GetCurrentFilePath()});
        theApp.WriteErrorLog(wstring{ info });
    }
    m_error_code = error_code_tmp;
    return true;
}

bool CPlayer::IsError() const
{
    if (m_loading)		//��������б����ڼ��أ��򲻼�����
        return false;
    else
        return (m_error_code != 0 || m_pCore->GetHandle() == 0);
}

void CPlayer::SetTitle() const
{
//#ifdef _DEBUG
//	SetWindowText(theApp.m_pMainWnd->m_hWnd, (m_current_file_name + L" - MusicPlayer2(DEBUGģʽ)").c_str());		//�õ�ǰ���ڲ��ŵĸ�������Ϊ���ڱ���
//#else
//	SetWindowText(theApp.m_pMainWnd->m_hWnd, (m_current_file_name + L" - MusicPlayer2").c_str());		//�õ�ǰ���ڲ��ŵĸ�������Ϊ���ڱ���
//#endif
    SendMessage(theApp.m_pMainWnd->m_hWnd, WM_SET_TITLE, 0, 0);
}

void CPlayer::SaveConfig() const
{
    CIniHelper ini(theApp.m_config_path);

    //ini.WriteString(L"config", L"path", m_path.c_str());
    //ini.WriteInt(L"config", L"track", m_index);
    ini.WriteInt(L"config", L"volume", m_volume);
    //ini.WriteInt(L"config", L"position", m_current_position_int);
    ini.WriteInt(L"config", L"repeat_mode", static_cast<int>(m_repeat_mode));
    ini.WriteBool(L"config", L"lyric_karaoke_disp", theApp.m_lyric_setting_data.lyric_karaoke_disp);
    ini.WriteString(L"config", L"lyric_path", theApp.m_lyric_setting_data.lyric_path);
    ini.WriteInt(L"config", L"sort_mode", static_cast<int>(m_sort_mode));
    ini.WriteBool(L"config", L"lyric_fuzzy_match", theApp.m_lyric_setting_data.lyric_fuzzy_match);
    ini.WriteString(L"config", L"default_album_file_name", CCommon::StringMerge(theApp.m_app_setting_data.default_album_name, L','));
    ini.WriteBool(L"config", L"from_playlist", m_from_playlist);

    //����������趨
    ini.WriteBool(L"equalizer", L"equalizer_enable", m_equ_enable);
    //����ÿ��������ͨ��������
    //if (m_equ_style == 9)
    //{
    //	wchar_t buff[16];
    //	for (int i{}; i < EQU_CH_NUM; i++)
    //	{
    //		swprintf_s(buff, L"channel%d", i + 1);
    //		ini.WriteInt(L"equalizer", buff, m_equalizer_gain[i]);
    //	}
    //}
    //��������趨
    ini.WriteInt(L"reverb", L"reverb_enable", m_reverb_enable);
    ini.WriteInt(L"reverb", L"reverb_mix", m_reverb_mix);
    ini.WriteInt(L"reverb", L"reverb_time", m_reverb_time);

    ini.Save();
}

void CPlayer::LoadConfig()
{
    CIniHelper ini(theApp.m_config_path);

    //ini.GetString(L"config", L"path", L".\\songs\\");
    //m_path = buff;
    //m_index =ini.GetInt(L"config", L"track", 0);
    m_volume = ini.GetInt(L"config", L"volume", 60);
    //m_current_position_int =ini.GetInt(L"config", L"position", 0);
    //m_current_position.int2time(m_current_position_int);
    m_repeat_mode = static_cast<RepeatMode>(ini.GetInt(L"config", L"repeat_mode", 0));
    theApp.m_lyric_setting_data.lyric_path = ini.GetString(L"config", L"lyric_path", L".\\lyrics\\");
    if (!theApp.m_lyric_setting_data.lyric_path.empty() && theApp.m_lyric_setting_data.lyric_path.back() != L'/' && theApp.m_lyric_setting_data.lyric_path.back() != L'\\')
        theApp.m_lyric_setting_data.lyric_path.append(1, L'\\');
    theApp.m_lyric_setting_data.lyric_karaoke_disp = ini.GetBool(L"config", L"lyric_karaoke_disp", true);
    m_sort_mode = static_cast<SortMode>(ini.GetInt(L"config", L"sort_mode", 0));
    theApp.m_lyric_setting_data.lyric_fuzzy_match = ini.GetBool(L"config", L"lyric_fuzzy_match", true);
    wstring default_album_name = ini.GetString(L"config", L"default_album_file_name", L"cover");
    CCommon::StringSplit(default_album_name, L',', theApp.m_app_setting_data.default_album_name);

    m_from_playlist = ini.GetBool(L"config", L"from_playlist", m_from_playlist);

    //��ȡ�������趨
    m_equ_enable = ini.GetBool(L"equalizer", L"equalizer_enable", false);
    m_equ_style = ini.GetInt(L"equalizer", L"equalizer_style", 0);	//��ȡ������Ԥ��
    if (m_equ_style == 9)		//���������Ԥ��Ϊ���Զ��塱
    {
        //��ȡÿ��������ͨ��������
        for (int i{}; i < EQU_CH_NUM; i++)
        {
            wchar_t buff[16];
            swprintf_s(buff, L"channel%d", i + 1);
            m_equalizer_gain[i] = ini.GetInt(L"equalizer", buff, 0);
        }
    }
    else if (m_equ_style >= 0 && m_equ_style < 9)		//���򣬸��ݾ�����Ԥ������ÿ��ͨ��������
    {
        for (int i{}; i < EQU_CH_NUM; i++)
        {
            m_equalizer_gain[i] = EQU_STYLE_TABLE[m_equ_style][i];
        }
    }
    //��ȡ�����趨
    m_reverb_enable = ini.GetBool(L"reverb", L"reverb_enable", 0);
    m_reverb_mix = ini.GetInt(L"reverb", L"reverb_mix", 45);		//����ǿ��Ĭ��Ϊ50
    m_reverb_time = ini.GetInt(L"reverb", L"reverb_time", 100);	//����ʱ��Ĭ��Ϊ1s
}

void CPlayer::ExplorePath(int track) const
{
    if (GetSongNum() > 0)
    {
        CString str;
        if (track < 0)		//trackС��0������Դ��������ѡ�е�ǰ���ŵ��ļ�
            str.Format(_T("/select,\"%s\""), GetCurrentFilePath().c_str());
        else if (track < GetSongNum())		//trackΪ�����б��е�һ����ţ�����Դ��������ѡ��ָ�����ļ�
            str.Format(_T("/select,\"%s\""), m_playlist[track].file_path.c_str());
        else								//track���������б����ļ�������������Դ��������ѡ���κ��ļ�
            str = m_path.c_str();
        ShellExecute(NULL, _T("open"), _T("explorer"), str, NULL, SW_SHOWNORMAL);
    }
}

void CPlayer::ExploreLyric() const
{
    if (!m_Lyrics.IsEmpty())
    {
        CString str;
        str.Format(_T("/select,\"%s\""), m_Lyrics.GetPathName().c_str());
        ShellExecute(NULL, _T("open"), _T("explorer"), str, NULL, SW_SHOWNORMAL);
    }
}


Time CPlayer::GetAllSongLength(int track) const
{
    if (track >= 0 && track < GetSongNum())
        return m_playlist[track].lengh;
    else
        return Time();
}

int CPlayer::GetSongNum() const
{
    return static_cast<int>(m_playlist.size());
}

wstring CPlayer::GetCurrentDir() const
{
    wstring current_file_path = GetCurrentFilePath();
    CFilePathHelper path_helper(current_file_path);
    return path_helper.GetDir();
}

wstring CPlayer::GetCurrentFolderOrPlaylistName() const
{
    if (m_from_playlist)
    {
        CFilePathHelper file_path{ m_playlist_path };
        wstring playlist_name = file_path.GetFileName();
        if (playlist_name == DEFAULT_PLAYLIST_NAME)
            return wstring(CCommon::LoadText(_T("["), IDS_DEFAULT, _T("]")));
        else
            return file_path.GetFileNameWithoutExtension();
    }
    else
    {
        return m_path;
    }
}

wstring CPlayer::GetCurrentFilePath() const
{
    if (m_index >= 0 && m_index < GetSongNum())
    {
        if (m_playlist[m_index].file_path.empty())
            return m_path + m_playlist[m_index].file_name;
        else
            return m_playlist[m_index].file_path;
    }
    else
        return wstring();
}

wstring CPlayer::GetFileName() const
{
    return (GetCurrentFileName().empty() ? CCommon::LoadText(IDS_FILE_NOT_FOUND).GetString() : GetCurrentFileName());
}

void CPlayer::DeleteAlbumCover()
{
    if (!m_inner_cover)
    {
        if (CCommon::DeleteAFile(theApp.m_pMainWnd->GetSafeHwnd(), m_album_cover_path.c_str()) == 0)
            m_album_cover.Destroy();
    }
}

void CPlayer::ReloadPlaylist()
{
    if (m_loading) return;
    MusicControl(Command::CLOSE);
    m_current_file_name_tmp = GetCurrentFileName();	//���浱ǰ���ŵ���Ŀ���ļ����������ڲ����б��ʼ������ʱȷ�����ŵĻ���֮ǰ���ŵ���Ŀ
    if(!m_from_playlist)
    {
        m_playlist.clear();		//��ղ����б�
        IniPlayList(false, true);		//������·�����³�ʼ�������б�
    }
    else
    {
        m_playlist.clear();
        CPlaylist playlist;
        playlist.LoadFromFile(m_playlist_path);
        playlist.ToSongList(m_playlist);

        IniPlayList(true, true);
    }
}

void CPlayer::RemoveSong(int index)
{
    if (m_loading) return;
    if (index >= 0 && index < GetSongNum())
    {
        m_playlist.erase(m_playlist.begin() + index);
        //m_song_num--;
        if (index == m_index)		//���Ҫɾ������Ŀ�����ڲ��ŵ���Ŀ�����²��ŵ�ǰ��Ŀ
        {
            if(GetSongNum() > 0)
                PlayTrack(m_index);
        }
        else if (index < m_index)	//���Ҫɾ������Ŀ�����ڲ��ŵ���Ŀ֮ǰ�������ڲ��ŵ���Ŀ��ż�1
        {
            m_index--;
        }
        if (GetSongNum() == 0)
            m_playlist.push_back(SongInfo());
    }
}

void CPlayer::RemoveSongs(vector<int> indexes)
{
    int size = indexes.size();
    for (int i{}; i < size; i++)
    {
        RemoveSong(indexes[i]);
        if (i <= size - 2 && indexes[i + 1] > indexes[i])
        {
            for (int j{ i + 1 }; j < size; j++)
                indexes[j]--;
        }
    }
}

int CPlayer::RemoveSameSongs()
{
    int removed = 0;
    for (int i = 0; i < GetSongNum(); i++)
    {
        for (int j = i + 1; j < GetSongNum(); j++)
        {
            if (m_playlist[i].file_path == m_playlist[j].file_path)
            {
                RemoveSong(j);
                removed++;
            }
        }
    }
    return removed;
}

int CPlayer::RemoveInvalidSongs()
{
    int removed = 0;
    for (int i = 0; i < GetSongNum(); i++)
    {
        if(!CCommon::FileExist(m_playlist[i].file_path) || m_playlist[i].lengh.isZero())
        {
            RemoveSong(i);
            removed++;
        }
    }
    return removed;
}

void CPlayer::ClearPlaylist()
{
    if (m_loading) return;
    m_playlist.clear();
    //m_song_num = 0;
    MusicControl(Command::STOP);
}

bool CPlayer::MoveUp(int first, int last)
{
    if (!m_from_playlist)
        return false;

    if (first <= 0 || last >= GetSongNum() || last < first)
        return false;
    
    if (m_index >= first && m_index <= last)
        m_index--;
    else if (m_index == first - 1)
        m_index = last;

    for (int i = first; i <= last; i++)
    {
        std::swap(m_playlist[i - 1], m_playlist[i]);
    }
    SaveCurrentPlaylist();
    return true;
}

bool CPlayer::MoveDown(int first, int last)
{
    if (!m_from_playlist)
        return false;
    
    if (first < 0 || last >= GetSongNum() - 1 || last < first)
        return false;

    if (m_index >= first && m_index <= last)
        m_index++;
    else if (m_index == last + 1)
        m_index = first;

    for (int i = last + 1; i > first; i--)
    {
        std::swap(m_playlist[i], m_playlist[i - 1]);
    }
    SaveCurrentPlaylist();
    return true;
}

void CPlayer::SeekTo(int position)
{
    if (position > m_song_length_int)
        position = m_song_length_int;
    m_current_position_int = position;
    m_current_position.int2time(position);
    if (m_playlist[m_index].is_cue)
    {
        position += m_playlist[m_index].start_pos.time2int();
    }
    m_pCore->SetCurPosition(position);
    GetBASSError();
}

void CPlayer::SeekTo(double position)
{
    int pos = static_cast<int>(m_song_length_int * position);
    SeekTo(pos);
}

void CPlayer::SeekTo(HSTREAM hStream, int position)
{
    double pos_sec = static_cast<double>(position) / 1000.0;
    QWORD pos_bytes;
    pos_bytes = BASS_ChannelSeconds2Bytes(hStream, pos_sec);
    BASS_ChannelSetPosition(hStream, pos_bytes, BASS_POS_BYTE);
    GetInstance().GetBASSError();
}

void CPlayer::ClearLyric()
{
    m_Lyrics = CLyrics{};
    m_playlist[m_index].lyric_file.clear();
}

wstring CPlayer::GetTimeString() const
{
    wchar_t buff[16];
    swprintf_s(buff, L"%d:%.2d/%d:%.2d", m_current_position.min, m_current_position.sec, m_song_length.min, m_song_length.sec);
    return wstring(buff);
}

wstring CPlayer::GetPlayingState() const
{
    if (m_error_code != 0)
        return CCommon::LoadText(IDS_PLAY_ERROR).GetString();
    switch (m_playing)
    {
    case 0:
        return CCommon::LoadText(IDS_STOPED).GetString();
    case 1:
        return CCommon::LoadText(IDS_PAUSED).GetString();
    case 2:
        return CCommon::LoadText(IDS_NOW_PLAYING).GetString();
    }
    return wstring();
}

const SongInfo & CPlayer::GetCurrentSongInfo() const
{
    if (m_index >= 0 && m_index < GetSongNum())
        return m_playlist[m_index];
    else return m_no_use;
}

void CPlayer::SetRelatedSongID(wstring song_id)
{
    if (m_index >= 0 && m_index < GetSongNum())
    {
        m_playlist[m_index].song_id = song_id;
        if(!m_playlist[m_index].is_cue)
        {
            theApp.m_song_data[m_path + m_playlist[m_index].file_name] = m_playlist[m_index];
            theApp.SetSongDataModified();
        }
    }
}

void CPlayer::SetRelatedSongID(int index, wstring song_id)
{
    if (index >= 0 && index < GetSongNum())
    {
        m_playlist[index].song_id = song_id;
        if (!m_playlist[index].is_cue)
        {
            theApp.m_song_data[m_path + m_playlist[index].file_name] = m_playlist[index];
            theApp.SetSongDataModified();
        }
    }
}

void CPlayer::AddListenTime(int sec)
{
    if (m_index >= 0 && m_index < GetSongNum())
    {
        m_playlist[m_index].listen_time += sec;
        if (!m_playlist[m_index].is_cue)
        {
            theApp.m_song_data[m_path + m_playlist[m_index].file_name] = m_playlist[m_index];
            theApp.SetSongDataModified();
        }
    }
}

int CPlayer::GetChannels()
{
    return m_pCore->GetChannels();
}

int CPlayer::GetFreq()
{
    return m_pCore->GetFReq();
}

void CPlayer::ReIniPlayerCore(bool replay)
{
    int playing = m_playing;
    UnInitPlayerCore();
    IniPlayerCore();
    MusicControl(Command::OPEN);
    MusicControl(Command::SEEK);
    if (replay && playing == 2)
    {
        MusicControl(Command::PLAY);
    }
    else
    {
        m_playing = 0;
    }
}

void CPlayer::SortPlaylist(bool change_index)
{
    if (m_loading) return;
    int track_number = m_playlist[m_index].track;
    wstring current_file_name = GetCurrentFileName();
    switch (m_sort_mode)
    {
    case SM_FILE:
        std::sort(m_playlist.begin(), m_playlist.end(), SongInfo::ByFileName);
        break;
    case SM_TITLE:
        std::sort(m_playlist.begin(), m_playlist.end(), SongInfo::ByTitle);
        break;
    case SM_ARTIST:
        std::sort(m_playlist.begin(), m_playlist.end(), SongInfo::ByArtist);
        break;
    case SM_ALBUM:
        std::sort(m_playlist.begin(), m_playlist.end(), SongInfo::ByAlbum);
        break;
    case SM_TRACK:
        std::sort(m_playlist.begin(), m_playlist.end(), SongInfo::ByTrack);
        break;
    default:
        break;
    }

    if (change_index)
    {
        //�����б�����󣬲������ڲ�����Ŀ�����
        if (!m_playlist[m_index].is_cue)
        {
            for (int i{}; i < GetSongNum(); i++)
            {
                if (current_file_name == m_playlist[i].file_name)
                {
                    m_index = i;
                    break;
                }
            }
        }
        else
        {
            for (int i{}; i < GetSongNum(); i++)
            {
                if (track_number == m_playlist[i].track)
                {
                    m_index = i;
                    break;
                }
            }
        }
    }
    SaveCurrentPlaylist();
}


void CPlayer::SaveRecentPath() const
{
    // �򿪻����½��ļ�
    CFile file;
    BOOL bRet = file.Open(theApp.m_recent_path_dat_path.c_str(),
                          CFile::modeCreate | CFile::modeWrite);
    if (!bRet)		//���ļ�ʧ��
    {
        return;
    }
    // ����CArchive����
    CArchive ar(&file, CArchive::store);
    // д����
    ar << m_recent_path.size();		//д��m_recent_path�����Ĵ�С
    for (auto& path_info : m_recent_path)
    {
        ar << CString(path_info.path.c_str())
           << path_info.track
           << path_info.position
           << static_cast<int>(path_info.sort_mode)
           << path_info.track_num
           << path_info.total_time;
    }
    // �ر�CArchive����
    ar.Close();
    // �ر��ļ�
    file.Close();

}

void CPlayer::OnExit()
{
    SaveConfig();
    //�˳�ʱ������󲥷ŵ���Ŀ��λ��
    if (!m_from_playlist && !m_recent_path.empty() && GetSongNum() > 0 && !m_playlist[0].file_name.empty())
    {
        m_recent_path[0].track = m_index;
        m_recent_path[0].position = m_current_position_int;
    }
    SaveRecentPath();
    EmplaceCurrentPlaylistToRecent();
    m_recent_playlist.SavePlaylistData();
    SaveCurrentPlaylist();
}

void CPlayer::LoadRecentPath()
{
    // ���ļ�
    CFile file;
    BOOL bRet = file.Open(theApp.m_recent_path_dat_path.c_str(), CFile::modeRead);
    if (!bRet)		//�ļ�������
    {
        m_path = L".\\songs\\";		//Ĭ�ϵ�·��
        return;
    }
    // ����CArchive����
    CArchive ar(&file, CArchive::load);
    // ������
    size_t size{};
    PathInfo path_info;
    CString temp;
    int sort_mode;
    try
    {
        ar >> size;		//��ȡӳ�������ĳ���
        for (size_t i{}; i < size; i++)
        {
            ar >> temp;
            path_info.path = temp;
            ar >> path_info.track;
            ar >> path_info.position;
            ar >> sort_mode;
            path_info.sort_mode = static_cast<SortMode>(sort_mode);
            ar >> path_info.track_num;
            ar >> path_info.total_time;
            if (path_info.path.empty() || path_info.path.size() < 2) continue;		//���·��Ϊ�ջ�·��̫�̣��ͺ�����
            if (path_info.path.back() != L'/' && path_info.path.back() != L'\\')	//�����ȡ����·��ĩβû��б�ܣ�����ĩβ����һ��
                path_info.path.push_back(L'\\');
            m_recent_path.push_back(path_info);
        }
    }
    catch (CArchiveException* exception)
    {
        //�������л�ʱ���ֵ��쳣
        CString info;
        info = CCommon::LoadTextFormat(IDS_RECENT_PATH_SERIALIZE_ERROR_LOG, { exception->m_cause });
        theApp.WriteErrorLog(wstring{ info });
    }
    // �رն���
    ar.Close();
    // �ر��ļ�
    file.Close();

    //��recent_path�ļ��л�ȡ·�������ŵ�����Ŀ��λ��
    if(!m_from_playlist)
    {
        if (!m_recent_path.empty())
        {
            m_path = m_recent_path[0].path;
            if (!m_path.empty() && m_path.back() != L'/' && m_path.back() != L'\\')		//�����ȡ������·��ĩβû��б�ܣ�����ĩβ����һ��
                m_path.push_back(L'\\');

            m_index = m_recent_path[0].track;
            m_current_position_int = m_recent_path[0].position;
            m_current_position.int2time(m_current_position_int);
        }
        else
        {
            m_path = L".\\songs\\";		//Ĭ�ϵ�·��
        }
    }
}

void CPlayer::LoadRecentPlaylist()
{
    m_recent_playlist.LoadPlaylistData();
    if(m_from_playlist)
    {
        if (m_recent_playlist.m_use_default_playlist)
        {
            m_playlist_path = m_recent_playlist.m_default_playlist.path;
            m_index = m_recent_playlist.m_default_playlist.track;
            m_current_position_int = m_recent_playlist.m_default_playlist.position;
            m_current_position.int2time(m_current_position_int);
        }
        else if (!m_recent_playlist.m_recent_playlists.empty())
        {
            m_playlist_path = m_recent_playlist.m_recent_playlists.front().path;
            m_index = m_recent_playlist.m_recent_playlists.front().track;
            m_current_position_int = m_recent_playlist.m_recent_playlists.front().position;
            m_current_position.int2time(m_current_position_int);

        }
    }
}

void CPlayer::SaveCurrentPlaylist()
{
    if(m_from_playlist)
    {
        wstring current_playlist;
        if (m_recent_playlist.m_use_default_playlist || m_recent_playlist.m_recent_playlists.empty())
            current_playlist = m_recent_playlist.m_default_playlist.path;
        else
            current_playlist = m_recent_playlist.m_recent_playlists.front().path;
        CPlaylist playlist;
        playlist.FromSongList(m_playlist);
        playlist.SaveToFile(m_playlist_path);
    }
}

void CPlayer::EmplaceCurrentPathToRecent()
{
    if (m_from_playlist)
        return;

    for (size_t i{ 0 }; i < m_recent_path.size(); i++)
    {
        if (m_path == m_recent_path[i].path)
            m_recent_path.erase(m_recent_path.begin() + i);		//�����ǰ·���Ѿ������·���У��Ͱ������·����ɾ��
    }
    if (GetSongNum() == 0 || m_playlist[0].file_name.empty()) return;		//�����ǰ·����û���ļ����Ͳ�����
    PathInfo path_info;
    path_info.path = m_path;
    path_info.track = m_index;
    path_info.position = m_current_position_int;
    path_info.sort_mode = m_sort_mode;
    path_info.track_num = GetSongNum();
    path_info.total_time = m_total_time;
    if (GetSongNum() > 0)
        m_recent_path.push_front(path_info);		//��ǰ·�����뵽m_recent_path��ǰ��
}


void CPlayer::EmplaceCurrentPlaylistToRecent()
{
    if (!m_from_playlist)
        return;

    if (m_recent_playlist.m_use_default_playlist)
    {
        m_recent_playlist.m_default_playlist.position = m_current_position_int;
        m_recent_playlist.m_default_playlist.track = m_index;
        m_recent_playlist.m_default_playlist.track_num = GetSongNum();
        m_recent_playlist.m_default_playlist.total_time = m_total_time;
    }
    else
    {
        m_recent_playlist.EmplacePlaylist(m_playlist_path, m_index, m_current_position_int, GetSongNum(), m_total_time);
    }
}

//void CPlayer::SetFXHandle()
//{
//    GetBASSError();
//}
//
//void CPlayer::RemoveFXHandle()
//{
//    GetBASSError();
//}

void CPlayer::ApplyEqualizer(int channel, int gain)
{
    m_pCore->ApplyEqualizer(channel, gain);
    GetBASSError();
}

void CPlayer::SetEqualizer(int channel, int gain)
{
    if (channel < 0 || channel >= EQU_CH_NUM) return;
    m_equalizer_gain[channel] = gain;
    ApplyEqualizer(channel, gain);
}

int CPlayer::GeEqualizer(int channel)
{
    if (channel < 0 || channel >= EQU_CH_NUM) return 0;
    //BASS_DX8_PARAMEQ parameq;
    //int rtn;
    //rtn = BASS_FXGetParameters(m_equ_handle[channel], &parameq);
    //return static_cast<int>(parameq.fGain);
    return m_equalizer_gain[channel];
}

void CPlayer::SetAllEqualizer()
{
    for (int i{}; i < EQU_CH_NUM; i++)
    {
        ApplyEqualizer(i, m_equalizer_gain[i]);
    }
}

void CPlayer::ClearAllEqulizer()
{
    for (int i{}; i < EQU_CH_NUM; i++)
    {
        ApplyEqualizer(i, 0);
    }
}

void CPlayer::EnableEqualizer(bool enable)
{
    if (enable)
        SetAllEqualizer();
    else
        ClearAllEqulizer();
    m_equ_enable = enable;
}

void CPlayer::EnableReverb(bool enable)
{
    if (enable)
    {
        if (m_reverb_mix < 0) m_reverb_mix = 0;
        if (m_reverb_mix > 100) m_reverb_mix = 100;
        if (m_reverb_time < 1) m_reverb_time = 1;
        if (m_reverb_time > 300) m_reverb_time = 300;
        m_pCore->SetReverb(m_reverb_mix, m_reverb_time);
    }
    else
    {
        m_pCore->ClearReverb();
    }
    m_reverb_enable = enable;
    GetBASSError();
}


void CPlayer::ConnotPlayWarning() const
{
    if (m_pCore->IsMidiConnotPlay())
        PostMessage(theApp.m_pMainWnd->GetSafeHwnd(), WM_CONNOT_PLAY_WARNING, 0, 0);
}

void CPlayer::SearchAlbumCover()
{
    //static wstring last_file_path;
    //if (last_file_path != GetCurrentFilePath())		//��ֹͬһ���ļ���λ�ȡר������
    //{
    m_album_cover.Destroy();
    if ((!theApp.m_app_setting_data.use_out_image || theApp.m_app_setting_data.use_inner_image_first) && !IsOsuFolder())
    {
        //���ļ���ȡר������
        CAudioTag audio_tag(m_pCore->GetHandle(), GetCurrentFilePath(), m_playlist[m_index]);
        m_album_cover_path = audio_tag.GetAlbumCover(m_album_cover_type);
        m_album_cover.Load(m_album_cover_path.c_str());
    }
    m_inner_cover = !m_album_cover.IsNull();

    if (/*theApp.m_app_setting_data.use_out_image && */m_album_cover.IsNull())
    {
        //��ȡ����ר������ʱ����ʹ���ⲿͼƬ��Ϊ����
        SearchOutAlbumCover();
    }
    //AlbumCoverGaussBlur();
    //}
    //last_file_path = GetCurrentFilePath();
}

void CPlayer::AlbumCoverGaussBlur()
{
    if (!theApp.m_app_setting_data.background_gauss_blur || !theApp.m_app_setting_data.album_cover_as_background)
        return;
    if (m_album_cover.IsNull())
    {
        m_album_cover_blur.Destroy();
    }
    else
    {
        CImage image_tmp;
        CSize image_size(m_album_cover.GetWidth(), m_album_cover.GetHeight());
        //��ͼƬ��С�Լ�С��˹ģ���ļ�����
        CCommon::SizeZoom(image_size, 300);		//ͼƬ��С���������ţ�ʹ���ߵ���300
        if (!CDrawCommon::BitmapStretch(&m_album_cover, &image_tmp, image_size))		//����ͼƬ
            return;
#ifdef _DEBUG
        image_tmp.Save(_T("..\\Debug\\image_tmp.bmp"), Gdiplus::ImageFormatBMP);
#endif // _DEBUG

        //ִ�и�˹ģ��
        CGaussBlur gauss_blur;
        gauss_blur.SetSigma(static_cast<double>(theApp.m_app_setting_data.gauss_blur_radius) / 10);		//���ø�˹ģ���뾶
        gauss_blur.DoGaussBlur(image_tmp, m_album_cover_blur);
    }
}

wstring CPlayer::GetCurrentFileName() const
{
    if (m_index >= 0 && m_index < GetSongNum())
        return m_playlist[m_index].file_name;
    else
        return wstring();
}

void CPlayer::AcquireSongInfo(HSTREAM hStream, const wstring& file_path, SongInfo & song_info, bool osu_song)
{
    //��ȡ����
    song_info.lengh = CBassCore::GetBASSSongLength(hStream);
    //��ȡ������
    float bitrate{};
    BASS_ChannelGetAttribute(hStream, BASS_ATTRIB_BITRATE, &bitrate);
    song_info.bitrate = static_cast<int>(bitrate + 0.5f);
    if (osu_song)
    {
        COSUPlayerHelper::GetOSUAudioTitleArtist(song_info);
    }
    else
    {
        //��ȡ��Ƶ��ǩ
        CAudioTag audio_tag(hStream, file_path, song_info);
        audio_tag.GetAudioTag(theApp.m_general_setting_data.id3v2_first);
        //��ȡmidi���ֵı���
        if (CBassCore::m_bass_midi_lib.IsSuccessed() && audio_tag.GetAudioType() == AU_MIDI)
        {
            BASS_MIDI_MARK mark;
            if (CBassCore::m_bass_midi_lib.BASS_MIDI_StreamGetMark(hStream, BASS_MIDI_MARK_TRACK, 0, &mark) && !mark.track)
            {
                song_info.title = CCommon::StrToUnicode(mark.text);
                song_info.info_acquired = true;
            }
        }
        CFilePathHelper c_file_path(file_path);
        song_info.file_name = c_file_path.GetFileName();
    }
    //���������Ϣ
    SongInfo& song = theApp.m_song_data[file_path];
    song.CopyAudioTag(song_info);
    song.lengh = song_info.lengh;
    song.bitrate = song_info.bitrate;
}

void CPlayer::SearchOutAlbumCover()
{
    if (m_is_ous_folder)
    {
        m_album_cover_path = COSUPlayerHelper::GetAlbumCover(GetCurrentFilePath());
        if (m_album_cover_path.empty())
            m_album_cover_path = theApp.m_nc_setting_data.default_osu_img;
    }
    else
    {
        m_album_cover_path = GetRelatedAlbumCover(GetCurrentFilePath(), GetCurrentSongInfo());
    }
    if (!m_album_cover.IsNull())
        m_album_cover.Destroy();
    m_album_cover.Load(m_album_cover_path.c_str());
}

wstring CPlayer::GetRelatedAlbumCover(const wstring& file_path, const SongInfo& song_info)
{
    vector<wstring> files;
    wstring file_name;
    //�����ļ��͸�����һ�µ�ͼƬ�ļ�
    CFilePathHelper c_file_path(file_path);
    //file_name = m_path + c_file_name.GetFileNameWithoutExtension() + L".*";
    c_file_path.ReplaceFileExtension(L"*");
    wstring dir{ c_file_path.GetDir() };
    CCommon::GetImageFiles(c_file_path.GetFilePath(), files);
    if (files.empty() && !song_info.album.empty())
    {
        //û���ҵ��͸�����һ�µ�ͼƬ�ļ���������ļ���Ϊ����Ƭ�������ļ�
        wstring album_name{ song_info.album };
        CCommon::FileNameNormalize(album_name);
        file_name = dir + album_name + L".*";
        CCommon::GetImageFiles(file_name, files);
    }
    //if (files.empty() && !theApp.m_app_setting_data.default_album_name.empty())
    //{
    //	//û���ҵ���Ƭ��Ϊ�ļ������ļ��������ļ���ΪDEFAULT_ALBUM_NAME���ļ�
    //	file_name = m_path + theApp.m_app_setting_data.default_album_name + L".*";
    //	CCommon::GetImageFiles(file_name, files);
    //}
    //û���ҵ���Ƭ��Ϊ�ļ������ļ��������ļ���Ϊ���õ�ר�����������ļ�
    if (theApp.m_app_setting_data.use_out_image)
    {
        for (const auto& album_name : theApp.m_app_setting_data.default_album_name)
        {
            if (!files.empty())
                break;
            if (!album_name.empty())
            {
                file_name = dir + album_name + L".*";
                CCommon::GetImageFiles(file_name, files);
            }
        }
    }
    //if (files.empty())
    //{
    //	//û���ҵ��ļ���ΪDEFAULT_ALBUM_NAME���ļ���������Ϊ��Folder���ļ���
    //	file_name = m_path + L"Folder" + L".*";
    //	CCommon::GetImageFiles(file_name, files);
    //}
    if (!files.empty())
    {
        //m_album_cover_path = m_path + files[0];
        //if (!m_album_cover.IsNull())
        //	m_album_cover.Destroy();
        //m_album_cover.Load(m_album_cover_path.c_str());
        return wstring(dir + files[0]);
    }
    else
    {
        return wstring();
    }
}

void CPlayer::SetPlaylistPath(const wstring& playlist_path)
{
    m_playlist_path = playlist_path;
}

wstring CPlayer::GetPlaylistPath() const
{
    return m_playlist_path;
}
