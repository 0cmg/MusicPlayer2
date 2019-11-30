#pragma once
#include "DrawCommon.h"
#include "CPlayerUIHelper.h"

class CUIDrawer :
    public CDrawCommon
{
public:
    enum SpectrumCol        //����Ƶ�׷�������������
    {
        SC_64,
        SC_32,
        SC_16,
        SC_8
    };

public:
    CUIDrawer(UIColors& colors);
    ~CUIDrawer();

    void DrawLryicCommon(CRect rect);

    int GetLyricTextHeight() const;
    virtual void Create(CDC* pDC, CWnd* pMainWnd /* = nullptr */) override;
    bool IsDrawMultiLine(int height) const;			//����һ���߶��ж��Ƿ���ƶ��и��
    void SetForCortanaLyric(bool for_cortana_lyric = true);

    void DrawLyricTextMultiLine(CRect rect);
    void DrawLyricTextSingleLine(CRect rect, bool double_line = true);

    //����Ƶ�׷���
    //rect��Ƶ�׵ľ�������
    //col��Ƶ�׵����ε�����
    //draw_reflex���Ƿ���Ƶ�Ӱ
    void DrawSpectrum(CRect rect, SpectrumCol col = SC_64, bool draw_reflex = false);
    
    //����Ƶ�׷���
    //col_width��ÿһ�����εĿ��
    //gap_width�����μ��϶�Ŀ��
    //cols��Ƶ�׵����ε�����������Ϊ2��n�η�����С�ڻ����SPECTRUM_COL
    //color��Ƶ�׷�������ɫ
    //draw_reflex���Ƿ���Ƶ�Ӱ
    void DrawSpectrum(CRect rect, int col_width, int gap_width, int cols, COLORREF color, bool draw_reflex = false);

private:
    void SetLyricFont();
    void SetLyricFontTranslated();
    void DrawLyricDoubleLine(CRect rect, LPCTSTR lyric, LPCTSTR next_lyric, int progress, int fade_percent = 100);

private:
    UIColors& m_colors;
    bool m_for_cortana_lyric{ false };		//�Ƿ�������ʾ��������
};

