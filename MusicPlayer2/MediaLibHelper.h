#pragma once
#include <set>
#include "SongInfo.h"

#define STR_OTHER_CLASSIFY_TYPE L"eRk0Q6ov"

//不区分大小写的字符串比较器
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

    typedef std::map<std::wstring, std::vector<SongInfo>, StringComparerNoCase> MediaList;      //定义保存分类结果的map容器，使用不区分大小写的比较器，以实现分类时不区分大小写

public:
    CMediaClassifier(ClassificationType type, bool hide_only_one_classification = false);
    ~CMediaClassifier();

    const MediaList& GetMeidaList() const;
    MediaList& GetMeidaList();
    void ClassifyMedia();
    static bool IsStringYear(std::wstring str);

private:

private:
    MediaList m_media_list;
    ClassificationType m_type;
    bool m_hide_only_one_classification;       //如果为true，则把只有一项的类别归到“其他”里面
};

