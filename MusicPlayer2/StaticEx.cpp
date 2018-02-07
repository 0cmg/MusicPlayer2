#include "stdafx.h"
#include "StaticEx.h"


CStaticEx::CStaticEx()
{
}

CStaticEx::~CStaticEx()
{
}

void CStaticEx::DrawWindowText(LPCTSTR lpszString)
{
	CDC* pDC = GetDC();
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(this->GetFont());
	CRect rect;
	this->GetClientRect(&rect);
	DrawThemeParentBackground(m_hWnd, pDC->GetSafeHdc(), &rect);	//�ػ�ؼ������Խ�������ص�������
	pDC->SetTextColor(m_TextColor);
	pDC->DrawText(lpszString, rect, (m_center ? DT_CENTER : 0) | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX);
	ReleaseDC(pDC);
	m_text = lpszString;
}

void CStaticEx::DrawWindowText(LPCTSTR lpszString, int split)
{
	if (split < 0) split = 0;
	if (split > 1000) split = 1000;
	CDC* pDC = GetDC();
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(this->GetFont());
	CRect rect;		//�����ؼ��ľ���
	CSize text_size;	//�ı��Ĵ�С
	int text_top, text_left;		//����ı���top��leftλ��
	this->GetClientRect(&rect);
	//���û�ͼ�ļ������򣬷�ֹ������������ؼ�����
	CRgn rgn;
	rgn.CreateRectRgnIndirect(rect);
	pDC->SelectClipRgn(&rgn);
	//��ȡ���ֵĿ�Ⱥ͸߶�
	text_size = pDC->GetTextExtent(lpszString);
	//�������ֵ���ʼ����
	text_top = rect.top + (rect.Height() - text_size.cy) / 2;
	if (m_center)
		text_left = rect.left + (rect.Width() - text_size.cx) / 2;
	else
		text_left = 0;
	//���㱳�����ֺ͸������ֵľ�������
	CRect text_rect{ CPoint{text_left, text_top}, text_size};		//�������ֵ�����
	CRect text_f_rect{ CPoint{ text_left, text_top }, CSize{ text_size.cx * split / 1000, text_size.cy} };		//�������ֵ�����
	//����ı���ȴ��ڿؼ���ȣ���Ҫ���ݷָ��λ�ù����ı�
	if (text_size.cx > rect.Width())
	{
		//����ָ��λ�ã���ʽ��ȣ�ʣ�µĿ���Ѿ�С�ڿؼ���ȵ�һ�룬��ʱʹ�ı��Ҳ�Ϳؼ��Ҳ����
		if (text_rect.Width() - text_f_rect.Width() < rect.Width() / 2)
		{
			text_rect.MoveToX(rect.left-(text_rect.Width()-rect.Width()));
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

	DrawThemeParentBackground(m_hWnd, pDC->GetSafeHdc(), &rect);	//�ػ�ؼ������Խ�������ص�������
	pDC->SetTextColor(m_TextBackColor);
	pDC->DrawText(lpszString, text_rect, DT_SINGLELINE | DT_NOPREFIX);		//���Ʊ�������
	pDC->SetTextColor(m_TextColor);
	pDC->DrawText(lpszString, text_f_rect, DT_SINGLELINE | DT_NOPREFIX);		//���Ƹ�������

	ReleaseDC(pDC);
	m_text = lpszString;
}

void CStaticEx::DrawScrollText(LPCTSTR lpszString, int pixel, bool reset)
{
	static int shift_cnt;		//�ƶ��Ĵ���
	static bool shift_dir;		//�ƶ��ķ�������Ϊfalse������Ϊtrue
	static int freez;			//���ñ�������0ʱ���ı���������ֱ��С�ڵ���0Ϊֹ
	static bool dir_changed{ false };	//����������˱仯����Ϊtrue
	if (reset)
	{
		shift_cnt = 0;
		shift_dir = false;
		freez = 20;
		dir_changed = false;
	}
	CDC* pDC = GetDC();
	pDC->SetTextColor(m_TextColor);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(this->GetFont());
	CRect rect;		//�����ؼ��ľ���
	CSize text_size;	//�ı��Ĵ�С
	int text_top, text_left;		//����ı���top��leftλ��
	this->GetClientRect(&rect);
	//���û�ͼ�ļ������򣬷�ֹ������������ؼ�����
	CRgn rgn;
	rgn.CreateRectRgnIndirect(rect);
	pDC->SelectClipRgn(&rgn);
	//��ȡ���ֵĿ�Ⱥ͸߶�
	text_size = pDC->GetTextExtent(lpszString);
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
		text_rect.MoveToX(rect.left - shift_cnt*pixel);
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
	DrawThemeParentBackground(m_hWnd, pDC->GetSafeHdc(), &rect);	//�ػ�ؼ������Խ�������ص�������
	pDC->DrawText(lpszString, text_rect, DT_SINGLELINE | DT_NOPREFIX);
	ReleaseDC(pDC);
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
	m_text = lpszString;
}

void CStaticEx::SetTextColor(COLORREF textColor)
{
	m_TextColor = textColor;
	//DrawWindowText(m_text);
}

void CStaticEx::SetText2Color(COLORREF textColor)
{
	m_TextBackColor = textColor;
}

CString CStaticEx::GetString() const
{
	return m_text;
}

void CStaticEx::SetFillColor(COLORREF fill_color)
{
	m_fill_color = fill_color;
	m_fill_color_enable = true;
	Invalidate();
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
	if (m_fill_color_enable)
	{
		CRect rect;
		GetClientRect(rect);
		dc.FillSolidRect(rect, m_fill_color);
	}
}
