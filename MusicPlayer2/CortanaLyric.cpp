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

		::GetClientRect(m_cortana_hwnd, m_cortana_rect);	//��ȡCortana������ľ�������
		CRect cortana_static_rect;		//Cortana��������static�ؼ��ľ�������
		::GetClientRect(m_hCortanaStatic, cortana_static_rect);	//��ȡCortana��������static�ؼ��ľ�������
		//if (cortana_static_rect.Width() > 0)
		//{
			m_cortana_left_space = m_cortana_rect.Width() - cortana_static_rect.Width();
			m_cortana_rect.right = m_cortana_rect.left + cortana_static_rect.Width();		//����Cortana���ھ��εĿ��
		//}
		//else
		//{
		//	m_cortana_left_space = 0;
		//}

		if (m_cortana_left_space < m_cortana_rect.Height() / 2)		//��ʾ�ı��������С�ȴ������߿�̫������˵��С�ȱ����ã����û��С��ͼ��
		{
			m_cortana_disabled = true;
			m_cortana_rect.right -= (m_cortana_rect.Height() - m_cortana_left_space);
			m_cortana_left_space = m_cortana_rect.Height();
		}
		else
		{
			m_cortana_disabled = false;
		}
		m_icon_rect.right = m_cortana_left_space;
		m_icon_rect.bottom = m_cortana_rect.Height();

		m_cortana_pDC = m_cortana_wnd->GetDC();
		m_cortana_draw.Create(m_cortana_pDC, m_cortana_wnd);


		//��ȡ�������С���Ƿ�Ϊ��ɫģʽ�Ĳ����������
		CRect rect;
		::GetWindowRect(m_cortana_hwnd, rect);
		if (!m_cortana_disabled)
		{
			m_check_dark_point.x = rect.right + 3;
			m_check_dark_point.y = rect.top + 1;
		}
		else
		{
			m_check_dark_point.x = rect.right - 1;
			m_check_dark_point.y = rect.top + 1;
		}

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

void CCortanaLyric::SetCortanaColor(int color)
{
	m_cortana_color = color;
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
		m_cortana_draw.FillRect(m_cortana_rect, m_back_color);
		static CDrawCommon::ScrollInfo cortana_scroll_info;
		COLORREF color;
		color = (m_dark_mode ? m_colors.light3 : m_colors.dark2);
		CRect text_rect{ m_cortana_rect };
		text_rect.DeflateRect(DPI(4), 0);
		m_cortana_draw.DrawScrollText(text_rect, str, color, scroll_pixel, false, cortana_scroll_info, reset);
		//��������DC�е�ͼ�񿽱�����Ļ����ʾ
		CRect rect{ m_cortana_rect };
		rect.MoveToX(m_cortana_left_space);
		if (!m_dark_mode)		//����ɫģʽ�£��������������Ʊ߿�
			m_cortana_draw.DrawRectTopFrame(m_cortana_rect, m_border_color);
		CDrawCommon::SetDrawArea(m_cortana_pDC, rect);
		m_cortana_pDC->BitBlt(m_cortana_left_space, 0, m_cortana_rect.Width(), m_cortana_rect.Height(), &MemDC, 0, 0, SRCCOPY);
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
		m_cortana_draw.FillRect(m_cortana_rect, m_back_color);
		CRect text_rect{ m_cortana_rect };
		text_rect.DeflateRect(DPI(4), 0);
		if (m_dark_mode)
			m_cortana_draw.DrawWindowText(text_rect, str, m_colors.light3, m_colors.light1, progress, false);
		else
			m_cortana_draw.DrawWindowText(text_rect, str, m_colors.dark3, m_colors.dark1, progress, false);
		//��������DC�е�ͼ�񿽱�����Ļ����ʾ
		CRect rect{ m_cortana_rect };
		rect.MoveToX(m_cortana_left_space);
		if (!m_dark_mode)		//����ɫģʽ�£��������������Ʊ߿�
			m_cortana_draw.DrawRectTopFrame(m_cortana_rect, m_border_color);
		CDrawCommon::SetDrawArea(m_cortana_pDC, rect);
		m_cortana_pDC->BitBlt(m_cortana_left_space, 0 , m_cortana_rect.Width(), m_cortana_rect.Height(), &MemDC, 0, 0, SRCCOPY);
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
		CRect text_rect{ m_cortana_rect };
		text_rect.DeflateRect(DPI(4), DPI(2));
		CRect up_rect{ text_rect }, down_rect{ text_rect };		//�ϰ벿�ֺ��°벿�ָ�ʵľ�������
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
		m_cortana_draw.FillRect(m_cortana_rect, m_back_color);
		if (!swap)
		{
			m_cortana_draw.DrawWindowText(up_rect, lyric, color1, color2, progress, false);
			m_cortana_draw.DrawWindowText(down_rect, next_lyric, color2, false);
		}
		else
		{
			m_cortana_draw.DrawWindowText(up_rect, next_lyric, color2, false);
			m_cortana_draw.DrawWindowText(down_rect, lyric, color1, color2, progress, false);
		}

		//��������DC�е�ͼ�񿽱�����Ļ����ʾ
		CRect rect{ m_cortana_rect };
		rect.MoveToX(m_cortana_left_space);
		if (!m_dark_mode)		//����ɫģʽ�£��������������Ʊ߿�
			m_cortana_draw.DrawRectTopFrame(m_cortana_rect, m_border_color);
		CDrawCommon::SetDrawArea(m_cortana_pDC, rect);
		m_cortana_pDC->BitBlt(m_cortana_left_space, 0, m_cortana_rect.Width(), m_cortana_rect.Height(), &MemDC, 0, 0, SRCCOPY);
		MemBitmap.DeleteObject();
		MemDC.DeleteDC();
	}
}

void CCortanaLyric::DrawAlbumCover(const CImage & album_cover)
{
	if (m_enable && m_show_album_cover)
	{
		m_cortana_draw.SetDC(m_cortana_pDC);
		if (album_cover.IsNull())
		{
			if(m_dark_mode)
				m_cortana_draw.DrawBitmap(IDB_CORTANA_BLACK, m_icon_rect.TopLeft(), m_icon_rect.Size(), CDrawCommon::StretchMode::FILL);
			else
				m_cortana_draw.DrawBitmap(IDB_CORTANA_WHITE, m_icon_rect.TopLeft(), m_icon_rect.Size(), CDrawCommon::StretchMode::FILL);
		}
		else
			m_cortana_draw.DrawBitmap(album_cover, m_icon_rect.TopLeft(), m_icon_rect.Size(), CDrawCommon::StretchMode::FILL);
	}
}

void CCortanaLyric::ResetCortanaText()
{
	if (m_enable && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		m_cortana_draw.SetFont(&m_cortana_font);
		COLORREF color;		//CortanaĬ���ı�����ɫ
		color = (m_dark_mode ? GRAY(173) : GRAY(16));
		m_cortana_draw.SetDC(m_cortana_pDC);
		CRect rect{ m_cortana_rect };
		//�Ȼ���Cortanaͼ��
		if (m_dark_mode)
			m_cortana_draw.DrawBitmap(IDB_CORTANA_BLACK, m_icon_rect.TopLeft(), m_icon_rect.Size(), CDrawCommon::StretchMode::FILL);
		else
			m_cortana_draw.DrawBitmap(IDB_CORTANA_WHITE, m_icon_rect.TopLeft(), m_icon_rect.Size(), CDrawCommon::StretchMode::FILL);
		//�ٻ���CortanaĬ���ı�
		rect.MoveToXY(rect.left + m_cortana_left_space, 0);
		m_cortana_draw.FillRect(rect, m_back_color);
		m_cortana_draw.DrawWindowText(rect, m_cortana_default_text.c_str(), color, false);
		if (!m_dark_mode)
		{
			rect.left -= m_cortana_left_space;
			m_cortana_draw.DrawRectTopFrame(rect, m_border_color);
		}
		//m_cortana_wnd->Invalidate();
	}
}

void CCortanaLyric::CheckDarkMode()
{
	if (m_enable)
	{
		if (m_cortana_color == 1)
		{
			m_dark_mode = true;
		}
		else if (m_cortana_color == 2)
		{
			m_dark_mode = false;
		}
		else
		{
			HDC hDC = ::GetDC(NULL);
			COLORREF color;
			//��ȡCortana���Ͻǵ����ɫ
			color = ::GetPixel(hDC, m_check_dark_point.x, m_check_dark_point.y);
			int brightness;
			brightness = (GetRValue(color) + GetGValue(color) + GetBValue(color)) / 3;		//R��G��B��ƽ��ֵ
			m_dark_mode = (brightness < 220);
		}

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

void CCortanaLyric::AlbumCoverEnable(bool enable)
{
	bool last_enable;
	last_enable = m_show_album_cover;
	m_show_album_cover = enable;
	if (last_enable && !enable)
	{
		CDrawCommon::SetDrawArea(m_cortana_pDC, m_icon_rect);
		m_cortana_pDC->FillSolidRect(m_icon_rect, m_back_color);
	}
}
