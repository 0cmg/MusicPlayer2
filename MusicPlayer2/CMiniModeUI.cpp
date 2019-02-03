#include "stdafx.h"
#include "CMiniModeUI.h"
#include "PlayListCtrl.h"


CMiniModeUI::CMiniModeUI(SMiniModeUIData& ui_data, CWnd* pMiniModeWnd)
	: m_ui_data(ui_data), m_pMiniModeWnd(pMiniModeWnd)
{
}


CMiniModeUI::~CMiniModeUI()
{
}

void CMiniModeUI::SetToolTip(CToolTipCtrl * pToolTip)
{
	m_tool_tip = pToolTip;
}

bool CMiniModeUI::PointInControlArea(CPoint point) const
{
	bool point_in_control = false;
	for (const auto& btn : m_buttons)
	{
		if(btn.first!=BTN_COVER)
			point_in_control |= (btn.second.rect.PtInRect(point) != FALSE);
	}
	return point_in_control;
}

void CMiniModeUI::Init(CDC * pDC)
{
	m_pDC = pDC;
	m_draw.Create(m_pDC, theApp.m_pMainWnd);
	m_first_draw = true;
}

void CMiniModeUI::DrawInfo(bool reset)
{
	//������ɫ
	m_colors = CPlayerUIHelper::GetUIColors(theApp.m_app_setting_data.theme_color);

	//���û����DC
	CDC MemDC;
	CBitmap MemBitmap;
	MemDC.CreateCompatibleDC(NULL);

	CRect draw_rect(CPoint(0,0), CSize(m_ui_data.widnow_width, m_ui_data.window_height));

	MemBitmap.CreateCompatibleBitmap(m_pDC, m_ui_data.widnow_width, m_ui_data.window_height);
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
	m_draw.SetDC(&MemDC);	//��m_draw�еĻ�ͼDC����Ϊ�����DC
	m_draw.SetFont(theApp.m_pMainWnd->GetFont());

	//���Ʊ���
	if (theApp.m_app_setting_data.album_cover_as_background)
	{
		if (CPlayer::GetInstance().AlbumCoverExist())
		{
			CImage& back_image{ theApp.m_app_setting_data.background_gauss_blur ? CPlayer::GetInstance().GetAlbumCoverBlur() : CPlayer::GetInstance().GetAlbumCover() };
			m_draw.DrawBitmap(back_image, CPoint(0, 0), draw_rect.Size(), CDrawCommon::StretchMode::FILL);
		}
		else
		{
			m_draw.DrawBitmap(*m_ui_data.pDefaultBackground, CPoint(0, 0), draw_rect.Size(), CDrawCommon::StretchMode::FILL);
		}
	}

	//��䱳����ɫ
	bool draw_background{ theApp.m_app_setting_data.album_cover_as_background && (CPlayer::GetInstance().AlbumCoverExist() || !m_ui_data.pDefaultBackground->IsNull()) };		//�Ƿ���Ҫ����ͼƬ����
	if (draw_background)
		m_draw.FillAlphaRect(draw_rect, m_colors.color_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency));
	else
		m_draw.FillRect(draw_rect, m_colors.color_back);

	//����ר������
	int cover_side = m_ui_data.window_height - 2 * m_ui_data.margin;
	CRect cover_rect{CPoint(m_ui_data.margin, m_ui_data.margin), CSize(cover_side, cover_side)};
	if (theApp.m_app_setting_data.show_album_cover && CPlayer::GetInstance().AlbumCoverExist())
	{
		m_draw.DrawBitmap(CPlayer::GetInstance().GetAlbumCover(), cover_rect.TopLeft(), cover_rect.Size(), theApp.m_app_setting_data.album_cover_fit);
	}
	else
	{
		if (draw_background)
			m_draw.FillAlphaRect(cover_rect, m_colors.color_spectrum_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency) * 2 / 3);
		else
			m_draw.FillRect(cover_rect, m_colors.color_spectrum_back);
		cover_rect.DeflateRect(theApp.DPI(4), theApp.DPI(4));
		m_draw.DrawIcon(theApp.m_default_cover.GetIcon(), cover_rect.TopLeft(), cover_rect.Size());
	}
	m_buttons[BTN_COVER].rect = cover_rect;

	//���Ʋ��ſ��ư�ť
	CRect rc_tmp;
	rc_tmp.MoveToXY(m_ui_data.window_height, m_ui_data.margin);
	rc_tmp.right = rc_tmp.left + theApp.DPI(27);
	rc_tmp.bottom = rc_tmp.top + theApp.DPI(22);
	DrawUIButton(rc_tmp, m_buttons[BTN_PREVIOUS], theApp.m_previous_icon.GetIcon(), draw_background);

	rc_tmp.MoveToX(rc_tmp.right + m_ui_data.margin);
	if(CPlayer::GetInstance().IsPlaying())
		DrawUIButton(rc_tmp, m_buttons[BTN_PLAY_PAUSE], theApp.m_pause_icon.GetIcon(), draw_background);
	else
		DrawUIButton(rc_tmp, m_buttons[BTN_PLAY_PAUSE], theApp.m_play_icon.GetIcon(), draw_background);

	rc_tmp.MoveToX(rc_tmp.right + m_ui_data.margin);
	DrawUIButton(rc_tmp, m_buttons[BTN_NEXT], theApp.m_next_icon.GetIcon(), draw_background);


	//����Ƶ�׷���
	rc_tmp.MoveToX(rc_tmp.right + m_ui_data.margin);
	rc_tmp.right = rc_tmp.left + theApp.DPI(30);

	if (draw_background)
		m_draw.FillAlphaRect(rc_tmp, m_colors.color_spectrum_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency) * 2 / 3);
	else
		m_draw.FillRect(rc_tmp, m_colors.color_spectrum_back);

	static const int DATA_ROW{ 16 };			//Ƶ���������ݵ�����������Ϊ2�������η���С�ڻ����SPECTRUM_ROW��
	float spectral_data[DATA_ROW]{};
	float spectral_data_peak[DATA_ROW]{};
	CRect rects[DATA_ROW];		//ÿ�����εľ�������

	int width{ rc_tmp.Width()/9 };	//ÿ�����εĿ��
	int gap{ rc_tmp.Width() / 22 };	//���εļ��
	rects[0] = rc_tmp;
	rects[0].right = rects[0].left + width - gap;
	for (int i{ 1 }; i < DATA_ROW; i++)
	{
		rects[i] = rects[0];
		rects[i].left += (i * width);
		rects[i].right += (i * width);
	}

	for (int i{}; i < 64; i++)
	{
		spectral_data[i / (64 / DATA_ROW)] += CPlayer::GetInstance().GetSpectralData()[i];
		spectral_data_peak[i / (64 / DATA_ROW)] += CPlayer::GetInstance().GetSpectralPeakData()[i];
	}
	for (int i{}; i < DATA_ROW; i++)
	{
		CRect rect_tmp{ rects[i] };
		int spetral_height = static_cast<int>(spectral_data[i] * rc_tmp.Height() / 60 * theApp.m_app_setting_data.sprctrum_height / 100);
		int peak_height = static_cast<int>(spectral_data_peak[i] * rc_tmp.Height() / 60 * theApp.m_app_setting_data.sprctrum_height / 100);
		if (spetral_height <= 0 || CPlayer::GetInstance().IsError()) spetral_height = 1;		//Ƶ�׸߶�����Ϊ1�����أ�������ų���Ҳ����ʾƵ��
		rect_tmp.top = rect_tmp.bottom - spetral_height;
		if (rect_tmp.top < 0) rect_tmp.top = 0;
		m_draw.FillRect(rect_tmp, m_colors.color_spectrum, true);

		CRect rc_peak = rect_tmp;
		rc_peak.bottom = rect_tmp.bottom - peak_height - theApp.DPI(2);
		rc_peak.top = rc_peak.bottom - theApp.DPI(1);
		m_draw.FillRect(rc_peak, m_colors.color_spectrum, true);
	}

	//���Ʋ���ʱ��
	rc_tmp.MoveToX(rc_tmp.right + m_ui_data.margin);
	rc_tmp.right = m_ui_data.widnow_width - 3 * theApp.DPI(20) - 4 * m_ui_data.margin;
	rc_tmp.bottom = rc_tmp.top + theApp.DPI(16);
	CString str;
	if (m_ui_data.m_show_volume)
		str.Format(CCommon::LoadText(IDS_VOLUME, _T(": %d%%")), CPlayer::GetInstance().GetVolume());
	else if (CPlayer::GetInstance().IsError())
		str = CCommon::LoadText(IDS_PLAY_ERROR);
	else
		str = CPlayer::GetInstance().GetTimeString().c_str();
	m_draw.DrawWindowText(rc_tmp, str, m_colors.color_text, Alignment::CENTER);

	//���ƽ�����
	rc_tmp.MoveToY(rc_tmp.bottom);
	rc_tmp.bottom = rc_tmp.top + theApp.DPI(6);
	CRect progress_rect = rc_tmp;
	int progress_height = theApp.DPI(2);
	progress_rect.top = progress_rect.top + (rc_tmp.Height() - progress_height) / 2;
	progress_rect.bottom = progress_rect.top + progress_height;

	if (draw_background)
		m_draw.FillAlphaRect(progress_rect, m_colors.color_spectrum_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency) * 2 / 3);
	else
		m_draw.FillRect(progress_rect, m_colors.color_spectrum_back);

	m_buttons[BTN_PROGRESS].rect = progress_rect;
	m_buttons[BTN_PROGRESS].rect.InflateRect(0, theApp.DPI(2));

	double progress = static_cast<double>(CPlayer::GetInstance().GetCurrentPosition()) / CPlayer::GetInstance().GetSongLength();
	progress_rect.right = progress_rect.left + static_cast<int>(progress * progress_rect.Width());
	if (progress_rect.right > progress_rect.left)
		m_draw.FillRect(progress_rect, m_colors.color_spectrum);

	//�������Ͻǰ�ť
	rc_tmp.right = m_ui_data.widnow_width - m_ui_data.margin;
	rc_tmp.left = rc_tmp.right - theApp.DPI(20);;
	rc_tmp.top = m_ui_data.margin;
	rc_tmp.bottom = rc_tmp.top + theApp.DPI(20);
	DrawTextButton(rc_tmp, m_buttons[BTN_CLOSE], _T("��"), draw_background);

	rc_tmp.MoveToX(rc_tmp.left - rc_tmp.Width() - m_ui_data.margin);
	DrawTextButton(rc_tmp, m_buttons[BTN_RETURN], _T("��"), draw_background);

	rc_tmp.MoveToX(rc_tmp.left - rc_tmp.Width() - m_ui_data.margin);
	DrawTextButton(rc_tmp, m_buttons[BTN_PLAYLIST], _T("��"), draw_background);

	//������ʾ�ı���Ϣ
	rc_tmp.MoveToXY(m_ui_data.window_height, m_ui_data.margin + theApp.DPI(22));
	rc_tmp.right = m_ui_data.widnow_width - m_ui_data.margin;
	rc_tmp.bottom = m_ui_data.window_height;
	if (CPlayer::GetInstance().IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric && !CPlayer::GetInstance().MidiNoLyric())
	{
		wstring current_lyric{ CPlayer::GetInstance().GetMidiLyric() };
		m_draw.DrawWindowText(rc_tmp, current_lyric.c_str(), m_colors.color_text, Alignment::CENTER);
	}
	else if (CPlayer::GetInstance().m_Lyrics.IsEmpty())	//û�и��ʱ��ʾ���ŵ��ļ���
	{
		//���ڲ��ŵ��ļ����Թ�������ʽ��ʾ���������Ҫ��ǿ��ˢ�£������ù���λ��
		CDrawCommon::ScrollInfo scroll_info;
		m_draw.DrawScrollText(rc_tmp, CPlayListCtrl::GetDisplayStr(CPlayer::GetInstance().GetCurrentSongInfo(), *m_ui_data.pDisplayFormat).c_str(),
			m_colors.color_text, theApp.DPI(1), true, scroll_info, reset);
	}
	else		//��ʾ���
	{
		COLORREF color2 = (theApp.m_lyric_setting_data.lyric_karaoke_disp ? m_colors.color_text_2 : m_colors.color_text);
		wstring current_lyric{ CPlayer::GetInstance().m_Lyrics.GetLyric(Time(CPlayer::GetInstance().GetCurrentPosition()), 0).text };	//��ȡ�����
		int progress{ CPlayer::GetInstance().m_Lyrics.GetLyricProgress(Time(CPlayer::GetInstance().GetCurrentPosition())) };		//��ȡ��ǰ��ʽ��ȣ���ΧΪ0~1000��
		if (current_lyric.empty())		//�����ǰ���Ϊ�հף�����ʾΪʡ�Ժ�
			current_lyric = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
		m_draw.DrawWindowText(rc_tmp, current_lyric.c_str(), m_colors.color_text, color2, progress, true);
	}



	//��������DC�е�ͼ�񿽱�����Ļ����ʾ
	m_pDC->BitBlt(0, 0, m_ui_data.widnow_width, m_ui_data.window_height, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBit);
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();

	if (m_first_draw)
	{
		AddToolTips();
	}
	m_first_draw = false;
}

void CMiniModeUI::LButtonDown(CPoint point)
{
	for (auto& btn : m_buttons)
	{
		if (btn.second.rect.PtInRect(point) != FALSE)
			btn.second.pressed = true;
	}
}

void CMiniModeUI::RButtonUp(CPoint point)
{
}

void CMiniModeUI::MouseMove(CPoint point)
{
	for(auto& btn : m_buttons)
	{
		btn.second.hover = (btn.second.rect.PtInRect(point) != FALSE);
	}


	//���ָ�������ʱ��ʾ��λ�����ּ���
	if (m_buttons[BTN_PROGRESS].hover)
	{
		__int64 song_pos;
		song_pos = static_cast<__int64>(point.x - m_buttons[BTN_PROGRESS].rect.left) * CPlayer::GetInstance().GetSongLength() / m_buttons[BTN_PROGRESS].rect.Width();
		Time song_pos_time;
		song_pos_time.int2time(static_cast<int>(song_pos));
		CString str;
		static int last_sec{};
		if (last_sec != song_pos_time.sec)		//ֻ�����ָ��λ�ö�Ӧ�������仯�˲Ÿ��������ʾ
		{
			str.Format(CCommon::LoadText(IDS_SEEK_TO_MINUTE_SECOND), song_pos_time.min, song_pos_time.sec);
			UpdateMouseToolTip(BTN_PROGRESS, str);
			last_sec = song_pos_time.sec;
		}
	}

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_pMiniModeWnd->GetSafeHwnd();
	tme.dwFlags = TME_LEAVE | TME_HOVER;
	tme.dwHoverTime = 1;
	_TrackMouseEvent(&tme);

}

void CMiniModeUI::LButtonUp(CPoint point)
{
	for (auto& btn : m_buttons)
	{
		btn.second.hover = false;
		btn.second.pressed = false;
		
		if (btn.second.rect.PtInRect(point))
		{
			switch (btn.first)
			{
			case BTN_PREVIOUS:
				theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_PREVIOUS);
				break;
			case BTN_PLAY_PAUSE:
				theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_PLAY_PAUSE);
				break;
			case BTN_NEXT:
				theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_NEXT);
				break;
			case BTN_PLAYLIST:
				m_pMiniModeWnd->SendMessage(WM_COMMAND, ID_SHOW_PLAY_LIST);
				break;
			case BTN_RETURN:
				//m_buttons[BTN_RETURN].hover = false;
				m_pMiniModeWnd->SendMessage(WM_COMMAND, IDOK);
				break;
			case BTN_CLOSE:
				if (theApp.m_general_setting_data.minimize_to_notify_icon)
					m_pMiniModeWnd->ShowWindow(HIDE_WINDOW);
				else
					m_pMiniModeWnd->SendMessage(WM_COMMAND, ID_MINI_MODE_EXIT);
				break;
			default:
				break;
			case BTN_COVER:
				break;
			case BTN_PROGRESS:
			{
				int ckick_pos = point.x - btn.second.rect.left;
				double progress = static_cast<double>(ckick_pos) / btn.second.rect.Width();
				CPlayer::GetInstance().SeekTo(progress);
			}
				break;
			}

		}
	}
}

void CMiniModeUI::MouseLeave()
{
	for (auto& btn : m_buttons)
	{
		btn.second.hover = false;
	}
}

void CMiniModeUI::OnSizeRedraw(int cx, int cy)
{
}

bool CMiniModeUI::SetCursor()
{
	if (m_buttons[BTN_PROGRESS].hover)
	{
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(32649)));
		return true;
	}
	return false;
}

CRect CMiniModeUI::GetThumbnailClipArea()
{
	return CRect();
}

void CMiniModeUI::UpdateSongInfoTip(LPCTSTR str_tip)
{
	UpdateMouseToolTip(BTN_COVER, str_tip);
}

void CMiniModeUI::UpdatePlayPauseButtonTip()
{
	if (CPlayer::GetInstance().IsPlaying() && !CPlayer::GetInstance().IsError())
		UpdateMouseToolTip(BTN_PLAY_PAUSE, CCommon::LoadText(IDS_PAUSE));
	else
		UpdateMouseToolTip(BTN_PLAY_PAUSE, CCommon::LoadText(IDS_PLAY));
}

void CMiniModeUI::DrawUIButton(CRect rect, IPlayerUI::UIButton & btn, HICON icon, bool draw_background)
{
	CRect rc_tmp = rect;
	m_draw.SetDrawArea(rc_tmp);

	BYTE alpha;
	if (draw_background)
		alpha = ALPHA_CHG(theApp.m_app_setting_data.background_transparency);
	else
		alpha = 255;

	if (btn.pressed && btn.hover)
		m_draw.FillAlphaRect(rc_tmp, m_colors.color_button_pressed, alpha);
	else if (btn.hover)
		m_draw.FillAlphaRect(rc_tmp, m_colors.color_text_2, alpha);
	else
		m_draw.FillAlphaRect(rc_tmp, m_colors.color_button_back, alpha);

	btn.rect = rc_tmp;

	int icon_size = theApp.DPI(16);
	rc_tmp.left = rc_tmp.left + (rc_tmp.Width() - icon_size) / 2;
	rc_tmp.right = rc_tmp.left + icon_size;
	rc_tmp.top = rc_tmp.top + (rc_tmp.Height() - icon_size) / 2;
	rc_tmp.bottom = rc_tmp.top + icon_size;
	m_draw.DrawIcon(icon, rc_tmp.TopLeft(), rc_tmp.Size());
}

void CMiniModeUI::DrawTextButton(CRect rect, IPlayerUI::UIButton & btn, LPCTSTR text, bool draw_background)
{
	m_draw.SetDrawArea(rect);

	BYTE alpha;
	if (draw_background)
		alpha = ALPHA_CHG(theApp.m_app_setting_data.background_transparency);
	else
		alpha = 255;
	if (btn.pressed && btn.hover)
		m_draw.FillAlphaRect(rect, m_colors.color_button_pressed, alpha);
	else if (btn.hover)
		m_draw.FillAlphaRect(rect, m_colors.color_text_2, alpha);
	else
		m_draw.FillAlphaRect(rect, m_colors.color_button_back, alpha);

	btn.rect = rect;
	m_draw.DrawWindowText(rect, text, m_colors.color_text, Alignment::CENTER);
}

void CMiniModeUI::AddMouseToolTip(BtnKey btn, LPCTSTR str)
{
	m_tool_tip->AddTool(m_pMiniModeWnd, str, m_buttons[btn].rect, btn + 1);
}

void CMiniModeUI::UpdateMouseToolTip(BtnKey btn, LPCTSTR str)
{
	//if (m_buttons[btn].hover)
	//{
		m_tool_tip->UpdateTipText(str, m_pMiniModeWnd, btn + 1);
	//}
}

void CMiniModeUI::AddToolTips()
{
	AddMouseToolTip(BTN_PREVIOUS, CCommon::LoadText(IDS_PREVIOUS));
	AddMouseToolTip(BTN_PLAY_PAUSE, CPlayer::GetInstance().IsPlaying() ? CCommon::LoadText(IDS_PAUSE) : CCommon::LoadText(IDS_PLAY));
	AddMouseToolTip(BTN_NEXT, CCommon::LoadText(IDS_NEXT));
	AddMouseToolTip(BTN_PLAYLIST, CCommon::LoadText(IDS_SHOW_HIDE_PLAYLIST));
	AddMouseToolTip(BTN_RETURN, CCommon::LoadText(IDS_BACK_TO_NARMAL));
	AddMouseToolTip(BTN_CLOSE, CCommon::LoadText(IDS_CLOSE));
	AddMouseToolTip(BTN_COVER, _T(""));
	AddMouseToolTip(BTN_PROGRESS, CCommon::LoadText(IDS_SEEK_TO));
}
