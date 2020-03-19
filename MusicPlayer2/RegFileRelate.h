#pragma once
class CRegFileRelate
{
public:
    CRegFileRelate();
    ~CRegFileRelate();

    void SetModelPath(LPCTSTR model_path);
    void SetAppName(LPCTSTR app_name);

    //���һ���ļ����͵Ĺ���
    //file_ext: Ҫ�������ļ���չ��
    bool AddFileTypeRelate(LPCTSTR file_ext, LPCTSTR ico_path = nullptr, bool default_icon = false, LPCTSTR description = NULL);
    bool AddFileTypeRelate(LPCTSTR file_ext, int ico_index, bool default_icon = false, LPCTSTR description = NULL);
    //�ж�һ���ļ������Ƿ��ѹ���
    bool IsFileTypeRelated(LPCTSTR file_ext);
    //ɾ��һ���ļ����͵Ĺ���
    bool DeleteFileTypeRelate(LPCTSTR file_ext);

    //��ȡ�����ѹ�����������չ��
    bool GetAllRelatedExtensions(std::vector<wstring>& extensions);

private:
    bool OpenItem(CRegKey& key, LPCTSTR item_str);

private:
    CString m_model_path;
    CString m_app_name = APP_NAME;
};

