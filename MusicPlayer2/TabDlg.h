//��������Tab��ǩ�е��ӶԻ�����
#pragma once
class CTabDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTabDlg)
public:
	CTabDlg(UINT nIDTemplate, CWnd *pParent = NULL);
	~CTabDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
    CWnd* GetParentWindow();
protected:
	CWnd* m_pParent;
};

