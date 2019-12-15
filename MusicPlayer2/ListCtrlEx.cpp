#include "stdafx.h"
#include "ListCtrlEx.h"
#include "MusicPlayer2.h"
#include <set>

IMPLEMENT_DYNAMIC(CListCtrlEx, CListCtrl)

CListCtrlEx::CListCtrlEx()
	: m_theme_color(theApp.m_app_setting_data.theme_color)
{
	
	//��ʼ����ɫ
	//m_theme_color.original_color = GRAY(180);
	//CColorConvert::ConvertColor(m_theme_color);
    m_drag_cursor = AfxGetApp()->LoadCursor(IDC_DRAG_CURSOR);
}


CListCtrlEx::~CListCtrlEx()
{
}


//void CListCtrlEx::SetColor(const ColorTable & colors)
//{
//	m_theme_color = colors;
//	if (m_hWnd != NULL)
//		Invalidate();
//}

void CListCtrlEx::GetItemSelected(vector<int>& item_selected) const
{
	item_selected.clear();
	POSITION pos = GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = GetNextSelectedItem(pos);
			item_selected.push_back(nItem);
		}
	}
}

int CListCtrlEx::GetCurSel() const
{
	vector<int> item_selected;
	GetItemSelected(item_selected);
	if (!item_selected.empty())
		return item_selected[0];
	else
		return -1;
}

void CListCtrlEx::SetCurSel(int select)
{
	int size = GetItemCount();
	if (select >= 0 && select < size)
	{
		SetItemState(select, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);	//ѡ����
		EnsureVisible(select, FALSE);		//ʹѡ���б��ֿɼ�
	}
	else
	{
		//ȡ�������е�ѡ��
		for(int i{}; i<size; i++)
			SetItemState(i, 0, LVIS_SELECTED);
	}
}

void CListCtrlEx::SetCurSel(int first, int last)
{
    int itemCnt = GetItemCount();
    if (first >= 0 && last < itemCnt && first <= last)
    {
        for (int i = 0; i < itemCnt; i++)
        {
            if(i>=first && i<=last)
                SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
            else
                SetItemState(i, 0, LVIS_SELECTED);
        }
    }
}

void CListCtrlEx::SelectAll()
{
	int itemCnt = GetItemCount();
	for (int i = 0; i < itemCnt; i++)
	{
		SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
	}
}

void CListCtrlEx::SelectNone()
{
    int itemCnt = GetItemCount();
    for (int i = 0; i < itemCnt; i++)
    {
        SetItemState(i, 0, LVIS_SELECTED);
    }
}

void CListCtrlEx::SelectReverse()
{
    std::vector<int> selected_vect;
    GetItemSelected(selected_vect);
    std::set<int> selected_set;
    for (auto n : selected_vect)
        selected_set.insert(n);
    int size = GetItemCount();
    for (int i = 0; i < size; i++)
    {
        if(selected_set.find(i)==selected_set.end())
            SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
        else
            SetItemState(i, 0, LVIS_SELECTED);
    }
}

bool CListCtrlEx::SetRowHeight(int height)
{
	if (height > 0 && height <= 512)
	{
		CImageList imgList;		//ΪClistCtrl����һ��ͼ���б��������и�
		BOOL rtn = imgList.Create(1, height, ILC_COLOR, 1, 1);
		if (rtn != FALSE)
		{
			SetImageList(&imgList, LVSIL_SMALL);
			return true;
		}
	}
	return false;
}


void CListCtrlEx::ShowPopupMenu(CMenu* pMenu, int item_index, CWnd* pWnd)
{
    CPoint point;			//����һ������ȷ�����λ�õ�λ��
    GetCursorPos(&point);	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����
    if(item_index >= 0)
    {
        CRect item_rect;
        GetItemRect(item_index, item_rect, LVIR_BOUNDS);		//��ȡѡ����Ŀ�ľ����������б�ؼ����Ͻ�Ϊԭ�㣩
        CRect window_rect;
        GetWindowRect(window_rect);		//��ȡ�б�ؼ��ľ�����������Ļ���Ͻ�Ϊԭ�㣩
        point.y = window_rect.top + item_rect.bottom;	//�������Ҫ������y����Ϊѡ����Ŀ���±߿�λ�ã���ֹ�Ҽ��˵���סѡ�е���Ŀ
    }
    pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWnd); //��ָ��λ����ʾ�����˵�
}

void CListCtrlEx::SetListData(const ListData& list_data)
{
    int item_num_before = GetItemCount();
    int item_num_after = list_data.size();
    //�����ǰ�б�����Ŀ������С��ԭ���ģ���ֱ�����ԭ���б������е���Ŀ���������
    if (item_num_after < item_num_before)
    {
        DeleteAllItems();
        item_num_before = 0;
    }
    for (int i{}; i < item_num_after; i++)
    {
        const RowData& data_row = list_data[i];
        if (i >= item_num_before)	//�����ǰ�б��е���Ŀ��������֮ǰ������������Ҫ�ڲ���ʱ�����µ���Ŀ
            InsertItem(i, data_row.at(0).c_str());
        for (const auto& item : data_row)
        {
            SetItemText(i, item.first, item.second.c_str());
        }
    }
}

BEGIN_MESSAGE_MAP(CListCtrlEx, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListCtrlEx::OnNMCustomdraw)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETFOCUS()
    ON_NOTIFY_REFLECT(LVN_BEGINDRAG, &CListCtrlEx::OnLvnBegindrag)
    ON_WM_LBUTTONUP()
    ON_WM_SETCURSOR()
    ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void CListCtrlEx::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = CDRF_DODEFAULT;
	LPNMLVCUSTOMDRAW lplvdr = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	NMCUSTOMDRAW& nmcd = lplvdr->nmcd;
	static bool this_item_select = false;
	switch (lplvdr->nmcd.dwDrawStage)	//�ж�״̬   
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:			//���Ϊ��ITEM֮ǰ��Ҫ������ɫ�ĸı�
		this_item_select = false;
		if (IsWindowEnabled())
		{
			//��ѡ�������Ǹ�����ʱ������ɫ
			if (GetItemState(nmcd.dwItemSpec, LVIS_SELECTED) == LVIS_SELECTED && nmcd.dwItemSpec == m_highlight_item)
			{
				this_item_select = true;
				SetItemState(nmcd.dwItemSpec, 0, LVIS_SELECTED);
				lplvdr->clrText = m_theme_color.light3;
				lplvdr->clrTextBk = m_theme_color.dark1;
			}
			//����ѡ���е���ɫ
			else if (GetItemState(nmcd.dwItemSpec, LVIS_SELECTED) == LVIS_SELECTED)
			{
				this_item_select = true;
				SetItemState(nmcd.dwItemSpec, 0, LVIS_SELECTED);
				lplvdr->clrText = m_theme_color.dark3;
				lplvdr->clrTextBk = m_theme_color.light2;
			}
			//���ø����е���ɫ
			else if (nmcd.dwItemSpec == m_highlight_item)
			{
				lplvdr->clrText = m_theme_color.dark2;
				//lplvdr->clrText = 0;
				lplvdr->clrTextBk = m_theme_color.light3;
			}
			//����ż���е���ɫ
			else if (nmcd.dwItemSpec % 2 == 0)
			{
				lplvdr->clrText = CColorConvert::m_gray_color.dark3;
				lplvdr->clrTextBk = CColorConvert::m_gray_color.light3;
			}
			//���������е���ɫ
			else
			{
				lplvdr->clrText = CColorConvert::m_gray_color.dark3;
				lplvdr->clrTextBk = CColorConvert::m_gray_color.light4;
			}

			//�ñ���ɫ��䵥Ԫ����ȥ��ÿ��ǰ��Ŀհ�
			//CRect rect = nmcd.rc;
			//CDC* pDC = CDC::FromHandle(nmcd.hdc);		//��ȡ��ͼDC
			//pDC->FillSolidRect(rect, lplvdr->clrTextBk);
		}
		else		//���ؼ�������ʱ����ʾ�ı���Ϊ��ɫ
		{
			lplvdr->clrText = GRAY(140);
			lplvdr->clrTextBk = GRAY(240);
		}
		*pResult = CDRF_NOTIFYPOSTPAINT;
		break;
	case CDDS_ITEMPOSTPAINT:
		if (this_item_select)
			SetItemState(nmcd.dwItemSpec, 0xFF, LVIS_SELECTED);
        //�ñ���ɫ��䵥Ԫ�����Ŀհ�����
        CRect rect = nmcd.rc;
        rect.right = rect.left + theApp.DPI(4);
        CDC* pDC = CDC::FromHandle(nmcd.hdc);		//��ȡ��ͼDC
        pDC->FillSolidRect(rect, lplvdr->clrTextBk);
        //*pResult = CDRF_DODEFAULT;
		break;
	}
}


void CListCtrlEx::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	CListCtrl::PreSubclassWindow();

    DWORD style = GetExtendedStyle();
    SetExtendedStyle(style | LVS_EX_DOUBLEBUFFER);      //����˫�����ͼ

	SetBkColor(m_background_color);
	//SetHightItem(-1);
	SetRowHeight(theApp.DPI(22));
}


void CListCtrlEx::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	this->SetFocus();
    m_dragging = false;
    CListCtrl::OnLButtonDown(nFlags, point);
}


void CListCtrlEx::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	this->SetFocus();
    m_dragging = false;
    CListCtrl::OnRButtonDown(nFlags, point);
}


BOOL CListCtrlEx::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	//if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_CHAR)		//�����б�ؼ��ļ�����Ϣ����ֹÿ�ΰ���һ����ʱ�б�ѡ���л��������ġ��ѿ������߿�
	//	return TRUE;

	return CListCtrl::PreTranslateMessage(pMsg);
}


void CListCtrlEx::OnSetFocus(CWnd* pOldWnd)
{
	CListCtrl::OnSetFocus(pOldWnd);

	// TODO: �ڴ˴������Ϣ����������

	SendMessage(WM_KILLFOCUS);				//��ֹ�б�ؼ���ȡ���㣬��ֹѡ���л�����ѿ������߿�
}


void CListCtrlEx::OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    if (m_drag_enable)
    {
        m_dragging = true;
    }

    *pResult = 0;
}


void CListCtrlEx::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if (m_dragging)
    {
        CPoint pt(point);
        int drop_index = this->HitTest(pt);     //����ɿ�ʱ����Ŀ���
        CWnd* pParent{ GetParent() };
        if (pParent != nullptr)
        {
            pParent->SendMessage(WM_LIST_ITEM_DRAGGED, drop_index, 0);       //�����Ϸ�ʱ�򸸴��ڷ�����Ϣ�������ϷŽ���λ������
        }
        m_dragging = false;
        SendMessage(WM_SETCURSOR);      //����ɿ�ʱˢ�¹��
    }

    CListCtrl::OnLButtonUp(nFlags, point);
}


BOOL CListCtrlEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if (m_dragging)
    {
        ::SetCursor(m_drag_cursor);
        return TRUE;
    }

    return CListCtrl::OnSetCursor(pWnd, nHitTest, message);
}


BOOL CListCtrlEx::OnEraseBkgnd(CDC* pDC)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    //return CListCtrl::OnEraseBkgnd(pDC);
    return TRUE;
}
