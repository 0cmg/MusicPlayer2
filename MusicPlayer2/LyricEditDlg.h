#pragma once
#include "afxwin.h"
#include "Time.h"
#include "EditEx.h"

// CLyricEditDlg �Ի���

class CLyricEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CLyricEditDlg)

public:
	//�������ʱ���ǩ��ö��
	enum class TagOpreation
	{
		INSERT,		//����
		REPLACE,	//�滻
		DELETE_		//ɾ��
	};

	bool m_dlg_exist{ false };		//����Ի�����ڣ���Ϊtrue

	CLyricEditDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLyricEditDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LYRIC_EDIT_DIALOG };
#endif

protected:
	CFont m_font;			//��ʱ༭��������
	wstring m_lyric_string;		//����ַ���
	wstring m_lyric_path;		//���·��
	wstring m_original_lyric_path;	//ԭ���ĸ�ʣ���ǰ���Ÿ�����Ӧ�ĸ�ʣ���·��
	CodeType m_code_type;		//��ʱ���
	wstring m_current_song_name;	//���ڲ��ŵĸ�����
	bool m_modified{ false };	//�������Ѹ��ģ���Ϊtrue
	bool m_lyric_saved{ false };	//���ִ�й������������Ϊtrue

	CToolTipCtrl m_Mytip;		//�����ʾ
	CStatusBarCtrl m_status_bar;	//״̬��

	void OpreateTag(TagOpreation operation);
	bool SaveLyric(const wchar_t* path, CodeType code_type);
	void UpdateStatusbarInfo();			//����״̬����Ϣ

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
protected:
	CEditEx m_lyric_edit;
public:
	afx_msg void OnBnClickedInsertTagButton();
	afx_msg void OnBnClickedReplaceTagButton();
	afx_msg void OnBnClickedDeleteTag();
	afx_msg void OnBnClickedSaveLyricButton();
	afx_msg void OnBnClickedSaveAsButton5();
	afx_msg void OnDestroy();
	afx_msg void OnEnChangeEdit1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnBnClickedOpenLyricButton();
};
