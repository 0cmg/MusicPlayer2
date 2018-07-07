#include "stdafx.h"
#include "Player.h"
#include "MusicPlayer2.h"


CPlayer::CPlayer()
{
	//����BASS���
	m_no_ape_plugin = (BASS_PluginLoad("bass_ape.dll", 0) == 0);
}

CPlayer::~CPlayer()
{
	BASS_Stop();	//ֹͣ���
	BASS_Free();	//�ͷ�Bass��Դ
}

void CPlayer::IniBASS()
{
	//��ʼ��BASE��Ƶ��
	BASS_Init(
		-1,//Ĭ���豸
		44100,//���������44100������ֵ��
		BASS_DEVICE_CPSPEAKERS,//�źţ�BASS_DEVICE_CPSPEAKERS ע��ԭ�����£�
							   /* Use the Windows control panel setting to detect the number of speakers.*/
							   /* Soundcards generally have their own control panel to set the speaker config,*/
							   /* so the Windows control panel setting may not be accurate unless it matches that.*/
							   /* This flag has no effect on Vista, as the speakers are already accurately detected.*/
		theApp.m_pMainWnd->m_hWnd,//���򴰿�,0���ڿ���̨����
		NULL//���ʶ��,0ʹ��Ĭ��ֵ
	);
}

void CPlayer::Create()
{
	IniBASS();
	LoadConfig();
	LoadRecentPath();
	IniPlayList();	//��ʼ�������б�
	//EmplaceCurrentPathToRecent();
	SetTitle();		//�õ�ǰ���ڲ��ŵĸ�������Ϊ���ڱ���
}

void CPlayer::Create(const vector<wstring>& files)
{
	IniBASS();
	LoadConfig();
	LoadRecentPath();
	size_t index;
	index = files[0].find_last_of(L'\\');
	m_path = files[0].substr(0, index + 1);
	SongInfo song_info;
	for (const auto& file : files)
	{
		index = file.find_last_of(L'\\');
		song_info.file_name = file.substr(index + 1);
		m_playlist.push_back(song_info);
	}
	IniPlayList(true);
	//EmplaceCurrentPathToRecent();
	m_current_position_int = 0;
	m_current_position = { 0,0,0 };
	m_index = 0;
	SetTitle();		//�õ�ǰ���ڲ��ŵĸ�������Ϊ���ڱ���
}

void CPlayer::Create(const wstring& path)
{
	IniBASS();
	LoadConfig();
	LoadRecentPath();
	//IniPlayList();	//��ʼ�������б�
	//EmplaceCurrentPathToRecent();
	OpenFolder(path);
	SetTitle();		//�õ�ǰ���ڲ��ŵĸ�������Ϊ���ڱ���
}

void CPlayer::IniPlayList(bool cmd_para, bool refresh_info)
{
	if (!m_loading)
	{
		if (!cmd_para)
		{
			CAudioCommon::GetAudioFiles(m_path, m_playlist, MAX_SONG_NUM);
		}
		//m_index = 0;
		m_song_num = m_playlist.size();
		m_index_tmp = m_index;		//����������
		if (m_index < 0 || m_index >= m_song_num) m_index = 0;		//ȷ����ǰ������Ų��ᳬ����������

		m_loading = true;
		//m_thread_info.playlist = &m_playlist;
		m_thread_info.refresh_info = refresh_info;
		m_thread_info.sort = !cmd_para;
		//m_thread_info.path = m_path;
		m_thread_info.player = this;
		//������ʼ�������б�Ĺ����߳�
		m_pThread = AfxBeginThread(IniPlaylistThreadFunc, &m_thread_info);

		m_song_length = { 0,0,0 };
		//m_current_position = {0,0,0};
		if (m_song_num == 0)
		{
			m_playlist.push_back(SongInfo{});		//û�и���ʱ�򲥷��б����һ���յ�SongInfo����
		}
		m_current_file_name = m_playlist[m_index].file_name;
	}
}

UINT CPlayer::IniPlaylistThreadFunc(LPVOID lpParam)
{
	SendMessage(theApp.m_pMainWnd->GetSafeHwnd(), WM_PLAYLIST_INI_START, 0, 0);
	ThreadInfo* pInfo = (ThreadInfo*)lpParam;
	//��ȡ�����б���ÿһ�׸�������Ϣ
	//���ֻ��ȡMAX_NUM_LENGTH�׸�ĳ��ȣ�����MAX_NUM_LENGTH�����ĸ����ĳ����ڴ�ʱ��á���ֹ�ļ�������Ƶ�ļ����ർ�µȴ�ʱ�����
	int song_num = pInfo->player->m_playlist.size();
	int song_count = min(song_num, MAX_NUM_LENGTH);
	for (int i{}, count{}; count < song_count && i < song_num; i++)
	{
		pInfo->process_percent = i * 100 / song_count + 1;

		if (!pInfo->refresh_info)
		{
			wstring file_name{ pInfo->player->m_playlist[i].file_name };
			auto iter = theApp.m_song_data.find(pInfo->player->m_path + pInfo->player->m_playlist[i].file_name);
			if (iter != theApp.m_song_data.end())		//���������Ϣ�������Ѿ������ø���������Ҫ�ٻ�ȡ������Ϣ
			{
				pInfo->player->m_playlist[i] = iter->second;
				pInfo->player->m_playlist[i].file_name = file_name;
				continue;
			}
		}
		wstring current_file_name = pInfo->player->m_playlist[i].file_name;
		HSTREAM hStream;
		hStream = BASS_StreamCreateFile(FALSE, (pInfo->player->m_path + current_file_name).c_str(), 0, 0, BASS_SAMPLE_FLOAT);
		//��ȡ����
		pInfo->player->m_playlist[i].lengh = GetBASSSongLength(hStream);
		//��ȡ������
		float bitrate{};
		BASS_ChannelGetAttribute(hStream, BASS_ATTRIB_BITRATE, &bitrate);
		pInfo->player->m_playlist[i].bitrate = static_cast<int>(bitrate + 0.5f);
		//��ȡ��Ƶ��ǩ
		//AudioType type = CAudioCommon::GetAudioType(current_file_name);
		//CAudioCommon::GetAudioTags(hStream, type, pInfo->player->m_path, pInfo->player->m_playlist[i]);
		CAudioTag audio_tag(hStream, pInfo->player->m_path, pInfo->player->m_playlist[i]);
		audio_tag.GetAudioTag(theApp.m_general_setting_data.id3v2_first);
		BASS_StreamFree(hStream);
		theApp.m_song_data[pInfo->player->m_path + pInfo->player->m_playlist[i].file_name] = pInfo->player->m_playlist[i];
		count++;
	}
	pInfo->player->m_loading = false;
	pInfo->player->IniPlaylistComplate(pInfo->sort);
	pInfo->player->IniLyrics();
	PostMessage(theApp.m_pMainWnd->GetSafeHwnd(), WM_PLAYLIST_INI_COMPLATE, 0, 0);
	return 0;
}

void CPlayer::IniPlaylistComplate(bool sort)
{
	CAudioCommon::CheckCueFiles(m_playlist, m_path);
	CAudioCommon::GetCueTracks(m_playlist, m_path);
	m_song_num = m_playlist.size();
	m_index = m_index_tmp;
	if (m_index < 0 || m_index >= m_song_num) m_index = 0;		//ȷ����ǰ������Ų��ᳬ����������
	//ͳ���б���ʱ��
	m_total_time = 0;
	for (const auto& somg : m_playlist)
	{
		m_total_time += somg.lengh.time2int();
	}

	//�Բ����б�����
	if (sort && m_playlist.size() > 1)
		SortPlaylist(false);

	if (m_song_num > 0)
	{
		if (m_playing == 0)		//�����б��ʼ����ɣ���������ɺ������ʱû���ڲ��ţ����������ò��ŵ��ļ�
		{
			if (!m_current_file_name_tmp.empty())		//���ִ����ReloadPlaylist��m_current_file_name_tmp��Ϊ�գ������m_current_file_name_tmp�������Ŀ������
			{
				//�������벥���б�󣬲������ڲ�����Ŀ�����
				MusicControl(Command::CLOSE);
				for (int i{}; i < m_playlist.size(); i++)
				{
					if (m_current_file_name_tmp == m_playlist[i].file_name)
					{
						m_index = i;
						m_current_file_name = m_current_file_name_tmp;
						break;
					}
				}
				m_current_file_name_tmp.clear();
				MusicControl(Command::OPEN);
				MusicControl(Command::SEEK);
				//MusicControl(Command::PLAY);
			}
			else		//����ֱ�Ӵ򿪲��ŵ�index����Ŀ
			{
				MusicControl(Command::CLOSE);
				m_current_file_name = m_playlist[m_index].file_name;
				MusicControl(Command::OPEN);
				MusicControl(Command::SEEK);
			}
		}
		else		//����û��ڲ��ų�ʼ���Ĺ����н����˲��ţ���������ڲ��ŵ��ļ������²������ڲ��ŵ����
		{
			for (int i{}; i < m_playlist.size(); i++)
			{
				if (m_current_file_name == m_playlist[i].file_name)
				{
					m_index = i;
					break;
				}
			}
		}
	}
	if(!sort)		//����ļ���ͨ�������в����򿪵ģ���sort��Ϊfalse����ʱ�򿪺�ֱ�Ӳ���
		MusicControl(Command::PLAY);

	EmplaceCurrentPathToRecent();
	SetTitle();
	SearchLyrics();
	m_shuffle_list.clear();
	if (m_repeat_mode == RM_PLAY_SHUFFLE)
		m_shuffle_list.push_back(m_index);
}

void CPlayer::SearchLyrics(/*bool refresh*/)
{
	//��������ļ�
	//���������ģ��ƥ�䣬�Ƚ����еĸ���ļ����ļ������浽�������Թ�ģ��ƥ��ʱ����
	if (theApp.m_play_setting_data.lyric_fuzzy_match)
	{
		m_current_path_lyrics.clear();
		m_lyric_path_lyrics.clear();
		CAudioCommon::GetLyricFiles(m_path, m_current_path_lyrics);
		CAudioCommon::GetLyricFiles(theApp.m_play_setting_data.lyric_path, m_lyric_path_lyrics);
	}

	//���������б���ÿһ�׸����ĸ���ļ�����������ļ�·�����浽�б���
	for (auto& song : m_playlist)
	{
		if (song.file_name.size() < 3) continue;
		song.lyric_file.clear();		//�������ǰ�����֮ǰ�Ѿ��������ĸ��
		//if (!song.lyric_file.empty() && CCommon::FileExist(song.lyric_file))		//���������Ϣ���и���ļ����Ҹ���ļ����ڣ�����Ҫ�ٻ�ȡ���
		//	continue;
		CFilePathHelper lyric_path{ m_path + song.file_name };		//�õ�·��+�ļ������ַ���
		lyric_path.ReplaceFileExtension(L"lrc");		//���ļ���չ�滻��lrc
		CFilePathHelper lyric_path2{ theApp.m_play_setting_data.lyric_path + song.file_name };
		lyric_path2.ReplaceFileExtension(L"lrc");
		//���Ҹ���ļ����͸����ļ�����ȫƥ��ĸ��
		if (CCommon::FileExist(lyric_path.GetFilePath()))
		{
			song.lyric_file = lyric_path.GetFilePath();
		}
		else if (CCommon::FileExist(lyric_path2.GetFilePath()))		//��ǰĿ¼��û�ж�Ӧ�ĸ���ļ�ʱ������theApp.m_play_setting_data.m_lyric_pathĿ¼��Ѱ�Ҹ���ļ�
		{
			song.lyric_file = lyric_path2.GetFilePath();
		}
		else if (theApp.m_play_setting_data.lyric_fuzzy_match)
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
						matched_lyric = theApp.m_play_setting_data.lyric_path + str;
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
						matched_lyric = theApp.m_play_setting_data.lyric_path + str;
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
	switch (command)
	{
	case Command::OPEN:
		m_error_code = 0;
		m_musicStream = BASS_StreamCreateFile(FALSE, (m_path + m_current_file_name).c_str(), 0, 0, BASS_SAMPLE_FLOAT);
		if (m_song_num > 0)
		{
			//if (m_index >= MAX_NUM_LENGTH && m_playlist[m_index].lengh.isZero())	//�����ǰ�򿪵��ļ�û���ڳ�ʼ�������б�ʱ�����Ϣ�����ʱ���»�ȡ
			//AudioType type = CAudioCommon::GetAudioType(m_current_file_name);
			if (!m_playlist[m_index].info_acquired)	//�����ǰ�򿪵��ļ�û���ڳ�ʼ�������б�ʱ�����Ϣ�����ʱ���»�ȡ
			{
				GetBASSSongLength();			//�򿪺����»�ȡ�ļ�����
				m_playlist[m_index].lengh = m_song_length;		//���ļ����ٴν���ȡ���ļ����ȱ��浽m_playlist������
				float bitrate{};
				BASS_ChannelGetAttribute(m_musicStream, BASS_ATTRIB_BITRATE, &bitrate);
				m_playlist[m_index].bitrate = static_cast<int>(bitrate + 0.5f);
				//CAudioCommon::GetAudioTags(m_musicStream, type, m_path, m_playlist[m_index]);
				CAudioTag audio_tag(m_musicStream, m_path, m_playlist[m_index]);
				audio_tag.GetAudioTag(theApp.m_general_setting_data.id3v2_first);
				theApp.m_song_data[m_path + m_current_file_name] = m_playlist[m_index];
			}
			else
			{
				m_song_length = m_playlist[m_index].lengh;
				m_song_length_int = m_song_length.time2int();
			}

			//��ʱ��ȡר������
			SearchAlbumCover();
		}
		if (m_playlist[m_index].is_cue)
		{
			//SeekTo(0);
			m_song_length = GetCurrentSongInfo().lengh;
			m_song_length_int = m_song_length.time2int();
		}
		SetVolume();
		memset(m_spectral_data, 0, sizeof(m_spectral_data));		//���ļ�ʱ���Ƶ�׷���������
		SetFXHandle();
		if (m_equ_enable)
			SetAllEqualizer();
		if (m_reverb_enable)
			SetReverb(m_reverb_mix, m_reverb_time);
		else
			ClearReverb();
		PostMessage(theApp.m_pMainWnd->m_hWnd, WM_MUSIC_STREAM_OPENED, 0, 0);
		break;
	case Command::PLAY:
		ConnotPlayWarning();
		BASS_ChannelPlay(m_musicStream, FALSE); m_playing = 2;
		break;
	case Command::CLOSE:
		RemoveFXHandle();
		BASS_StreamFree(m_musicStream);
		m_playing = 0;
		break;
	case Command::PAUSE: BASS_ChannelPause(m_musicStream); m_playing = 1; break;
	case Command::STOP:
		BASS_ChannelStop(m_musicStream);
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
			BASS_ChannelPause(m_musicStream);
			m_playing = 1;
		}
		else
		{
			ConnotPlayWarning();
			BASS_ChannelPlay(m_musicStream, FALSE);
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
	default: break;
	}
}

bool CPlayer::SongIsOver() const
{
	if (GetCurrentSongInfo().is_cue)
	{
		return m_current_position_int >= m_song_length_int;
	}
	else
	{
		bool song_is_over;
		static int last_pos;
		if ((m_playing == 2 && m_current_position_int == last_pos && m_current_position_int != 0	//������ڲ����ҵ�ǰ���ŵ�λ��û�з����仯�ҵ�ǰ����λ�ò�Ϊ0��
			&& m_current_position_int > m_song_length_int-1000)		//�Ҳ��Ž��ȵ������һ��
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
	QWORD lenght_bytes;
	lenght_bytes = BASS_ChannelGetLength(m_musicStream, BASS_POS_BYTE);
	double length_sec;
	length_sec = BASS_ChannelBytes2Seconds(m_musicStream, lenght_bytes);
	m_song_length_int = static_cast<int>(length_sec * 1000);
	if (m_song_length_int == -1000) m_song_length_int = 0;
	m_song_length.int2time(m_song_length_int);		//������ת����Time�ṹ
}

Time CPlayer::GetBASSSongLength(HSTREAM hStream)
{
	QWORD lenght_bytes;
	lenght_bytes = BASS_ChannelGetLength(hStream, BASS_POS_BYTE);
	double length_sec;
	length_sec = BASS_ChannelBytes2Seconds(hStream, lenght_bytes);
	int song_length_int = static_cast<int>(length_sec * 1000);
	if (song_length_int == -1000) song_length_int = 0;
	return Time(song_length_int);		//������ת����Time�ṹ
}

void CPlayer::GetBASSCurrentPosition()
{
	QWORD pos_bytes;
	pos_bytes = BASS_ChannelGetPosition(m_musicStream, BASS_POS_BYTE);
	double pos_sec;
	pos_sec = BASS_ChannelBytes2Seconds(m_musicStream, pos_bytes);
	m_current_position_int = static_cast<int>(pos_sec * 1000);
	if (m_current_position_int == -1000) m_current_position_int = 0;
	if (m_playlist[m_index].is_cue)
	{
		m_current_position_int -= m_playlist[m_index].start_pos.time2int();
	}
	m_current_position.int2time(m_current_position_int);
}


void CPlayer::SetVolume()
{
	float volume = static_cast<float>(m_volume) / 100.0f;
	volume = volume * theApp.m_nc_setting_data.volume_map / 100;
	BASS_ChannelSetAttribute(m_musicStream, BASS_ATTRIB_VOL, volume);
}


void CPlayer::GetBASSSpectral()
{
	if (m_musicStream && m_current_position_int < m_song_length_int - 500)	//ȷ����Ƶ�����Ϊ�գ����Ҹ������500���벻��ʾƵ�ף��Է�ֹ��������ĩβ�޷���ȡƵ�׵Ĵ���
	{
		const int ROW{ 32 };			//Ƶ�׷������ε�����
		BASS_ChannelGetData(m_musicStream, m_fft, BASS_DATA_FFT256);
		memset(m_spectral_data, 0, sizeof(m_spectral_data));
		for (int i{}; i < FFT_NUM; i++)
		{
			m_spectral_data[i / (FFT_NUM / ROW)] += m_fft[i];
		}

		for (int i{}; i < ROW; i++)
		{
			m_spectral_data[i] /= (FFT_NUM / ROW);
			m_spectral_data[i] *= 300;
		}
	}
	else
	{
		memset(m_spectral_data, 0, sizeof(m_spectral_data));
	}
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
			song_track = rand() % m_song_num;
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
			if (song_track >= m_song_num) song_track = 0;
			if (song_track < 0) song_track = m_song_num - 1;
		}
		if (song_track == PREVIOUS)		//������һ��
		{
			song_track = m_index - 1;
			if (song_track >= m_song_num) song_track = 0;
			if (song_track < 0) song_track = m_song_num - 1;
		}
		break;
	case RM_LOOP_TRACK:		//����ѭ��
		if (song_track == NEXT || song_track == PREVIOUS)
			song_track = m_index;
	}

	if (song_track >= 0 && song_track < m_song_num)
	{
		MusicControl(Command::CLOSE);
		m_index = song_track;
		m_current_file_name = m_playlist[m_index].file_name;
		MusicControl(Command::OPEN);
		IniLyrics();
		if (m_playlist[m_index].is_cue)
			SeekTo(0);
		MusicControl(Command::PLAY);
		GetBASSCurrentPosition();
		SetTitle();
		SaveConfig();
		return true;
	}
	else
	{
		MusicControl(Command::CLOSE);
		m_index = 0;
		m_current_file_name = m_playlist[m_index].file_name;
		MusicControl(Command::OPEN);
		IniLyrics();
		GetBASSCurrentPosition();
		SetTitle();
		SaveConfig();
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
	EmplaceCurrentPathToRecent();
	m_sort_mode = sort_mode;
	ChangePath(path, track);
	m_current_position_int = position;
	m_current_position.int2time(m_current_position_int);
	//MusicControl(Command::SEEK);
	EmplaceCurrentPathToRecent();		//�����µ�·�������·��
	
}

void CPlayer::OpenFolder(wstring path)
{
	if (m_loading) return;
	if (path.empty() || (path.back() != L'/' && path.back() != L'\\'))		//����򿪵���·��Ϊ�ջ�ĩβû��б�ܣ�����ĩβ����һ��
		path.append(1, L'\\');
	bool path_exist{ false };
	int track;
	int position;
	if (m_song_num>0) EmplaceCurrentPathToRecent();		//�����ǰ·���и������ͱ��浱ǰ·�������·��
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
	if (files.empty()) return;
	if (m_loading) return;
	MusicControl(Command::CLOSE);
	if (m_song_num>0) EmplaceCurrentPathToRecent();		//�ȱ��浱ǰ·���Ͳ��Ž��ȵ����·��
	size_t index;
	wstring path;
	index = files[0].find_last_of(L'\\');
	path = files[0].substr(0, index + 1);		//��ȡ·��
	if (path != m_path)		//����򿪵��ļ����µ�·���У�����������б�������ԭ���б������
	{
		m_path = path;
		m_playlist.clear();
		m_current_position_int = 0;
		m_current_position = { 0,0,0 };
		m_index = 0;
	}
	//EmplaceCurrentPathToRecent();
	SongInfo song_info;
	for (const auto& file : files)
	{
		index = file.find_last_of(L'\\');
		song_info.file_name = file.substr(index + 1);
		m_playlist.push_back(song_info);	//���ļ������浽�����б�
	}
	IniPlayList(true);
	MusicControl(Command::OPEN);
	MusicControl(Command::SEEK);
	if (play)
		//MusicControl(Command::PLAY);
		PlayTrack(m_song_num - files.size());	//���ļ��󲥷���ӵĵ�1����Ŀ
	IniLyrics();
	SetTitle();		//�õ�ǰ���ڲ��ŵĸ�������Ϊ���ڱ���
	//SetVolume();
}

void CPlayer::OpenAFile(wstring file)
{
	if (file.empty()) return;
	if (m_loading) return;
	MusicControl(Command::CLOSE);
	if (m_song_num>0) EmplaceCurrentPathToRecent();		//�ȱ��浱ǰ·���Ͳ��Ž��ȵ����·��
	size_t index;
	wstring path;
	index = file.rfind(L'\\');
	path = file.substr(0, index + 1);		//��ȡ·��
	m_path = path;
	m_playlist.clear();
	m_current_position_int = 0;
	m_current_position = { 0,0,0 };
	m_index = 0;
	m_current_file_name = file.substr(index + 1);
	m_song_num = 1;

	//��ȡ��·��������ʽ
	m_sort_mode = SortMode::SM_FILE;
	for (const auto& path_info : m_recent_path)
	{
		if (m_path == path_info.path)
			m_sort_mode = path_info.sort_mode;
	}

	//��ʼ�������б�
	m_current_file_name_tmp = m_current_file_name;
	IniPlayList(false, false);		//������·�����³�ʼ�������б�
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
		wchar_t buff[32];
		swprintf_s(buff, L"BASS��Ƶ�ⷢ���˴��󣬴�����룺%d", error_code_tmp);
		CCommon::WriteLog((CCommon::GetExePath() + L"error.log").c_str(), wstring{ buff });
	}
	m_error_code = error_code_tmp;
	return true;
}

bool CPlayer::IsError() const
{
	if (m_loading)		//��������б����ڼ��أ��򲻼�����
		return false;
	else
		return (m_error_code != 0 || m_musicStream == 0);
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
	//WritePrivateProfileStringW(L"config", L"path", m_path.c_str(), theApp.m_config_path.c_str());
	//CCommon::WritePrivateProfileIntW(L"config", L"track", m_index, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"config", L"volume", m_volume, theApp.m_config_path.c_str());
	//CCommon::WritePrivateProfileIntW(L"config", L"position", m_current_position_int, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"config", L"repeat_mode", static_cast<int>(m_repeat_mode), theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"config", L"lyric_karaoke_disp", theApp.m_play_setting_data.lyric_karaoke_disp, theApp.m_config_path.c_str());
	WritePrivateProfileStringW(L"config",L"lyric_path", theApp.m_play_setting_data.lyric_path.c_str(), theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"config", L"sort_mode", static_cast<int>(m_sort_mode), theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"config", L"lyric_fuzzy_match", theApp.m_play_setting_data.lyric_fuzzy_match, theApp.m_config_path.c_str());
	WritePrivateProfileStringW(L"config",L"default_album_file_name", CCommon::StringMerge(theApp.m_app_setting_data.default_album_name, L',').c_str(), theApp.m_config_path.c_str());

	//����������趨
	CCommon::WritePrivateProfileIntW(L"equalizer", L"equalizer_enable", m_equ_enable, theApp.m_config_path.c_str());
	//����ÿ��������ͨ��������
	//if (m_equ_style == 9)
	//{
	//	wchar_t buff[16];
	//	for (int i{}; i < EQU_CH_NUM; i++)
	//	{
	//		swprintf_s(buff, L"channel%d", i + 1);
	//		CCommon::WritePrivateProfileIntW(L"equalizer", buff, m_equalizer_gain[i], theApp.m_config_path.c_str());
	//	}
	//}
	//��������趨
	CCommon::WritePrivateProfileIntW(L"reverb", L"reverb_enable", m_reverb_enable, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"reverb", L"reverb_mix", m_reverb_mix, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"reverb", L"reverb_time", m_reverb_time, theApp.m_config_path.c_str());
}

void CPlayer::LoadConfig()
{
	wchar_t buff[256];
	//GetPrivateProfileStringW(L"config", L"path", L".\\songs\\", buff, 255, theApp.m_config_path.c_str());
	//m_path = buff;
	if (!m_path.empty() && m_path.back() != L'/' && m_path.back() != L'\\')		//�����ȡ������·��ĩβû��б�ܣ�����ĩβ����һ��
		m_path.append(1, L'\\');
	//m_index = GetPrivateProfileIntW(L"config", L"track", 0, theApp.m_config_path.c_str());
	m_volume = GetPrivateProfileIntW(L"config", L"volume", 60, theApp.m_config_path.c_str());
	//m_current_position_int = GetPrivateProfileIntW(L"config", L"position", 0, theApp.m_config_path.c_str());
	//m_current_position.int2time(m_current_position_int);
	m_repeat_mode = static_cast<RepeatMode>(GetPrivateProfileIntW(L"config", L"repeat_mode", 0, theApp.m_config_path.c_str()));
	GetPrivateProfileStringW(L"config", L"lyric_path", L".\\lyrics\\", buff, 255, theApp.m_config_path.c_str());
	theApp.m_play_setting_data.lyric_path = buff;
	if (!theApp.m_play_setting_data.lyric_path.empty() && theApp.m_play_setting_data.lyric_path.back() != L'/' && theApp.m_play_setting_data.lyric_path.back() != L'\\')
		theApp.m_play_setting_data.lyric_path.append(1, L'\\');
	theApp.m_play_setting_data.lyric_karaoke_disp = (GetPrivateProfileIntW(L"config", L"lyric_karaoke_disp", 1, theApp.m_config_path.c_str()) != 0);
	m_sort_mode = static_cast<SortMode>(GetPrivateProfileIntW(L"config", L"sort_mode", 0, theApp.m_config_path.c_str()));
	theApp.m_play_setting_data.lyric_fuzzy_match = (GetPrivateProfileIntW(L"config", L"lyric_fuzzy_match", 1, theApp.m_config_path.c_str()) != 0);
	GetPrivateProfileStringW(L"config", L"default_album_file_name", L"Folder", buff, 255, theApp.m_config_path.c_str());
	//theApp.m_app_setting_data.default_album_name = buff;
	CCommon::StringSplit(buff, L',', theApp.m_app_setting_data.default_album_name);

	//��ȡ�������趨
	m_equ_enable = (GetPrivateProfileIntW(L"equalizer", L"equalizer_enable", 0, theApp.m_config_path.c_str()) != 0);
	m_equ_style = GetPrivateProfileIntW(L"equalizer", L"equalizer_style", 0, theApp.m_config_path.c_str());	//��ȡ������Ԥ��
	if (m_equ_style == 9)		//���������Ԥ��Ϊ���Զ��塱
	{
		//��ȡÿ��������ͨ��������
		for (int i{}; i < EQU_CH_NUM; i++)
		{
			swprintf_s(buff, L"channel%d", i + 1);
			m_equalizer_gain[i] = GetPrivateProfileIntW(L"equalizer", buff, 0, theApp.m_config_path.c_str());
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
	m_reverb_enable = (GetPrivateProfileIntW(L"reverb", L"reverb_enable", 0, theApp.m_config_path.c_str()) != 0);
	m_reverb_mix = GetPrivateProfileIntW(L"reverb", L"reverb_mix", 45, theApp.m_config_path.c_str());		//����ǿ��Ĭ��Ϊ50
	m_reverb_time = GetPrivateProfileIntW(L"reverb", L"reverb_time", 100, theApp.m_config_path.c_str());	//����ʱ��Ĭ��Ϊ1s
}

void CPlayer::ExplorePath(int track) const
{
	if (m_song_num > 0)
	{
		CString str;
		if (track < 0)		//trackС��0������Դ��������ѡ�е�ǰ���ŵ��ļ�
			str.Format(_T("/select,\"%s%s\""), m_path.c_str(), m_current_file_name.c_str());
		else if (track < m_song_num)		//trackΪ�����б��е�һ����ţ�����Դ��������ѡ��ָ�����ļ�
			str.Format(_T("/select,\"%s%s\""), m_path.c_str(), m_playlist[track].file_name.c_str());
		else								//track���������б����ļ�������������Դ��������ѡ���κ��ļ�
			str = m_path.c_str();
		ShellExecute(NULL, _T("open"),_T("explorer"), str, NULL, SW_SHOWNORMAL);
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
	if (track >= 0 && track < m_playlist.size())
		return m_playlist[track].lengh;
	else
		return Time();
}

void CPlayer::ReloadPlaylist()
{
	if (m_loading) return;
	MusicControl(Command::CLOSE);
	m_playlist.clear();		//��ղ����б�
	//wstring current_file_name = m_current_file_name;	//���浱ǰ���ŵ���Ŀ���ļ���
	m_current_file_name_tmp = m_current_file_name;	//���浱ǰ���ŵ���Ŀ���ļ����������ڲ����б��ʼ������ʱȷ�����ŵĻ���֮ǰ���ŵ���Ŀ
	//��ʼ�������б�
	IniPlayList(false, true);		//������·�����³�ʼ�������б�


	////�������벥���б�󣬲������ڲ�����Ŀ�����
	//for (int i{}; i < m_playlist.size(); i++)
	//{
	//	if (current_file_name == m_playlist[i].file_name)
	//	{
	//		m_index = i;
	//		m_current_file_name = current_file_name;
	//		break;
	//	}
	//}

	//SetTitle();
	//MusicControl(Command::OPEN);
	//MusicControl(Command::SEEK);
	//SaveConfig();

}

void CPlayer::RemoveSong(int index)
{
	if (m_loading) return;
	if (index >= 0 && index < m_song_num)
	{
		m_playlist.erase(m_playlist.begin() + index);
		m_song_num--;
		if (index == m_index)		//���Ҫɾ������Ŀ�����ڲ��ŵ���Ŀ�����²��ŵ�ǰ��Ŀ
		{
			PlayTrack(m_index);
		}
		else if (index < m_index)	//���Ҫɾ������Ŀ�����ڲ��ŵ���Ŀ֮ǰ�������ڲ��ŵ���Ŀ��ż�1
		{
			m_index--;
		}
	}
}

void CPlayer::ClearPlaylist()
{
	if (m_loading) return;
	m_playlist.clear();
	m_song_num = 0;
	MusicControl(Command::STOP);
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
	double pos_sec = static_cast<double>(position) / 1000.0;
	QWORD pos_bytes;
	pos_bytes = BASS_ChannelSeconds2Bytes(m_musicStream, pos_sec);
	BASS_ChannelSetPosition(m_musicStream, pos_bytes, BASS_POS_BYTE);
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
		return L"���ų���";
	switch (m_playing)
	{
	case 0: return L"��ֹͣ";
	case 1: return L"����ͣ";
	case 2: return L"���ڲ���";
	}
	return wstring();
}

const SongInfo & CPlayer::GetCurrentSongInfo() const
{
	// TODO: �ڴ˴����� return ���
	if (m_index >= 0 && m_index < m_playlist.size())
		return m_playlist[m_index];
	else return m_no_use;
}

void CPlayer::ReIniBASS()
{
	BASS_Stop();	//ֹͣ���
	BASS_Free();	//�ͷ�Bass��Դ
	IniBASS();
	MusicControl(Command::OPEN);
	MusicControl(Command::SEEK);
	m_playing = 0;
}

void CPlayer::SortPlaylist(bool change_index)
{
	if (m_loading) return;
	int track_number = m_playlist[m_index].track;
	switch (m_sort_mode)
	{
	case SM_FILE: std::sort(m_playlist.begin(), m_playlist.end(), SongInfo::ByFileName);
		break;
	case SM_TITLE: std::sort(m_playlist.begin(), m_playlist.end(), SongInfo::ByTitle);
		break;
	case SM_ARTIST: std::sort(m_playlist.begin(), m_playlist.end(), SongInfo::ByArtist);
		break;
	case SM_ALBUM: std::sort(m_playlist.begin(), m_playlist.end(), SongInfo::ByAlbum);
		break;
	case SM_TRACK: std::sort(m_playlist.begin(), m_playlist.end(), SongInfo::ByTrack);
		break;
	default:
		break;
	}

	if (change_index)
	{
		//�����б�����󣬲������ڲ�����Ŀ�����
		if (!m_playlist[m_index].is_cue)
		{
			for (int i{}; i < m_playlist.size(); i++)
			{
				if (m_current_file_name == m_playlist[i].file_name)
				{
					m_index = i;
					break;
				}
			}
		}
		else
		{
			for (int i{}; i < m_playlist.size(); i++)
			{
				if (track_number == m_playlist[i].track)
				{
					m_index = i;
					break;
				}
			}
		}
	}
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
	if (!m_recent_path.empty() && m_song_num>0 && !m_playlist[0].file_name.empty())
	{
		m_recent_path[0].track = m_index;
		m_recent_path[0].position = m_current_position_int;
	}
	SaveRecentPath();
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
		info.Format(_T("��ȡ�������·���ļ��ļ�ʱ���������л��쳣���쳣���ͣ�CArchiveException��m_cause = %d��������recent_path.dat�ļ����𻵻�汾��ƥ����ɵġ�"), exception->m_cause);
		CCommon::WriteLog((CCommon::GetExePath() + L"error.log").c_str(), wstring{ info });
	}
	// �رն���
	ar.Close();
	// �ر��ļ�
	file.Close();

	//��recent_path�ļ��л�ȡ·�������ŵ�����Ŀ��λ��
	if (!m_recent_path.empty())
	{
		m_path = m_recent_path[0].path;
		m_index = m_recent_path[0].track;
		m_current_position_int = m_recent_path[0].position;
		m_current_position.int2time(m_current_position_int);
	}
	else
	{
		m_path = L".\\songs\\";		//Ĭ�ϵ�·��
	}
}

void CPlayer::EmplaceCurrentPathToRecent()
{
	for (int i{ 0 }; i < m_recent_path.size(); i++)
	{
		if (m_path == m_recent_path[i].path)
			m_recent_path.erase(m_recent_path.begin() + i);		//�����ǰ·���Ѿ������·���У��Ͱ������·����ɾ��
	}
	if (m_song_num == 0 || m_playlist[0].file_name.empty()) return;		//�����ǰ·����û���ļ����Ͳ�����
	PathInfo path_info;
	path_info.path = m_path;
	path_info.track = m_index;
	path_info.position = m_current_position_int;
	path_info.sort_mode = m_sort_mode;
	path_info.track_num = m_song_num;
	path_info.total_time = m_total_time;
	if (m_song_num > 0)
		m_recent_path.push_front(path_info);		//��ǰ·�����뵽m_recent_path��ǰ��
}


void CPlayer::SetFXHandle()
{
	if (m_musicStream == 0) return;
	//if (!m_equ_enable) return;
	//����ÿ��������ͨ���ľ��
	for (int i{}; i < EQU_CH_NUM; i++)
	{
		m_equ_handle[i] = BASS_ChannelSetFX(m_musicStream, BASS_FX_DX8_PARAMEQ, 1);
	}
	//���û���ľ��
	m_reverb_handle = BASS_ChannelSetFX(m_musicStream, BASS_FX_DX8_REVERB, 1);
}

void CPlayer::RemoveFXHandle()
{
	if (m_musicStream == 0) return;
	//�Ƴ�ÿ��������ͨ���ľ��
	for (int i{}; i < EQU_CH_NUM; i++)
	{
		if (m_equ_handle[i] != 0)
		{
			BASS_ChannelRemoveFX(m_musicStream, m_equ_handle[i]);
			m_equ_handle[i] = 0;
		}
	}
	//�Ƴ�����ľ��
	if (m_reverb_handle != 0)
	{
		BASS_ChannelRemoveFX(m_musicStream, m_reverb_handle);
		m_reverb_handle = 0;
	}
}

void CPlayer::ApplyEqualizer(int channel, int gain)
{
	if (channel < 0 || channel >= EQU_CH_NUM) return;
	//if (!m_equ_enable) return;
	if (gain < -15) gain = -15;
	if (gain > 15) gain = 15;
	BASS_DX8_PARAMEQ parameq;
	parameq.fBandwidth = 30;
	parameq.fCenter = FREQ_TABLE[channel];
	parameq.fGain = static_cast<float>(gain);
	BASS_FXSetParameters(m_equ_handle[channel], &parameq);
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

void CPlayer::SetReverb(int mix, int time)
{
	if (mix < 0) mix = 0;
	if (mix > 100) mix = 100;
	if (time < 1) time = 1;
	if (time > 300) time = 300;
	m_reverb_mix = mix;
	m_reverb_time = time;
	BASS_DX8_REVERB parareverb;
	parareverb.fInGain = 0;
	//parareverb.fReverbMix = static_cast<float>(mix) / 100 * 96 - 96;
	parareverb.fReverbMix = static_cast<float>(std::pow(static_cast<double>(mix) / 100, 0.1) * 96 - 96);
	parareverb.fReverbTime = static_cast<float>(time * 10);
	parareverb.fHighFreqRTRatio = 0.001f;
	BASS_FXSetParameters(m_reverb_handle, &parareverb);
}

void CPlayer::ClearReverb()
{
	BASS_DX8_REVERB parareverb;
	parareverb.fInGain = 0;
	parareverb.fReverbMix = -96;
	parareverb.fReverbTime = 0.001f;
	parareverb.fHighFreqRTRatio = 0.001f;
	BASS_FXSetParameters(m_reverb_handle, &parareverb);
}

void CPlayer::EnableReverb(bool enable)
{
	if (enable)
		SetReverb(m_reverb_mix, m_reverb_time);
	else
		ClearReverb();
	m_reverb_enable = enable;
}


void CPlayer::ConnotPlayWarning() const
{
	if (m_no_ape_plugin && CAudioCommon::GetAudioType(m_current_file_name) == AudioType::AU_APE)
		//AfxMessageBox(_T("�޷����� ape �ļ�����Ϊ�޷����� ape ���Ų������ȷ�ϳ�������Ŀ¼�Ƿ������bass_ape.dll���ļ���Ȼ������������������"), MB_ICONWARNING | MB_OK);
		PostMessage(theApp.m_pMainWnd->GetSafeHwnd(), WM_CONNOT_PLAY_WARNING, 0, 0);
}

void CPlayer::SearchAlbumCover()
{
	static wstring last_file_path;
	if (last_file_path != m_path + m_current_file_name)		//��ֹͬһ���ļ���λ�ȡר������
	{
		//AudioType type = CAudioCommon::GetAudioType(m_current_file_name);
		//if (type != AU_FLAC)
		//	m_album_cover_path = CAudioCommon::GetAlbumCover(m_musicStream, m_album_cover_type);		//��ȡר�����沢���浽��ʱĿ¼
		//else
		//	m_album_cover_path = CAudioCommon::GetFlacAlbumCover(m_path + m_current_file_name, m_album_cover_type);
		CAudioTag audio_tag(m_musicStream, m_path, m_playlist[m_index]);
		m_album_cover_path = audio_tag.GetAlbumCover(m_album_cover_type);
		m_album_cover.Destroy();
		m_album_cover.Load(m_album_cover_path.c_str());
		m_inner_cover = !m_album_cover.IsNull();
		if (/*theApp.m_app_setting_data.use_out_image && */m_album_cover.IsNull())
		{
			//��ȡ����ר������ʱ����ʹ���ⲿͼƬ��Ϊ����
			SearchOutAlbumCover();
		}
	}
	last_file_path = m_path + m_current_file_name;
}

void CPlayer::SearchOutAlbumCover()
{
	vector<wstring> files;
	wstring file_name;
	//�����ļ��͸�����һ�µ�ͼƬ�ļ�
	CFilePathHelper c_file_name(m_current_file_name);
	file_name = m_path + c_file_name.GetFileNameWithoutExtension() + L".*";
	CCommon::GetImageFiles(file_name, files);
	if (files.empty() && !GetCurrentSongInfo().album.empty())
	{
		//û���ҵ��͸�����һ�µ�ͼƬ�ļ���������ļ���Ϊ����Ƭ�������ļ�
		wstring album_name{ GetCurrentSongInfo().album };
		CCommon::FileNameNormalize(album_name);
		file_name = m_path + album_name + L".*";
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
				file_name = m_path + album_name + L".*";
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
		m_album_cover_path = m_path + files[0];
		if (!m_album_cover.IsNull())
			m_album_cover.Destroy();
		m_album_cover.Load(m_album_cover_path.c_str());
	}
}
