#pragma once
#include <set>
#include "SongInfo.h"

#define STR_OTHER_CLASSIFY_TYPE L"eRk0Q6ov"

//�����ִ�Сд���ַ����Ƚ���
class StringComparerNoCase
{
public:
    bool operator()(const std::wstring& a, const std::wstring& b) const
    {
        std::wstring _a{ a }, _b{ b };
        CCommon::StringTransform(_a, false);
        CCommon::StringTransform(_b, false);
        return _a < _b;
    }
};


class CMediaClassifier
{
public:
    enum ClassificationType
    {
        CT_ARTIST,
        CT_ALBUM,
        CT_GENRE,
        CT_YEAR
    };

    typedef std::map<std::wstring, std::vector<SongInfo>, StringComparerNoCase> MediaList;      //���屣���������map������ʹ�ò����ִ�Сд�ıȽ�������ʵ�ַ���ʱ�����ִ�Сд

public:
    CMediaClassifier(ClassificationType type, bool hide_only_one_classification = false);
    ~CMediaClassifier();

    const MediaList& GetMeidaList() const;
    MediaList& GetMeidaList();
    void ClassifyMedia();
    static bool IsStringYear(std::wstring str);
    void ClearResult();

private:

private:
    MediaList m_media_list;
    ClassificationType m_type;
    bool m_hide_only_one_classification;       //���Ϊtrue�����ֻ��һ������鵽������������
};

