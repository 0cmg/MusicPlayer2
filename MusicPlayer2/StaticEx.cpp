#include "stdafx.h"
#include "StaticEx.h"


CStaticEx::CStaticEx()
{
}

CStaticEx::~CStaticEx()
{
}

void CStaticEx::DrawWindowText(LPCTSTR lpszString, bool draw_back_ground)
{
	m_text = lpszString;
	m_text_type = TextType::NORMAL;
	m_draw_back_ground = draw_back_ground;
	if (m_draw_back_ground)
		Invalidate(FALSE);
	else
		Invalidate();
}

void CStaticEx::DrawWindowText(LPCTSTR lpszString, int split, bool draw_back_ground)
{
	m_text = lpszString;
	m_text_type = TextType::SPLIT;
	m_split = split;
	m_draw_back_ground = draw_back_ground;
	if (m_draw_back_ground)
		Invalidate(FALSE);
	else
		Invalidate();
}

void CStaticEx::DrawScrollText(LPCTSTR lpszString, int pixel, bool reset, bool draw_back_ground)
{
	m_text = lpszString;
	m_text_type = TextType::SCROLL;
	m_scroll_pixel = pixel;
	m_scroll_reset = reset;
	m_draw_back_ground = draw_back_ground;
	if (m_draw_back_ground)
		Invalidate(FALSE);
	else
		Invalidate();
}

void CStaticEx::SetTextColor(COLORREF textColor)
{
	m_text_color = textColor;
	//DrawWindowText(m_text);
}

void CStaticEx::SetText2Color(COLORREF textColor)
{
	m_text2_color = textColor;
}

void CStaticEx::SetBackColor(COLORREF back_color)
{
	m_back_color = back_color;
}

CString CStaticEx::GetString() const
{
	return m_text;
}


LRESULT CStaticEx::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (message == WM_SETTEXT)
	{
		CRect rect;
		CDC* pDC = GetDC();
		GetClientRect(rect);
		DrawThemeParentBackground(m_hWnd, pDC->GetSafeHdc(), &rect);
		ReleaseDC(pDC);
	}
	return CStatic::DefWindowProc(message, wParam, lParam);
}
BEGIN_MESSAGE_MAP(CStaticEx, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



void CStaticEx::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	GetWindowText(m_text);

	CStatic::PreSubclassWindow();
}


void CStaticEx::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()
	CRect rect;
	this->GetClientRect(&rect);
	switch (m_text_type)
	{
	case CStaticEx::TextType::NORMAL:
		if (m_draw_back_ground)
			dc.FillSolidRect(rect, m_back_color);
		else
			dc.SetBkMode(TRANSPARENT);
		dc.SelectObject(this->GetFont());
		if (!m_draw_back_ground)
			DrawThemeParentBackground(m_hWnd, dc.GetSafeHdc(), &rect);	//�ػ�ؼ������Խ�������ص�������
		dc.SetTextColor(m_text_color);
		dc.DrawText(m_text, rect, (m_center ? DT_CENTER : 0) | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
		break;
	case CStaticEx::TextType::SPLIT:
	{
		if (m_split < 0) m_split = 0;
		if (m_split > 1000) m_split = 1000;
		if (m_draw_back_ground)
			dc.FillSolidRect(rect, m_back_color);
		else
			dc.SetBkMode(TRANSPARENT);
		dc.SelectObject(this->GetFont());
		CSize text_size;	//�ı��Ĵ�С
		int text_top, text_left;		//����ı���top��leftλ��
		//���û�ͼ�ļ������򣬷�ֹ������������ؼ�����
		CRgn rgn;
		rgn.CreateRectRgnIndirect(rect);
		dc.SelectClipRgn(&rgn);
		//��ȡ���ֵĿ�Ⱥ͸߶�
		text_size = dc.GetTextExtent(m_text);
		//�������ֵ���ʼ����
		text_top = rect.top + (rect.Height() - text_size.cy) / 2;
		if (m_center)
			text_left = rect.left + (rect.Width() - text_size.cx) / 2;
		else
			text_left = 0;
		//���㱳�����ֺ͸������ֵľ�������
		CRect text_rect{ CPoint{ text_left, text_top }, text_size };		//�������ֵ�����
		CRect text_f_rect{ CPoint{ text_left, text_top }, CSize{ text_size.cx * m_split / 1000, text_size.cy } };		//�������ֵ�����
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
				text_rect.MoveToX(0);
				text_f_rect.MoveToX(0);
			}
		}

		if (!m_draw_back_ground)
			DrawThemeParentBackground(m_hWnd, dc.GetSafeHdc(), &rect);	//�ػ�ؼ������Խ�������ص�������
		dc.SetTextColor(m_text2_color);
		dc.DrawText(m_text, text_rect, DT_SINGLELINE | DT_NOPREFIX);		//���Ʊ�������
		dc.SetTextColor(m_text_color);
		dc.DrawText(m_text, text_f_rect, DT_SINGLELINE | DT_NOPREFIX);		//���Ƹ�������
	}
		break;
	case CStaticEx::TextType::SCROLL:
	{
		static int shift_cnt;		//�ƶ��Ĵ���
		static bool shift_dir;		//�ƶ��ķ�������Ϊfalse������Ϊtrue
		static int freez;			//���ñ�������0ʱ���ı���������ֱ��С�ڵ���0Ϊֹ
		static bool dir_changed{ false };	//����������˱仯����Ϊtrue
		if (m_scroll_reset)
		{
			shift_cnt = 0;
			shift_dir = false;
			freez = 20;
			dir_changed = false;
		}
		dc.SetTextColor(m_text_color);
		if (m_draw_back_ground)
			dc.FillSolidRect(rect, m_back_color);
		else
			dc.SetBkMode(TRANSPARENT);
		dc.SelectObject(this->GetFont());
		CSize text_size;	//�ı��Ĵ�С
		int text_top, text_left;		//����ı���top��leftλ��
		//���û�ͼ�ļ������򣬷�ֹ������������ؼ�����
		CRgn rgn;
		rgn.CreateRectRgnIndirect(rect);
		dc.SelectClipRgn(&rgn);
		//��ȡ���ֵĿ�Ⱥ͸߶�
		text_size = dc.GetTextExtent(m_text);
		//�������ֵ���ʼ����
		text_top = rect.top + (rect.Height() - text_size.cy) / 2;
		if (m_center)
			text_left = rect.left + (rect.Width() - text_size.cx) / 2;
		else
			text_left = 0;
		//�������ֵľ�������
		CRect text_rect{ CPoint{ text_left, text_top }, text_size };
		//����ı���ȴ��ڿؼ���ȣ��͹����ı�
		if (text_size.cx > rect.Width())
		{
			text_rect.MoveToX(rect.left - shift_cnt * m_scroll_pixel);
			if ((text_rect.right < rect.right || text_rect.left > rect.left))		//�ƶ����߽�ʱ������
			{
				if (!dir_changed)
				{
					shift_dir = !shift_dir;
					freez = 20;		//�任����ʱ��΢��ͣ����һ��ʱ��
				}
				dir_changed = true;
			}
			else
			{
				dir_changed = false;
			}
		}
		if (!m_draw_back_ground)
			DrawThemeParentBackground(m_hWnd, dc.GetSafeHdc(), &rect);	//�ػ�ؼ������Խ�������ص�������
		dc.DrawText(m_text, text_rect, DT_SINGLELINE | DT_NOPREFIX);
		if (freez <= 0)		//��freezΪ0��ʱ��Ź���
		{
			if (shift_dir)
				shift_cnt--;
			else
				shift_cnt++;
		}
		else
		{
			freez--;
		}
	}
		break;
	default:
		break;
	}
}
