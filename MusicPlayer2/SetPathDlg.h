#pragma once
#include "afxwin.h"
#include "Common.h"
#include "AudioCommon.h"
#include "afxcmn.h"
//#include "ListCtrlEx.h"

// CSetPathDlg �Ի���

class CSetPathDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetPathDlg)

public:
	CSetPathDlg(deque<PathInfo>& recent_path, wstring current_path, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetPathDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SET_PATH_DIALOG };
#endif

protected:
	deque<PathInfo>& m_recent_path;		//����򿪹���·��
	int m_path_selected{};		//ѡ���·��
	wstring m_current_path;

	CEdit m_path_name;
	CListCtrl m_path_list;

	CSize m_min_size;		//���ڵ���С��С

	void ShowPathList();
	void SetButtonsEnable(bool enable);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	wstring GetSelPath() const;		//��ȡѡ���·��
	int GetTrack() const;
	int GetPosition() const;
	SortMode GetSortMode() const;
	bool SelectValid() const;		//�ж�ѡ���Ƿ���Ч

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedDeletePathButton();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMClickPathList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickPathList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkPathList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
