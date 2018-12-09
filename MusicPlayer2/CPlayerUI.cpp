#include "stdafx.h"
#include "CPlayerUI.h"


CPlayerUI::CPlayerUI(UIData& ui_data)
	:m_ui_data(ui_data)
{
}


CPlayerUI::~CPlayerUI()
{
}

void CPlayerUI::Init(CDC* pDC)
{
	m_pDC = pDC;
	m_draw.Create(m_pDC, theApp.m_pMainWnd);
	m_pLayout = std::make_shared<SLayoutData>();
}

void CPlayerUI::DrawInfo(bool narrow_mode, bool reset)
{
	//���û���ĺ����������û�ͼ��ɫ
	CPlayerUIBase::DrawInfo(narrow_mode, reset);

	//������Ϣ����ľ���
	CRect info_rect;
	if (!narrow_mode)
		info_rect = CRect{ CPoint{m_pLayout->margin, m_pLayout->control_bar_height + m_pLayout->margin}, CSize{m_ui_data.client_width / 2 - 2 * m_pLayout->margin, m_pLayout->info_height2 - 3 * m_pLayout->margin } };
	else
		info_rect = CRect{ CPoint{ m_pLayout->margin, m_pLayout->control_bar_height + m_pLayout->progress_bar_height}, CSize{ m_ui_data.client_width - 2 * m_pLayout->margin, m_pLayout->info_height - 2 * m_pLayout->margin } };

	//���û����DC
	CDC MemDC;
	CBitmap MemBitmap;
	MemDC.CreateCompatibleDC(NULL);
	m_draw_data.draw_rect = info_rect;		//��ͼ����
	if (!narrow_mode)
		m_draw_data.draw_rect.bottom = m_ui_data.client_height - m_pLayout->margin;
	CRect draw_rect{ m_draw_data.draw_rect };
	draw_rect.MoveToXY(0, 0);
	MemBitmap.CreateCompatibleBitmap(m_pDC, m_draw_data.draw_rect.Width(), m_draw_data.draw_rect.Height());
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
	m_draw.SetDC(&MemDC);	//��m_draw�еĻ�ͼDC����Ϊ�����DC
	if (theApp.m_app_setting_data.album_cover_as_background)
	{
		if (theApp.m_player.AlbumCoverExist())
		{
			CImage& back_image{ theApp.m_app_setting_data.background_gauss_blur ? theApp.m_player.GetAlbumCoverBlur() : theApp.m_player.GetAlbumCover() };
			m_draw.DrawBitmap(back_image, CPoint(0, 0), m_draw_data.draw_rect.Size(), CDrawCommon::StretchMode::FILL);
		}
		else
		{
			//MemDC.FillSolidRect(0, 0, m_draw_data.draw_rect.Width(), m_draw_data.draw_rect.Height(), GetSysColor(COLOR_BTNFACE));	//������DC�Ļ�ͼ�������Ի���ı�����ɫ
			m_draw.DrawBitmap(m_ui_data.default_background, CPoint(0, 0), m_draw_data.draw_rect.Size(), CDrawCommon::StretchMode::FILL);
		}
	}
	else
	{
		m_draw.FillRect(draw_rect, m_colors.color_back);
	}

	//���������˻����ͼ����m_draw_data.draw_rect�����Ͻǵ����˻�ͼ��ԭ��
	info_rect.MoveToXY(0, 0);

	//��䱳����ɫ
	//CDrawCommon::SetDrawArea(&MemDC, info_rect);
	bool draw_background{ theApp.m_app_setting_data.album_cover_as_background && (theApp.m_player.AlbumCoverExist() || !m_ui_data.default_background.IsNull()) };		//�Ƿ���Ҫ����ͼƬ����
	if (draw_background)
		m_draw.FillAlphaRect(draw_rect, m_colors.color_back, ALPHA_CHG(m_colors.background_transparency));
	else
		m_draw.FillRect(draw_rect, m_colors.color_back);

	//if (!narrow_mode)
	//{
	//	CRect gap_rect{ info_rect };
	//	gap_rect.top = info_rect.bottom;
	//	gap_rect.bottom = gap_rect.top + 2*m_pLayout->margin;
	//	//CDrawCommon::SetDrawArea(&MemDC, gap_rect);
	//	//MemDC.FillSolidRect(gap_rect, GetSysColor(COLOR_BTNFACE));
	//	m_draw.FillAlphaRect(gap_rect, color_back, ALPHA_CHG(m_colors.background_transparency));
	//}

	CPoint text_start{ info_rect.left + m_pLayout->spectral_size.cx + 2 * m_pLayout->margin, info_rect.top + m_pLayout->margin };		//�ı�����ʼ����
	int text_height{ theApp.DPI(18) };		//�ı��ĸ߶�

	//��ʾ������Ϣ
	m_draw.SetFont(theApp.m_pMainWnd->GetFont());
	//m_draw.SetBackColor(color_back);
	CRect tmp{ text_start, CSize{1,text_height} };
	wchar_t buff[64];
	if (theApp.m_player.m_loading)
	{
		tmp.right = info_rect.right - m_pLayout->margin;
		static CDrawCommon::ScrollInfo scroll_info0;
		CString info;
		info.Format(_T("�ҵ�%d�׸��������ڶ�ȡ��Ƶ�ļ���Ϣ�������%d%%�����Ժ򡭡�"), theApp.m_player.GetSongNum(), theApp.m_player.m_thread_info.process_percent);
		m_draw.DrawScrollText(tmp, info, m_colors.color_text, theApp.DPI(1.5), false, scroll_info0, reset);
	}
	else
	{
		//��ʾ���ڲ��ŵ��ļ���
		tmp.MoveToX(text_start.x + theApp.DPI(82));
		tmp.right = info_rect.right - m_pLayout->margin;
		static CDrawCommon::ScrollInfo scroll_info1;
		m_draw.DrawScrollText(tmp, theApp.m_player.GetFileName().c_str(), m_colors.color_text, theApp.DPI(1.5), false, scroll_info1, reset);
		//��ʾ���ڲ��ŵ���Ŀ���
		tmp.MoveToX(text_start.x + theApp.DPI(52));
		tmp.right = tmp.left + theApp.DPI(30);
		swprintf_s(buff, sizeof(buff) / 2, L"%.3d", theApp.m_player.GetIndex() + 1);
		m_draw.DrawWindowText(tmp, buff, m_colors.color_text_2);
		//��ʾ����״̬
		tmp.MoveToX(text_start.x);
		tmp.right = tmp.left + theApp.DPI(52);
		m_draw.DrawWindowText(tmp, theApp.m_player.GetPlayingState().c_str(), m_colors.color_text_lable);
	}

	wstring lable1_str, lable1_content;
	wstring lable2_str, lable2_content;
	wstring lable3_str, lable3_content;
	wstring lable4_str, lable4_content;
	lable1_str = _T("���⣺");
	lable1_content = theApp.m_player.GetCurrentSongInfo().title;
	if (theApp.m_player.IsMidi())
	{
		const MidiInfo& midi_info{ theApp.m_player.GetMidiInfo() };
		lable2_str = _T("���ģ�");
		swprintf_s(buff, L"%d/%d (%dbpm)", midi_info.midi_position, midi_info.midi_length, midi_info.speed);
		lable2_content = buff;

		//lable3_str = _T("�ٶȣ�");
		//swprintf_s(buff, L"%d bpm", midi_info.speed);
		//lable3_content = buff;

		lable3_str = _T("��ɫ�⣺");
		lable3_content = theApp.m_player.GetSoundFontName();
	}
	else
	{
		lable2_str = _T("�����ң�");
		lable2_content = theApp.m_player.GetCurrentSongInfo().artist;
		lable3_str = _T("��Ƭ����");
		lable3_content = theApp.m_player.GetCurrentSongInfo().album;
	}
	lable4_str = _T("��ʽ��");
	const BASS_CHANNELINFO channel_info{ theApp.m_player.GetChannelInfo() };
	CString chans_str;
	if (channel_info.chans == 1)
		chans_str = _T("������");
	else if (channel_info.chans == 2)
		chans_str = _T("������");
	else if (channel_info.chans > 2)
		chans_str.Format(_T("%d����"));
	if (!theApp.m_player.IsMidi())
		swprintf_s(buff, L"%s %.1fkHz %dkbps %s", theApp.m_player.GetCurrentFileType().c_str(), channel_info.freq / 1000.0f, theApp.m_player.GetCurrentSongInfo().bitrate, chans_str.GetString());
	else
		swprintf_s(buff, L"%s %.1fkHz %s", theApp.m_player.GetCurrentFileType().c_str(), channel_info.freq / 1000.0f, chans_str.GetString());
	lable4_content = buff;
	//��ʾ����
	tmp.MoveToXY(text_start.x, text_start.y + text_height);
	tmp.right = tmp.left + theApp.DPI(52);
	m_draw.DrawWindowText(tmp, lable1_str.c_str(), m_colors.color_text_lable);
	tmp.MoveToX(tmp.left + theApp.DPI(52));
	tmp.right = info_rect.right - m_pLayout->margin;
	static CDrawCommon::ScrollInfo scroll_info2;
	m_draw.DrawScrollText2(tmp, lable1_content.c_str(), m_colors.color_text, theApp.DPI(1), false, scroll_info2, reset);
	//��ʾ������
	tmp.MoveToXY(text_start.x, text_start.y + 2 * text_height);
	tmp.right = tmp.left + theApp.DPI(52);
	m_draw.DrawWindowText(tmp, lable2_str.c_str(), m_colors.color_text_lable);
	tmp.MoveToX(tmp.left + theApp.DPI(52));
	tmp.right = info_rect.right - m_pLayout->margin;
	static CDrawCommon::ScrollInfo scroll_info3;
	if (theApp.m_player.IsMidi())
		m_draw.DrawWindowText(tmp, lable2_content.c_str(), m_colors.color_text);
	else
		m_draw.DrawScrollText2(tmp, lable2_content.c_str(), m_colors.color_text, theApp.DPI(1), false, scroll_info3, reset);
	//��ʾ��Ƭ��
	tmp.MoveToXY(text_start.x, text_start.y + 3 * text_height);
	tmp.right = tmp.left + theApp.DPI(52);
	m_draw.DrawWindowText(tmp, lable3_str.c_str(), m_colors.color_text_lable);
	tmp.MoveToX(tmp.left + theApp.DPI(52));
	tmp.right = info_rect.right - m_pLayout->margin;
	static CDrawCommon::ScrollInfo scroll_info4;
	m_draw.DrawScrollText2(tmp, lable3_content.c_str(), m_colors.color_text, theApp.DPI(1), false, scroll_info4, reset);
	//��ʾ�ļ���ʽ�ͱ�����
	tmp.MoveToXY(text_start.x, text_start.y + 4 * text_height);
	tmp.right = tmp.left + theApp.DPI(52);
	m_draw.DrawWindowText(tmp, lable4_str.c_str(), m_colors.color_text_lable);
	tmp.MoveToX(tmp.left + theApp.DPI(52));
	tmp.right = info_rect.right - m_pLayout->margin;
	static CDrawCommon::ScrollInfo scroll_info5;
	m_draw.DrawScrollText2(tmp, lable4_content.c_str(), m_colors.color_text, theApp.DPI(1), false, scroll_info5, reset);

	//��ʾƵ�׷���
	CRect spectral_rect{ CPoint{info_rect.left + m_pLayout->margin, info_rect.top + m_pLayout->margin}, m_pLayout->spectral_size };
	//���Ʊ���
	if (draw_background)
		m_draw.FillAlphaRect(spectral_rect, m_colors.color_spectrum_back, ALPHA_CHG(m_colors.background_transparency) * 2 / 3);
	else
		m_draw.FillRect(spectral_rect, m_colors.color_spectrum_back);
	if (theApp.m_app_setting_data.show_album_cover)
	{
		//����ר������
		m_draw_data.cover_rect = spectral_rect;
		m_draw_data.cover_rect.DeflateRect(m_pLayout->margin / 2, m_pLayout->margin / 2);
		if (theApp.m_player.AlbumCoverExist())
		{
			m_draw.DrawBitmap(theApp.m_player.GetAlbumCover(), m_draw_data.cover_rect.TopLeft(), m_draw_data.cover_rect.Size(), theApp.m_app_setting_data.album_cover_fit);
		}
		else
		{
			int cover_side = m_draw_data.cover_rect.Height() * 3 / 4;
			int x = m_draw_data.cover_rect.left + (m_draw_data.cover_rect.Width() - cover_side) / 2;
			int y = m_draw_data.cover_rect.top + (m_draw_data.cover_rect.Height() - cover_side) / 2;
			::DrawIconEx(m_draw.GetDC()->GetSafeHdc(), x, y, theApp.m_default_cover, cover_side, cover_side, 0, NULL, DI_NORMAL);
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
			float spetral_data = (theApp.m_player.GetSpectralData()[i * 2] + theApp.m_player.GetSpectralData()[i * 2 + 1]) / 2;
			float peak_data = (theApp.m_player.GetSpectralPeakData()[i * 2] + theApp.m_player.GetSpectralPeakData()[i * 2 + 1]) / 2;

			CRect rect_tmp{ rects[i] };
			int spetral_height = static_cast<int>(spetral_data * rects[0].Height() / 30 * theApp.m_app_setting_data.sprctrum_height / 100);
			int peak_height = static_cast<int>(peak_data * rects[0].Height() / 30 * theApp.m_app_setting_data.sprctrum_height / 100);
			if (spetral_height <= 0 || theApp.m_player.IsError()) spetral_height = 1;		//Ƶ�׸߶�����Ϊ1�����أ�������ų���Ҳ����ʾƵ��
			if (peak_height <= 0 || theApp.m_player.IsError()) peak_height = 1;		//Ƶ�׸߶�����Ϊ1�����أ�������ų���Ҳ����ʾƵ��
			rect_tmp.top = rect_tmp.bottom - spetral_height;
			if (rect_tmp.top < rects[0].top) rect_tmp.top = rects[0].top;
			COLORREF color;
			if (theApp.m_app_setting_data.show_album_cover && theApp.m_player.AlbumCoverExist())
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

	//��ʾ����������Ϣ
	//���Ʊ���
	CPoint point{ spectral_rect.left, spectral_rect.bottom };
	point.y += 2 * m_pLayout->margin;
	CRect other_info_rect{ point, CSize(info_rect.Width() - 2 * m_pLayout->margin,theApp.DPI(24)) };
	if (draw_background)
		m_draw.FillAlphaRect(other_info_rect, m_colors.color_control_bar_back, ALPHA_CHG(m_colors.background_transparency));
	else
		m_draw.FillRect(other_info_rect, m_colors.color_control_bar_back);
	//��ʾ������Ϣ
	//m_draw.SetBackColor(theApp.m_app_setting_data.theme_color.light3);
	//��ʾѭ��ģʽ
	tmp = other_info_rect;
	tmp.left += m_pLayout->margin;
	tmp.right = tmp.left + theApp.DPI(60);
	m_draw_data.repetemode_rect = tmp;
	m_draw_data.repetemode_rect.DeflateRect(0, theApp.DPI(4));
	CString repeat_mode_str;
	switch (theApp.m_player.GetRepeatMode())
	{
	case RepeatMode::RM_PLAY_ORDER: repeat_mode_str += _T("˳�򲥷�"); break;
	case RepeatMode::RM_LOOP_PLAYLIST: repeat_mode_str += _T("�б�ѭ��"); break;
	case RepeatMode::RM_LOOP_TRACK: repeat_mode_str += _T("����ѭ��"); break;
	case RepeatMode::RM_PLAY_SHUFFLE: repeat_mode_str += _T("�������"); break;
	}
	if (m_draw_data.repetemode_hover)		//���ָ��ѭ��ģʽ��ʱ��������һ����ɫ��ʾ
		m_draw.DrawWindowText(m_draw_data.repetemode_rect, repeat_mode_str, m_colors.color_text_heighlight);
	else
		m_draw.DrawWindowText(m_draw_data.repetemode_rect, repeat_mode_str, m_colors.color_text);
	m_draw_data.repetemode_rect.MoveToXY(CPoint{ m_draw_data.repetemode_rect.left + m_draw_data.draw_rect.left, m_draw_data.repetemode_rect.top + m_draw_data.draw_rect.top });	//����������任Ϊ�Կͻ������Ͻ�Ϊԭ��

	//�����л����水ť
	tmp.right = other_info_rect.right;
	tmp.left = tmp.right - other_info_rect.Height();
	CRect rc_icon = tmp;
	rc_icon.DeflateRect(theApp.DPI(2), theApp.DPI(2));
	m_draw.SetDrawArea(rc_icon);

	BYTE alpha;
	if (draw_background)
		alpha = ALPHA_CHG(m_colors.background_transparency);
	else
		alpha = 255;
	if (m_draw_data.skin_btn.hover)
		m_draw.FillAlphaRect(rc_icon, m_colors.color_text_2, alpha);
	else if (!theApp.m_app_setting_data.dark_mode)
		m_draw.FillAlphaRect(rc_icon, m_colors.color_button_back, alpha);

	m_draw_data.skin_btn.rect = DrawAreaToClient(rc_icon, m_draw_data.draw_rect);

	rc_icon = tmp;
	rc_icon.DeflateRect(theApp.DPI(4), theApp.DPI(4));
	m_draw.DrawIcon(theApp.m_skin_icon, rc_icon.TopLeft(), rc_icon.Size());

	//��ʾ<<<<
	int progress;
	Time time{ theApp.m_player.GetCurrentPosition() };
	if (theApp.m_player.IsMidi())
	{
		////progress = (theApp.m_player.GetMidiInfo().midi_position % 16 + 1) *1000 / 16;
		//if (theApp.m_player.GetMidiInfo().tempo == 0)
		//	progress = 0;
		//else
		//	progress = (time.time2int() * 1000 / theApp.m_player.GetMidiInfo().tempo % 4 + 1) * 250;
		progress = (theApp.m_player.GetMidiInfo().midi_position % 4 + 1) * 250;
	}
	else
	{
		progress = (time.sec % 4 * 1000 + time.msec) / 4;
	}
	tmp.right = tmp.left;
	tmp.left = tmp.right - theApp.DPI(49);
	m_draw.DrawWindowText(tmp, _T("<<<<"), m_colors.color_text, m_colors.color_text_2, progress, false);

	//��ʾ����																																									//��ʾ����
	tmp.right = tmp.left;
	tmp.left = tmp.right - theApp.DPI(75);
	m_draw_data.volume_btn.rect = tmp;
	m_draw_data.volume_btn.rect.DeflateRect(0, theApp.DPI(4));
	m_draw_data.volume_btn.rect.right -= theApp.DPI(12);
	swprintf_s(buff, L"������%d%%", theApp.m_player.GetVolume());
	if (m_draw_data.volume_btn.hover)		//���ָ����������ʱ��������һ����ɫ��ʾ
		m_draw.DrawWindowText(tmp, buff, m_colors.color_text_heighlight);
	else
		m_draw.DrawWindowText(tmp, buff, m_colors.color_text);
	//��������������ť��λ��
	m_draw_data.volume_down_rect = m_draw_data.volume_btn.rect;
	m_draw_data.volume_down_rect.bottom += theApp.DPI(4);
	m_draw_data.volume_down_rect.MoveToY(m_draw_data.volume_btn.rect.bottom);
	m_draw_data.volume_down_rect.right = m_draw_data.volume_btn.rect.left + m_draw_data.volume_btn.rect.Width() / 2;
	m_draw_data.volume_up_rect = m_draw_data.volume_down_rect;
	m_draw_data.volume_up_rect.MoveToX(m_draw_data.volume_down_rect.right);
	m_draw_data.volume_btn.rect.MoveToXY(CPoint{ m_draw_data.volume_btn.rect.left + m_draw_data.draw_rect.left, m_draw_data.volume_btn.rect.top + m_draw_data.draw_rect.top });	//����������任Ϊ�Կͻ������Ͻ�Ϊԭ��

	//��ʾ���
	m_draw.SetFont(&m_ui_data.lyric_font);
	CRect lyric_rect;
	if (narrow_mode)
	{
		lyric_rect = other_info_rect;
		lyric_rect.MoveToY(other_info_rect.bottom + m_pLayout->margin);
		DrawLyricsSingleLine(lyric_rect);
	}
	else
	{
		//if (theApp.m_player.IsPlaying() || reset)
		//{
		lyric_rect = info_rect;
		lyric_rect.MoveToY(info_rect.bottom/* + 2*m_pLayout->margin*/);
		lyric_rect.bottom = m_draw_data.draw_rect.Height()/* - m_pLayout->margin*/;
		DrawLyricsMulityLine(lyric_rect, &MemDC);
		//}
	}

	//������������ť����Ϊ�������ϲ㣬���Ա����ڸ�ʻ�����ɺ����
	if (m_draw_data.show_volume_adj)
	{
		//m_draw.SetBackColor(theApp.m_app_setting_data.theme_color.light1);
		if (draw_background)
		{
			m_draw.FillAlphaRect(m_draw_data.volume_down_rect, m_colors.color_text_2, ALPHA_CHG(m_colors.background_transparency));
			m_draw.FillAlphaRect(m_draw_data.volume_up_rect, m_colors.color_text_2, ALPHA_CHG(m_colors.background_transparency));
		}
		else
		{
			m_draw.FillRect(m_draw_data.volume_down_rect, m_colors.color_text_2);
			m_draw.FillRect(m_draw_data.volume_up_rect, m_colors.color_text_2);
		}
		m_draw.DrawWindowText(m_draw_data.volume_down_rect, L"-", ColorTable::WHITE, Alignment::CENTER);
		m_draw.DrawWindowText(m_draw_data.volume_up_rect, L"+", ColorTable::WHITE, Alignment::CENTER);
	}
	m_draw_data.volume_down_rect.MoveToXY(CPoint{ m_draw_data.volume_down_rect.left + m_draw_data.draw_rect.left, m_draw_data.volume_down_rect.top + m_draw_data.draw_rect.top });	//����������任Ϊ�Կͻ������Ͻ�Ϊԭ��
	m_draw_data.volume_up_rect.MoveToXY(CPoint{ m_draw_data.volume_up_rect.left + m_draw_data.draw_rect.left, m_draw_data.volume_up_rect.top + m_draw_data.draw_rect.top });	//����������任Ϊ�Կͻ������Ͻ�Ϊԭ��

	//��������DC�е�ͼ�񿽱�����Ļ����ʾ
	m_pDC->BitBlt(m_draw_data.draw_rect.left, m_draw_data.draw_rect.top, m_draw_data.draw_rect.Width(), m_draw_data.draw_rect.Height(), &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBit);
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}

void CPlayerUI::DrawLyricsSingleLine(CRect lyric_rect)
{
	if (theApp.m_app_setting_data.lyric_background)
	{
		bool draw_background{ theApp.m_app_setting_data.album_cover_as_background && (theApp.m_player.AlbumCoverExist() || !m_ui_data.default_background.IsNull()) };
		if (draw_background)
			m_draw.FillAlphaRect(lyric_rect, m_colors.color_lyric_back, ALPHA_CHG(m_colors.background_transparency) * 3 / 5);
		else
			m_draw.FillRect(lyric_rect, m_colors.color_lyric_back);
	}
	if (theApp.m_player.IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric && !theApp.m_player.MidiNoLyric())
	{
		wstring current_lyric{ theApp.m_player.GetMidiLyric() };
		m_draw.DrawWindowText(lyric_rect, current_lyric.c_str(), m_colors.color_text, Alignment::CENTER, false, false, true);
	}
	else if (theApp.m_player.m_Lyrics.IsEmpty())
	{
		m_draw.DrawWindowText(lyric_rect, _T("��ǰ����û�и��"), m_colors.color_text_2, Alignment::CENTER);
	}
	else
	{
		wstring current_lyric{ theApp.m_player.m_Lyrics.GetLyric(Time(theApp.m_player.GetCurrentPosition()), 0).text };	//��ȡ�����
		if (current_lyric.empty())		//�����ǰ���Ϊ�հף�����ʾΪʡ�Ժ�
			current_lyric = DEFAULT_LYRIC_TEXT;
		if (theApp.m_lyric_setting_data.lyric_karaoke_disp)		//����Կ���OK��ʽ��ʾʱ
		{
			int progress{ theApp.m_player.m_Lyrics.GetLyricProgress(Time(theApp.m_player.GetCurrentPosition())) };		//��ȡ��ǰ��ʽ��ȣ���ΧΪ0~1000��
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
	bool draw_background{ theApp.m_app_setting_data.album_cover_as_background && (theApp.m_player.AlbumCoverExist() || !m_ui_data.default_background.IsNull()) };
	bool midi_lyric{ theApp.m_player.IsMidi() && theApp.m_general_setting_data.midi_use_inner_lyric && !theApp.m_player.MidiNoLyric() };
	//��ʾ������㡱
	CRect tmp;
	tmp = lyric_rect;
	tmp.left += 2 * m_pLayout->margin;
	tmp.bottom = tmp.top + theApp.DPI(28);
	m_draw.SetFont(theApp.m_pMainWnd->GetFont());
	m_draw.DrawWindowText(tmp, _T("����㣺"), m_colors.color_text);
	//��ʾ���밴ť
	CRect translate_rect{ tmp };
	translate_rect.DeflateRect(theApp.DPI(4), theApp.DPI(4));
	translate_rect.right = lyric_rect.right - 2 * m_pLayout->margin;
	translate_rect.left = translate_rect.right - translate_rect.Height();
	m_draw_data.translate_btn.rect = translate_rect;
	m_draw_data.translate_btn.rect.MoveToXY(CPoint{ translate_rect.left + m_draw_data.draw_rect.left, translate_rect.top + m_draw_data.draw_rect.top });	//����������任Ϊ�Կͻ������Ͻ�Ϊԭ��
	m_draw_data.translate_btn.enable = theApp.m_player.m_Lyrics.IsTranslated() && !midi_lyric;
	if (m_draw_data.translate_btn.enable)
	{
		BYTE alpha;
		if (draw_background)
			alpha = ALPHA_CHG(m_colors.background_transparency);
		else
			alpha = 255;
		if (m_draw_data.translate_btn.hover)
			m_draw.FillAlphaRect(translate_rect, m_colors.color_text_2, alpha);
		else if (m_ui_data.show_translate)
			m_draw.FillAlphaRect(translate_rect, m_colors.color_button_back, alpha);
		m_draw.DrawWindowText(translate_rect, L"��", m_colors.color_text, Alignment::CENTER);
	}
	else
	{
		m_draw.DrawWindowText(translate_rect, L"��", GRAY(200), Alignment::CENTER);
	}
	//��������򱳾�ɫ
	m_draw.SetFont(&m_ui_data.lyric_font);
	CRect lyric_area = lyric_rect;
	lyric_area.DeflateRect(2 * m_pLayout->margin, 2 * m_pLayout->margin);
	lyric_area.top += theApp.DPI(20);
	if (theApp.m_app_setting_data.lyric_background)
	{
		if (draw_background)
			m_draw.FillAlphaRect(lyric_area, m_colors.color_lyric_back, ALPHA_CHG(m_colors.background_transparency) * 3 / 5);
		else
			m_draw.FillRect(lyric_area, m_colors.color_lyric_back);
	}
	//���ø����������
	lyric_area.DeflateRect(2 * m_pLayout->margin, 2 * m_pLayout->margin);
	CDrawCommon::SetDrawArea(pDC, lyric_area);

	//���Ƹ���ı�
	DrawLyricTextMultiLine(lyric_area, &m_ui_data.lyric_font, &m_ui_data.lyric_translate_font,
		m_colors.color_text, m_colors.color_text_2, m_ui_data.show_translate, midi_lyric);
}

void CPlayerUI::RButtonUp(CPoint point, bool narrow_mode)
{
	if (m_draw_data.volume_btn.rect.PtInRect(point) == FALSE)
		m_draw_data.show_volume_adj = false;

	//������ʾ��Ϣ����ʾ��ʵ�����
	CRect info_rect{ m_draw_data.draw_rect }, lyric_rect{ m_draw_data.draw_rect };
	if (!narrow_mode)
	{
		int height = m_pLayout->info_height2 - 3 * m_pLayout->margin;
		info_rect.bottom = info_rect.top + height;
		lyric_rect.top = info_rect.bottom + 2 * m_pLayout->margin;
	}
	else
	{
		info_rect.bottom -= theApp.DPI(30);
		lyric_rect.top = info_rect.bottom;
	}

	CPoint point1;		//����һ������ȷ�����λ�õ�λ��  
	GetCursorPos(&point1);	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը����꣬��λ������Ļ���Ͻǵ�Ϊԭ�㣬point���Կͻ������Ͻ�Ϊԭ��
	if (m_draw_data.repetemode_rect.PtInRect(point))		//����ڡ�ѭ��ģʽ���ľ��������ڵ������Ҽ����򵯳���ѭ��ģʽ�����Ӳ˵�
	{
		CMenu* pMenu = m_main_popup_menu.GetSubMenu(0)->GetSubMenu(1);
		if (pMenu != NULL)
			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, theApp.m_pMainWnd);
	}
	else if (info_rect.PtInRect(point))
	{
		m_main_popup_menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, theApp.m_pMainWnd);
	}
	else if (lyric_rect.PtInRect(point))
	{
		m_popup_menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, theApp.m_pMainWnd);
	}
}

void CPlayerUI::MouseMove(CPoint point)
{
	m_draw_data.repetemode_hover = (m_draw_data.repetemode_rect.PtInRect(point) != FALSE);		//������ƶ�����ѭ��ģʽ�����ڵľ��ο���ʱ����m_draw_data.repetemode_hover��Ϊtrue
	m_draw_data.volume_btn.hover = (m_draw_data.volume_btn.rect.PtInRect(point) != FALSE);
	m_draw_data.skin_btn.hover = (m_draw_data.skin_btn.rect.PtInRect(point) != FALSE);
	m_draw_data.translate_btn.hover = (m_draw_data.translate_btn.rect.PtInRect(point) != FALSE);

	//��ʾ��ʷ���������ʾ
	static bool last_translate_hover{ false };
	AddMouseToolTip(m_draw_data.translate_btn, _T("��ʾ��ʷ���"), &last_translate_hover);

	//��ʾ�����������ʾ
	static bool last_volume_hover{ false };
	AddMouseToolTip(m_draw_data.volume_btn, _T("�����ֵ�������"), &last_volume_hover);

	static bool last_skin_hover{ false };
	AddMouseToolTip(m_draw_data.skin_btn, _T("�л�����"), &last_skin_hover);

	//��ʾר���������ʾ
	if (theApp.m_nc_setting_data.show_cover_tip && theApp.m_app_setting_data.show_album_cover)
	{
		CRect cover_rect{ m_draw_data.cover_rect };
		cover_rect.MoveToXY(m_draw_data.draw_rect.left + m_draw_data.cover_rect.left, m_draw_data.draw_rect.top + m_draw_data.cover_rect.top);
		bool show_cover_tip{ cover_rect.PtInRect(point) != FALSE };
		static bool last_show_cover_tip{ false };
		if (!last_show_cover_tip && show_cover_tip)
		{
			CString info;
			if (theApp.m_player.AlbumCoverExist())
			{
				info = _T("ר������: ");
				//CFilePathHelper cover_path(theApp.m_player.GetAlbumCoverPath());
				//if (cover_path.GetFileNameWithoutExtension() == ALBUM_COVER_NAME)
				if (theApp.m_player.IsInnerCover())
				{
					info += _T("��ǶͼƬ\r\nͼƬ��ʽ: ");
					switch (theApp.m_player.GetAlbumCoverType())
					{
					case 0: info += _T("jpg"); break;
					case 1: info += _T("png"); break;
					case 2: info += _T("gif"); break;
					}
				}
				else
				{
					info += _T("�ⲿͼƬ\r\n");
					info += theApp.m_player.GetAlbumCoverPath().c_str();
				}
			}
			m_tool_tip->AddTool(theApp.m_pMainWnd, info);
			m_tool_tip->SetMaxTipWidth(theApp.DPI(400));
			m_tool_tip->Pop();
		}
		if (last_show_cover_tip && !show_cover_tip)
		{
			m_tool_tip->AddTool(theApp.m_pMainWnd, _T(""));
			m_tool_tip->Pop();
		}
		last_show_cover_tip = show_cover_tip;
	}

}

void CPlayerUI::LButtonUp(CPoint point)
{
	if (m_draw_data.repetemode_rect.PtInRect(point))	//����ˡ�ѭ��ģʽ��ʱ������ѭ��ģʽ
	{
		theApp.m_player.SetRepeatMode();
	}

	if (!m_draw_data.show_volume_adj)		//���������ʾ����������ť�����������������ʾ����������ť
		m_draw_data.show_volume_adj = (m_draw_data.volume_btn.rect.PtInRect(point) != FALSE);
	else		//����Ѿ���ʾ������������ť��������������ʱ��������������ť����ʾ
		m_draw_data.show_volume_adj = (m_draw_data.volume_up_rect.PtInRect(point) || m_draw_data.volume_down_rect.PtInRect(point));

	if (m_draw_data.show_volume_adj && m_draw_data.volume_up_rect.PtInRect(point))	//�������������ť�е�������ʱ��������
	{
		theApp.m_player.MusicControl(Command::VOLUME_UP, theApp.m_nc_setting_data.volum_step);
	}
	if (m_draw_data.show_volume_adj && m_draw_data.volume_down_rect.PtInRect(point))	//�������������ť�е�������ʱ������С
	{
		theApp.m_player.MusicControl(Command::VOLUME_DOWN, theApp.m_nc_setting_data.volum_step);
	}

	if (m_draw_data.translate_btn.rect.PtInRect(point) && m_draw_data.translate_btn.enable)	//����ˡ���ʷ��롱ʱ��������رո�ʷ���
	{
		m_ui_data.show_translate = !m_ui_data.show_translate;
	}

	if (m_draw_data.skin_btn.rect.PtInRect(point))
	{
		theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_SWITCH_UI);
	}

}

void CPlayerUI::OnSizeRedraw(int cx, int cy, bool narrow_mode)
{
	m_draw_data.show_volume_adj = false;
	CRect redraw_rect{ m_draw_data.draw_rect };
	if (!narrow_mode)	//����ͨ����ģʽ��
	{
		if (cx < m_ui_data.client_width)	//��������ȱ�խ��
		{
			//���½���ͼ�����Ҳ�����ľ����������Ϊ�Ի��򱳾�ɫ
			redraw_rect.left = cx / 2 - 3 * m_pLayout->margin;
			redraw_rect.right = m_ui_data.client_width / 2 + m_pLayout->margin;
			m_pDC->FillSolidRect(redraw_rect, GetSysColor(COLOR_BTNFACE));
		}
		else if (cy < m_ui_data.client_height)	//�������߶ȱ�С��
		{
			//���½���ͼ�����·�����ľ����������Ϊ�Ի��򱳾�ɫ
			redraw_rect.top = cy - 2 * m_pLayout->margin;
			redraw_rect.bottom = cy;
			m_pDC->FillSolidRect(redraw_rect, GetSysColor(COLOR_BTNFACE));
		}
	}
	else if (narrow_mode && cx < m_ui_data.client_width)	//��խ����ģʽ�£������ȱ�խ��
	{
		//���½���ͼ�����Ҳ�����ľ����������Ϊ�Ի��򱳾�ɫ
		redraw_rect.left = cx - 2 * m_pLayout->margin;
		redraw_rect.right = cx;
		m_pDC->FillSolidRect(redraw_rect, GetSysColor(COLOR_BTNFACE));
	}

}

CRect CPlayerUI::GetThumbnailClipArea(bool narrow_mode)
{
	CRect info_rect;
	if (!narrow_mode)
		info_rect = CRect{ CPoint{ m_pLayout->margin, m_pLayout->control_bar_height + m_pLayout->margin + theApp.DPI(20) }, CSize{ m_ui_data.client_width / 2 - 2 * m_pLayout->margin, m_pLayout->info_height2 - 3 * m_pLayout->margin } };
	else
		info_rect = CRect{ CPoint{ m_pLayout->margin, m_pLayout->control_bar_height + m_pLayout->progress_bar_height + theApp.DPI(20) }, CSize{ m_ui_data.client_width - 2 * m_pLayout->margin, m_pLayout->info_height - 2 * m_pLayout->margin } };

	return info_rect;
}

