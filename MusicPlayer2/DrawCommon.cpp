#include "stdafx.h"
#include "DrawCommon.h"


CDrawCommon::CDrawCommon()
{
}

CDrawCommon::~CDrawCommon()
{
}

void CDrawCommon::Create(CDC * pDC, CWnd * pMainWnd)
{
	m_pDC = pDC;
	m_pMainWnd = pMainWnd;
	m_pfont = m_pMainWnd->GetFont();
}

void CDrawCommon::SetBackColor(COLORREF back_color)
{
	m_backColor = back_color;
}

void CDrawCommon::SetFont(CFont * pfont)
{
	m_pfont = pfont;
}

void CDrawCommon::SetDC(CDC * pDC)
{
	m_pDC = pDC;
}

void CDrawCommon::DrawWindowText(CRect rect, LPCTSTR lpszString, COLORREF color, bool center, bool no_clip_area)
{
	m_pDC->SetTextColor(color);
	//m_pDC->SetBkMode(TRANSPARENT);
	m_pDC->SelectObject(m_pfont);
	//���û�ͼ�ļ�������
	if (!no_clip_area)
	{
		SetDrawArea(m_pDC, rect);
	}
	CSize text_size = m_pDC->GetTextExtent(lpszString);
	//�ñ���ɫ����������
	m_pDC->FillSolidRect(rect, m_backColor);
	if (text_size.cx > rect.Width())		//����ı���ȳ����˾�������Ŀ�ȣ������������
		m_pDC->DrawText(lpszString, rect, DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
	else
		m_pDC->DrawText(lpszString, rect, (center ? DT_CENTER : 0) | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
}

void CDrawCommon::DrawWindowText(CRect rect, LPCTSTR lpszString, COLORREF color1, COLORREF color2, int split, bool center, bool no_clip_area)
{
	if (split < 0) split = 0;
	if (split > 1000) split = 1000;
	m_pDC->SetBkMode(TRANSPARENT);
	m_pDC->SelectObject(m_pfont);
	CSize text_size;	//�ı��Ĵ�С
	int text_top, text_left;		//����ı���top��leftλ��
	//���û�ͼ�ļ������򣬷�ֹ������������ؼ�����
	if (!no_clip_area)
	{
		SetDrawArea(m_pDC, rect);
	}
	//��ȡ���ֵĿ�Ⱥ͸߶�
	text_size = m_pDC->GetTextExtent(lpszString);
	//�������ֵ���ʼ����
	text_top = rect.top + (rect.Height() - text_size.cy) / 2;
	if (center)
		text_left = rect.left + (rect.Width() - text_size.cx) / 2;
	else
		text_left = rect.left;
	//���㱳�����ֺ͸������ֵľ�������
	CRect text_rect{ CPoint{ text_left, text_top }, text_size };		//�������ֵ�����
	CRect text_f_rect{ CPoint{ text_left, text_top }, CSize{ text_size.cx * split / 1000, text_size.cy } };		//�������ֵ�����
																												//����ı���ȴ��ڿؼ���ȣ���Ҫ���ݷָ��λ�ù����ı�
	if (text_size.cx > rect.Width())
	{
		//����ָ��λ�ã���ʽ��ȣ�ʣ�µĿ���Ѿ�С�ڿؼ���ȵ�һ�룬��ʱʹ�ı��Ҳ�Ϳؼ��Ҳ����
		if (text_rect.Width() - text_f_rect.Width() < rect.Width() / 2)
		{
			text_rect.MoveToX(rect.left - (text_rect.Width() - rect.Width()));
			text_f_rect.MoveToX(text_rect.left);
		}
		//�ָ�λ��ʣ�µĿ�Ȼ�û�е�С�ڿؼ���ȵ�һ�룬���Ƿָ�λ�õĿ���Ѿ����ڿؼ���ȵ�һ��ʱ����Ҫ�ƶ��ı�ʹ�ָ�λ�������ڿؼ����м�
		else if (text_f_rect.Width() > rect.Width() / 2)
		{
			text_rect.MoveToX(rect.left - (text_f_rect.Width() - rect.Width() / 2));
			text_f_rect.MoveToX(text_rect.left);
		}
		//�ָ�λ�û������ؼ���ȵ�һ��ʱ��ʹ�ı����Ϳؼ�������
		else
		{
			text_rect.MoveToX(rect.left);
			text_f_rect.MoveToX(rect.left);
		}
	}

	//�ñ���ɫ����������
	m_pDC->FillSolidRect(rect, m_backColor);
	//����ı�
	m_pDC->SetTextColor(color2);
	m_pDC->DrawText(lpszString, text_rect, DT_SINGLELINE | DT_NOPREFIX);		//���Ʊ�������
	m_pDC->SetTextColor(color1);
	m_pDC->DrawText(lpszString, text_f_rect, DT_SINGLELINE | DT_NOPREFIX);		//���Ƹ�������
}

void CDrawCommon::DrawScrollText(CRect rect, LPCTSTR lpszString, COLORREF color, int pixel, bool center, ScrollInfo& scroll_info, bool reset)
{
	//static int shift_cnt;		//�ƶ��Ĵ���
	//static bool shift_dir;		//�ƶ��ķ�������Ϊfalse������Ϊtrue
	//static int freez;			//���ñ�������0ʱ���ı���������ֱ��С�ڵ���0Ϊֹ
	//static bool dir_changed{ false };	//����������˱仯����Ϊtrue
	if (reset)
	{
		scroll_info.shift_cnt = 0;
		scroll_info.shift_dir = false;
		scroll_info.freez = 20;
		scroll_info.dir_changed = false;
	}
	m_pDC->SetTextColor(color);
	m_pDC->SetBkMode(TRANSPARENT);
	m_pDC->SelectObject(m_pfont);
	CSize text_size;	//�ı��Ĵ�С
	int text_top, text_left;		//����ı���top��leftλ��
	//���û�ͼ�ļ������򣬷�ֹ������������ؼ�����
	SetDrawArea(m_pDC, rect);
	//��ȡ���ֵĿ�Ⱥ͸߶�
	text_size = m_pDC->GetTextExtent(lpszString);
	//�������ֵ���ʼ����
	text_top = rect.top + (rect.Height() - text_size.cy) / 2;
	if (center)
		text_left = rect.left + (rect.Width() - text_size.cx) / 2;
	else
		text_left = rect.left;
	//�������ֵľ�������
	CRect text_rect{ CPoint{ text_left, text_top }, text_size };
	//����ı���ȴ��ڿؼ���ȣ��͹����ı�
	if (text_size.cx > rect.Width())
	{
		text_rect.MoveToX(rect.left - scroll_info.shift_cnt*pixel);
		if ((text_rect.right < rect.right || text_rect.left > rect.left))		//�ƶ����߽�ʱ������
		{
			if (!scroll_info.dir_changed)
			{
				scroll_info.shift_dir = !scroll_info.shift_dir;
				scroll_info.freez = 20;		//�任����ʱ��΢��ͣ����һ��ʱ��
			}
			scroll_info.dir_changed = true;
		}
		else
		{
			scroll_info.dir_changed = false;
		}
	}
	//�ñ���ɫ����������
	m_pDC->FillSolidRect(rect, m_backColor);
	//����ı�
	m_pDC->DrawText(lpszString, text_rect, DT_SINGLELINE | DT_NOPREFIX);
	if (scroll_info.freez <= 0)		//��freezΪ0��ʱ��Ź���
	{
		if (scroll_info.shift_dir)
			scroll_info.shift_cnt--;
		else
			scroll_info.shift_cnt++;
	}
	else
	{
		scroll_info.freez--;
	}
}

void CDrawCommon::DrawScrollText2(CRect rect, LPCTSTR lpszString, COLORREF color, int pixel, bool center, ScrollInfo & scroll_info, bool reset)
{
	if (reset)
	{
		scroll_info.shift_cnt = 0;
		scroll_info.freez = 20;
	}
	m_pDC->SetTextColor(color);
	m_pDC->SetBkMode(TRANSPARENT);
	m_pDC->SelectObject(m_pfont);
	CSize text_size;	//�ı��Ĵ�С
	int text_top, text_left;		//����ı���top��leftλ��
	//���û�ͼ�ļ������򣬷�ֹ������������ؼ�����
	SetDrawArea(m_pDC, rect);
	//��ȡ���ֵĿ�Ⱥ͸߶�
	text_size = m_pDC->GetTextExtent(lpszString);
	//�������ֵ���ʼ����
	text_top = rect.top + (rect.Height() - text_size.cy) / 2;
	if (center)
		text_left = rect.left + (rect.Width() - text_size.cx) / 2;
	else
		text_left = rect.left;
	//�������ֵľ�������
	CRect text_rect{ CPoint{ text_left, text_top }, text_size };
	//����ı���ȴ��ڿؼ���ȣ��͹����ı�
	if (text_size.cx > rect.Width())
	{
		text_rect.MoveToX(rect.left - scroll_info.shift_cnt*pixel);
		if ((text_rect.right < rect.right || text_rect.left > rect.left))		//�ƶ������߽�ʱ��ͣ������freez��20��ʼ�ݼ�
		{
			scroll_info.shift_cnt--;	//���ı������ƶ�һ�Σ���ֹ�����ж�Ϊ�����߽�
			text_rect.MoveToX(rect.left - scroll_info.shift_cnt*pixel);
			scroll_info.freez = 20;		//�任����ʱ��΢��ͣ����һ��ʱ��
		}
	}
	//�ñ���ɫ����������
	m_pDC->FillSolidRect(rect, m_backColor);
	//����ı�
	m_pDC->DrawText(lpszString, text_rect, DT_SINGLELINE | DT_NOPREFIX);
	if (scroll_info.freez <= 0)		//��freezΪ0��ʱ��Ź���
	{
		scroll_info.shift_cnt++;
	}
	else
	{
		scroll_info.freez--;
		if (scroll_info.freez == 10)		//��freez�ݼ���һ��ʱ���ı���λ
			scroll_info.shift_cnt = 0;
	}
}

//void CDrawCommon::FillRect(CDC * pDC, CRect rect, COLORREF color)
//{
//	CBrush BGBrush, *pOldBrush;
//	BGBrush.CreateSolidBrush(color);
//	pOldBrush = pDC->SelectObject(&BGBrush);
//	pDC->FillRect(&rect, &BGBrush);
//	pDC->SelectObject(pOldBrush);
//	BGBrush.DeleteObject();
//}

void CDrawCommon::SetDrawArea(CDC * pDC, CRect rect)
{
	CRgn rgn;
	rgn.CreateRectRgnIndirect(rect);
	pDC->SelectClipRgn(&rgn);
}
