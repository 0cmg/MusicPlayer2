#pragma once
#include "SongInfo.h"
#include <functional>
class CSongDataManager
{
public:
    ~CSongDataManager();

    using SongDataMap = std::unordered_map<wstring, SongInfo>;

    static CSongDataManager& GetInstance();
    void SaveSongData(std::wstring path);		//�����и�����Ϣ�����л��ķ�ʽ���浽�ļ�
    void LoadSongData(std::wstring path);			//���ļ��������л��ķ�ʽ��ȡ���и�����Ϣ

    void SetSongDataModified();
    bool IsSongDataModified() const;

    void SaveSongInfo(const SongInfo& song_info);       //��һ��������Ϣ���浽m_song_data��

    SongInfo GetSongInfo(const wstring& file_path);
    SongInfo& GetSongInfoRef(const wstring& file_path);     //��ȡһ��������Ϣ�����ã���������ڲ�������µļ�¼��
    SongInfo& GetSongInfoRef2(const wstring& file_path);     //��ȡһ��������Ϣ�����ã���������ڻ�����µļ�¼��

    const SongDataMap& GetSongData();
    bool IsItemExist(const wstring& file_path) const;
    void AddItem(const wstring& file_path, SongInfo song);
    bool RemoveItem(const wstring& file_path);
    int RemoveItemIf(std::function<bool(const SongInfo&)> fun_condition);       //ɾ��������������Ŀ��������ɾ������

    void ClearPlayTime();       //�������ʱ��ͳ������
    void ClearLastPlayedTime();     //����ϴβ���ʱ��

    void UpdateFileModifiedTime(const wstring& file_path, bool update = false);      //���¸���������޸�ʱ�䣬���updateΪfalse��������ѻ�ȡ���ʱ��Ͳ����£����updateΪtrue�������Ǹ���
    void ChangeFilePath(const wstring& file_path, const wstring& new_path);

private:
    CSongDataManager();

    static CSongDataManager m_instance;

private: 
    SongDataMap m_song_data;		//�������и�����Ϣ���ݵ�ӳ������������ÿһ����Ƶ�ļ��ľ���·����������ÿһ����Ƶ�ļ�����Ϣ
    bool m_song_data_modified{};
};

