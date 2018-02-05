#include "stdafx.h"
#include "CortanaLyric.h"


CCortanaLyric::CCortanaLyric()
{
}


CCortanaLyric::~CCortanaLyric()
{
}

void CCortanaLyric::Init()
{
	if (m_enable)
	{
		HWND hTaskBar = ::FindWindow(_T("Shell_TrayWnd"), NULL);	//�������ľ��
		HWND hCortanaBar = ::FindWindowEx(hTaskBar, NULL, _T("TrayDummySearchControl"), NULL);	//Cortana���ľ�������а���3���Ӵ��ڣ�
		m_cortana_hwnd = ::FindWindowEx(hCortanaBar, NULL, _T("Button"), NULL);	//Cortana������������Ϊ��Button���Ĵ��ڵľ��
		m_hCortanaStatic = ::FindWindowEx(hCortanaBar, NULL, _T("Static"), NULL);		//Cortana������������Ϊ��Static���Ĵ��ڵľ��
		if (m_cortana_hwnd == NULL) return;
		wchar_t buff[32];
		::GetWindowText(m_cortana_hwnd, buff, 31);		//��ȡCortana��������ԭ�����ַ����������ڳ����˳�ʱ�ָ�
		m_cortana_default_text = buff;
		m_cortana_wnd = CWnd::FromHandle(m_cortana_hwnd);		//��ȡCortana�������CWnd���ָ��
		if (m_cortana_wnd == nullptr) return;

		//��ȡ���Ͻǵ������
		CRect rect;
		::GetWindowRect(m_cortana_hwnd, rect);
		m_lefttop_point = rect.TopLeft();
		m_lefttop_point += CPoint{ 1, 1 };

		::GetClientRect(m_cortana_hwnd, m_cortana_rect);	//��ȡCortana������ľ�������
		CRect cortana_static_rect;		//Cortana��������static�ؼ��ľ�������
		::GetClientRect(m_hCortanaStatic, cortana_static_rect);	//��ȡCortana��������static�ؼ��ľ�������
		if (cortana_static_rect.Width() > 0)
		{
			m_cortana_left_space = m_cortana_rect.Width() - cortana_static_rect.Width();
			m_cortana_rect.right = m_cortana_rect.left + cortana_static_rect.Width();		//����Cortana���ھ��εĿ��
		}
		else
		{
			m_cortana_left_space = 0;
		}

		m_cortana_pDC = m_cortana_wnd->GetDC();
		m_cortana_draw.Create(m_cortana_pDC, m_cortana_wnd);

		CheckDarkMode();
		
		//��������
		LOGFONT lf;
		SystemParametersInfo(SPI_GETICONTITLELOGFONT, sizeof(LOGFONT), &lf, 0);		//��ȡϵͳĬ������
		if (m_cortana_font.m_hObject)		//���m_font�Ѿ�������һ��������Դ�������ͷ���
			m_cortana_font.DeleteObject();
		m_cortana_font.CreatePointFont(110, lf.lfFaceName);
		m_font_double_line.CreatePointFont(110, lf.lfFaceName);
		int a = 0;
	}
}

void CCortanaLyric::SetEnable(bool enable)
{
	m_enable = enable;
}

void CCortanaLyric::SetColors(ColorTable colors)
{
	m_colors = colors;
}

void CCortanaLyric::DrawCortanaText(LPCTSTR str, bool reset, int scroll_pixel)
{
	if (m_enable && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		m_cortana_draw.SetFont(&m_cortana_font);
		//���û����DC
		CDC MemDC;
		CBitmap MemBitmap;
		MemDC.CreateCompatibleDC(NULL);
		MemBitmap.CreateCompatibleBitmap(m_cortana_pDC, m_cortana_rect.Width(), m_cortana_rect.Height());
		CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
		//ʹ��m_cortana_draw��ͼ
		m_cortana_draw.SetDC(&MemDC);
		static CDrawCommon::ScrollInfo cortana_scroll_info;
		COLORREF color;
		color = (m_dark_mode ? m_colors.light3 : m_colors.dark2);
		m_cortana_draw.DrawScrollText(m_cortana_rect, str, color, scroll_pixel, false, cortana_scroll_info, reset);
		//��������DC�е�ͼ�񿽱�����Ļ����ʾ
		m_cortana_pDC->BitBlt(m_cortana_left_space, (m_dark_mode ? 0 : 1), m_cortana_rect.Width(), m_cortana_rect.Height(), &MemDC, 0, 0, SRCCOPY);
		MemBitmap.DeleteObject();
		MemDC.DeleteDC();
	}
}

void CCortanaLyric::DrawCortanaText(LPCTSTR str, int progress)
{
	if (m_enable && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		m_cortana_draw.SetFont(&m_cortana_font);
		//���û����DC
		CDC MemDC;
		CBitmap MemBitmap;
		MemDC.CreateCompatibleDC(NULL);
		MemBitmap.CreateCompatibleBitmap(m_cortana_pDC, m_cortana_rect.Width(), m_cortana_rect.Height());
		CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
		//ʹ��m_cortana_draw��ͼ
		m_cortana_draw.SetDC(&MemDC);
		MemDC.FillSolidRect(m_cortana_rect, m_back_color);
		if (m_dark_mode)
			m_cortana_draw.DrawWindowText(m_cortana_rect, str, m_colors.light3, m_colors.light1, progress, false, true);
		else
			m_cortana_draw.DrawWindowText(m_cortana_rect, str, m_colors.dark3, m_colors.dark1, progress, false, true);
		//��������DC�е�ͼ�񿽱�����Ļ����ʾ
		m_cortana_pDC->BitBlt(m_cortana_left_space, (m_dark_mode ? 0 : 1) , m_cortana_rect.Width(), m_cortana_rect.Height(), &MemDC, 0, 0, SRCCOPY);
		MemBitmap.DeleteObject();
		MemDC.DeleteDC();
	}
}

void CCortanaLyric::DrawLyricDoubleLine(LPCTSTR lyric, LPCTSTR next_lyric, int progress)
{
	if (m_enable && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		m_cortana_draw.SetFont(&m_font_double_line);
		static bool swap;
		static int last_progress;
		if (last_progress > progress)
		{
			swap = !swap;
		}
		last_progress = progress;

		//���û����DC
		CDC MemDC;
		CBitmap MemBitmap;
		MemDC.CreateCompatibleDC(NULL);
		MemBitmap.CreateCompatibleBitmap(m_cortana_pDC, m_cortana_rect.Width(), m_cortana_rect.Height());
		CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);

		//ʹ��m_cortana_draw��ͼ
		m_cortana_draw.SetDC(&MemDC);
		CRect up_rect{ m_cortana_rect }, down_rect{ m_cortana_rect };		//�ϰ벿�ֺ��°벿�ָ�ʵľ�������
		up_rect.bottom = up_rect.top + (up_rect.Height() / 2);
		down_rect.top = down_rect.bottom - (down_rect.Height() / 2);
		//������һ���ʵ��ı�������Ҫ�Ŀ�ȣ��Ӷ�ʵ����һ�и���Ҷ���
		MemDC.SelectObject(&m_font_double_line);
		int width;
		if (!swap)
			width = MemDC.GetTextExtent(next_lyric).cx;
		else
			width = MemDC.GetTextExtent(lyric).cx;
		if(width<m_cortana_rect.Width())
			down_rect.left = down_rect.right - width;

		COLORREF color1, color2;		//�Ѳ��Ÿ����ɫ��δ���Ÿ�ʵ���ɫ
		if (m_dark_mode)
		{
			color1 = m_colors.light3;
			color2 = m_colors.light1;
		}
		else
		{
			color1 = m_colors.dark3;
			color2 = m_colors.dark1;
		}
		MemDC.FillSolidRect(m_cortana_rect, m_back_color);
		if (!swap)
		{
			m_cortana_draw.DrawWindowText(up_rect, lyric, color1, color2, progress, false, true);
			m_cortana_draw.DrawWindowText(down_rect, next_lyric, color2, false, true);
		}
		else
		{
			m_cortana_draw.DrawWindowText(up_rect, next_lyric, color2, false, true);
			m_cortana_draw.DrawWindowText(down_rect, lyric, color1, color2, progress, false, true);
		}

		//��������DC�е�ͼ�񿽱�����Ļ����ʾ
		m_cortana_pDC->BitBlt(m_cortana_left_space, (m_dark_mode ? 0 : 1), m_cortana_rect.Width(), m_cortana_rect.Height(), &MemDC, 0, 0, SRCCOPY);
		MemBitmap.DeleteObject();
		MemDC.DeleteDC();
	}
}

void CCortanaLyric::ResetCortanaText()
{
	if (m_enable && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		m_cortana_draw.SetFont(&m_cortana_font);
		COLORREF color;
		color = (m_dark_mode ? GRAY(173) : GRAY(16));
		m_cortana_draw.SetDC(m_cortana_pDC);
		CRect rect{ m_cortana_rect };
		rect.MoveToXY(rect.left + m_cortana_left_space, (m_dark_mode ? 0 : 1));
		m_cortana_draw.FillRect(rect, m_back_color);
		m_cortana_draw.DrawWindowText(rect, m_cortana_default_text.c_str(), color, false);
		m_cortana_wnd->Invalidate();
	}
}

void CCortanaLyric::CheckDarkMode()
{
	if (m_enable)
	{
		HDC hDC = ::GetDC(NULL);
		COLORREF color;
		//��ȡCortana���Ͻǵ����ɫ
		color = ::GetPixel(hDC, m_lefttop_point.x, m_lefttop_point.y);
		m_dark_mode = (color % 256 < 220);

		//������ǳɫģʽ���ñ�����ɫ
		if (m_dark_mode)
		{
			DWORD dwStyle = GetWindowLong(m_hCortanaStatic, GWL_STYLE);
			if ((dwStyle & WS_VISIBLE) != 0)		//����Cortana��������static�ؼ��Ƿ���WS_VISIBLE����Ϊ��ͼ�������ò�ͬ�ı���ɫ
				m_back_color = GRAY(47);	//���û�ͼ�ı�����ɫ
			else
				m_back_color = GRAY(10);	//���û�ͼ�ı�����ɫ
		}
		else
		{
			m_back_color = GRAY(240);
		}
	}
}
