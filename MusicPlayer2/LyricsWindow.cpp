// LyricsWindow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LyricsWindow.h"
#include "Resource.h"
#include "Common.h"
#include "MusicPlayer2.h"
#include "GdiPlusTool.h"


// CLyricsWindow

const Gdiplus::REAL TRANSLATE_FONT_SIZE_FACTOR = 0.88f;		//��ʷ����ı���Сռ����ı���С�ı���
const int TOOL_ICON_SIZE = 24;

IMPLEMENT_DYNAMIC(CLyricsWindow, CWnd)

CLyricsWindow::CLyricsWindow()
{
	HDC hDC=::GetDC(NULL);
	m_hCacheDC=::CreateCompatibleDC(hDC);
	::ReleaseDC(NULL,hDC);
	//---------------------------------
	m_nHighlight=NULL ; //������ʵİٷֱ� 0--100
	m_TextGradientMode=LyricsGradientMode_Two ; //��ͨ��ʽ���ģʽ
	m_pTextPen=NULL ; //��ͨ��ʱ߿򻭱�
	m_HighlightGradientMode=LyricsGradientMode_Two ; //������ʽ���ģʽ
	m_pHighlightPen=NULL ; //������ʱ߿򻭱�
	m_pShadowBrush=NULL ; //��Ӱ��ˢ,GDIPlus��ˢ 
	m_nShadowOffset=1 ; //��Ӱƫ��
	m_pFont=NULL ; //GDIPlus����
	m_FontStyle=NULL ; 
	m_FontSize=NULL ; 
	m_pTextFormat=NULL;
	//---------------------------------
	m_pFontFamily=new Gdiplus::FontFamily();
	m_pTextFormat=new Gdiplus::StringFormat();
	m_pTextFormat->SetFormatFlags(Gdiplus::StringFormatFlagsNoWrap);//������
	m_pTextFormat->SetAlignment(Gdiplus::StringAlignmentCenter); //��ˮƽ���뷽ʽ
	m_pTextFormat->SetLineAlignment(Gdiplus::StringAlignmentNear); //�ô�ֱ���뷽ʽ
	//---------------------------------
	//SetLyricsFont(L"΢���ź�", 40, Gdiplus::FontStyle::FontStyleRegular);
	//SetLyricsColor(Gdiplus::Color::Red,Gdiplus::Color(255,172,0),LyricsGradientMode_Three);
	//SetLyricsBorder(Gdiplus::Color::Black,1);
	SetLyricsShadow(Gdiplus::Color(150,0,0,0),1);
	//SetHighlightColor(Gdiplus::Color(255,100,26),Gdiplus::Color(255,255,0),LyricsGradientMode_Three);
	//SetHighlightBorder(Gdiplus::Color::Black,1);
	
}

CLyricsWindow::~CLyricsWindow()
{
	if(m_pTextPen){
		delete m_pTextPen;
		m_pTextPen=NULL;
	}
	if(m_pHighlightPen){
		delete m_pHighlightPen;
		m_pHighlightPen=NULL;
	}
	if(m_pShadowBrush){
		delete m_pShadowBrush;
		m_pShadowBrush=NULL;
	}
	if(m_pFontFamily){
		delete m_pFontFamily;
		m_pFontFamily=NULL;
	}	
	if(m_pTextFormat){
		delete m_pTextFormat;
		m_pTextFormat=NULL;
	}	
	if(m_pFont){
		delete m_pFont;
		m_pFont=NULL;
	}
}


BEGIN_MESSAGE_MAP(CLyricsWindow, CWnd)

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_MOUSEHOVER()
    ON_WM_MOUSELEAVE()
    ON_WM_SIZING()
    ON_WM_RBUTTONUP()
    ON_WM_GETMINMAXINFO()
    ON_MESSAGE(WM_INITMENU, &CLyricsWindow::OnInitmenu)
    ON_WM_TIMER()
END_MESSAGE_MAP()



BOOL CLyricsWindow::Create(int nHeight)
{
	return CLyricsWindow::Create(_T("CometLyricsWindow"), -1, nHeight);
}
BOOL CLyricsWindow::Create(LPCTSTR lpszClassName)
{
	return CLyricsWindow::Create(lpszClassName,-1,-1);
}
BOOL CLyricsWindow::Create(LPCTSTR lpszClassName,int nWidth,int nHeight)
{
	if(!RegisterWndClass(lpszClassName))
	{
		TRACE("Class��Registration��Failedn");
	}

    m_popupMenu.LoadMenu(IDR_DESKTOP_LYRIC_POPUP_MENU);

	//--------------------------------------------
	//ȡ�����湤������
	RECT rcWork;
	SystemParametersInfo (SPI_GETWORKAREA,NULL,&rcWork,NULL);
	int nWorkWidth=rcWork.right-rcWork.left;
	int nWorkHeight=rcWork.bottom-rcWork.top;
	//δ���ݿ�ȡ��߶Ȳ���ʱ���ø�Ĭ��ֵ
	if(nWidth<0)nWidth=nWorkWidth*2/3;      //Ĭ�Ͽ��Ϊ�����ȵ�2/3
	if(nHeight<0)nHeight=150;
	//������ߡ�����λ��,�ô�������Ļ�·�
	int x=rcWork.left+( (nWorkWidth-nWidth)/2 );
	int y=rcWork.bottom-nHeight;
	//--------------------------------------------
	DWORD dwStyle=WS_POPUP|WS_VISIBLE|WS_THICKFRAME;
	DWORD dwExStyle=WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_LAYERED;
    BOOL rtn = CWnd::CreateEx(dwExStyle, lpszClassName, NULL, dwStyle, x, y, nWidth, nHeight, NULL, NULL);

    //��ʼ����ʾ��Ϣ
    m_tool_tip.Create(this, TTS_ALWAYSTIP);
    m_tool_tip.SetMaxTipWidth(theApp.DPI(400));

    //��ʼ����ʱ��
    SetTimer(TIMER_DESKTOP_LYRIC, 200, NULL);

    return rtn;
}
BOOL CLyricsWindow::RegisterWndClass(LPCTSTR lpszClassName)
{
	HINSTANCE hInstance=AfxGetInstanceHandle();
	WNDCLASSEX wndcls;
	memset(&wndcls,0,sizeof(WNDCLASSEX));
	wndcls.cbSize=sizeof(WNDCLASSEX);
	if(GetClassInfoEx(hInstance,lpszClassName,&wndcls))
	{
		return TRUE;
	}
	if(GetClassInfoEx(NULL,lpszClassName,&wndcls))
	{
		return TRUE;
	}

	wndcls.style=CS_DBLCLKS|CS_HREDRAW|CS_VREDRAW;
	wndcls.lpfnWndProc=::DefWindowProc;
	wndcls.hInstance=hInstance;
	wndcls.hIcon=NULL;
	wndcls.hCursor=::LoadCursor(NULL,IDC_ARROW);
	wndcls.hbrBackground=(HBRUSH)(COLOR_BTNFACE+1);
	wndcls.lpszMenuName=NULL;
	wndcls.lpszClassName=lpszClassName;
	if(!RegisterClassEx(&wndcls))
	{
		return FALSE;
	}
	return TRUE;
}


void CLyricsWindow::AddToolTips()
{
    AddMouseToolTip(BTN_APP, CCommon::LoadText(IDS_MAIN_MENU));
    AddMouseToolTip(BTN_STOP, CCommon::LoadText(IDS_STOP));
    AddMouseToolTip(BTN_PREVIOUS, CCommon::LoadText(IDS_PREVIOUS));
    AddMouseToolTip(BTN_PLAY_PAUSE, CCommon::LoadText(IDS_PLAY_PAUSE));
    AddMouseToolTip(BTN_NEXT, CCommon::LoadText(IDS_NEXT));
    AddMouseToolTip(BTN_SETTING, CCommon::LoadText(IDS_SETTINGS));
    AddMouseToolTip(BTN_DOUBLE_LINE, CCommon::LoadText(IDS_LYRIC_DOUBLE_LINE));
    AddMouseToolTip(BTN_LOCK, CCommon::LoadText(IDS_LOCK_DESKTOP_LYRIC));
    AddMouseToolTip(BTN_CLOSE, CCommon::LoadText(IDS_CLOSE_DESKTOP_LYRIC));

    m_tool_tip.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
}

void CLyricsWindow::AddMouseToolTip(BtnKey btn, LPCTSTR str)
{
    m_tool_tip.AddTool(this, str, m_buttons[btn].rect, btn + 1000);

}

//���¸��(����ı�,�������Ȱٷֱ�)
void CLyricsWindow::UpdateLyrics(LPCTSTR lpszLyrics,int nHighlight)
{
    m_lpszLyrics = lpszLyrics;
    UpdateLyrics(nHighlight);
}
//���¸�������(�������Ȱٷֱ�)
void CLyricsWindow::UpdateLyrics(int nHighlight)
{
	m_nHighlight=nHighlight;
	if(m_nHighlight<0)
		m_nHighlight=0;
	if(m_nHighlight>1000)
		m_nHighlight=1000;
	Draw();
}

void CLyricsWindow::UpdateLyricTranslate(LPCTSTR lpszLyricTranslate)
{
	m_strTranslate = lpszLyricTranslate;
}

//�ػ���ʴ���
void CLyricsWindow::Draw()
{
	//CRect rcWindow;
	GetWindowRect(m_rcWindow);
	m_nWidth= m_rcWindow.Width();
	m_nHeight= m_rcWindow.Height();
    CRect rcClient;
    GetClientRect(rcClient);
    m_frameSize.cx = (m_rcWindow.Width() - rcClient.Width()) / 2;
    m_frameSize.cy = (m_rcWindow.Height() - rcClient.Height()) / 2;

	//----------------------------------
	BITMAPINFO bitmapinfo;
	bitmapinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapinfo.bmiHeader.biBitCount = 32;
	bitmapinfo.bmiHeader.biHeight = m_nHeight;
	bitmapinfo.bmiHeader.biWidth = m_nWidth;
	bitmapinfo.bmiHeader.biPlanes = 1;
	bitmapinfo.bmiHeader.biCompression=BI_RGB;
	bitmapinfo.bmiHeader.biXPelsPerMeter=0;
	bitmapinfo.bmiHeader.biYPelsPerMeter=0;
	bitmapinfo.bmiHeader.biClrUsed=0;
	bitmapinfo.bmiHeader.biClrImportant=0;
	bitmapinfo.bmiHeader.biSizeImage = bitmapinfo.bmiHeader.biWidth * bitmapinfo.bmiHeader.biHeight * bitmapinfo.bmiHeader.biBitCount / 8;
	HBITMAP hBitmap=CreateDIBSection (m_hCacheDC,&bitmapinfo, 0,NULL, 0, 0);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject (m_hCacheDC,hBitmap);
	//----------------------------------
	Gdiplus::Graphics* pGraphics=new Gdiplus::Graphics(m_hCacheDC);
	pGraphics->SetSmoothingMode (Gdiplus::SmoothingModeAntiAlias);
	pGraphics->SetTextRenderingHint (Gdiplus::TextRenderingHintAntiAlias);

	//���ư�͸������
	if(m_bDrawBackground)
	{
        BYTE alpha = (m_bHover) ? 96 : 1;
		Gdiplus::Brush* pBrush = new Gdiplus::SolidBrush(Gdiplus::Color(alpha, 255, 255, 255));
		pGraphics->FillRectangle(pBrush, 0, 0, m_rcWindow.Width(), m_rcWindow.Height());
		delete pBrush;
	}
    if (m_bDoubleLine && !m_strNextLyric.IsEmpty())
        DrawLyricsDoubleLine(pGraphics);
    else
        DrawLyrics(pGraphics);

    //���ƹ�����
    if (m_bDrawBackground && (m_bHover))
    {
        m_first_draw = true;
        DrawToolbar(pGraphics);
    }

	delete pGraphics;
	//----------------------------------
	//����͸������
	CPoint DestPt(0,0);
	CSize psize(m_nWidth,m_nHeight);
	BLENDFUNCTION blendFunc32bpp;
	blendFunc32bpp.AlphaFormat = AC_SRC_ALPHA;
	blendFunc32bpp.BlendFlags = 0;
	blendFunc32bpp.BlendOp = AC_SRC_OVER;
	blendFunc32bpp.SourceConstantAlpha = m_alpha;
	HDC hDC=::GetDC(m_hWnd);
	::UpdateLayeredWindow(m_hWnd,hDC,NULL,&psize,m_hCacheDC,&DestPt,0,&blendFunc32bpp,ULW_ALPHA);
	//----------------------------------
	//�ͷ���Դ
	::SelectObject (m_hCacheDC,hOldBitmap);
	::DeleteObject(hBitmap);
	::ReleaseDC(m_hWnd,hDC);

    if (m_first_draw)
    {
        AddToolTips();
        m_first_draw = false;
    }
}

void CLyricsWindow::DrawLyricText(Gdiplus::Graphics* pGraphics, LPCTSTR strText, Gdiplus::RectF rect, bool bDrawHighlight, bool bDrawTranslate)
{
	Gdiplus::REAL fontSize = bDrawTranslate ? m_FontSize * TRANSLATE_FONT_SIZE_FACTOR : m_FontSize;
	if (fontSize < 1)
		fontSize = m_FontSize;

    Gdiplus::REAL textWidth = rect.Width;
    Gdiplus::REAL highlighWidth = rect.Width * m_nHighlight / 1000;

    if (!bDrawHighlight && !bDrawTranslate)
    {
        if (rect.X < 0)
            rect.X = 0;
    }
    else
    {
        //����ı���ȴ��ڿؼ���ȣ���Ҫ���ݷָ��λ�ù����ı�
        if (textWidth > m_nWidth)
        {
            //����ָ��λ�ã���ʽ��ȣ�ʣ�µĿ���Ѿ�С�ڿؼ���ȵ�һ�룬��ʱʹ�ı��Ҳ�Ϳؼ��Ҳ����
            if (textWidth - highlighWidth < m_nWidth / 2)
            {
                rect.X = m_nWidth - textWidth;
            }
            //�ָ�λ��ʣ�µĿ�Ȼ�û�е�С�ڿؼ���ȵ�һ�룬���Ƿָ�λ�õĿ���Ѿ����ڿؼ���ȵ�һ��ʱ����Ҫ�ƶ��ı�ʹ�ָ�λ�������ڿؼ����м�
            else if (highlighWidth > m_nWidth / 2)
            {
                rect.X = m_nWidth / 2 - highlighWidth;
            }
            //�ָ�λ�û������ؼ���ȵ�һ��ʱ��ʹ�ı����Ϳؼ�������
            else
            {
                rect.X = 0;
            }
        }
    }

	//-----------------------------------------------------------
	//������Ӱ
	if (m_pShadowBrush) {
		Gdiplus::RectF layoutRect(0, 0, 0, 0);
		layoutRect = rect;
		layoutRect.X = layoutRect.X + m_nShadowOffset;
		layoutRect.Y = layoutRect.Y + m_nShadowOffset;
		Gdiplus::GraphicsPath* pShadowPath = new Gdiplus::GraphicsPath(Gdiplus::FillModeAlternate);//����·��
		pShadowPath->AddString(strText, -1, m_pFontFamily, m_FontStyle, fontSize, layoutRect, m_pTextFormat); //�����ּ���·��
		pGraphics->FillPath(m_pShadowBrush, pShadowPath);//���·��
		delete pShadowPath; //����·��
	}

	//-----------------------------------------------------------
	//�������
	Gdiplus::GraphicsPath* pStringPath = new Gdiplus::GraphicsPath(Gdiplus::FillModeAlternate);//����·��
	pStringPath->AddString(strText, -1, m_pFontFamily, m_FontStyle, fontSize, rect, m_pTextFormat); //�����ּ���·��
	if (m_pTextPen) {
		pGraphics->DrawPath(m_pTextPen, pStringPath);//��·��,���ֱ߿�
	}
	Gdiplus::Brush* pBrush = CreateGradientBrush(m_TextGradientMode, m_TextColor1, m_TextColor2, rect);
	pGraphics->FillPath(pBrush, pStringPath);//���·��
	delete pBrush;//���ٻ�ˢ
	if(bDrawHighlight)
		DrawHighlightLyrics(pGraphics, pStringPath, rect);
	delete pStringPath; //����·��
}

//���Ƹ��
void CLyricsWindow::DrawLyrics(Gdiplus::Graphics* pGraphics)
{
    int lyricHeight = m_nHeight - theApp.DPI(TOOL_ICON_SIZE);
	//��ȡ�����ֿ�Ⱥ͸߶�
	Gdiplus::RectF layoutRect(0,0,0,0);
	Gdiplus::RectF boundingBox;
	pGraphics->MeasureString (m_lpszLyrics, -1, m_pFont,layoutRect, m_pTextFormat,&boundingBox, 0, 0);
	//�����ʻ�����λ��
	Gdiplus::RectF dstRect;		//���ֵľ���
	Gdiplus::RectF transRect;	//�����ı��ľ���
    bool bDrawTranslate = m_bShowTranslate && !m_strTranslate.IsEmpty();
	if(!bDrawTranslate)
	{
		dstRect = Gdiplus::RectF((m_nWidth - boundingBox.Width) / 2, theApp.DPI(TOOL_ICON_SIZE) + (lyricHeight - boundingBox.Height) / 2, boundingBox.Width, boundingBox.Height);
	}
	else
	{
		Gdiplus::RectF transBoundingBox;
		pGraphics->MeasureString(m_strTranslate, -1, m_pFont, layoutRect, m_pTextFormat, &transBoundingBox, 0, 0);
		Gdiplus::REAL translateHeight = boundingBox.Height * TRANSLATE_FONT_SIZE_FACTOR;
		Gdiplus::REAL maxWidth = max(boundingBox.Width, transBoundingBox.Width);
		Gdiplus::REAL gapHeight = boundingBox.Height * 0.2f;	//��ʺͷ���֮��ļ�϶
		Gdiplus::REAL height = boundingBox.Height + gapHeight + translateHeight;
        Gdiplus::RectF textRect((m_nWidth - maxWidth) / 2, theApp.DPI(TOOL_ICON_SIZE) + (lyricHeight - height) / 2, maxWidth, height);
		dstRect = textRect;
		dstRect.Height = boundingBox.Height;
		transRect = textRect;
		transRect.Y += (boundingBox.Height + gapHeight);
		transRect.Height = translateHeight;
	}

	DrawLyricText(pGraphics, m_lpszLyrics, dstRect, true);
	if (bDrawTranslate)
		DrawLyricText(pGraphics, m_strTranslate, transRect, false, true);
}

void CLyricsWindow::DrawLyricsDoubleLine(Gdiplus::Graphics* pGraphics)
{
    int lyricHeight = m_nHeight - theApp.DPI(TOOL_ICON_SIZE);
    static bool bSwap = false;
    if (m_lyricChangeFlag)      //�����ʷ����˸ı䣬�򽻻���ǰ��ʺ���һ���ʵ�λ��
        bSwap = !bSwap;
    //��ȡ�����ֿ�Ⱥ͸߶�
    Gdiplus::RectF layoutRect(0, 0, 0, 0);
    Gdiplus::RectF boundingBox;
    pGraphics->MeasureString(m_lpszLyrics, -1, m_pFont, layoutRect, m_pTextFormat, &boundingBox, 0, 0);
    Gdiplus::RectF nextBoundingBox;
    pGraphics->MeasureString(m_strNextLyric, -1, m_pFont, layoutRect, m_pTextFormat, &nextBoundingBox, 0, 0);
    //�����ʻ�����λ��
    Gdiplus::RectF dstRect;		//���ֵľ���
    Gdiplus::RectF nextRect;	//��һ���ı��ľ���

    dstRect = Gdiplus::RectF(0, theApp.DPI(TOOL_ICON_SIZE) + (lyricHeight / 2 - boundingBox.Height) / 2, boundingBox.Width, boundingBox.Height);
    nextRect = Gdiplus::RectF(m_nWidth - nextBoundingBox.Width, dstRect.Y + lyricHeight / 2, nextBoundingBox.Width, nextBoundingBox.Height);

    if (bSwap)
    {
        std::swap(dstRect.Y, nextRect.Y);
        nextRect.X = 0;
        dstRect.X = m_nWidth - dstRect.Width;
    }

    DrawLyricText(pGraphics, m_lpszLyrics, dstRect, true);
    DrawLyricText(pGraphics, m_strNextLyric, nextRect, false);
}

//���Ƹ������
void CLyricsWindow::DrawHighlightLyrics(Gdiplus::Graphics* pGraphics,Gdiplus::GraphicsPath* pPath, Gdiplus::RectF& dstRect)
{
	if(m_nHighlight<=0)return;
	Gdiplus::Region* pRegion=NULL;
	if(m_nHighlight<1000){
		Gdiplus::RectF CliptRect(dstRect);
		CliptRect.Width=CliptRect.Width * m_nHighlight / 1000;
		pRegion=new Gdiplus::Region(CliptRect);
		pGraphics->SetClip(pRegion, Gdiplus::CombineModeReplace);
	}
	//--------------------------------------------
	if(m_pHighlightPen){
		pGraphics->DrawPath (m_pHighlightPen,pPath);//��·��,���ֱ߿�
	}
	Gdiplus::Brush* pBrush = CreateGradientBrush(m_HighlightGradientMode, m_HighlightColor1,m_HighlightColor2,dstRect);
	pGraphics->FillPath (pBrush,pPath);//���·��
	delete pBrush;//���ٻ�ˢ
	//--------------------------------------------
	if(pRegion){
		pGraphics->ResetClip();
		delete pRegion;
	}
}

void CLyricsWindow::DrawToolbar(Gdiplus::Graphics* pGraphics)
{
    const int toolbar_num = 9;
    const int btn_size = theApp.DPI(TOOL_ICON_SIZE);
    int toolbar_width = toolbar_num * btn_size;
    Gdiplus::Rect toolbar_rect;
    toolbar_rect.Y = 0;
    toolbar_rect.X = (m_nWidth - toolbar_width) / 2;
    toolbar_rect.Width = toolbar_width;
    toolbar_rect.Height = btn_size;

    //���Ʊ���
    Gdiplus::Color back_color = CGdiPlusTool::COLORREFToGdiplusColor(theApp.m_app_setting_data.theme_color.light2, 180);
    Gdiplus::Brush* pBrush = new Gdiplus::SolidBrush(back_color);
    pGraphics->FillRectangle(pBrush, toolbar_rect);
    delete pBrush;

    CRect rcIcon = CRect(toolbar_rect.X, toolbar_rect.Y, toolbar_rect.GetRight(), toolbar_rect.GetBottom());
    rcIcon.right = rcIcon.left + btn_size;

    DrawToolIcon(pGraphics, theApp.m_icon_set.app, rcIcon, BTN_APP);
    rcIcon.MoveToX(rcIcon.right);
    DrawToolIcon(pGraphics, theApp.m_icon_set.stop_l, rcIcon, BTN_STOP);
    rcIcon.MoveToX(rcIcon.right);
    DrawToolIcon(pGraphics, theApp.m_icon_set.previous, rcIcon, BTN_PREVIOUS);
    rcIcon.MoveToX(rcIcon.right);
    IconRes hPlayPauseIcon = (CPlayer::GetInstance().IsPlaying() ? theApp.m_icon_set.pause : theApp.m_icon_set.play);
    DrawToolIcon(pGraphics, hPlayPauseIcon, rcIcon, BTN_PLAY_PAUSE);
    rcIcon.MoveToX(rcIcon.right);
    DrawToolIcon(pGraphics, theApp.m_icon_set.next, rcIcon, BTN_NEXT);
    rcIcon.MoveToX(rcIcon.right);
    DrawToolIcon(pGraphics, theApp.m_icon_set.setting, rcIcon, BTN_SETTING);
    rcIcon.MoveToX(rcIcon.right);
    DrawToolIcon(pGraphics, theApp.m_icon_set.double_line, rcIcon, BTN_DOUBLE_LINE, theApp.m_lyric_setting_data.desktop_lyric_data.lyric_double_line);
    rcIcon.MoveToX(rcIcon.right);
    DrawToolIcon(pGraphics, theApp.m_icon_set.lock, rcIcon, BTN_LOCK);
    rcIcon.MoveToX(rcIcon.right);
    DrawToolIcon(pGraphics, theApp.m_icon_set.close, rcIcon, BTN_CLOSE);
}

void CLyricsWindow::DrawToolIcon(Gdiplus::Graphics* pGraphics, IconRes icon, CRect rect, BtnKey btn_key, bool checked)
{
    rect.DeflateRect(theApp.DPI(2), theApp.DPI(2));
    auto& btn = m_buttons[btn_key];
    btn.rect = rect;

    if (btn.pressed)
        rect.MoveToXY(rect.left + theApp.DPI(1), rect.top + theApp.DPI(1));

    Gdiplus::Color back_color;
    bool draw_background = false;
    if (btn.pressed && btn.hover)
    {
        back_color = CGdiPlusTool::COLORREFToGdiplusColor(theApp.m_app_setting_data.theme_color.dark1, 180);
        draw_background = true;
    }
    else if (btn.hover)
    {
        back_color = CGdiPlusTool::COLORREFToGdiplusColor(theApp.m_app_setting_data.theme_color.light1, 180);
        draw_background = true;
    }
    else if(checked)
    {
        back_color = CGdiPlusTool::COLORREFToGdiplusColor(theApp.m_app_setting_data.theme_color.light1, 110);
        draw_background = true;
    }
    if(draw_background)
    {
        Gdiplus::SolidBrush brush(back_color);
        pGraphics->FillRectangle(&brush, rect.left, rect.top, rect.Width(), rect.Height());
    }

    CRect rc_tmp = rect;
    //ʹͼ���ھ����о���
    CSize icon_size = icon.GetSize();
    rc_tmp.left = rect.left + (rect.Width() - icon_size.cx) / 2;
    rc_tmp.top = rect.top + (rect.Height() - icon_size.cy) / 2;
    rc_tmp.right = rc_tmp.left + icon_size.cx;
    rc_tmp.bottom = rc_tmp.top + icon_size.cy;

    HDC hDC = pGraphics->GetHDC();
    CDrawCommon drawer;
    drawer.Create(CDC::FromHandle(hDC));
    drawer.DrawIcon(icon.GetIcon(true), rc_tmp.TopLeft(), rc_tmp.Size());
    pGraphics->ReleaseHDC(hDC);
}

//�������仭ˢ
Gdiplus::Brush* CLyricsWindow::CreateGradientBrush(LyricsGradientMode TextGradientMode,Gdiplus::Color& Color1,Gdiplus::Color& Color2, Gdiplus::RectF& dstRect)
{
	Gdiplus::PointF pt1;
	Gdiplus::PointF pt2;
	Gdiplus::Brush* pBrush=NULL;
	switch (TextGradientMode)
	{
	case LyricsGradientMode_Two://��ɫ����
		{
			Gdiplus::PointF point1(dstRect.X,dstRect.Y);
			Gdiplus::PointF point2(dstRect.X,dstRect.Y+dstRect.Height);
			pBrush=new Gdiplus::LinearGradientBrush(point1,point2,Color1,Color2);
			((Gdiplus::LinearGradientBrush*)pBrush)->SetWrapMode(Gdiplus::WrapModeTileFlipXY);
			break;
		}

	case LyricsGradientMode_Three://��ɫ����
		{
			Gdiplus::PointF point1(dstRect.X,dstRect.Y);
			Gdiplus::PointF point2(dstRect.X,dstRect.Y+dstRect.Height/2);
			pBrush=new Gdiplus::LinearGradientBrush(point1,point2,Color1,Color2);
			((Gdiplus::LinearGradientBrush*)pBrush)->SetWrapMode(Gdiplus::WrapModeTileFlipXY);
			break;
		}

	default://�޽���
		{
			pBrush=new Gdiplus::SolidBrush(Color1);
			break;
		}
	}
	return pBrush;
}

//���ø����ɫ
void CLyricsWindow::SetLyricsColor(Gdiplus::Color TextColor1)
{
	CLyricsWindow::SetLyricsColor(TextColor1,Gdiplus::Color::Black,LyricsGradientMode_None);
}
void CLyricsWindow::SetLyricsColor(Gdiplus::Color TextColor1,Gdiplus::Color TextColor2,LyricsGradientMode TextGradientMode)
{
	m_TextColor1=TextColor1;
	m_TextColor2=TextColor2;
	m_TextGradientMode=TextGradientMode;

}
//���ø�ʱ߿�
void CLyricsWindow::SetLyricsBorder(Gdiplus::Color BorderColor, Gdiplus::REAL BorderWidth)
{
	if(m_pTextPen){
		delete m_pTextPen;
		m_pTextPen=NULL;
	}
	if(BorderColor.GetA()>0 && BorderWidth>0)
		m_pTextPen=new Gdiplus::Pen(BorderColor,BorderWidth);
}
//���ø��������ɫ
void CLyricsWindow::SetHighlightColor(Gdiplus::Color TextColor1)
{
	CLyricsWindow::SetHighlightColor(TextColor1,Gdiplus::Color::Black,LyricsGradientMode_None);
}
void CLyricsWindow::SetHighlightColor(Gdiplus::Color TextColor1,Gdiplus::Color TextColor2,LyricsGradientMode TextGradientMode)
{
	m_HighlightColor1=TextColor1;
	m_HighlightColor2=TextColor2;
	m_HighlightGradientMode=TextGradientMode;

}
//���ø�����ʱ߿�
void CLyricsWindow::SetHighlightBorder(Gdiplus::Color BorderColor, Gdiplus::REAL BorderWidth)
{
	if(m_pHighlightPen){
		delete m_pHighlightPen;
		m_pHighlightPen=NULL;
	}
	if(BorderColor.GetA()>0 && BorderWidth>0)
		m_pHighlightPen=new Gdiplus::Pen(BorderColor,BorderWidth);
}
//���ø����Ӱ
void CLyricsWindow::SetLyricsShadow(Gdiplus::Color ShadowColor,int nShadowOffset)
{
	if(m_pShadowBrush){
		delete m_pShadowBrush;
		m_pShadowBrush=NULL;
	}
	if(ShadowColor.GetA()>0 && nShadowOffset>0){
		m_nShadowOffset=nShadowOffset;
		m_pShadowBrush=new Gdiplus::SolidBrush(ShadowColor);
	}else{
		m_nShadowOffset=0;
	}
}
//���ø������
void CLyricsWindow::SetLyricsFont(const WCHAR * familyName, Gdiplus::REAL emSize,INT style, Gdiplus::Unit unit)
{
	if(m_pFont){
		delete m_pFont;
		m_pFont=NULL;
	}
	Gdiplus::FontFamily family(familyName,NULL);
	Gdiplus::Status lastResult = family.GetLastStatus();
	if (lastResult != Gdiplus::Ok)
	{
		HFONT hFont=(HFONT)GetStockObject(DEFAULT_GUI_FONT);
		LOGFONTW lf;
		ZeroMemory(&lf,sizeof(LOGFONTW));
		GetObjectW(hFont,sizeof(LOGFONTW),&lf);
		Gdiplus::FontFamily family2(lf.lfFaceName,NULL);
		m_pFont=new Gdiplus::Font(&family2,emSize,style,unit);
	}else{
		m_pFont=new Gdiplus::Font(&family,emSize,style,unit);
	}
	 //----------------
	//����һЩ��������,����·��ʱҪ�õ�
	m_pFont->GetFamily (m_pFontFamily);
	m_FontSize=m_pFont->GetSize ();
	m_FontStyle=m_pFont->GetStyle ();

	
	
}

void CLyricsWindow::SetLyricDoubleLine(bool doubleLine)
{
	m_bDoubleLine = doubleLine;
}

void CLyricsWindow::SetNextLyric(LPCTSTR lpszNextLyric)
{
	m_strNextLyric = lpszNextLyric;
}

void CLyricsWindow::SetDrawBackground(bool drawBackground)
{
	m_bDrawBackground = drawBackground;
}

void CLyricsWindow::SetShowTranslate(bool showTranslate)
{
    m_bShowTranslate = showTranslate;
}

void CLyricsWindow::SetAlpha(int alpha)
{
    m_alpha = alpha;
}

const CString& CLyricsWindow::GetLyricStr() const
{
    return m_lpszLyrics;
}

void CLyricsWindow::SetLyricChangeFlag(bool bFlag)
{
    m_lyricChangeFlag = bFlag;
}

void CLyricsWindow::OnLButtonDown(UINT nFlags, CPoint point)
{
    CPoint point1 = point;
    point1.x += m_frameSize.cx;
    point1.y += m_frameSize.cy;

    bool point_in_btns = false;
    for (auto& btn : m_buttons)
    {
        if (btn.second.rect.PtInRect(point1) != FALSE)
        {
            btn.second.pressed = true;
            point_in_btns = true;
        }
    }
    if (!point_in_btns)
        PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point1.x, point1.y));

	CWnd::OnLButtonDown(nFlags, point);
	//ReleaseCapture();
	//SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,NULL);
}

void CLyricsWindow::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CPoint point1 = point;
    point1.x += m_frameSize.cx;
    point1.y += m_frameSize.cy;
    
    for (auto& btn : m_buttons)
    {
        btn.second.pressed = false;

        if (btn.second.rect.PtInRect(point1) && btn.second.enable)
        {
            switch (btn.first)
            {
            case BTN_APP:
            {
                CPoint cur_point;
                GetCursorPos(&cur_point);
                theApp.m_menu_set.m_main_menu_popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, cur_point.x, cur_point.y, this);
            }
                return;

            case BTN_STOP:
                theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_STOP);
                return;

            case BTN_PREVIOUS:
                theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_PREVIOUS);
                return;
            case BTN_PLAY_PAUSE:
                theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_PLAY_PAUSE);
                return;

            case BTN_NEXT:
                theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_NEXT);
                return;

            case BTN_SETTING:
                theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_OPTION_SETTINGS);
                return;

            case BTN_DOUBLE_LINE:
                theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_LYRIC_DISPLAYED_DOUBLE_LINE);
                return;

            case BTN_LOCK:
                theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_LOCK_DESKTOP_LRYIC);
                return;

            case BTN_CLOSE:
                theApp.m_pMainWnd->SendMessage(WM_COMMAND, ID_CLOSE_DESKTOP_LYRIC);
                return;

            default:
                break;
            }
        }
    }

	CWnd::OnLButtonUp(nFlags, point);
}


void CLyricsWindow::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CPoint point1 = point;
    point1.x += m_frameSize.cx;
    point1.y += m_frameSize.cy;
    for (auto& btn : m_buttons)
    {
        btn.second.hover = (btn.second.rect.PtInRect(point1) != FALSE);
    }

    TRACKMOUSEEVENT tme;
    tme.cbSize = sizeof(tme);
    tme.hwndTrack = m_hWnd;
    tme.dwFlags = TME_LEAVE | TME_HOVER;
    tme.dwHoverTime = 1;
    _TrackMouseEvent(&tme);

    CWnd::OnMouseMove(nFlags, point);
}


void CLyricsWindow::OnMouseHover(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if (!m_bHover)
    {
        m_bHover = true;
        //Invalidate();
    }
    else
    {
        CWnd::OnMouseHover(nFlags, point);
    }
}


void CLyricsWindow::OnMouseLeave()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    m_bHover = false;
    //Invalidate();
    for (auto& btn : m_buttons)
    {
        btn.second.pressed = false;
    }

    CWnd::OnMouseLeave();
}


void CLyricsWindow::OnSizing(UINT fwSide, LPRECT pRect)
{
    CWnd::OnSizing(fwSide, pRect);

    // TODO: �ڴ˴������Ϣ����������
    m_bHover = true;
}


void CLyricsWindow::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    CPoint point1;		//����һ������ȷ�����λ�õ�λ��
    GetCursorPos(&point1);	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը����꣬��λ������Ļ���Ͻǵ�Ϊԭ�㣬point���Կͻ������Ͻ�Ϊԭ��
    CMenu* pMenu = m_popupMenu.GetSubMenu(0);
    if (pMenu != NULL)
        pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this);

    //CWnd::OnRButtonUp(nFlags, point);
}


BOOL CLyricsWindow::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // TODO: �ڴ����ר�ô����/����û���
    WORD command = LOWORD(wParam);
    if (CCommon::IsMenuItemInMenu(m_popupMenu.GetSubMenu(0), command) || CCommon::IsMenuItemInMenu(&theApp.m_menu_set.m_main_menu, command))
        AfxGetMainWnd()->SendMessage(WM_COMMAND, wParam, lParam);		//���˵�����ת����������

    return CWnd::OnCommand(wParam, lParam);
}


void CLyricsWindow::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    lpMMI->ptMinTrackSize.x = theApp.DPI(400);
    lpMMI->ptMinTrackSize.y = theApp.DPI(100);

    CWnd::OnGetMinMaxInfo(lpMMI);
}


afx_msg LRESULT CLyricsWindow::OnInitmenu(WPARAM wParam, LPARAM lParam)
{
    AfxGetMainWnd()->SendMessage(WM_INITMENU, wParam, lParam);        //��WM_INITMENU��Ϣת����������
    return 0;
}


BOOL CLyricsWindow::PreTranslateMessage(MSG* pMsg)
{
    // TODO: �ڴ����ר�ô����/����û���
    if (pMsg->message == WM_MOUSEMOVE)
        m_tool_tip.RelayEvent(pMsg);

    return CWnd::PreTranslateMessage(pMsg);
}


void CLyricsWindow::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
    if (nIDEvent == TIMER_DESKTOP_LYRIC)
    {
        CPoint point;
        GetCursorPos(&point);
        m_bMouseInWindowRect = m_rcWindow.PtInRect(point);
    }

    CWnd::OnTimer(nIDEvent);
}
