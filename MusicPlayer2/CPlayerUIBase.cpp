#include "stdafx.h"
#include "CPlayerUIBase.h"


CPlayerUIBase::CPlayerUIBase()
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

void CPlayerUIBase::DrawInfo(bool narrow_mode, bool reset)
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
}

void CPlayerUIBase::RButtonUp(CPoint point, bool narrow_mode)
{
	if (volume_btn.rect.PtInRect(point) == FALSE)
		show_volume_adj = false;

	CPoint point1;		//����һ������ȷ�����λ�õ�λ��  
	GetCursorPos(&point1);	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը����꣬��λ������Ļ���Ͻǵ�Ϊԭ�㣬point���Կͻ������Ͻ�Ϊԭ��
	if (repetemode_btn.rect.PtInRect(point))		//����ڡ�ѭ��ģʽ���ľ��������ڵ������Ҽ����򵯳���ѭ��ģʽ�����Ӳ˵�
	{
		CMenu* pMenu = m_main_popup_menu.GetSubMenu(0)->GetSubMenu(1);
		if (pMenu != NULL)
			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, theApp.m_pMainWnd);
	}

}

void CPlayerUIBase::MouseMove(CPoint point)
{
	repetemode_btn.hover = (repetemode_btn.rect.PtInRect(point) != FALSE);		//������ƶ�����ѭ��ģʽ�����ڵľ��ο���ʱ����m_draw_data.repetemode_hover��Ϊtrue
	volume_btn.hover = (volume_btn.rect.PtInRect(point) != FALSE);
	skin_btn.hover = (skin_btn.rect.PtInRect(point) != FALSE);
	translate_btn.hover = (translate_btn.rect.PtInRect(point) != FALSE);

	//��ʾ��ʷ���������ʾ
	static bool last_translate_hover{ false };
	AddMouseToolTip(translate_btn, _T("��ʾ��ʷ���"), &last_translate_hover);

	//��ʾ�����������ʾ
	static bool last_volume_hover{ false };
	AddMouseToolTip(volume_btn, _T("�����ֵ�������"), &last_volume_hover);

	static bool last_skin_hover{ false };
	AddMouseToolTip(skin_btn, _T("�л�����"), &last_skin_hover);
}

void CPlayerUIBase::LButtonUp(CPoint point)
{
	if (repetemode_btn.rect.PtInRect(point))	//����ˡ�ѭ��ģʽ��ʱ������ѭ��ģʽ
	{
		theApp.m_player.SetRepeatMode();
	}

	if (!show_volume_adj)		//���������ʾ����������ť�����������������ʾ����������ť
		show_volume_adj = (volume_btn.rect.PtInRect(point) != FALSE);
	else		//����Ѿ���ʾ������������ť��������������ʱ��������������ť����ʾ
		show_volume_adj = (volume_up_rect.PtInRect(point) || volume_down_rect.PtInRect(point));

	if (show_volume_adj && volume_up_rect.PtInRect(point))	//�������������ť�е�������ʱ��������
	{
		theApp.m_player.MusicControl(Command::VOLUME_UP, theApp.m_nc_setting_data.volum_step);
	}
	if (show_volume_adj && volume_down_rect.PtInRect(point))	//�������������ť�е�������ʱ������С
	{
		theApp.m_player.MusicControl(Command::VOLUME_DOWN, theApp.m_nc_setting_data.volum_step);
	}

	if (skin_btn.rect.PtInRect(point))
	{
		theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_SWITCH_UI);
	}

}

void CPlayerUIBase::OnSizeRedraw(int cx, int cy, bool narrow_mode)
{
}

void CPlayerUIBase::DrawLyricTextMultiLine(CRect lyric_area, CFont* font, CFont* translate_font, bool show_translate, bool midi_lyric)
{
	//�����ı��߶�
	m_pDC->SelectObject(font);
	int text_height = m_pDC->GetTextExtent(L"��").cy;	//���ݵ�ǰ���������ü����ı��ĸ߶�
	int lyric_height = text_height + theApp.m_app_setting_data.lyric_line_space;			//�ı��߶ȼ����м��
	int lyric_height2 = lyric_height * 2 + theApp.m_app_setting_data.lyric_line_space;		//��������ĸ�ʸ߶�

	m_draw.SetFont(font);
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
			if (!theApp.m_player.m_Lyrics.GetLyric(i).translate.empty() && show_translate)
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
		if (!theApp.m_player.m_Lyrics.GetLyric(lyric_index).translate.empty() && show_translate)
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
			if (!theApp.m_player.m_Lyrics.GetLyric(i).translate.empty() && show_translate)
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
				if (!theApp.m_player.m_Lyrics.GetLyric(i).translate.empty() && show_translate)
				{
					rect_text.MoveToY(rect_text.top + theApp.m_app_setting_data.lyric_line_space);
					rect_text.bottom = rect_text.top + text_height;
					rect_translate = rect_text;
					rect_translate.MoveToY(rect_text.bottom + theApp.m_app_setting_data.lyric_line_space);
				}

				if (i == lyric_index)		//�������ڲ��ŵĸ��
				{
					//���Ƹ���ı�
					m_draw.SetFont(font);
					if (theApp.m_lyric_setting_data.lyric_karaoke_disp)
						m_draw.DrawWindowText(rect_text, theApp.m_player.m_Lyrics.GetLyric(i).text.c_str(), m_colors.color_text, m_colors.color_text_2, progress, true, true);
					else
						m_draw.DrawWindowText(rect_text, theApp.m_player.m_Lyrics.GetLyric(i).text.c_str(), m_colors.color_text, m_colors.color_text, progress, true, true);
					//���Ʒ����ı�
					if (!theApp.m_player.m_Lyrics.GetLyric(i).translate.empty() && show_translate)
					{
						m_draw.SetFont(translate_font);
						m_draw.DrawWindowText(rect_translate, theApp.m_player.m_Lyrics.GetLyric(i).translate.c_str(), m_colors.color_text, m_colors.color_text, progress, true, true);
					}
				}
				else		//���Ʒ����ڲ��ŵĸ��
				{
					//���Ƹ���ı�
					m_draw.SetFont(font);
					m_draw.DrawWindowText(rect_text, theApp.m_player.m_Lyrics.GetLyric(i).text.c_str(), m_colors.color_text_2, Alignment::CENTER, true);
					//���Ʒ����ı�
					if (!theApp.m_player.m_Lyrics.GetLyric(i).translate.empty() && show_translate)
					{
						m_draw.SetFont(translate_font);
						m_draw.DrawWindowText(rect_translate, theApp.m_player.m_Lyrics.GetLyric(i).translate.c_str(), m_colors.color_text_2, Alignment::CENTER, true);
					}
				}
			}
		}
	}

}

void CPlayerUIBase::DrawLyricTextSingleLine(CRect rect, CFont* font, CFont* translate_font, bool show_translate, bool midi_lyric)
{
	m_draw.SetFont(font);
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

		if ((!theApp.m_player.m_Lyrics.IsTranslated() || !show_translate) && rect.Height() > theApp.DPI(32))
		{
			wstring next_lyric_text = theApp.m_player.m_Lyrics.GetLyric(Time(theApp.m_player.GetCurrentPosition()), 1).text;
			if (next_lyric_text.empty())
				next_lyric_text = DEFAULT_LYRIC_TEXT;
			DrawLyricDoubleLine(lyric_rect, font, current_lyric.text.c_str(), next_lyric_text.c_str(), progress);
		}
		else
		{
			if (show_translate && !current_lyric.translate.empty() && rect.Height()>theApp.DPI(32))
			{
				lyric_rect.bottom = lyric_rect.top + rect.Height() / 2;
				CRect translate_rect = lyric_rect;
				translate_rect.MoveToY(lyric_rect.bottom);

				m_draw.SetFont(translate_font);
				m_draw.DrawWindowText(translate_rect, current_lyric.translate.c_str(), m_colors.color_text, m_colors.color_text, progress, true, true);
			}

			m_draw.SetFont(font);
			if (theApp.m_lyric_setting_data.lyric_karaoke_disp)
				m_draw.DrawWindowText(lyric_rect, current_lyric.text.c_str(), m_colors.color_text, m_colors.color_text_2, progress, true, true);
			else
				m_draw.DrawWindowText(lyric_rect, current_lyric.text.c_str(), m_colors.color_text, m_colors.color_text, progress, true, true);
		}

		m_draw.SetFont(theApp.m_pMainWnd->GetFont());
	}

}

//void CPlayerUIBase::DrawControlBar(bool draw_background, CRect rect, bool draw_translate_button)
//{
//}

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

void CPlayerUIBase::DrawLyricDoubleLine(CRect rect, CFont * font, LPCTSTR lyric, LPCTSTR next_lyric, int progress)
{
	m_draw.SetFont(font);
	static bool swap;
	static int last_progress;
	if (last_progress > progress)
	{
		swap = !swap;
	}
	last_progress = progress;


	//ʹ��m_cortana_draw��ͼ
	CRect up_rect{ rect }, down_rect{ rect };		//�ϰ벿�ֺ��°벿�ָ�ʵľ�������
	up_rect.bottom = up_rect.top + (up_rect.Height() / 2);
	down_rect.top = down_rect.bottom - (down_rect.Height() / 2);
	//������һ���ʵ��ı�������Ҫ�Ŀ�ȣ��Ӷ�ʵ����һ�и���Ҷ���
	m_draw.GetDC()->SelectObject(font);
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

