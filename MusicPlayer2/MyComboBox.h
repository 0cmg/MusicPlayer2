﻿#pragma once
class CMyComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CMyComboBox)

public:
	CMyComboBox();
	virtual ~CMyComboBox();

	void SetReadOnly(bool read_only = true);
	void SetEditReadOnly(bool read_only = true);
    CEdit* GetEditCtrl();
    void ResetModified();

protected:
	bool m_read_only{ false };
	CRect m_arrow_rect;		//下拉箭头的矩形区域
    bool m_modified{};

public:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg BOOL OnCbnSelchange();
};

