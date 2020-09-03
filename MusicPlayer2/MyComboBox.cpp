﻿#include "stdafx.h"
#include "MyComboBox.h"
#include "MusicPlayer2.h"

IMPLEMENT_DYNAMIC(CMyComboBox, CComboBox)

CMyComboBox::CMyComboBox()
{
}


CMyComboBox::~CMyComboBox()
{
}

void CMyComboBox::SetReadOnly(bool read_only)
{
    CEdit* pEdit = GetEditCtrl();
    if (pEdit != nullptr)
        pEdit->SetReadOnly(read_only);		//将Endit控件设为只读
    m_read_only = read_only;
}

void CMyComboBox::SetEditReadOnly(bool read_only)
{
    CEdit* pEdit = GetEditCtrl();
    if (pEdit != nullptr)
        pEdit->SetReadOnly(read_only);		//将Endit控件设为只读
}

CEdit* CMyComboBox::GetEditCtrl()
{
    return (CEdit*)GetWindow(GW_CHILD);
}

void CMyComboBox::ResetModified()
{
    m_modified = false;

    CEdit* pEdit = GetEditCtrl();
    if (pEdit != nullptr)
        pEdit->SetModify(FALSE);

    Invalidate(FALSE);
}

BEGIN_MESSAGE_MAP(CMyComboBox, CComboBox)
    ON_WM_CTLCOLOR()
    ON_CONTROL_REFLECT_EX(CBN_SELCHANGE, &CMyComboBox::OnCbnSelchange)
END_MESSAGE_MAP()



BOOL CMyComboBox::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_read_only)
	{
		if (pMsg->message == WM_MOUSEWHEEL)		//如果只读，则不响应鼠标滚轮消息
			return TRUE;
		if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONDBLCLK || pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_LBUTTONUP)
		{
			CPoint point1 = pMsg->pt;
			CPoint point;
			GetCursorPos(&point);
			//获取箭头区域的位置
			GetWindowRect(m_arrow_rect);
			m_arrow_rect.left = m_arrow_rect.right - theApp.DPI(18);
			if(m_arrow_rect.PtInRect(point))		//如果鼠标指针的位置位于箭头区域，则不响应以上的鼠标消息
				return TRUE;
		}
	}

	return CComboBox::PreTranslateMessage(pMsg);
}


HBRUSH CMyComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

    // TODO:  在此更改 DC 的任何特性
    CEdit* pEdit = GetEditCtrl();
    if (pEdit != nullptr)
    {
        DWORD style = pEdit->GetStyle();
        bool is_read_only = ((style & ES_READONLY) != 0);
        if ((pEdit->GetModify() || m_modified) && !is_read_only)
            pDC->SetTextColor(theApp.m_app_setting_data.theme_color.dark1);        //如果文本已修改，则显示为主题色
    }

    // TODO:  如果默认的不是所需画笔，则返回另一个画笔
    return hbr;
}


BOOL CMyComboBox::OnCbnSelchange()
{
    // TODO: 在此添加控件通知处理程序代码
    m_modified = true;
    return FALSE;
}
