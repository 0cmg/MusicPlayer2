#include "stdafx.h"
#include "AudioCommon.h"


CAudioCommon::CAudioCommon()
{
}


CAudioCommon::~CAudioCommon()
{
}

bool CAudioCommon::FileIsAudio(const wstring & file_name)
{
	size_t index;
	index = file_name.find_last_of(L'.');
	wstring type;
	if (index != string::npos)
		type = file_name.substr(index);			//��ȡ��չ��
	std::transform(type.begin(), type.end(), type.begin(), tolower);		//����չ��ת����Сд
	return (type == L".mp3" || type == L".wma" || type == L".wav"|| type == L".ogg" || type == L".flac"
		|| type == L".m4a" || type == L".cue" || type == L".ape");
}

AudioType CAudioCommon::GetAudioType(const wstring & file_name)
{
	size_t index;
	index = file_name.find_last_of(L'.');
	wstring type;
	if (index != string::npos)
		type = file_name.substr(index);			//��ȡ��չ��
	std::transform(type.begin(), type.end(), type.begin(), tolower);		//����չ��ת����Сд
	if (type == L".mp3")
		return AU_MP3;
	else if (type == L".wma")
		return AU_WMA;
	else if (type == L".ogg")
		return AU_OGG;
	else if (type == L".m4a")
		return AU_MP4;
	else if (type == L".mp4")
		return AU_MP4;
	else if (type == L".flac")
		return AU_FLAC;
	else if (type == L".cue")
		return AU_CUE;
	else if (type == L".ape")
		return AU_APE;
	else
		return AU_OTHER;
}

void CAudioCommon::GetAudioFiles(wstring path, vector<SongInfo>& files, size_t max_file)
{
	//�ļ���� 
	int hFile = 0;
	//�ļ���Ϣ����Unicode����ʹ��_wfinddata_t�����ֽ��ַ���ʹ��_finddata_t��
	_wfinddata_t fileinfo;
	SongInfo song_info;
	if ((hFile = _wfindfirst(path.append(L"\\*.*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if (files.size() >= max_file) break;
			if (FileIsAudio(wstring(fileinfo.name)))	//����ҵ����ļ�����Ƶ�ļ����򱣴浽������
			{
				song_info.file_name = fileinfo.name;
				files.push_back(song_info);
			}
		} while (_wfindnext(hFile, &fileinfo) == 0);
	}
	_findclose(hFile);
}

void CAudioCommon::GetLyricFiles(wstring path, vector<wstring>& files)
{
	//�ļ���� 
	int hFile = 0;
	//�ļ���Ϣ����Unicode����ʹ��_wfinddata_t�����ֽ��ַ���ʹ��_finddata_t��
	_wfinddata_t fileinfo;
	//wstring file_path;
	if ((hFile = _wfindfirst(path.append(L"\\*.lrc").c_str(), &fileinfo)) != -1)
	{
		do
		{
			files.push_back(fileinfo.name);  //���ļ�������
		} while (_wfindnext(hFile, &fileinfo) == 0);
	}
	_findclose(hFile);
}

void CAudioCommon::GetCueTracks(vector<SongInfo>& files, wstring path)
{
	for (size_t i{}; i < files.size(); i++)
	{
		//���μ���б��е�ÿ�׸����Ƿ�Ϊcue�ļ�
		if (GetAudioType(files[i].file_name) == AU_CUE)
		{
			wstring cue_file_name{ files[i].file_name };		//cue�ļ����ļ���
			files.erase(files.begin() + i);		//���б���ɾ��cue�ļ�
			wstring cue_file_name2;			//cue�ļ����ļ�����������չ����
			size_t index = cue_file_name.rfind(L'.');
			cue_file_name2 = cue_file_name.substr(0, index);
			//���Һ�cue�ļ�ͬ������Ƶ�ļ�
			wstring play_file_name;		//���ҵ��ĺ�cue�ļ�ͬ�����ļ���
			wstring play_file_name2;		//���ҵ��ĺ�cue�ļ�ͬ�����ļ�����������չ����
			int bitrate;
			Time total_length;
			bool matched_file_found{ false };		//������ҵ��˺�cue�ļ���ͬ���ļ�������Ϊtrue
			for (size_t j{}; j < files.size(); j++)
			{
				if (GetAudioType(files[j].file_name) != AU_CUE && !files[j].is_cue)	//ȷ�����ļ�����cue�ļ����Ҳ����Ѿ���������cue����
				{
					play_file_name = files[j].file_name;		//�ű����ļ���
					bitrate = files[j].bitrate;			//�����ȡ���ı�����
					total_length = files[j].lengh;
					size_t index2 = play_file_name.rfind(L'.');
					play_file_name2 = play_file_name.substr(0, index2);
					if (play_file_name2 == cue_file_name2)
					{
						files.erase(files.begin() + j);		//���б���ɾ�����ļ�
						matched_file_found = true;
						break;
					}
				}
			}
			if (!matched_file_found)		//���û���ҵ���cueͬ�����ļ��������������һ��cue�ļ�
				continue;

			//����cue�ļ�
			string cue_file_contents;
			ifstream OpenFile{ path + cue_file_name };
			if (OpenFile.fail())
				return;
			string current_line;
			char ch;
			while (!OpenFile.eof())
			{
				//std::getline(OpenFile, current_line);
				//cue_file_contents += current_line;
				OpenFile.get(ch);
				cue_file_contents.push_back(ch);
				if (cue_file_contents.size() > 102400) break;	//����cue�ļ����Ϊ100KB
			}
			CodeType code_type{ CodeType::AUTO };		//cue�ļ��ı�������
			if (cue_file_contents.size() >= 3 && cue_file_contents[0] == -17 && cue_file_contents[1] == -69 && cue_file_contents[2] == -65)
				code_type = CodeType::UTF8;
			//��ȡcue�ļ���ר������
			string album_name;
			size_t index1 = cue_file_contents.find("TITLE");
			size_t index2 = cue_file_contents.find('\"', index1);
			size_t index3 = cue_file_contents.find('\"', index2 + 1);
			album_name = cue_file_contents.substr(index2 + 1, index3 - index2 - 1);

			SongInfo song_info{};
			song_info.album = CCommon::StrToUnicode(album_name, code_type);
			song_info.file_name = play_file_name;
			song_info.bitrate = bitrate;
			song_info.is_cue = true;
			song_info.info_acquired = true;

			size_t index_track{};
			size_t index_title{};
			size_t index_artist{};
			size_t index_pos{};
			while (true)
			{
				//������Ŀ���
				index_track = cue_file_contents.find("TRACK ", index_track + 6);
				if (index_track == string::npos)
					break;
				string track_str = cue_file_contents.substr(index_track + 6, 3);
				song_info.track = atoi(track_str.c_str());
				size_t next_track_index = cue_file_contents.find("TRACK ", index_track + 6);
				//������Ŀ����
				index_title = cue_file_contents.find("TITLE ", index_track + 6);
				if (index_title < next_track_index)
				{
					index2 = cue_file_contents.find('\"', index_title);
					index3 = cue_file_contents.find('\"', index2 + 1);
					song_info.title = CCommon::StrToUnicode(cue_file_contents.substr(index2 + 1, index3 - index2 - 1), code_type);
				}
				else
				{
					song_info.title = DEFAULT_TITLE;
				}
				//������Ŀ������
				index_artist = cue_file_contents.find("PERFORMER ", index_track + 6);
				if (index_artist < next_track_index)
				{
					index2 = cue_file_contents.find('\"', index_artist);
					index3 = cue_file_contents.find('\"', index2 + 1);
					song_info.artist = CCommon::StrToUnicode(cue_file_contents.substr(index2 + 1, index3 - index2 - 1), code_type);
				}
				else
				{
					song_info.artist = DEFAULT_ARTIST;
				}
				//������Ŀλ��
				index_pos = cue_file_contents.find("INDEX ", index_track + 6);
				index1 = cue_file_contents.find(":", index_pos + 6);
				index2 = cue_file_contents.rfind(" ", index1);
				string tmp;
				Time time;
				//��ȡ����
				tmp = cue_file_contents.substr(index2 + 1, index1 - index2 - 1);
				time.min = atoi(tmp.c_str());
				//��ȡ����
				tmp = cue_file_contents.substr(index1 + 1, 2);
				time.sec = atoi(tmp.c_str());
				//��ȡ����
				tmp = cue_file_contents.substr(index1 + 4, 2);
				time.msec = atoi(tmp.c_str()) * 10;

				song_info.start_pos = time;
				if (!time.isZero() && !files.empty())
				{
					files.back().end_pos = time;
					files.back().lengh = Time(time - files.back().start_pos);
				}

				files.push_back(song_info);
			}
			files.back().lengh = Time(total_length - files.back().start_pos);
			i--;		//������һ��cue�ļ������ڸ�cue�ļ��Ѿ����Ƴ������Խ�ѭ��������1
		}
	}
}

void CAudioCommon::CheckCueFiles(vector<SongInfo>& files, wstring path)
{
	bool audio_exist;
	int size = files.size();
	for (int i{}; i < size; i++)
	{
		if (GetAudioType(files[i].file_name) == AU_CUE)		//�����б��е�cue�ļ�
		{
			audio_exist = false;
			wstring file_name;
			size_t index;
			index = files[i].file_name.rfind(L'.');
			file_name = files[i].file_name.substr(0, index);		//��ȡ�ļ�����������չ����
			//���Һ�cue�ļ�ƥ�����Ƶ�ļ�
			for (int j{}; j < size; j++)
			{
				if (GetAudioType(files[j].file_name) != AU_CUE)
				{
					wstring audio_file_name;
					index = files[j].file_name.rfind(L'.');
					audio_file_name = files[j].file_name.substr(0, index);
					if (file_name == audio_file_name)
					{
						audio_exist = true;
						break;
					}
				}
			}
			//û���ҵ�ƥ�����Ƶ�ļ�������Ŀ¼������ƥ�����Ƶ�ļ�
			if (!audio_exist)
			{
				vector<wstring> audio_files;
				CString find_file_name;
				find_file_name.Format(_T("%s%s.*"), path.c_str(), file_name.c_str());
				CCommon::GetFiles(wstring(find_file_name), audio_files);
				for (const auto& file : audio_files)
				{
					if (GetAudioType(file) != AU_CUE)
					{
						SongInfo song_info;
						song_info.file_name = file;
						files.push_back(song_info);
						return;
					}
				}
			}
		}
	}
}


wstring CAudioCommon::GetGenre(BYTE genre)
{
	if (genre < GENRE_MAX)
		return GENRE_TABLE[genre];
	else
		return L"<δ֪����>";
}

wstring CAudioCommon::GenreConvert(wstring genre)
{
	if(genre.empty())
		return wstring();
	if (genre[0] == L'(')		//���ǰ�������ţ���ɾ������
		genre = genre.substr(1);
	if (genre.back() == L')')
		genre.pop_back();
	if (CCommon::StrIsNumber(genre))		//���������Ϣ�����֣���ת��Ϊ��׼������Ϣ
	{
		int n_genre = _wtoi(genre.c_str());
		if(n_genre<256)
			return GetGenre(static_cast<BYTE>(n_genre));
	}
	return genre;
}


void CAudioCommon::TagStrNormalize(wstring & str)
{
	for (size_t i{}; i < str.size(); i++)
	{
		if (str[i] < 32 || str[i] >= static_cast<wchar_t>(0xfff0))
		{
			str = str.substr(0, i);
			return;
		}
	}

}

