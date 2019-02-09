#include "stdafx.h"
#include "CortanaLyric.h"
#include "PlayListCtrl.h"
#include "CPlayerUIBase.h"


CCortanaLyric::CCortanaLyric()
{
}


CCortanaLyric::~CCortanaLyric()
{
	if (m_pDC != nullptr)
		m_cortana_wnd->ReleaseDC(m_pDC);
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

		m_cover_width = m_cortana_rect.Width() - cortana_static_rect.Width();
		m_cortana_disabled = m_cover_width < m_cortana_rect.Height() / 2;
		if (m_cortana_disabled)
			m_cover_width = m_cortana_rect.Height();

		m_pDC = m_cortana_wnd->GetDC();
		m_draw.Create(m_pDC, m_cortana_wnd);


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
		if (m_font_double_line.m_hObject)
			m_font_double_line.DeleteObject();
		m_font_double_line.CreatePointFont(110, lf.lfFaceName);
		if (m_font_translate.m_hObject)
			m_font_translate.DeleteObject();
		m_font_translate.CreatePointFont(100, lf.lfFaceName);
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

void CCortanaLyric::DrawInfo()
{
	if (!m_enable)
		return;

	m_draw.SetFont(&m_cortana_font);
	//���û����DC
	CDC MemDC;
	CBitmap MemBitmap;
	MemDC.CreateCompatibleDC(NULL);
	MemBitmap.CreateCompatibleBitmap(m_pDC, m_cortana_rect.Width(), m_cortana_rect.Height());
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
	//ʹ��m_draw��ͼ
	m_draw.SetDC(&MemDC);
	m_draw.FillRect(m_cortana_rect, m_back_color);

	bool is_midi_lyric = CPlayerUIBase::IsMidiLyric();
	if(!theApp.m_lyric_setting_data.cortana_lyric_compatible_mode)
	{
		if (is_midi_lyric)
		{
			wstring current_lyric{ CPlayer::GetInstance().GetMidiLyric() };
			DrawCortanaTextSimple(current_lyric.c_str(), Alignment::LEFT);
		}
		else if (!CPlayer::GetInstance().m_Lyrics.IsEmpty())		//�и��ʱ��ʾ���
		{
			Time time{ CPlayer::GetInstance().GetCurrentPosition() };
			CLyrics::Lyric lyric = CPlayer::GetInstance().m_Lyrics.GetLyric(time, 0);
			int progress = CPlayer::GetInstance().m_Lyrics.GetLyricProgress(time);
			if (CPlayer::GetInstance().m_Lyrics.IsTranslated() && theApp.m_ui_data.show_translate)
			{
				if (lyric.text.empty()) lyric.text = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
				if (lyric.translate.empty()) lyric.translate = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
				DrawLyricWithTranslate(lyric.text.c_str(), lyric.translate.c_str(), progress);
			}
			else if (!theApp.m_lyric_setting_data.cortana_lyric_double_line)
			{
				if (lyric.text.empty()) lyric.text = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
				DrawCortanaText(lyric.text.c_str(), progress);
			}
			else
			{
				wstring next_lyric = CPlayer::GetInstance().m_Lyrics.GetLyric(time, 1).text;
				if (lyric.text.empty()) lyric.text = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT_CORTANA);
				if (next_lyric.empty()) next_lyric = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT_CORTANA);
				DrawLyricDoubleLine(lyric.text.c_str(), next_lyric.c_str(), progress);
			}
		}
		else
		{
			//û�и��ʱ��Cortana���������Թ����ķ�ʽ��ʾ��ǰ���Ÿ������ļ���
			static int index{};
			static wstring song_name{};
			//�����ǰ���ŵĸ��������仯��DrawCortanaText�����ĵ�2����Ϊtrue�������ù���λ��
			if (index != CPlayer::GetInstance().GetIndex() || song_name != CPlayer::GetInstance().GetFileName())
			{
				DrawCortanaText((CCommon::LoadText(IDS_NOW_PLAYING, _T(": ")) + CPlayListCtrl::GetDisplayStr(CPlayer::GetInstance().GetCurrentSongInfo(), theApp.m_ui_data.display_format).c_str()), true, theApp.DPI(2));
				index = CPlayer::GetInstance().GetIndex();
				song_name = CPlayer::GetInstance().GetFileName();
			}
			else
			{
				DrawCortanaText((CCommon::LoadText(IDS_NOW_PLAYING, _T(": ")) + CPlayListCtrl::GetDisplayStr(CPlayer::GetInstance().GetCurrentSongInfo(), theApp.m_ui_data.display_format).c_str()), false, theApp.DPI(2));
			}
		}
		//}

		//����Ƶ�ף�����Ƶ�׷�ֵʹCortanaͼ����ʾ��̬Ч��
		float spectrum_avr{};		//ȡǰ��N��Ƶ��Ƶ��ֵ��ƽ��ֵ
		const int N = 8;
		for (int i{}; i < N; i++)
			spectrum_avr += CPlayer::GetInstance().GetFFTData()[i];
		spectrum_avr /= N;
		int spetraum = static_cast<int>(spectrum_avr * 4000);		//�����˺ź������ֵ���Ե���Cortanaͼ������ʱ���ŵĴ�С
		SetSpectrum(spetraum);
		//��ʾר�����棬���û��ר�����棬����ʾCortanaͼ��
		AlbumCoverEnable(theApp.m_lyric_setting_data.cortana_show_album_cover/* && CPlayer::GetInstance().AlbumCoverExist()*/);
		DrawAlbumCover(CPlayer::GetInstance().GetAlbumCover());
	
		//��������DC�е�ͼ�񿽱�����Ļ����ʾ
		if (!m_dark_mode)		//����ɫģʽ�£��������������Ʊ߿�
		{
			CRect rect{ m_cortana_rect };
			rect.left += m_cover_width;
			m_draw.DrawRectTopFrame(rect, m_border_color);
		}
		CDrawCommon::SetDrawArea(m_pDC, m_cortana_rect);
		m_pDC->BitBlt(0, 0, m_cortana_rect.Width(), m_cortana_rect.Height(), &MemDC, 0, 0, SRCCOPY);
		MemBitmap.DeleteObject();
		MemDC.DeleteDC();
	}

	else
	{
		CWnd* pWnd = CWnd::FromHandle(m_hCortanaStatic);
		if (pWnd != nullptr)
		{
			static wstring str_disp_last;
			wstring str_disp;
			if (is_midi_lyric)
			{
				str_disp = CPlayer::GetInstance().GetMidiLyric();
			}
			else if (!CPlayer::GetInstance().m_Lyrics.IsEmpty())		//�и��ʱ��ʾ���
			{
				Time time{ CPlayer::GetInstance().GetCurrentPosition() };
				str_disp = CPlayer::GetInstance().m_Lyrics.GetLyric(time, 0).text;
			}
			else
			{
				//û�и��ʱ��ʾ��ǰ���Ÿ���������
				str_disp = CCommon::LoadText(IDS_NOW_PLAYING, _T(": ")).GetString() + CPlayListCtrl::GetDisplayStr(CPlayer::GetInstance().GetCurrentSongInfo(), theApp.m_ui_data.display_format);
			}

			if(str_disp != str_disp_last)
			{
				pWnd->SetWindowText(str_disp.c_str());
				pWnd->Invalidate();
				str_disp_last = str_disp;
			}
		}
	}
}

void CCortanaLyric::DrawCortanaTextSimple(LPCTSTR str, Alignment align)
{
	if (m_enable && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		COLORREF color;
		color = (m_dark_mode ? m_colors.light3 : m_colors.dark2);
		CRect text_rect{ TextRect() };
		m_draw.DrawWindowText(text_rect, str, color, align, false, false, true);
	}
}

void CCortanaLyric::DrawCortanaText(LPCTSTR str, bool reset, int scroll_pixel)
{
	if (m_enable && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		static CDrawCommon::ScrollInfo cortana_scroll_info;
		COLORREF color;
		color = (m_dark_mode ? m_colors.light3 : m_colors.dark2);
		CRect text_rect{ TextRect() };
		m_draw.DrawScrollText(text_rect, str, color, scroll_pixel, false, cortana_scroll_info, reset);
	}
}

void CCortanaLyric::DrawCortanaText(LPCTSTR str, int progress)
{
	if (m_enable && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		CRect text_rect{ TextRect() };
		if (m_dark_mode)
			m_draw.DrawWindowText(text_rect, str, m_colors.light3, m_colors.light1, progress, false);
		else
			m_draw.DrawWindowText(text_rect, str, m_colors.dark3, m_colors.dark1, progress, false);
	}
}

void CCortanaLyric::DrawLyricDoubleLine(LPCTSTR lyric, LPCTSTR next_lyric, int progress)
{
	if (m_enable && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		m_draw.SetFont(&m_font_double_line);
		static bool swap;
		static int last_progress;
		if (last_progress > progress)
		{
			swap = !swap;
		}
		last_progress = progress;

		CRect text_rect{ TextRect() };
		CRect up_rect{ text_rect }, down_rect{ text_rect };		//�ϰ벿�ֺ��°벿�ָ�ʵľ�������
		up_rect.bottom = up_rect.top + (up_rect.Height() / 2);
		down_rect.top = down_rect.bottom - (down_rect.Height() / 2);
		//������һ���ʵ��ı�������Ҫ�Ŀ�ȣ��Ӷ�ʵ����һ�и���Ҷ���
		//MemDC.SelectObject(&m_font_double_line);
		int width;
		if (!swap)
			width = m_draw.GetDC()->GetTextExtent(next_lyric).cx;
		else
			width = m_draw.GetDC()->GetTextExtent(lyric).cx;
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
		m_draw.FillRect(m_cortana_rect, m_back_color);
		if (!swap)
		{
			m_draw.DrawWindowText(up_rect, lyric, color1, color2, progress, false);
			m_draw.DrawWindowText(down_rect, next_lyric, color2);
		}
		else
		{
			m_draw.DrawWindowText(up_rect, next_lyric, color2);
			m_draw.DrawWindowText(down_rect, lyric, color1, color2, progress, false);
		}
	}
}

void CCortanaLyric::DrawLyricWithTranslate(LPCTSTR lyric, LPCTSTR translate, int progress)
{
	if (m_enable && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		CRect text_rect{ TextRect() };
		CRect up_rect{ text_rect }, down_rect{ text_rect };		//�ϰ벿�ֺ��°벿�ָ�ʵľ�������
		up_rect.bottom = up_rect.top + (up_rect.Height() / 2);
		down_rect.top = down_rect.bottom - (down_rect.Height() / 2);

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
		m_draw.FillRect(m_cortana_rect, m_back_color);
		m_draw.SetFont(&m_cortana_font);
		m_draw.DrawWindowText(up_rect, lyric, color1, color2, progress, false);
		m_draw.SetFont(&m_font_translate);
		m_draw.DrawWindowText(down_rect, translate, color1, color1, progress, false);
	}
}

void CCortanaLyric::DrawAlbumCover(const CImage & album_cover)
{
	if (m_enable)
	{
		CRect cover_rect = CoverRect();
		m_draw.SetDrawArea(cover_rect);
		if (album_cover.IsNull() || !m_show_album_cover)
		{
			int cortana_img_id{ m_dark_mode ? IDB_CORTANA_BLACK : IDB_CORTANA_WHITE };
			if (*m_cortana_icon_beat)
			{
				m_draw.FillRect(cover_rect, (m_dark_mode ? GRAY(47) : GRAY(240)));
				CRect rect{ cover_rect };
				rect.DeflateRect(theApp.DPI(4), theApp.DPI(4));
				int inflate;
				inflate = m_spectrum * theApp.DPI(14) / 1000;
				rect.InflateRect(inflate, inflate);
				m_draw.DrawBitmap(cortana_img_id, rect.TopLeft(), rect.Size(), CDrawCommon::StretchMode::FIT);
			}
			else
			{
				m_draw.DrawBitmap(cortana_img_id, cover_rect.TopLeft(), cover_rect.Size(), CDrawCommon::StretchMode::FIT);
			}

			if(!m_dark_mode)
				m_draw.DrawRectTopFrame(cover_rect, m_border_color);
		}
		else
		{
			m_draw.DrawBitmap(album_cover, cover_rect.TopLeft(), cover_rect.Size(), CDrawCommon::StretchMode::FILL);
		}
	}
}

CRect CCortanaLyric::TextRect() const
{
	CRect text_rect{ m_cortana_rect };
	text_rect.left += m_cover_width;
	text_rect.DeflateRect(theApp.DPI(4), theApp.DPI(2));
	text_rect.top -= theApp.DPI(1);
	return text_rect;
}

CRect CCortanaLyric::CoverRect() const
{
	CRect cover_rect = m_cortana_rect;
	cover_rect.right = cover_rect.left + m_cover_width;
	return cover_rect;
}

void CCortanaLyric::ResetCortanaText()
{
	if (m_enable && m_cortana_hwnd != NULL && m_cortana_wnd != nullptr)
	{
		if (!theApp.m_lyric_setting_data.cortana_lyric_compatible_mode)
		{
			m_draw.SetFont(&m_cortana_font);
			COLORREF color;		//CortanaĬ���ı�����ɫ
			color = (m_dark_mode ? GRAY(173) : GRAY(16));
			m_draw.SetDC(m_pDC);
			//�Ȼ���Cortanaͼ��
			CRect cover_rect = CoverRect();
			if (m_dark_mode)
				m_draw.DrawBitmap(IDB_CORTANA_BLACK, cover_rect.TopLeft(), cover_rect.Size(), CDrawCommon::StretchMode::FILL);
			else
				m_draw.DrawBitmap(IDB_CORTANA_WHITE, cover_rect.TopLeft(), cover_rect.Size(), CDrawCommon::StretchMode::FILL);
			//�ٻ���CortanaĬ���ı�
			CRect rect{ m_cortana_rect };
			rect.MoveToXY(rect.left + m_cover_width, 0);
			m_draw.FillRect(rect, m_back_color);
			m_draw.DrawWindowText(rect, m_cortana_default_text.c_str(), color);
			if (!m_dark_mode)
			{
				rect.left -= m_cover_width;
				m_draw.DrawRectTopFrame(rect, m_border_color);
			}
			//m_cortana_wnd->Invalidate();
		}

		CWnd* pWnd = CWnd::FromHandle(m_hCortanaStatic);
		if (pWnd != nullptr)
		{
			CString str;
			pWnd->GetWindowText(str);
			if(str!=m_cortana_default_text.c_str())
			{
				pWnd->SetWindowText(m_cortana_default_text.c_str());
				pWnd->Invalidate();
			}
		}

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
			::ReleaseDC(NULL, hDC);
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
		CRect cover_rect = CoverRect();
		CDrawCommon::SetDrawArea(m_pDC, cover_rect);
		m_pDC->FillSolidRect(cover_rect, m_back_color);
	}
}

void CCortanaLyric::SetSpectrum(int spectrum)
{
	m_spectrum = spectrum;
	if (m_spectrum < 0) m_spectrum = 0;
	if (m_spectrum > 2000) m_spectrum = 2000;
}

void CCortanaLyric::SetCortanaIconBeat(bool * beat)
{
	m_cortana_icon_beat = beat;
}
