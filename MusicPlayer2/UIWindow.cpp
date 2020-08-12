#include "stdafx.h"
#include "UIWindow.h"
#include "MusicPlayer2.h"
#include "MusicPlayerDlg.h"


void CUIWindow::PreSubclassWindow()
{
    // TODO: �ڴ����ר�ô����/����û���
    DWORD dwStyle = GetStyle();
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

    CStatic::PreSubclassWindow();
}


BOOL CUIWindow::PreTranslateMessage(MSG* pMsg)
{
    // TODO: �ڴ����ר�ô����/����û���
    if (pMsg->message == WM_MOUSEMOVE)
    {
        m_pUI->GetToolTipCtrl().RelayEvent(pMsg);
    }

    return CStatic::PreTranslateMessage(pMsg);
}
BEGIN_MESSAGE_MAP(CUIWindow, CStatic)
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
//    ON_WM_RBUTTONDBLCLK()
    ON_WM_MOUSEMOVE()
    ON_WM_RBUTTONUP()
    ON_WM_PAINT()
END_MESSAGE_MAP()


void CUIWindow::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    CMusicPlayerDlg* pMainWindow = dynamic_cast<CMusicPlayerDlg*>(theApp.m_pMainWnd);
    if (pMainWindow != nullptr && !pMainWindow->m_no_lbtnup)
        m_pUI->LButtonUp(point);

    CStatic::OnLButtonUp(nFlags, point);
}


void CUIWindow::OnLButtonDown(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    m_pUI->LButtonDown(point);

    CStatic::OnLButtonDown(nFlags, point);
}


void CUIWindow::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    CStatic::OnLButtonDblClk(nFlags, point);
}


//void CUIWindow::OnRButtonDblClk(UINT nFlags, CPoint point)
//{
//    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//    CStatic::OnRButtonDblClk(nFlags, point);
//}


void CUIWindow::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    m_pUI->MouseMove(point);

    CStatic::OnMouseMove(nFlags, point);
}


void CUIWindow::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if (nFlags == MK_SHIFT)		//��סShift���������Ҽ�ʱ������ϵͳ�˵�
    {
        CPoint point1;
        GetCursorPos(&point1);
        theApp.m_menu_set.m_main_menu_popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, theApp.m_pMainWnd);
    }
    else
    {
        m_pUI->RButtonUp(point);
    }

    CStatic::OnRButtonUp(nFlags, point);
}


void CUIWindow::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: �ڴ˴������Ϣ����������
                       // ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()
    CMusicPlayerDlg* pMainWindow = dynamic_cast<CMusicPlayerDlg*>(theApp.m_pMainWnd);
    //��Ҫ�ػ�ʱ֪ͨ�߳�ǿ���ػ�
    if (pMainWindow != nullptr)
        pMainWindow->m_ui_thread_para.ui_force_refresh = true;
}
