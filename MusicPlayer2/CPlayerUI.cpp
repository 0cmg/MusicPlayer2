#include "stdafx.h"
#include "CPlayerUI.h"


CPlayerUI::CPlayerUI(UIData& ui_data)
	: CPlayerUIBase(ui_data)
{
}


CPlayerUI::~CPlayerUI()
{
}

void CPlayerUI::DrawInfo(bool reset)
{
	//���û���ĺ����������û�ͼ��ɫ
	PreDrawInfo();

	//���û����DC
	CDC MemDC;
	CBitmap MemBitmap;
	MemDC.CreateCompatibleDC(NULL);
	CRect draw_rect{ m_draw_rect };
	draw_rect.MoveToXY(0, 0);
	MemBitmap.CreateCompatibleBitmap(m_pDC, m_draw_rect.Width(), m_draw_rect.Height());
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
	m_draw.SetDC(&MemDC);	//��m_draw�еĻ�ͼDC����Ϊ�����DC
	if (theApp.m_app_setting_data.album_cover_as_background)
	{
		if (CPlayer::GetInstance().AlbumCoverExist())
		{
			CImage& back_image{ theApp.m_app_setting_data.background_gauss_blur ? CPlayer::GetInstance().GetAlbumCoverBlur() : CPlayer::GetInstance().GetAlbumCover() };
			m_draw.DrawBitmap(back_image, CPoint(0, 0), m_draw_rect.Size(), CDrawCommon::StretchMode::FILL);
		}
		else
		{
			//MemDC.FillSolidRect(0, 0, m_draw_rect.Width(), m_draw_rect.Height(), GetSysColor(COLOR_BTNFACE));	//������DC�Ļ�ͼ�������Ի���ı�����ɫ
			m_draw.DrawBitmap(m_ui_data.default_background, CPoint(0, 0), m_draw_rect.Size(), CDrawCommon::StretchMode::FILL);
		}
	}
	else
	{
		m_draw.FillRect(draw_rect, m_colors.color_back);
	}

	//��䱳����ɫ
	//CDrawCommon::SetDrawArea(&MemDC, draw_rect);
	bool draw_background{ theApp.m_app_setting_data.album_cover_as_background && (CPlayer::GetInstance().AlbumCoverExist() || !m_ui_data.default_background.IsNull()) };		//�Ƿ���Ҫ����ͼƬ����
	if (draw_background)
		m_draw.FillAlphaRect(draw_rect, m_colors.color_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency));
	else
		m_draw.FillRect(draw_rect, m_colors.color_back);

	CPoint text_start{ draw_rect.left + m_pLayout->spectral_size.cx + 2 * m_pLayout->margin, draw_rect.top + m_pLayout->margin };		//�ı�����ʼ����
	int text_height{ theApp.DPI(18) };		//�ı��ĸ߶�

	//��ʾ������Ϣ
	m_draw.SetFont(theApp.m_pMainWnd->GetFont());
	//m_draw.SetBackColor(color_back);
	CRect tmp{ text_start, CSize{1,text_height} };
	tmp.right = draw_rect.right - m_pLayout->margin;
	DrawSongInfo(tmp, reset);

	wchar_t buff[64];
	wstring lable1_str, lable1_content;
	wstring lable2_str, lable2_content;
	wstring lable3_str, lable3_content;
	wstring lable4_str, lable4_content;
	lable1_str = CCommon::LoadText(IDS_TITLE, _T(": "));
	lable1_content = CPlayer::GetInstance().GetCurrentSongInfo().title;
	if (CPlayer::GetInstance().IsMidi())
	{
		const MidiInfo& midi_info{ CPlayer::GetInstance().GetMidiInfo() };
		lable2_str = CCommon::LoadText(IDS_RHYTHM, _T(": "));
		swprintf_s(buff, L"%d/%d (%dbpm)", midi_info.midi_position, midi_info.midi_length, midi_info.speed);
		lable2_content = buff;

		//lable3_str = _T("�ٶȣ�");
		//swprintf_s(buff, L"%d bpm", midi_info.speed);
		//lable3_content = buff;

		lable3_str = CCommon::LoadText(IDS_SOUND_FONT, _T(": ")) ;
		lable3_content = CPlayer::GetInstance().GetSoundFontName();
	}
	else
	{
		lable2_str = CCommon::LoadText(IDS_ARTIST, _T(": "));
		lable2_content = CPlayer::GetInstance().GetCurrentSongInfo().artist;
		lable3_str = CCommon::LoadText(IDS_ALBUM, _T(": "));
		lable3_content = CPlayer::GetInstance().GetCurrentSongInfo().album;
	}
	lable4_str = CCommon::LoadText(IDS_FORMAT, _T(": "));
	const BASS_CHANNELINFO channel_info{ CPlayer::GetInstance().GetChannelInfo() };
	CString chans_str;
	if (channel_info.chans == 1)
		chans_str = CCommon::LoadText(IDS_MONO);
	else if (channel_info.chans == 2)
		chans_str = CCommon::LoadText(IDS_STEREO);
	else if (channel_info.chans > 2)
		chans_str.Format(CCommon::LoadText(_T("%d "), IDS_CHANNEL), channel_info.chans);
	if (!CPlayer::GetInstance().IsMidi())
		swprintf_s(buff, L"%s %.1fkHz %dkbps %s", CPlayer::GetInstance().GetCurrentFileType().c_str(), channel_info.freq / 1000.0f, CPlayer::GetInstance().GetCurrentSongInfo().bitrate, chans_str.GetString());
	else
		swprintf_s(buff, L"%s %.1fkHz %s", CPlayer::GetInstance().GetCurrentFileType().c_str(), channel_info.freq / 1000.0f, chans_str.GetString());
	lable4_content = buff;
	//��ʾ����
	tmp.MoveToXY(text_start.x, text_start.y + text_height);
	tmp.right = tmp.left + theApp.DPI(52);
	m_draw.DrawWindowText(tmp, lable1_str.c_str(), m_colors.color_text_lable);
	tmp.MoveToX(tmp.left + theApp.DPI(52));
	tmp.right = draw_rect.right - m_pLayout->margin;
	static CDrawCommon::ScrollInfo scroll_info2;
	m_draw.DrawScrollText2(tmp, lable1_content.c_str(), m_colors.color_text, theApp.DPI(1), false, scroll_info2, reset);
	//��ʾ������
	tmp.MoveToXY(text_start.x, text_start.y + 2 * text_height);
	tmp.right = tmp.left + theApp.DPI(52);
	m_draw.DrawWindowText(tmp, lable2_str.c_str(), m_colors.color_text_lable);
	tmp.MoveToX(tmp.left + theApp.DPI(52));
	tmp.right = draw_rect.right - m_pLayout->margin;
	static CDrawCommon::ScrollInfo scroll_info3;
	if (CPlayer::GetInstance().IsMidi())
		m_draw.DrawWindowText(tmp, lable2_content.c_str(), m_colors.color_text);
	else
		m_draw.DrawScrollText2(tmp, lable2_content.c_str(), m_colors.color_text, theApp.DPI(1), false, scroll_info3, reset);
	//��ʾ��Ƭ��
	tmp.MoveToXY(text_start.x, text_start.y + 3 * text_height);
	tmp.right = tmp.left + theApp.DPI(52);
	m_draw.DrawWindowText(tmp, lable3_str.c_str(), m_colors.color_text_lable);
	tmp.MoveToX(tmp.left + theApp.DPI(52));
	tmp.right = draw_rect.right - m_pLayout->margin;
	static CDrawCommon::ScrollInfo scroll_info4;
	m_draw.DrawScrollText2(tmp, lable3_content.c_str(), m_colors.color_text, theApp.DPI(1), false, scroll_info4, reset);
	//��ʾ�ļ���ʽ�ͱ�����
	tmp.MoveToXY(text_start.x, text_start.y + 4 * text_height);
	tmp.right = tmp.left + theApp.DPI(52);
	m_draw.DrawWindowText(tmp, lable4_str.c_str(), m_colors.color_text_lable);
	tmp.MoveToX(tmp.left + theApp.DPI(52));
	tmp.right = draw_rect.right - m_pLayout->margin;
	static CDrawCommon::ScrollInfo scroll_info5;
	m_draw.DrawScrollText2(tmp, lable4_content.c_str(), m_colors.color_text, theApp.DPI(1), false, scroll_info5, reset);

	//��ʾƵ�׷���
	CRect spectral_rect{ CPoint{draw_rect.left + m_pLayout->margin, draw_rect.top + m_pLayout->margin}, m_pLayout->spectral_size };
	//���Ʊ���
	if (draw_background)
		m_draw.FillAlphaRect(spectral_rect, m_colors.color_spectrum_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency) * 2 / 3);
	else
		m_draw.FillRect(spectral_rect, m_colors.color_spectrum_back);
	if (theApp.m_app_setting_data.show_album_cover)
	{
		//����ר������
		m_draw_data.cover_rect = spectral_rect;
		m_draw_data.cover_rect.DeflateRect(m_pLayout->margin / 2, m_pLayout->margin / 2);
		if (CPlayer::GetInstance().AlbumCoverExist())
		{
			m_draw.DrawBitmap(CPlayer::GetInstance().GetAlbumCover(), m_draw_data.cover_rect.TopLeft(), m_draw_data.cover_rect.Size(), theApp.m_app_setting_data.album_cover_fit);
		}
		else
		{
			int cover_side = m_draw_data.cover_rect.Height() * 3 / 4;
			int x = m_draw_data.cover_rect.left + (m_draw_data.cover_rect.Width() - cover_side) / 2;
			int y = m_draw_data.cover_rect.top + (m_draw_data.cover_rect.Height() - cover_side) / 2;
			::DrawIconEx(m_draw.GetDC()->GetSafeHdc(), x, y, theApp.m_default_cover.GetIcon(), cover_side, cover_side, 0, NULL, DI_NORMAL);
		}
	}

	if (theApp.m_app_setting_data.show_spectrum)
	{
		const int ROWS = 31;		//Ҫ��ʾ��Ƶ�����ε�����
		int gap_width{ theApp.DPIRound(1, 0.4) };		//Ƶ�����μ�϶���
		CRect rects[ROWS];
		int width = (spectral_rect.Width() - (ROWS - 1)*gap_width) / (ROWS - 1);
		rects[0] = spectral_rect;
		rects[0].DeflateRect(m_pLayout->margin / 2, m_pLayout->margin / 2);
		rects[0].right = rects[0].left + width;
		for (int i{ 1 }; i < ROWS; i++)
		{
			rects[i] = rects[0];
			rects[i].left += (i * (width + gap_width));
			rects[i].right += (i * (width + gap_width));
		}
		for (int i{}; i < ROWS; i++)
		{
			float spetral_data = (CPlayer::GetInstance().GetSpectralData()[i * 2] + CPlayer::GetInstance().GetSpectralData()[i * 2 + 1]) / 2;
			float peak_data = (CPlayer::GetInstance().GetSpectralPeakData()[i * 2] + CPlayer::GetInstance().GetSpectralPeakData()[i * 2 + 1]) / 2;

			CRect rect_tmp{ rects[i] };
			int spetral_height = static_cast<int>(spetral_data * rects[0].Height() / 30 * theApp.m_app_setting_data.sprctrum_height / 100);
			int peak_height = static_cast<int>(peak_data * rects[0].Height() / 30 * theApp.m_app_setting_data.sprctrum_height / 100);
			if (spetral_height <= 0 || CPlayer::GetInstance().IsError()) spetral_height = 1;		//Ƶ�׸߶�����Ϊ1�����أ�������ų���Ҳ����ʾƵ��
			if (peak_height <= 0 || CPlayer::GetInstance().IsError()) peak_height = 1;		//Ƶ�׸߶�����Ϊ1�����أ�������ų���Ҳ����ʾƵ��
			rect_tmp.top = rect_tmp.bottom - spetral_height;
			if (rect_tmp.top < rects[0].top) rect_tmp.top = rects[0].top;
			COLORREF color;
			if (theApp.m_app_setting_data.show_album_cover && CPlayer::GetInstance().AlbumCoverExist())
				color = m_colors.color_spectrum_cover;
			else
				color = m_colors.color_spectrum;
			MemDC.FillSolidRect(rect_tmp, color);

			CRect rect_peak{ rect_tmp };
			rect_peak.bottom = rect_tmp.bottom - peak_height - theApp.DPIRound(1.1);
			rect_peak.top = rect_peak.bottom - theApp.DPIRound(1.1);
			//if (peak_height > 1)
			MemDC.FillSolidRect(rect_peak, color);
		}
	}

	//���ƹ�����
	CPoint point{ spectral_rect.left, spectral_rect.bottom };
	point.y += 2 * m_pLayout->margin;
	CRect other_info_rect{ point, CSize(draw_rect.Width() - 2 * m_pLayout->margin,theApp.DPI(24)) };
	DrawToolBar(draw_background, other_info_rect, false, &m_ui_data);

	//��ʾ���
	m_draw.SetFont(&m_ui_data.lyric_font);
	CRect lyric_rect;
	if (DrawNarrowMode())
	{
		lyric_rect = other_info_rect;
		lyric_rect.MoveToY(other_info_rect.bottom + m_pLayout->margin);
		DrawLyricsSingleLine(lyric_rect);
	}
	else
	{
		//if (CPlayer::GetInstance().IsPlaying() || reset)
		//{
		lyric_rect = draw_rect;
		lyric_rect.MoveToY(other_info_rect.bottom + m_pLayout->margin + theApp.DPI(36));
		lyric_rect.bottom = m_draw_rect.Height()/* - m_pLayout->margin*/;
		DrawLyricsMulityLine(lyric_rect, &MemDC);
		//}
	}
	m_draw_data.lyric_rect = lyric_rect;
	m_draw_data.thumbnail_rect = draw_rect;
	if (!DrawNarrowMode())
	{
		m_draw_data.thumbnail_rect.bottom = lyric_rect.top;
	}

	//������������ť����Ϊ�������ϲ㣬���Ա����ڸ�ʻ�����ɺ����
	DrawVolumnAdjBtn(draw_background);

	//���Ʋ��ſ�����
	CRect rc_control_bar;
	if (DrawNarrowMode())
	{
		rc_control_bar.top = lyric_rect.bottom + m_pLayout->margin;
		rc_control_bar.left = m_pLayout->margin;
		rc_control_bar.right = draw_rect.right - m_pLayout->margin;
		rc_control_bar.bottom = draw_rect.bottom - m_pLayout->margin;
	}
	else
	{
		rc_control_bar.top = other_info_rect.bottom + m_pLayout->margin;
		rc_control_bar.left = m_pLayout->margin;
		rc_control_bar.right = draw_rect.right - m_pLayout->margin;
		rc_control_bar.bottom = lyric_rect.top;
	}
	DrawControlBar(rc_control_bar, draw_background);

	//��������DC�е�ͼ�񿽱�����Ļ����ʾ
	m_pDC->BitBlt(m_draw_rect.left, m_draw_rect.top, m_draw_rect.Width(), m_draw_rect.Height(), &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBit);
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();

	CPlayerUIBase::DrawInfo(reset);
}

void CPlayerUI::DrawLyricsSingleLine(CRect lyric_rect)
{
	if (theApp.m_app_setting_data.lyric_background)
	{
		bool draw_background{ theApp.m_app_setting_data.album_cover_as_background && (CPlayer::GetInstance().AlbumCoverExist() || !m_ui_data.default_background.IsNull()) };
		if (draw_background)
			m_draw.FillAlphaRect(lyric_rect, m_colors.color_lyric_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency) * 3 / 5);
		else
			m_draw.FillRect(lyric_rect, m_colors.color_lyric_back);
	}
	if (CPlayer::GetInstance().IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric && !CPlayer::GetInstance().MidiNoLyric())
	{
		wstring current_lyric{ CPlayer::GetInstance().GetMidiLyric() };
		m_draw.DrawWindowText(lyric_rect, current_lyric.c_str(), m_colors.color_text, Alignment::CENTER, false, false, true);
	}
	else if (CPlayer::GetInstance().m_Lyrics.IsEmpty())
	{
		m_draw.DrawWindowText(lyric_rect, CCommon::LoadText(IDS_NO_LYRIC_INFO), m_colors.color_text_2, Alignment::CENTER);
	}
	else
	{
		wstring current_lyric{ CPlayer::GetInstance().m_Lyrics.GetLyric(Time(CPlayer::GetInstance().GetCurrentPosition()), 0).text };	//��ȡ�����
		if (current_lyric.empty())		//�����ǰ���Ϊ�հף�����ʾΪʡ�Ժ�
			current_lyric = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
		if (theApp.m_lyric_setting_data.lyric_karaoke_disp)		//����Կ���OK��ʽ��ʾʱ
		{
			int progress{ CPlayer::GetInstance().m_Lyrics.GetLyricProgress(Time(CPlayer::GetInstance().GetCurrentPosition())) };		//��ȡ��ǰ��ʽ��ȣ���ΧΪ0~1000��
			m_draw.DrawWindowText(lyric_rect, current_lyric.c_str(), m_colors.color_text, m_colors.color_text_2, progress, true);
		}
		else				//��ʲ��Կ���OK��ʽ��ʾʱ
		{
			m_draw.DrawWindowText(lyric_rect, current_lyric.c_str(), m_colors.color_text, Alignment::CENTER);
		}
	}
}

void CPlayerUI::DrawLyricsMulityLine(CRect lyric_rect, CDC * pDC)
{
	bool draw_background{ theApp.m_app_setting_data.album_cover_as_background && (CPlayer::GetInstance().AlbumCoverExist() || !m_ui_data.default_background.IsNull()) };
	bool midi_lyric{ CPlayer::GetInstance().IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric && !CPlayer::GetInstance().MidiNoLyric() };
	//��ʾ������㡱
	CRect tmp;
	tmp = lyric_rect;
	tmp.left += 2 * m_pLayout->margin;
	tmp.bottom = tmp.top + theApp.DPI(28);
	m_draw.SetFont(theApp.m_pMainWnd->GetFont());
	m_draw.DrawWindowText(tmp, CCommon::LoadText(IDS_LYRIC_SHOW, _T(": ")), m_colors.color_text);
	//��ʾ���밴ť
	CRect translate_rect{ tmp };
	translate_rect.DeflateRect(theApp.DPI(4), theApp.DPI(4));
	translate_rect.right = lyric_rect.right - 2 * m_pLayout->margin;
	translate_rect.left = translate_rect.right - translate_rect.Height();
	m_buttons[BTN_TRANSLATE].rect = translate_rect;
	m_buttons[BTN_TRANSLATE].rect.MoveToXY(CPoint{ translate_rect.left + m_draw_rect.left, translate_rect.top + m_draw_rect.top });	//����������任Ϊ�Կͻ������Ͻ�Ϊԭ��
	m_buttons[BTN_TRANSLATE].enable = CPlayer::GetInstance().m_Lyrics.IsTranslated() && !midi_lyric;
	if (m_buttons[BTN_TRANSLATE].enable)
	{
		BYTE alpha;
		if (draw_background)
			alpha = ALPHA_CHG(theApp.m_app_setting_data.background_transparency) * 2 / 3;
		else
			alpha = 255;
		if (m_buttons[BTN_TRANSLATE].hover)
			m_draw.FillAlphaRect(translate_rect, m_colors.color_text_2, alpha);
		else if (m_ui_data.show_translate)
			m_draw.FillAlphaRect(translate_rect, m_colors.color_button_back, alpha);
		m_draw.DrawWindowText(translate_rect, CCommon::LoadText(IDS_TRAS), m_colors.color_text, Alignment::CENTER);
	}
	else
	{
		m_draw.DrawWindowText(translate_rect, CCommon::LoadText(IDS_TRAS), GRAY(200), Alignment::CENTER);
	}
	//��������򱳾�ɫ
	m_draw.SetFont(&m_ui_data.lyric_font);
	CRect lyric_area = lyric_rect;
	lyric_area.DeflateRect(2 * m_pLayout->margin, 2 * m_pLayout->margin);
	lyric_area.top += theApp.DPI(20);
	if (theApp.m_app_setting_data.lyric_background)
	{
		if (draw_background)
			m_draw.FillAlphaRect(lyric_area, m_colors.color_lyric_back, ALPHA_CHG(theApp.m_app_setting_data.background_transparency) * 3 / 5);
		else
			m_draw.FillRect(lyric_area, m_colors.color_lyric_back);
	}
	//���ø����������
	lyric_area.DeflateRect(2 * m_pLayout->margin, 2 * m_pLayout->margin);
	CDrawCommon::SetDrawArea(pDC, lyric_area);

	//���Ƹ���ı�
	DrawLyricTextMultiLine(lyric_area, midi_lyric);
}

void CPlayerUI::RButtonUp(CPoint point)
{
	CPlayerUIBase::RButtonUp(point);

	if (m_buttons[BTN_REPETEMODE].rect.PtInRect(point))
		return;

	CPoint point1;		//����һ������ȷ�����λ�õ�λ��  
	GetCursorPos(&point1);	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը����꣬��λ������Ļ���Ͻǵ�Ϊԭ�㣬point���Կͻ������Ͻ�Ϊԭ��

	if (!m_draw_data.lyric_rect.PtInRect(point))
	{
		m_main_popup_menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, theApp.m_pMainWnd);
	}
	else
	{
		m_popup_menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, theApp.m_pMainWnd);
	}

}

void CPlayerUI::MouseMove(CPoint point)
{
	CPlayerUIBase::MouseMove(point);

	////��ʾר���������ʾ
	//if (theApp.m_nc_setting_data.show_cover_tip && theApp.m_app_setting_data.show_album_cover)
	//{
	//	CRect cover_rect{ m_draw_data.cover_rect };
	//	cover_rect.MoveToXY(m_draw_rect.left + m_draw_data.cover_rect.left, m_draw_rect.top + m_draw_data.cover_rect.top);
	//	bool show_cover_tip{ cover_rect.PtInRect(point) != FALSE };
	//	static bool last_show_cover_tip{ false };
	//	if (!last_show_cover_tip && show_cover_tip)
	//	{
	//		CString info;
	//		if (CPlayer::GetInstance().AlbumCoverExist())
	//		{
	//			info = CCommon::LoadText(IDS_ALBUM_COVER, _T(": "));
	//			//CFilePathHelper cover_path(CPlayer::GetInstance().GetAlbumCoverPath());
	//			//if (cover_path.GetFileNameWithoutExtension() == ALBUM_COVER_NAME)
	//			if (CPlayer::GetInstance().IsInnerCover())
	//			{
	//				info += CCommon::LoadText(IDS_INNER_ALBUM_COVER_TIP_INFO);
	//				switch (CPlayer::GetInstance().GetAlbumCoverType())
	//				{
	//				case 0: info += _T("jpg"); break;
	//				case 1: info += _T("png"); break;
	//				case 2: info += _T("gif"); break;
	//				}
	//			}
	//			else
	//			{
	//				info += CCommon::LoadText(IDS_OUT_IMAGE, _T("\r\n"));
	//				info += CPlayer::GetInstance().GetAlbumCoverPath().c_str();
	//			}
	//		}
	//		m_tool_tip->AddTool(theApp.m_pMainWnd, info);
	//		m_tool_tip->SetMaxTipWidth(theApp.DPI(400));
	//		m_tool_tip->Pop();
	//	}
	//	if (last_show_cover_tip && !show_cover_tip)
	//	{
	//		m_tool_tip->AddTool(theApp.m_pMainWnd, _T(""));
	//		m_tool_tip->Pop();
	//	}
	//	last_show_cover_tip = show_cover_tip;
	//}

}

void CPlayerUI::LButtonUp(CPoint point)
{
	CPlayerUIBase::LButtonUp(point);

	if (m_buttons[BTN_TRANSLATE].rect.PtInRect(point) && m_buttons[BTN_TRANSLATE].enable)	//����ˡ���ʷ��롱ʱ��������رո�ʷ���
	{
		m_ui_data.show_translate = !m_ui_data.show_translate;
	}


}

void CPlayerUI::OnSizeRedraw(int cx, int cy)
{
	m_show_volume_adj = false;
	CPlayerUIBase::OnSizeRedraw(cx, cy);
}

//CRect CPlayerUI::GetThumbnailClipArea()
//{
//	//CRect info_rect;
//	//if (!DrawNarrowMode())
//	//	info_rect = CRect{ CPoint{ m_pLayout->margin, m_pLayout->margin + theApp.DPI(20) }, CSize{ m_ui_data.client_width / 2 - 2 * m_pLayout->margin, m_pLayout->info_height2 - 3 * m_pLayout->margin } };
//	//else
//	//	info_rect = CRect{ CPoint{ m_pLayout->margin, theApp.DPI(20) }, CSize{ m_ui_data.client_width - 2 * m_pLayout->margin, m_pLayout->info_height - 2 * m_pLayout->margin } };
//
//	return info_rect;
//}

void CPlayerUI::AddMouseToolTip(BtnKey btn, LPCTSTR str)
{
	m_tool_tip->AddTool(theApp.m_pMainWnd, str, m_buttons[btn].rect, btn + 1000);
}

void CPlayerUI::UpdateMouseToolTip(BtnKey btn, LPCTSTR str)
{
	m_tool_tip->UpdateTipText(str, theApp.m_pMainWnd, btn + 1000);
}

void CPlayerUI::UpdateToolTipPosition()
{
	for (const auto& btn : m_buttons)
	{
		m_tool_tip->SetToolRect(theApp.m_pMainWnd, btn.first + 1000, btn.second.rect);
	}
}

