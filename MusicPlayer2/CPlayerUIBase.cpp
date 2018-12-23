#include "stdafx.h"
#include "CPlayerUIBase.h"


CPlayerUIBase::CPlayerUIBase(UIData& ui_data)
	:m_ui_data(ui_data)
{
	m_popup_menu.LoadMenu(IDR_LYRIC_POPUP_MENU);	//װ�ظ���Ҽ��˵�
	m_main_popup_menu.LoadMenu(IDR_MAIN_POPUP_MENU);
}


CPlayerUIBase::~CPlayerUIBase()
{
}

void CPlayerUIBase::SetToolTip(CToolTipCtrl * pToolTip)
{
	m_tool_tip = pToolTip;
}

void CPlayerUIBase::DrawInfo(bool reset)
{
	//������ɫ
	if (theApp.m_app_setting_data.dark_mode)
	{
		m_colors.color_text = ColorTable::WHITE;
		m_colors.color_text_lable = theApp.m_app_setting_data.theme_color.light2;
		m_colors.color_text_2 = theApp.m_app_setting_data.theme_color.light1;
		m_colors.color_text_heighlight = theApp.m_app_setting_data.theme_color.light2;
		m_colors.color_back = GRAY(96);
		m_colors.color_lyric_back = theApp.m_app_setting_data.theme_color.dark3;
		m_colors.color_control_bar_back = theApp.m_app_setting_data.theme_color.dark2;
		m_colors.color_spectrum = theApp.m_app_setting_data.theme_color.light2;
		m_colors.color_spectrum_cover = theApp.m_app_setting_data.theme_color.original_color;
		m_colors.color_spectrum_back = theApp.m_app_setting_data.theme_color.dark1;
		m_colors.color_button_back = theApp.m_app_setting_data.theme_color.dark3;

		m_colors.background_transparency = theApp.m_app_setting_data.background_transparency;
	}
	else
	{
		m_colors.color_text = theApp.m_app_setting_data.theme_color.dark2;
		m_colors.color_text_lable = theApp.m_app_setting_data.theme_color.original_color;
		m_colors.color_text_2 = theApp.m_app_setting_data.theme_color.light1;
		m_colors.color_text_heighlight = theApp.m_app_setting_data.theme_color.dark1;
		m_colors.color_back = ColorTable::WHITE;
		m_colors.color_lyric_back = theApp.m_app_setting_data.theme_color.light3;
		m_colors.color_control_bar_back = theApp.m_app_setting_data.theme_color.light3;
		m_colors.color_spectrum = theApp.m_app_setting_data.theme_color.original_color;
		m_colors.color_spectrum_cover = theApp.m_app_setting_data.theme_color.original_color;
		m_colors.color_spectrum_back = theApp.m_app_setting_data.theme_color.light3;
		m_colors.color_button_back = theApp.m_app_setting_data.theme_color.light2;

		m_colors.background_transparency = theApp.m_app_setting_data.background_transparency;
	}

	//if (m_repeat_mode_tip.IsEmpty())
	SetRepeatModeToolTipText();

	//�����ı��߶�
	m_pDC->SelectObject(&m_ui_data.lyric_font);
	m_lyric_text_height = m_pDC->GetTextExtent(L"��").cy;	//���ݵ�ǰ���������ü����ı��ĸ߶�

}

void CPlayerUIBase::RButtonUp(CPoint point)
{
	if (m_volume_btn.rect.PtInRect(point) == FALSE)
		m_show_volume_adj = false;

	CPoint point1;		//����һ������ȷ�����λ�õ�λ��  
	GetCursorPos(&point1);	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը����꣬��λ������Ļ���Ͻǵ�Ϊԭ�㣬point���Կͻ������Ͻ�Ϊԭ��
	if (m_repetemode_btn.rect.PtInRect(point))		//����ڡ�ѭ��ģʽ���ľ��������ڵ������Ҽ����򵯳���ѭ��ģʽ�����Ӳ˵�
	{
		CMenu* pMenu = m_main_popup_menu.GetSubMenu(0)->GetSubMenu(1);
		if (pMenu != NULL)
			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, theApp.m_pMainWnd);
	}

}

void CPlayerUIBase::MouseMove(CPoint point)
{
	m_repetemode_btn.hover = (m_repetemode_btn.rect.PtInRect(point) != FALSE);		//������ƶ�����ѭ��ģʽ�����ڵľ��ο���ʱ����m_draw_data.repetemode_hover��Ϊtrue
	m_volume_btn.hover = (m_volume_btn.rect.PtInRect(point) != FALSE);
	m_skin_btn.hover = (m_skin_btn.rect.PtInRect(point) != FALSE);
	m_translate_btn.hover = (m_translate_btn.rect.PtInRect(point) != FALSE);
	m_eq_btn.hover = (m_eq_btn.rect.PtInRect(point) != FALSE);
	m_setting_btn.hover = (m_setting_btn.rect.PtInRect(point) != FALSE);

	//��ʾѭ��ģʽ�������ʾ
	static bool last_repetemode_hover{ false };
	AddMouseToolTip(m_repetemode_btn, m_repeat_mode_tip, &last_repetemode_hover);

	//��ʾ��ʷ���������ʾ
	static bool last_translate_hover{ false };
	AddMouseToolTip(m_translate_btn, _T("��ʾ��ʷ���"), &last_translate_hover);

	//��ʾ�����������ʾ
	static bool last_volume_hover{ false };
	AddMouseToolTip(m_volume_btn, _T("�����ֵ�������"), &last_volume_hover);

	static bool last_skin_hover{ false };
	AddMouseToolTip(m_skin_btn, _T("�л�����"), &last_skin_hover);

	static bool last_eq_hover{ false };
	AddMouseToolTip(m_eq_btn, _T("��Ч�趨"), &last_eq_hover);

	static bool last_setting_hover{ false };
	AddMouseToolTip(m_setting_btn, _T("����"), &last_setting_hover);
}

void CPlayerUIBase::LButtonUp(CPoint point)
{
	if (m_repetemode_btn.rect.PtInRect(point))	//����ˡ�ѭ��ģʽ��ʱ������ѭ��ģʽ
	{
		theApp.m_player.SetRepeatMode();

		SetRepeatModeToolTipText();
		//m_tool_tip->UpdateTipText(m_repeat_mode_tip, theApp.m_pMainWnd);
		m_tool_tip->AddTool(theApp.m_pMainWnd, m_repeat_mode_tip);
		m_tool_tip->Pop();

	}

	if (!m_show_volume_adj)		//���������ʾ����������ť�����������������ʾ����������ť
		m_show_volume_adj = (m_volume_btn.rect.PtInRect(point) != FALSE);
	else		//����Ѿ���ʾ������������ť��������������ʱ��������������ť����ʾ
		m_show_volume_adj = (m_volume_up_rect.PtInRect(point) || m_volume_down_rect.PtInRect(point));

	if (m_show_volume_adj && m_volume_up_rect.PtInRect(point))	//�������������ť�е�������ʱ��������
	{
		theApp.m_player.MusicControl(Command::VOLUME_UP, theApp.m_nc_setting_data.volum_step);
	}
	if (m_show_volume_adj && m_volume_down_rect.PtInRect(point))	//�������������ť�е�������ʱ������С
	{
		theApp.m_player.MusicControl(Command::VOLUME_DOWN, theApp.m_nc_setting_data.volum_step);
	}

	if (m_skin_btn.rect.PtInRect(point))
	{
		m_skin_btn.hover = false;
		theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_SWITCH_UI);
	}

	if (m_eq_btn.rect.PtInRect(point))
	{
		m_eq_btn.hover = false;
		theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_EQUALIZER);
	}

	if (m_setting_btn.rect.PtInRect(point))
	{
		m_setting_btn.hover = false;
		theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_OPTION_SETTINGS);
	}

}

void CPlayerUIBase::OnSizeRedraw(int cx, int cy)
{
}

void CPlayerUIBase::DrawLyricTextMultiLine(CRect lyric_area, bool midi_lyric)
{
	int lyric_height = m_lyric_text_height + theApp.m_app_setting_data.lyric_line_space;			//�ı��߶ȼ����м��
	int lyric_height2 = lyric_height * 2 + theApp.m_app_setting_data.lyric_line_space;		//��������ĸ�ʸ߶�

	m_draw.SetFont(&m_ui_data.lyric_font);
	if (midi_lyric)
	{
		wstring current_lyric{ theApp.m_player.GetMidiLyric() };
		m_draw.DrawWindowText(lyric_area, current_lyric.c_str(), m_colors.color_text, Alignment::CENTER, false, true);
	}
	else if (theApp.m_player.m_Lyrics.IsEmpty())
	{
		m_draw.DrawWindowText(lyric_area, _T("��ǰ����û�и��"), m_colors.color_text_2, Alignment::CENTER);
	}
	else
	{
		//CRect arect{ lyric_area };		//һ�и�ʵľ�������
		//arect.bottom = arect.top + lyric_height;
		//vector<CRect> rects(theApp.m_player.m_Lyrics.GetLyricCount() + 1, arect);
		//Ϊÿһ���ʴ���һ�����Σ�������������
		vector<CRect> rects;
		int lyric_count = theApp.m_player.m_Lyrics.GetLyricCount() + 1;		//��ȡ������������ڵ�һ�и����Ҫ��ʾ���⣬��������Ҫ+1��
		for (int i{}; i < lyric_count; i++)
		{
			CRect arect{ lyric_area };
			if (!theApp.m_player.m_Lyrics.GetLyric(i).translate.empty() && m_ui_data.show_translate)
				arect.bottom = arect.top + lyric_height2;
			else
				arect.bottom = arect.top + lyric_height;
			rects.push_back(arect);
		}
		int center_pos = (lyric_area.top + lyric_area.bottom) / 2;		//������������y����
		Time time{ theApp.m_player.GetCurrentPosition() };		//��ǰ����ʱ��
		int lyric_index = theApp.m_player.m_Lyrics.GetLyricIndex(time) + 1;		//��ǰ��ʵ���ţ���ʵĵ�һ��GetLyricIndex���ص���0��������ʾʱ��һ����Ҫ��ʾ���⣬��������Ҫ+1��
		int progress = theApp.m_player.m_Lyrics.GetLyricProgress(time);		//��ǰ��ʽ��ȣ���ΧΪ0~1000��
		int y_progress;			//��ǰ�����y���ϵĽ���
		if (!theApp.m_player.m_Lyrics.GetLyric(lyric_index).translate.empty() && m_ui_data.show_translate)
			y_progress = progress * lyric_height2 / 1000;
		else
			y_progress = progress * lyric_height / 1000;
		//int start_pos = center_pos - y_progress - (lyric_index + 1)*lyric_height;		//��1���ʵ���ʼy����
		//�����1���ʵ���ʼy����
		//���ڵ�ǰ�����Ҫ��ʾ�ڸ�����������λ�ã���˴�����λ�ÿ�ʼ����ȥ��ǰ�����Y���ϵĽ���
		//�����μ�ȥ֮ǰÿһ���ʵĸ߶ȣ����õ��˵�һ���ʵ���ʼλ��
		int start_pos;
		start_pos = center_pos - y_progress;
		for (int i{ lyric_index - 1 }; i >= 0; i--)
		{
			if (!theApp.m_player.m_Lyrics.GetLyric(i).translate.empty() && m_ui_data.show_translate)
				start_pos -= lyric_height2;
			else
				start_pos -= lyric_height;
		}

		//���λ���ÿһ����
		for (int i{}; i < rects.size(); i++)
		{
			//����ÿһ���ʵ�λ��
			if (i == 0)
				rects[i].MoveToY(start_pos);
			else
				rects[i].MoveToY(rects[i - 1].bottom);
			//���Ƹ���ı�
			if (!(rects[i] & lyric_area).IsRectEmpty())		//ֻ�е�һ���ʵľ�������͸������ľ����н���ʱ���Ż��Ƹ��
			{
				//���ø���ı��ͷ����ı��ľ�������
				CRect rect_text{ rects[i] };
				CRect rect_translate;
				if (!theApp.m_player.m_Lyrics.GetLyric(i).translate.empty() && m_ui_data.show_translate)
				{
					rect_text.MoveToY(rect_text.top + theApp.m_app_setting_data.lyric_line_space);
					rect_text.bottom = rect_text.top + m_lyric_text_height;
					rect_translate = rect_text;
					rect_translate.MoveToY(rect_text.bottom + theApp.m_app_setting_data.lyric_line_space);
				}

				if (i == lyric_index)		//�������ڲ��ŵĸ��
				{
					//���Ƹ���ı�
					m_draw.SetFont(&m_ui_data.lyric_font);
					if (theApp.m_lyric_setting_data.lyric_karaoke_disp)
						m_draw.DrawWindowText(rect_text, theApp.m_player.m_Lyrics.GetLyric(i).text.c_str(), m_colors.color_text, m_colors.color_text_2, progress, true, true);
					else
						m_draw.DrawWindowText(rect_text, theApp.m_player.m_Lyrics.GetLyric(i).text.c_str(), m_colors.color_text, m_colors.color_text, progress, true, true);
					//���Ʒ����ı�
					if (!theApp.m_player.m_Lyrics.GetLyric(i).translate.empty() && m_ui_data.show_translate)
					{
						m_draw.SetFont(&m_ui_data.lyric_translate_font);
						m_draw.DrawWindowText(rect_translate, theApp.m_player.m_Lyrics.GetLyric(i).translate.c_str(), m_colors.color_text, m_colors.color_text, progress, true, true);
					}
				}
				else		//���Ʒ����ڲ��ŵĸ��
				{
					//���Ƹ���ı�
					m_draw.SetFont(&m_ui_data.lyric_font);
					m_draw.DrawWindowText(rect_text, theApp.m_player.m_Lyrics.GetLyric(i).text.c_str(), m_colors.color_text_2, Alignment::CENTER, true);
					//���Ʒ����ı�
					if (!theApp.m_player.m_Lyrics.GetLyric(i).translate.empty() && m_ui_data.show_translate)
					{
						m_draw.SetFont(&m_ui_data.lyric_translate_font);
						m_draw.DrawWindowText(rect_translate, theApp.m_player.m_Lyrics.GetLyric(i).translate.c_str(), m_colors.color_text_2, Alignment::CENTER, true);
					}
				}
			}
		}
	}

}

void CPlayerUIBase::DrawLyricTextSingleLine(CRect rect, bool midi_lyric)
{
	m_draw.SetFont(&m_ui_data.lyric_font);
	if (midi_lyric)
	{
		wstring current_lyric{ theApp.m_player.GetMidiLyric() };
		m_draw.DrawWindowText(rect, current_lyric.c_str(), m_colors.color_text, Alignment::CENTER, false, true);
	}
	else if (theApp.m_player.m_Lyrics.IsEmpty())
	{
		m_draw.DrawWindowText(rect, _T("��ǰ����û�и��"), m_colors.color_text_2, Alignment::CENTER);
	}
	else
	{
		CRect lyric_rect = rect;
		CLyrics::Lyric current_lyric{ theApp.m_player.m_Lyrics.GetLyric(Time(theApp.m_player.GetCurrentPosition()), 0) };	//��ȡ�����
		if (current_lyric.text.empty())		//�����ǰ���Ϊ�հף�����ʾΪʡ�Ժ�
			current_lyric.text = DEFAULT_LYRIC_TEXT;
		int progress{ theApp.m_player.m_Lyrics.GetLyricProgress(Time(theApp.m_player.GetCurrentPosition())) };		//��ȡ��ǰ��ʽ��ȣ���ΧΪ0~1000��

		if ((!theApp.m_player.m_Lyrics.IsTranslated() || !m_ui_data.show_translate) && rect.Height() > static_cast<int>(m_lyric_text_height*1.73))
		{
			wstring next_lyric_text = theApp.m_player.m_Lyrics.GetLyric(Time(theApp.m_player.GetCurrentPosition()), 1).text;
			if (next_lyric_text.empty())
				next_lyric_text = DEFAULT_LYRIC_TEXT;
			DrawLyricDoubleLine(lyric_rect, current_lyric.text.c_str(), next_lyric_text.c_str(), progress);
		}
		else
		{
			if (m_ui_data.show_translate && !current_lyric.translate.empty() && rect.Height() > static_cast<int>(m_lyric_text_height*1.73))
			{
				lyric_rect.bottom = lyric_rect.top + rect.Height() / 2;
				CRect translate_rect = lyric_rect;
				translate_rect.MoveToY(lyric_rect.bottom);

				m_draw.SetFont(&m_ui_data.lyric_translate_font);
				m_draw.DrawWindowText(translate_rect, current_lyric.translate.c_str(), m_colors.color_text, m_colors.color_text, progress, true, true);
			}

			m_draw.SetFont(&m_ui_data.lyric_font);
			if (theApp.m_lyric_setting_data.lyric_karaoke_disp)
				m_draw.DrawWindowText(lyric_rect, current_lyric.text.c_str(), m_colors.color_text, m_colors.color_text_2, progress, true, true);
			else
				m_draw.DrawWindowText(lyric_rect, current_lyric.text.c_str(), m_colors.color_text, m_colors.color_text, progress, true, true);
		}

		m_draw.SetFont(theApp.m_pMainWnd->GetFont());
	}

}

void CPlayerUIBase::DrawSongInfo(CRect rect, bool reset)
{
	wchar_t buff[64];
	if (theApp.m_player.m_loading)
	{
		static CDrawCommon::ScrollInfo scroll_info0;
		CString info;
		info.Format(_T("�ҵ�%d�׸��������ڶ�ȡ��Ƶ�ļ���Ϣ�������%d%%�����Ժ򡭡�"), theApp.m_player.GetSongNum(), theApp.m_player.m_thread_info.process_percent);
		m_draw.DrawScrollText(rect, info, m_colors.color_text, theApp.DPI(1.5), false, scroll_info0, reset);
	}
	else
	{
		//���Ʋ���״̬
		CRect rc_tmp{ rect };
		rc_tmp.right = rc_tmp.left + theApp.DPI(52);
		m_draw.DrawWindowText(rc_tmp, theApp.m_player.GetPlayingState().c_str(), m_colors.color_text_lable);

		//���Ƹ������
		rc_tmp.MoveToX(rc_tmp.right);
		rc_tmp.right = rc_tmp.left + theApp.DPI(30);
		swprintf_s(buff, sizeof(buff) / 2, L"%.3d", theApp.m_player.GetIndex() + 1);
		m_draw.DrawWindowText(rc_tmp, buff, m_colors.color_text_2);

		//�����ļ���
		rc_tmp.MoveToX(rc_tmp.right);
		rc_tmp.right = rect.right;
		static CDrawCommon::ScrollInfo scroll_info1;
		m_draw.DrawScrollText(rc_tmp, theApp.m_player.GetFileName().c_str(), m_colors.color_text, theApp.DPI(1.5), false, scroll_info1, reset);
	}
}

void CPlayerUIBase::DrawControlBar(bool draw_background, CRect rect, bool draw_translate_button, UIData* pUIData)
{
	//���Ʊ���
	if (draw_background)
		m_draw.FillAlphaRect(rect, m_colors.color_control_bar_back, ALPHA_CHG(m_colors.background_transparency));
	else
		m_draw.FillRect(rect, m_colors.color_control_bar_back);

	CRect rc_tmp = rect;

	//����ѭ��ģʽ
	rc_tmp.right = rect.left + rect.Height();
	CRect rc_repeat_mode = rc_tmp;
	rc_repeat_mode.DeflateRect(theApp.DPI(2), theApp.DPI(2));
	m_draw.SetDrawArea(rc_repeat_mode);

	BYTE alpha;
	if (draw_background)
		alpha = ALPHA_CHG(m_colors.background_transparency);
	else
		alpha = 255;
	if (m_repetemode_btn.hover)
		m_draw.FillAlphaRect(rc_repeat_mode, m_colors.color_text_2, alpha);
	//else if (!theApp.m_app_setting_data.dark_mode)
	//	m_draw.FillAlphaRect(rc_repeat_mode, m_colors.color_button_back, alpha);

	m_repetemode_btn.rect = DrawAreaToClient(rc_repeat_mode, m_draw_rect);

	rc_repeat_mode = rc_tmp;
	rc_repeat_mode.DeflateRect(theApp.DPI(4), theApp.DPI(4));

		switch (theApp.m_player.GetRepeatMode())
		{
		case RepeatMode::RM_PLAY_ORDER:
			m_draw.DrawIcon(theApp.m_play_oder_icon.GetIcon(!theApp.m_app_setting_data.dark_mode), rc_repeat_mode.TopLeft(), rc_repeat_mode.Size());
			break;
		case RepeatMode::RM_LOOP_PLAYLIST:
			m_draw.DrawIcon(theApp.m_loop_playlist_icon.GetIcon(!theApp.m_app_setting_data.dark_mode), rc_repeat_mode.TopLeft(), rc_repeat_mode.Size());
			break;
		case RepeatMode::RM_LOOP_TRACK:
			m_draw.DrawIcon(theApp.m_loop_track_icon.GetIcon(!theApp.m_app_setting_data.dark_mode), rc_repeat_mode.TopLeft(), rc_repeat_mode.Size());
			break;
		case RepeatMode::RM_PLAY_SHUFFLE:
			m_draw.DrawIcon(theApp.m_play_shuffle_icon.GetIcon(!theApp.m_app_setting_data.dark_mode), rc_repeat_mode.TopLeft(), rc_repeat_mode.Size());
			break;
		}
	

	//�������ð�ť
	rc_tmp.MoveToX(rc_tmp.right);
	DrawUIButton(rc_tmp, m_setting_btn, theApp.m_setting_icon.GetIcon(!theApp.m_app_setting_data.dark_mode), draw_background);

	//���ƾ�������ť
	rc_tmp.MoveToX(rc_tmp.right);
	DrawUIButton(rc_tmp, m_eq_btn, theApp.m_eq_icon.GetIcon(!theApp.m_app_setting_data.dark_mode), draw_background);

	//�����л����水ť
	rc_tmp.MoveToX(rc_tmp.right);
	DrawUIButton(rc_tmp, m_skin_btn, theApp.m_skin_icon.GetIcon(!theApp.m_app_setting_data.dark_mode), draw_background);

	//���Ʒ��밴ť
	if (draw_translate_button && rect.Width()>=theApp.DPI(192))
	{
		rc_tmp.MoveToX(rc_tmp.right);
		CRect translate_rect = rc_tmp;
		translate_rect.DeflateRect(theApp.DPI(2), theApp.DPI(2));
		m_translate_btn.enable = theApp.m_player.m_Lyrics.IsTranslated();
		if (m_translate_btn.enable)
		{
			BYTE alpha;
			if (draw_background)
				alpha = ALPHA_CHG(m_colors.background_transparency);
			else
				alpha = 255;
			if (m_translate_btn.hover)
				m_draw.FillAlphaRect(translate_rect, m_colors.color_text_2, alpha);
			else if (pUIData!=nullptr && pUIData->show_translate)
				m_draw.FillAlphaRect(translate_rect, m_colors.color_button_back, alpha);
			m_draw.DrawWindowText(translate_rect, L"��", m_colors.color_text, Alignment::CENTER);
		}
		else
		{
			m_draw.DrawWindowText(translate_rect, L"��", GRAY(200), Alignment::CENTER);
		}
		m_translate_btn.rect = DrawAreaToClient(translate_rect, m_draw_rect);

	}

	//��ʾ<<<<
	rc_tmp.left = rc_tmp.right = rect.right;
	if (rect.Width() >= theApp.DPI(265))
	{
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
		rc_tmp.right = rect.right;
		rc_tmp.left = rc_tmp.right - theApp.DPI(44);
		m_draw.DrawWindowText(rc_tmp, _T("<<<<"), m_colors.color_text, m_colors.color_text_2, progress, false);
	}


	//��ʾ����
	wchar_t buff[64];
	rc_tmp.right = rc_tmp.left;
	rc_tmp.left = rc_tmp.right - theApp.DPI(72);
	swprintf_s(buff, L"������%d%%", theApp.m_player.GetVolume());
	if (m_volume_btn.hover)		//���ָ����������ʱ��������һ����ɫ��ʾ
		m_draw.DrawWindowText(rc_tmp, buff, m_colors.color_text_heighlight);
	else
		m_draw.DrawWindowText(rc_tmp, buff, m_colors.color_text);
	//��������������ť��λ��
	m_volume_btn.rect = DrawAreaToClient(rc_tmp, m_draw_rect);
	m_volume_btn.rect.DeflateRect(0, theApp.DPI(4));
	m_volume_btn.rect.right -= theApp.DPI(12);
	m_volume_down_rect = m_volume_btn.rect;
	m_volume_down_rect.bottom += theApp.DPI(4);
	m_volume_down_rect.MoveToY(m_volume_btn.rect.bottom);
	m_volume_down_rect.right = m_volume_btn.rect.left + m_volume_btn.rect.Width() / 2;
	m_volume_up_rect = m_volume_down_rect;
	m_volume_up_rect.MoveToX(m_volume_down_rect.right);


}

void CPlayerUIBase::AddMouseToolTip(const UIButton & btn, LPCTSTR str, bool* last_hover)
{
	if (!*last_hover && btn.hover)
	{
		m_tool_tip->AddTool(theApp.m_pMainWnd, str);
		m_tool_tip->SetMaxTipWidth(theApp.DPI(400));
		m_tool_tip->Pop();
	}
	if (*last_hover && !btn.hover)
	{
		m_tool_tip->AddTool(theApp.m_pMainWnd, _T(""));
		m_tool_tip->Pop();
	}
	*last_hover = btn.hover;

}

CRect CPlayerUIBase::DrawAreaToClient(CRect rect, CRect draw_area)
{
	rect.MoveToXY(rect.left + draw_area.left, rect.top + draw_area.top);
	return rect;
}

CRect CPlayerUIBase::ClientAreaToDraw(CRect rect, CRect draw_area)
{
	rect.MoveToXY(rect.left - draw_area.left, rect.top - draw_area.top);
	return rect;
}

void CPlayerUIBase::DrawLyricDoubleLine(CRect rect, LPCTSTR lyric, LPCTSTR next_lyric, int progress)
{
	m_draw.SetFont(&m_ui_data.lyric_font);
	static bool swap;
	static int last_progress;
	if (last_progress > progress)		//�����ǰ�ĸ�ʽ��ȱ��ϴε�С��˵������л�������һ��
	{
		swap = !swap;
	}
	last_progress = progress;


	CRect up_rect{ rect }, down_rect{ rect };		//�ϰ벿�ֺ��°벿�ָ�ʵľ�������
	up_rect.bottom = up_rect.top + (up_rect.Height() / 2);
	down_rect.top = down_rect.bottom - (down_rect.Height() / 2);
	//������һ���ʵ��ı�������Ҫ�Ŀ�ȣ��Ӷ�ʵ����һ�и���Ҷ���
	m_draw.GetDC()->SelectObject(&m_ui_data.lyric_font);
	int width;
	if (!swap)
		width = m_draw.GetDC()->GetTextExtent(next_lyric).cx;
	else
		width = m_draw.GetDC()->GetTextExtent(lyric).cx;
	if (width < rect.Width())
		down_rect.left = down_rect.right - width;

	COLORREF color1, color2;
	if (theApp.m_lyric_setting_data.lyric_karaoke_disp)
	{
		color1 = m_colors.color_text;
		color2 = m_colors.color_text_2;
	}
	else
	{
		color1 = color2 = m_colors.color_text;
	}

	if (!swap)
	{
		m_draw.DrawWindowText(up_rect, lyric, color1, color2, progress, false);
		m_draw.DrawWindowText(down_rect, next_lyric, m_colors.color_text_2);
	}
	else
	{
		m_draw.DrawWindowText(up_rect, next_lyric, m_colors.color_text_2);
		m_draw.DrawWindowText(down_rect, lyric, color1, color2, progress, false);
	}

}

void CPlayerUIBase::DrawUIButton(CRect rect, UIButton & btn, HICON icon, bool draw_background)
{
	CRect rc_tmp = rect;
	rc_tmp.DeflateRect(theApp.DPI(2), theApp.DPI(2));
	m_draw.SetDrawArea(rc_tmp);

	BYTE alpha;
	if (draw_background)
		alpha = ALPHA_CHG(m_colors.background_transparency);
	else
		alpha = 255;
	if (btn.hover)
		m_draw.FillAlphaRect(rc_tmp, m_colors.color_text_2, alpha);
	//else if (!theApp.m_app_setting_data.dark_mode)
	//	m_draw.FillAlphaRect(rc_tmp, m_colors.color_button_back, alpha);

	btn.rect = DrawAreaToClient(rc_tmp, m_draw_rect);

	rc_tmp = rect;
	rc_tmp.DeflateRect(theApp.DPI(4), theApp.DPI(4));
	m_draw.DrawIcon(icon, rc_tmp.TopLeft(), rc_tmp.Size());

}

void CPlayerUIBase::SetRepeatModeToolTipText()
{
	switch (theApp.m_player.GetRepeatMode())
	{
	case RepeatMode::RM_PLAY_ORDER:
		m_repeat_mode_tip = _T("ѭ��ģʽ��˳�򲥷�");
		break;
	case RepeatMode::RM_LOOP_PLAYLIST:
		m_repeat_mode_tip = _T("ѭ��ģʽ���б�ѭ��");
		break;
	case RepeatMode::RM_LOOP_TRACK:
		m_repeat_mode_tip = _T("ѭ��ģʽ������ѭ��");
		break;
	case RepeatMode::RM_PLAY_SHUFFLE:
		m_repeat_mode_tip = _T("ѭ��ģʽ���������");
		break;
	}
}

void CPlayerUIBase::DrawVolumnAdjBtn(bool draw_background)
{
	if (m_show_volume_adj)
	{
		CRect volume_down_rect = ClientAreaToDraw(m_volume_down_rect, m_draw_rect);
		CRect volume_up_rect = ClientAreaToDraw(m_volume_up_rect, m_draw_rect);

		if (draw_background)
		{
			m_draw.FillAlphaRect(volume_down_rect, m_colors.color_text_2, ALPHA_CHG(m_colors.background_transparency));
			m_draw.FillAlphaRect(volume_up_rect, m_colors.color_text_2, ALPHA_CHG(m_colors.background_transparency));
		}
		else
		{
			m_draw.FillRect(volume_down_rect, m_colors.color_text_2);
			m_draw.FillRect(volume_up_rect, m_colors.color_text_2);
		}
		m_draw.DrawWindowText(volume_down_rect, L"-", ColorTable::WHITE, Alignment::CENTER);
		m_draw.DrawWindowText(volume_up_rect, L"+", ColorTable::WHITE, Alignment::CENTER);
	}
}

