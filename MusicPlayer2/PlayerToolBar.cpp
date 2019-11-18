// ProgressStatic.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "PlayerToolBar.h"


// CPlayerToolBar

IMPLEMENT_DYNAMIC(CPlayerToolBar, CStatic)

CPlayerToolBar::CPlayerToolBar()
    : m_theme_color(theApp.m_app_setting_data.theme_color)
{
}

CPlayerToolBar::~CPlayerToolBar()
{
}

void CPlayerToolBar::AddToolButton(IconRes icon, LPCTSTR strText, LPCTSTR strToolTip, UINT cmdId, bool showText)
{
    ToolBtn btn;
    btn.cmd_id = cmdId;
    btn.is_cmd = true;
    btn.icon = icon;
    if (strText != nullptr)
        btn.text = strText;
    btn.show_text = showText;
    if (strToolTip != nullptr)
        btn.tooltip_text = strToolTip;
    m_buttons.push_back(btn);
}

void CPlayerToolBar::AddToolButton(IconRes icon, LPCTSTR strText, LPCTSTR strToolTip, CMenu * pMenu, bool showText)
{
    ToolBtn btn;
    btn.pMenu = pMenu;
    btn.is_cmd = false;
    btn.icon = icon;
    if (strText != nullptr)
        btn.text = strText;
    btn.show_text = showText;
    if(strToolTip != nullptr)
        btn.tooltip_text = strToolTip;
    m_buttons.push_back(btn);
}

void CPlayerToolBar::SetIconSize(int size)
{
    m_icon_size = size;
}


void CPlayerToolBar::AddToolTips()
{
    for (size_t i = 0; i < m_buttons.size(); i++)
    {
        if(!m_buttons[i].tooltip_text.IsEmpty())
            m_tool_tip.AddTool(this, m_buttons[i].tooltip_text, m_buttons[i].rect, i + 100);
    }
}


BEGIN_MESSAGE_MAP(CPlayerToolBar, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
    ON_WM_MOUSELEAVE()
    ON_WM_MOUSEHOVER()
    ON_MESSAGE(WM_INITMENU, &CPlayerToolBar::OnInitmenu)
END_MESSAGE_MAP()



// CPlayerToolBar ��Ϣ�������

void CPlayerToolBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()

    CRect rect;
    GetClientRect(rect);

    //���û����DC
    CDC MemDC;
    CBitmap MemBitmap;
    MemDC.CreateCompatibleDC(NULL);

    MemBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
    CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);

    CDrawCommon drawer;
    drawer.Create(&MemDC, this);

    //���Ʊ���
    drawer.FillRect(rect, CColorConvert::m_gray_color.light3);

    //����ͼ��
    CRect rc_icon = rect;
    rc_icon.left += theApp.DPI(2);
    rc_icon.top = (rect.Height() - m_icon_size) / 2;
    rc_icon.right = rc_icon.left;
    rc_icon.bottom = rc_icon.top + m_icon_size;

    for (auto iter = m_buttons.begin(); iter!=m_buttons.end(); iter++)
    {
        if(iter != m_buttons.begin())
        {
            rc_icon.left = (iter-1)->rect.right + theApp.DPI(2);
        }

        bool draw_text = iter->show_text && !(iter->text.IsEmpty());
        if (draw_text)
        {
            CSize text_size = drawer.GetTextExtent(iter->text);
            rc_icon.right = rc_icon.left + m_icon_size + text_size.cx + theApp.DPI(4);
        }
        else
        {
            rc_icon.right = rc_icon.left + m_icon_size;
        }

        iter->rect = rc_icon;
        if (iter->hover)
            drawer.FillRect(rc_icon, m_theme_color.light2_5);
        if(iter->pressed)
            drawer.FillRect(rc_icon, m_theme_color.light1_5);

        CRect rc_tmp = rc_icon;
        //ʹͼ���ھ����о���
        CSize icon_size = iter->icon.GetSize();
        if (draw_text)
            rc_tmp.left = rc_icon.left + theApp.DPI(2);
        else
            rc_tmp.left = rc_icon.left + (rc_icon.Width() - icon_size.cx) / 2;
        rc_tmp.top = rc_icon.top + (rc_icon.Height() - icon_size.cy) / 2;
        rc_tmp.right = rc_tmp.left + icon_size.cx;
        rc_tmp.bottom = rc_tmp.top + icon_size.cy;
        if (iter->pressed)
        {
            rc_tmp.MoveToX(rc_tmp.left + theApp.DPI(1));
            rc_tmp.MoveToY(rc_tmp.top + theApp.DPI(1));
        }
        drawer.SetDrawArea(rc_tmp);
        drawer.DrawIcon(iter->icon.GetIcon(true), rc_tmp.TopLeft(), rc_tmp.Size());
        //�����ı�
        if (draw_text)
        {
            CRect rc_text = rc_icon;
            rc_text.left = rc_tmp.right + theApp.DPI(2);
            COLORREF text_color;
            if (IsWindowEnabled())
                text_color = CColorConvert::m_gray_color.dark3;
            else
                text_color = CColorConvert::m_gray_color.dark1;
            if(iter->pressed)
                rc_text.MoveToY(rc_text.top + theApp.DPI(1));
            drawer.DrawWindowText(rc_text, iter->text, text_color);
        }

    }

    dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);
    MemDC.SelectObject(pOldBit);
    MemBitmap.DeleteObject();
    MemDC.DeleteDC();

    if (m_first_draw && !m_buttons.empty())
    {
        AddToolTips();              //�ڵ�һ�λ������֮����������ʾ����Ϊ�ڻ���֮ǰ�޷�ȷ����ť��������
        m_first_draw = false;
    }
}


void CPlayerToolBar::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	//Ϊ�ؼ�����SS_NOTIFY����
    ModifyStyle(NULL, SS_NOTIFY);

    //��ʼ����ʾ��Ϣ
    m_tool_tip.Create(this, TTS_ALWAYSTIP);
    m_tool_tip.SetMaxTipWidth(theApp.DPI(400));

	CStatic::PreSubclassWindow();
}


void CPlayerToolBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    for (auto& btn : m_buttons)
    {
        if (btn.rect.PtInRect(point) != FALSE)
        {
            btn.pressed = true;
            InvalidateRect(btn.rect);
        }
    }

	CStatic::OnLButtonDown(nFlags, point);
}


void CPlayerToolBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    for (auto& btn : m_buttons)
    {
        btn.pressed = false;
        if (btn.rect.PtInRect(point) && btn.enable)
        {
            if (btn.is_cmd)
                theApp.m_pMainWnd->SendMessage(WM_COMMAND, btn.cmd_id, 0);
            else if (btn.pMenu != nullptr)
            {
                CPoint point;
                point.x = btn.rect.left;
                point.y = btn.rect.bottom;
                ClientToScreen(&point);
                m_menu_poped_up = true;
                btn.pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
                m_menu_poped_up = false;
            }
        }
    }
    Invalidate();
    CStatic::OnLButtonUp(nFlags, point);
}


void CPlayerToolBar::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    for (auto& btn : m_buttons)
    {
        bool hover = (btn.rect.PtInRect(point) != FALSE);
        if(hover != btn.hover)
        {
            btn.hover = hover;
            InvalidateRect(btn.rect);
        }
    }

    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(tme);
    tme.hwndTrack = m_hWnd;
    tme.dwFlags = TME_LEAVE | TME_HOVER;
    tme.dwHoverTime = 1;
    _TrackMouseEvent(&tme);

	CStatic::OnMouseMove(nFlags, point);
}


BOOL CPlayerToolBar::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_tool_tip.GetSafeHwnd() && pMsg->message == WM_MOUSEMOVE)
	{
		m_tool_tip.RelayEvent(pMsg);
	}


	return CStatic::PreTranslateMessage(pMsg);
}


void CPlayerToolBar::OnMouseLeave()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if(!m_menu_poped_up)
    {
        for (auto& btn : m_buttons)
        {
            btn.pressed = false;
            btn.hover = false;
        }
        Invalidate();
    }

    CStatic::OnMouseLeave();
}


void CPlayerToolBar::OnMouseHover(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    CStatic::OnMouseHover(nFlags, point);
}


afx_msg LRESULT CPlayerToolBar::OnInitmenu(WPARAM wParam, LPARAM lParam)
{
    AfxGetMainWnd()->SendMessage(WM_INITMENU, wParam, lParam);        //��WM_INITMENU��Ϣת����������
    return 0;
}


BOOL CPlayerToolBar::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // TODO: �ڴ����ר�ô����/����û���
    CWnd* pParent = GetParent();
    if (pParent == nullptr)
        pParent = AfxGetMainWnd();
    pParent->SendMessage(WM_COMMAND, wParam, lParam);        //��WM_COMMAND��Ϣת����������

    return CStatic::OnCommand(wParam, lParam);
}
