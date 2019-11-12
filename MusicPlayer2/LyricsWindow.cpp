// LyricsWindow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LyricsWindow.h"
#include "Resource.h"


// CLyricsWindow

const Gdiplus::REAL TRANSLATE_FONT_SIZE_FACTOR = 0.88f;		//��ʷ����ı���Сռ����ı���С�ı���

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

    DrawBackgroundAndToolbar(pGraphics);

    if (m_bDoubleLine && !m_strNextLyric.IsEmpty())
        DrawLyricsDoubleLine(pGraphics);
    else
        DrawLyrics(pGraphics);

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
    int lyricHeight = m_nHeight - m_toobar_height;
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
		dstRect = Gdiplus::RectF((m_nWidth - boundingBox.Width) / 2, m_toobar_height + (lyricHeight - boundingBox.Height) / 2, boundingBox.Width, boundingBox.Height);
	}
	else
	{
		Gdiplus::RectF transBoundingBox;
		pGraphics->MeasureString(m_strTranslate, -1, m_pFont, layoutRect, m_pTextFormat, &transBoundingBox, 0, 0);
		Gdiplus::REAL translateHeight = boundingBox.Height * TRANSLATE_FONT_SIZE_FACTOR;
		Gdiplus::REAL maxWidth = max(boundingBox.Width, transBoundingBox.Width);
		Gdiplus::REAL gapHeight = boundingBox.Height * 0.2f;	//��ʺͷ���֮��ļ�϶
		Gdiplus::REAL height = boundingBox.Height + gapHeight + translateHeight;
        Gdiplus::RectF textRect((m_nWidth - maxWidth) / 2, m_toobar_height + (lyricHeight - height) / 2, maxWidth, height);
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
    int lyricHeight = m_nHeight - m_toobar_height;
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

    dstRect = Gdiplus::RectF(0, m_toobar_height + (lyricHeight / 2 - boundingBox.Height) / 2, boundingBox.Width, boundingBox.Height);
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

	CWnd::OnLButtonDown(nFlags, point);
	//ReleaseCapture();
	//SendMessage(WM_NCLBUTTONDOWN,HTCAPTION,NULL);
}

