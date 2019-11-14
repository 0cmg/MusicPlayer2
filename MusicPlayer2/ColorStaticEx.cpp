#include "stdafx.h"
#include "ColorStaticEx.h"


CColorStaticEx::CColorStaticEx()
{
}


CColorStaticEx::~CColorStaticEx()
{
}


COLORREF CColorStaticEx::GetFillColor() const
{
	return m_fill_color;
}

void CColorStaticEx::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	DWORD dwStyle = GetStyle();
	::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

	CColorStatic::PreSubclassWindow();
}
BEGIN_MESSAGE_MAP(CColorStaticEx, CColorStatic)
//	ON_CONTROL_REFLECT(STN_CLICKED, &CColorStaticEx::OnStnClicked)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


//void CColorStaticEx::OnStnClicked()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//}


void CColorStaticEx::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CColorDialog color_dlg(m_fill_color);
	if (color_dlg.DoModal() == IDOK)
	{
		SetFillColor(color_dlg.GetColor());

        //�򸸴��ڷ���WM_COLOR_SELECTED��Ϣ
        CWnd* pParent = GetParent();
        if(pParent!=nullptr)
            ::SendMessage(pParent->GetSafeHwnd(), WM_COLOR_SELECTED, (WPARAM)this, 0);
	}

	CColorStatic::OnLButtonUp(nFlags, point);
}
