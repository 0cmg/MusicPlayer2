#include "stdafx.h"
#include "CUIDrawer.h"
#include "MusicPlayer2.h"


CUIDrawer::CUIDrawer(UIColors& colors)
    : m_colors(colors)
{
}


CUIDrawer::~CUIDrawer()
{
}

void CUIDrawer::DrawLryicCommon(CRect rect)
{
    SetDrawArea(rect);

    if (!IsDrawMultiLine(rect.Height()))
        DrawLyricTextSingleLine(rect);
    else
        DrawLyricTextMultiLine(rect);
}

int CUIDrawer::GetLyricTextHeight() const
{
    //�����ı��߶�
    if(!m_for_cortana_lyric)
        m_pDC->SelectObject(&theApp.m_font_set.lyric.GetFont(theApp.m_ui_data.full_screen));
    else
        m_pDC->SelectObject(&theApp.m_font_set.cortana.GetFont());
    return m_pDC->GetTextExtent(L"��").cy;	//���ݵ�ǰ���������ü����ı��ĸ߶�
}

void CUIDrawer::Create(CDC * pDC, CWnd * pMainWnd)
{
    CDrawCommon::Create(pDC, pMainWnd);
}

bool CUIDrawer::IsDrawMultiLine(int height) const
{
    return height >= static_cast<int>(GetLyricTextHeight() * 3.5);
}

void CUIDrawer::SetForCortanaLyric(bool for_cortana_lyric)
{
    m_for_cortana_lyric = for_cortana_lyric;
}

void CUIDrawer::DrawLyricTextMultiLine(CRect lyric_area)
{
    int line_space{};
    if (m_for_cortana_lyric)
    {
        line_space = theApp.DPI(4);
    }
    else
    {
        line_space = theApp.m_app_setting_data.lyric_line_space;
        if (theApp.m_ui_data.full_screen)
            line_space = static_cast<int>(line_space * CONSTVAL::FULL_SCREEN_ZOOM_FACTOR);
    }

    int lyric_height = GetLyricTextHeight() + line_space;			//�ı��߶ȼ����м��
    int lyric_height2 = lyric_height * 2 + line_space;		//��������ĸ�ʸ߶�

    SetLyricFont();
    if (CPlayerUIHelper::IsMidiLyric())
    {
        wstring current_lyric{ CPlayer::GetInstance().GetMidiLyric() };
        DrawWindowText(lyric_area, current_lyric.c_str(), m_colors.color_text, Alignment::CENTER, false, true);
    }
    else if (CPlayer::GetInstance().m_Lyrics.IsEmpty())
    {
        DrawWindowText(lyric_area, CCommon::LoadText(IDS_NO_LYRIC_INFO), m_colors.color_text_2, Alignment::CENTER);
    }
    else
    {
        //CRect arect{ lyric_area };		//һ�и�ʵľ�������
        //arect.bottom = arect.top + lyric_height;
        //vector<CRect> rects(CPlayer::GetInstance().m_Lyrics.GetLyricCount() + 1, arect);
        //Ϊÿһ���ʴ���һ�����Σ�������������
        vector<CRect> rects;
        int lyric_count = CPlayer::GetInstance().m_Lyrics.GetLyricCount() + 1;		//��ȡ������������ڵ�һ�и����Ҫ��ʾ���⣬��������Ҫ+1��
        for (int i{}; i < lyric_count; i++)
        {
            CRect arect{ lyric_area };
            if (!CPlayer::GetInstance().m_Lyrics.GetLyric(i).translate.empty() && theApp.m_ui_data.show_translate)
                arect.bottom = arect.top + lyric_height2;
            else
                arect.bottom = arect.top + lyric_height;
            rects.push_back(arect);
        }
        int center_pos = (lyric_area.top + lyric_area.bottom) / 2;		//������������y����
        Time time{ CPlayer::GetInstance().GetCurrentPosition() };		//��ǰ����ʱ��
        int lyric_index = CPlayer::GetInstance().m_Lyrics.GetLyricIndex(time) + 1;		//��ǰ��ʵ���ţ���ʵĵ�һ��GetLyricIndex���ص���0��������ʾʱ��һ����Ҫ��ʾ���⣬��������Ҫ+1��
        int progress = CPlayer::GetInstance().m_Lyrics.GetLyricProgress(time);		//��ǰ��ʽ��ȣ���ΧΪ0~1000��
        int y_progress;			//��ǰ�����y���ϵĽ���
        if (!CPlayer::GetInstance().m_Lyrics.GetLyric(lyric_index).translate.empty() && theApp.m_ui_data.show_translate)
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
            if (!CPlayer::GetInstance().m_Lyrics.GetLyric(i).translate.empty() && theApp.m_ui_data.show_translate)
                start_pos -= lyric_height2;
            else
                start_pos -= lyric_height;
        }

        //���λ���ÿһ����
        for (size_t i{}; i < rects.size(); i++)
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
                if (!CPlayer::GetInstance().m_Lyrics.GetLyric(i).translate.empty() && theApp.m_ui_data.show_translate)
                {
                    rect_text.MoveToY(rect_text.top + line_space);
                    rect_text.bottom = rect_text.top + GetLyricTextHeight();
                    rect_translate = rect_text;
                    rect_translate.MoveToY(rect_text.bottom + line_space);
                }

                if (i == lyric_index && progress < 1000)		//�������ڲ��ŵĸ��
                {
                    CLyrics::Lyric lyric = CPlayer::GetInstance().m_Lyrics.GetLyric(i);
                    //����ʵ���ı��ӷǸ��������仯������Ч��
                    int last_time_span = time - lyric.time;     //��ǰ���ŵĸ���ѳ�����ʱ��
                    int fade_percent = last_time_span / 8;         //������ɫ�����仯�İٷֱȣ�����Խ�������ʱ��Խ����10��Ϊ1��
                    COLORREF text_color = CColorConvert::GetGradientColor(m_colors.color_text_2, m_colors.color_text, fade_percent);
                    //���Ƹ���ı�
                    SetLyricFont();
                    if (theApp.m_lyric_setting_data.lyric_karaoke_disp)
                        DrawWindowText(rect_text, lyric.text.c_str(), m_colors.color_text, m_colors.color_text_2, progress, true, true);
                    else
                        DrawWindowText(rect_text, lyric.text.c_str(), text_color, text_color, progress, true, true);
                    //���Ʒ����ı�
                    if (!lyric.translate.empty() && theApp.m_ui_data.show_translate)
                    {
                        SetLyricFontTranslated();
                        DrawWindowText(rect_translate, CPlayer::GetInstance().m_Lyrics.GetLyric(i).translate.c_str(), text_color, text_color, progress, true, true);
                    }
                }
                else		//���Ʒ����ڲ��ŵĸ��
                {
                    SetLyricFont();
                    COLORREF text_color;
                    if (i == lyric_index - 1)      //������һ���ʣ�����ʵ����һ������ɫ�Ӹ��������仯���Ǹ���Ч����
                    {
                        CLyrics::Lyric playing_lyric = CPlayer::GetInstance().m_Lyrics.GetLyric(lyric_index);
                        int last_time_span = time - playing_lyric.time;     //��ǰ���ŵĸ���ѳ�����ʱ��
                        int fade_percent = last_time_span / 20;         //������ɫ�����仯�İٷֱȣ�������ʱ��Ϊ2000����ʱΪ100%������ɫ�����仯��ʱ��Ϊ2��
                        text_color = CColorConvert::GetGradientColor(m_colors.color_text, m_colors.color_text_2, fade_percent);
                    }
                    else
                    {
                        text_color = m_colors.color_text_2;
                    }
                    //���Ƹ���ı�
                    DrawWindowText(rect_text, CPlayer::GetInstance().m_Lyrics.GetLyric(i).text.c_str(), text_color, Alignment::CENTER, true);
                    //���Ʒ����ı�
                    if (!CPlayer::GetInstance().m_Lyrics.GetLyric(i).translate.empty() && theApp.m_ui_data.show_translate)
                    {
                        SetLyricFontTranslated();
                        DrawWindowText(rect_translate, CPlayer::GetInstance().m_Lyrics.GetLyric(i).translate.c_str(), text_color, Alignment::CENTER, true);
                    }
                }
            }
        }
    }
}

void CUIDrawer::DrawLyricTextSingleLine(CRect rect, bool double_line)
{
    SetLyricFont();

    if (CPlayerUIHelper::IsMidiLyric())
    {
        wstring current_lyric{ CPlayer::GetInstance().GetMidiLyric() };
        DrawWindowText(rect, current_lyric.c_str(), m_colors.color_text, Alignment::CENTER, false, true);
    }
    else if (CPlayer::GetInstance().m_Lyrics.IsEmpty())
    {
        DrawWindowText(rect, CCommon::LoadText(IDS_NO_LYRIC_INFO), m_colors.color_text_2, Alignment::CENTER);
    }
    else
    {
        CRect lyric_rect = rect;
        Time time = CPlayer::GetInstance().GetCurrentPosition();
        CLyrics::Lyric current_lyric{ CPlayer::GetInstance().m_Lyrics.GetLyric(time, 0) };	//��ȡ�����
        if (current_lyric.text.empty())		//�����ǰ���Ϊ�հף�����ʾΪʡ�Ժ�
            current_lyric.text = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
        int progress{ CPlayer::GetInstance().m_Lyrics.GetLyricProgress(time) };		//��ȡ��ǰ��ʽ��ȣ���ΧΪ0~1000��

        if (double_line && (!CPlayer::GetInstance().m_Lyrics.IsTranslated() || !theApp.m_ui_data.show_translate) && rect.Height() > static_cast<int>(GetLyricTextHeight() * 1.73))
        {
            wstring next_lyric_text = CPlayer::GetInstance().m_Lyrics.GetLyric(time, 1).text;
            if (next_lyric_text.empty())
                next_lyric_text = CCommon::LoadText(IDS_DEFAULT_LYRIC_TEXT);
            //����ʵ���ı��ӷǸ��������仯������Ч��
            int last_time_span = time - current_lyric.time;     //��ǰ���ŵĸ���ѳ�����ʱ��
            int fade_percent = last_time_span / 8;         //������ɫ�����仯�İٷֱȣ�����Խ�������ʱ��Խ����10��Ϊ1��
            DrawLyricDoubleLine(lyric_rect, current_lyric.text.c_str(), next_lyric_text.c_str(), progress, fade_percent);
        }
        else
        {
            if (theApp.m_ui_data.show_translate && !current_lyric.translate.empty() && rect.Height() > static_cast<int>(GetLyricTextHeight() * 1.73))
            {
                lyric_rect.bottom = lyric_rect.top + rect.Height() / 2;
                CRect translate_rect = lyric_rect;
                translate_rect.MoveToY(lyric_rect.bottom);

                SetLyricFontTranslated();
                DrawWindowText(translate_rect, current_lyric.translate.c_str(), m_colors.color_text, m_colors.color_text, progress, true, true);
            }

            SetLyricFont();
            if (theApp.m_lyric_setting_data.lyric_karaoke_disp)
                DrawWindowText(lyric_rect, current_lyric.text.c_str(), m_colors.color_text, m_colors.color_text_2, progress, true, true);
            else
                DrawWindowText(lyric_rect, current_lyric.text.c_str(), m_colors.color_text, m_colors.color_text, progress, true, true);
        }

        SetFont(m_pMainWnd->GetFont());
    }
}

void CUIDrawer::DrawSpectrum(CRect rect, SpectrumCol col, bool draw_reflex /*= false*/)
{
    int cols;		//Ҫ��ʾ��Ƶ�����ε�����
    switch (col)
    {
    case CUIDrawer::SC_64:
        cols = 64;
        break;
    case CUIDrawer::SC_32:
        cols = 32;
        break;
    case CUIDrawer::SC_16:
        cols = 16;
        break;
    case CUIDrawer::SC_8:
        cols = 8;
        break;
    default:
        cols = SPECTRUM_COL;
        break;
    }
    int gap_width{ rect.Width() * (SPECTRUM_COL / cols) / 168 };		//Ƶ�����μ�϶���
    if (gap_width < 1)
        gap_width = 1;
    int width = (rect.Width() - (cols - 1) * gap_width) / (cols - 1);

    DrawSpectrum(rect, width, gap_width, cols, m_colors.color_spectrum, draw_reflex);
}

void CUIDrawer::DrawSpectrum(CRect rect, int col_width, int gap_width, int cols, COLORREF color, bool draw_reflex /*= false*/)
{
    CRect rc_spectrum_top = rect;
    if (draw_reflex)     //���Ҫ���Ƶ�Ӱ����Ӱռ�ܸ߶ȵ�1/3
        rc_spectrum_top.bottom = rect.top + (rect.Height() * 2 / 3);

    CRect rects[SPECTRUM_COL];
    rects[0] = rc_spectrum_top;
    rects[0].right = rects[0].left + col_width;
    for (int i{ 1 }; i < cols; i++)
    {
        rects[i] = rects[0];
        rects[i].left += (i * (col_width + gap_width));
        rects[i].right += (i * (col_width + gap_width));
    }
    for (int i{}; i < cols; i++)
    {
        float spetral_data = CPlayer::GetInstance().GetSpectralData()[i * (SPECTRUM_COL / cols)];
        float peak_data = CPlayer::GetInstance().GetSpectralPeakData()[i * (SPECTRUM_COL / cols)];

        CRect rect_tmp{ rects[i] };
        int spetral_height = static_cast<int>(spetral_data * rects[0].Height() / 30 * theApp.m_app_setting_data.sprctrum_height / 100);
        int peak_height = static_cast<int>(peak_data * rects[0].Height() / 30 * theApp.m_app_setting_data.sprctrum_height / 100);
        if (spetral_height <= 0 || CPlayer::GetInstance().IsError()) spetral_height = 1;		//Ƶ�׸߶�����Ϊ1�����أ�������ų���Ҳ����ʾƵ��
        if (peak_height <= 0 || CPlayer::GetInstance().IsError()) peak_height = 1;		//Ƶ�׸߶�����Ϊ1�����أ�������ų���Ҳ����ʾƵ��
        rect_tmp.top = rect_tmp.bottom - spetral_height;
        if (rect_tmp.top < rects[0].top) rect_tmp.top = rects[0].top;
        FillRect(rect_tmp, color, true);
        //���Ƶ�Ӱ
        if (draw_reflex)
        {
            CRect rc_invert = rect_tmp;
            rc_invert.bottom = rect_tmp.top + rect_tmp.Height() * 2 / 3;
            rc_invert.MoveToY(rect_tmp.bottom + gap_width);
            FillAlphaRect(rc_invert, color, 96, true);
        }

        //���ƶ���
        CRect rect_peak{ rect_tmp };
        rect_peak.bottom = rect_tmp.bottom - peak_height - gap_width;
        rect_peak.top = rect_peak.bottom - max(theApp.DPIRound(1.1), gap_width / 2);
        FillRect(rect_peak, color, true);
        ////���ƶ��˵�Ӱ
        //CRect rc_peak_invert = rect_peak;
        //rc_peak_invert.MoveToY(rc_invert.top + peak_height + theApp.DPIRound(1.1));
        //FillAlphaRect(rc_peak_invert, color, 96);
    }

}

void CUIDrawer::DrawLyricDoubleLine(CRect rect, LPCTSTR lyric, LPCTSTR next_lyric, int progress, int fade_percent)
{
    SetLyricFont();
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
    //GetDC()->SelectObject(&theApp.m_font_set.lyric.GetFont(theApp.m_ui_data.full_screen));
    int width;
    if (!swap)
        width = GetTextExtent(next_lyric).cx;
    else
        width = GetTextExtent(lyric).cx;
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
        color1 = color2 = CColorConvert::GetGradientColor(m_colors.color_text_2, m_colors.color_text, fade_percent);
    }

    if (!swap)
    {
        DrawWindowText(up_rect, lyric, color1, color2, progress, false);
        DrawWindowText(down_rect, next_lyric, m_colors.color_text_2);
    }
    else
    {
        DrawWindowText(up_rect, next_lyric, m_colors.color_text_2);
        DrawWindowText(down_rect, lyric, color1, color2, progress, false);
    }
}

void CUIDrawer::SetLyricFont()
{
    if (!m_for_cortana_lyric)
        SetFont(&theApp.m_font_set.lyric.GetFont(theApp.m_ui_data.full_screen));
    else
        SetFont(&theApp.m_font_set.cortana.GetFont());
}

void CUIDrawer::SetLyricFontTranslated()
{
    if (!m_for_cortana_lyric)
        SetFont(&theApp.m_font_set.lyric_translate.GetFont(theApp.m_ui_data.full_screen));
    else
        SetFont(&theApp.m_font_set.cortana_translate.GetFont());
}
