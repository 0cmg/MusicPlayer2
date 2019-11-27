#pragma once
#include <vector>
#include "Time.h"
#include "SongInfo.h"
#include "Common.h"

class CCueFile
{
public:
    CCueFile(const std::wstring& file_path);
    ~CCueFile();
    void SetTotalLength(Time length);       //����cue��Ӧ��Ƶ�ļ����ܳ��ȣ���Ҫ�ڽ�����ɺ����GetAudioFileName��ȡ����������Ƶ�ļ�·�����ٻ�ȡ����Ƶ�ļ��ĳ��ȣ�
    const std::vector<SongInfo>& GetAnalysisResult() const;
    std::wstring GetAudioFileName() const;

private:
    void DoAnalysis();
    Time PhaseIndex(size_t pos);
    string GetCommand(const string& str, size_t pos = 0);

private:
    std::wstring m_file_path;
    std::string m_file_content;
    CodeType m_code_type{ CodeType::AUTO };
    std::wstring m_audio_file_name;
    std::vector<SongInfo> m_result;
};

