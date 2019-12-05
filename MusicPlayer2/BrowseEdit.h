#pragma once
#include "ColorConvert.h"
#include <afxeditbrowsectrl.h>

#define WM_EDIT_BROWSE_CHANGED (WM_USER+126)        //ͨ�����������ťѡ����һ���ļ����ļ��к��ʹ���Ϣ
// CBrowseEdit

class CBrowseEdit : public CMFCEditBrowseCtrl
{
	DECLARE_DYNAMIC(CBrowseEdit)

public:
	CBrowseEdit();
	virtual ~CBrowseEdit();

    virtual void OnDrawBrowseButton(CDC* pDC, CRect rect, BOOL bIsButtonPressed, BOOL bIsButtonHot) override;
    virtual void OnChangeLayout() override;
    virtual void OnBrowse() override;
    virtual void OnAfterUpdate() override;

protected:
	DECLARE_MESSAGE_MAP()

private:
    ColorTable& m_theme_color;
    CString m_btn_str;
    CDC* m_pDC{};

    virtual void PreSubclassWindow();
public:
    afx_msg void OnDestroy();
};


