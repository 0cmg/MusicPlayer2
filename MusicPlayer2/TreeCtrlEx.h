#pragma once
#include <functional>


// CTreeCtrlEx

class CTreeCtrlEx : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTreeCtrlEx)

public:
	CTreeCtrlEx();
	virtual ~CTreeCtrlEx();

public:
    void InsertPath(CString path, HTREEITEM hRoot);

    //�����в���һ���ļ��нṹ
    //path: �ļ��еĸ�Ŀ¼
    //hRoot: Ҫ����ĸ�Ŀ¼�����ؼ��е�λ��
    //is_path_show: һ���������������ж�һ���ļ����Ƿ���Ҫ��ʾ
    void InsertPath(CString path, HTREEITEM hRoot, std::function<bool(const CString&)> is_path_show);
    CString GetItemPath(HTREEITEM hItem);

private:
    void _InsertPath(CString path, HTREEITEM hRoot, std::function<bool(const CString&)> is_path_show = [](const CString&) {return true; });

protected:
	DECLARE_MESSAGE_MAP()
};


