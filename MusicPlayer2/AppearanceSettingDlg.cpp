// AppearanceSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "AppearanceSettingDlg.h"
#include "afxdialogex.h"


// CAppearanceSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CAppearanceSettingDlg, CDialogEx)

CAppearanceSettingDlg::CAppearanceSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_APPEREANCE_SETTING_DLG, pParent)
	, m_line_space(0)
{

}

CAppearanceSettingDlg::~CAppearanceSettingDlg()
{
}

void CAppearanceSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRANSPARENT_SLIDER, m_transparency_slid);
	DDX_Control(pDX, IDC_COLOR_STATIC, m_color_static);
	DDX_Control(pDX, IDC_COLOR_STATIC2, m_color_static1);
	DDX_Control(pDX, IDC_COLOR_STATIC3, m_color_static2);
	DDX_Control(pDX, IDC_COLOR_STATIC4, m_color_static3);
	DDX_Control(pDX, IDC_COLOR_STATIC5, m_color_static4);
	DDX_Control(pDX, IDC_COLOR_STATIC6, m_color_static5);
	DDX_Control(pDX, IDC_COLOR_STATIC7, m_color_static6);
	DDX_Control(pDX, IDC_FOLLOW_SYSTEM_COLOR_CHECK, m_follow_system_color_check);
	DDX_Text(pDX, IDC_FONT_SIZE_EDIT, m_line_space);
	DDV_MinMaxInt(pDX, m_line_space, 0, MAX_LINE_SPACE);
	DDX_Control(pDX, IDC_SPECTRUM_HEIGHT_SLIDER, m_spectrum_height_slid);
}

void CAppearanceSettingDlg::SetTransparency()
{
	::SetWindowLong(m_hMainWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	::SetLayeredWindowAttributes(m_hMainWnd, 0, m_transparency * 255 / 100, LWA_ALPHA);  //͸����ȡֵ��ΧΪ0~255
}

void CAppearanceSettingDlg::ClickColor()
{
	//�����Ԥ����ɫ�е�����һ��ʱ��ȡ��������ϵͳ����ɫ����ѡ��ť��ѡ��
	CCommon::FillStaticColor(m_color_static, m_theme_color);
	m_theme_color_follow_system = false;
	m_follow_system_color_check.SetCheck(FALSE);
	//GetDlgItem(IDC_SET_PROGRESS_COLOR_BUTTON)->EnableWindow();
}

void CAppearanceSettingDlg::DrawColor()
{
	CCommon::FillStaticColor(m_color_static, m_theme_color);
	CCommon::FillStaticColor(m_color_static1, m_color1);
	CCommon::FillStaticColor(m_color_static2, m_color2);
	CCommon::FillStaticColor(m_color_static3, m_color3);
	CCommon::FillStaticColor(m_color_static4, m_color4);
	CCommon::FillStaticColor(m_color_static5, m_color5);
	CCommon::FillStaticColor(m_color_static6, m_color6);
}


BEGIN_MESSAGE_MAP(CAppearanceSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SET_FONT_BUTTON, &CAppearanceSettingDlg::OnBnClickedSetFontButton)
//	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_TRANSPARENT_SLIDER, &CAppearanceSettingDlg::OnNMReleasedcaptureTransparentSlider)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_SET_PROGRESS_COLOR_BUTTON, &CAppearanceSettingDlg::OnBnClickedSetThemeButton)
	ON_STN_CLICKED(IDC_COLOR_STATIC2, &CAppearanceSettingDlg::OnStnClickedColorStatic2)
	ON_STN_CLICKED(IDC_COLOR_STATIC3, &CAppearanceSettingDlg::OnStnClickedColorStatic3)
	ON_STN_CLICKED(IDC_COLOR_STATIC4, &CAppearanceSettingDlg::OnStnClickedColorStatic4)
	ON_STN_CLICKED(IDC_COLOR_STATIC5, &CAppearanceSettingDlg::OnStnClickedColorStatic5)
	ON_STN_CLICKED(IDC_COLOR_STATIC6, &CAppearanceSettingDlg::OnStnClickedColorStatic6)
	ON_STN_CLICKED(IDC_COLOR_STATIC7, &CAppearanceSettingDlg::OnStnClickedColorStatic7)
	ON_BN_CLICKED(IDC_FOLLOW_SYSTEM_COLOR_CHECK, &CAppearanceSettingDlg::OnBnClickedFollowSystemColorCheck)
	//ON_EN_CHANGE(IDC_FONT_NAME_EDIT, &CAppearanceSettingDlg::OnEnChangeFontNameEdit)
	ON_EN_CHANGE(IDC_FONT_SIZE_EDIT, &CAppearanceSettingDlg::OnEnChangeLineSpaceEdit)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CAppearanceSettingDlg::OnDeltaposSpin1)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CAppearanceSettingDlg ��Ϣ�������


BOOL CAppearanceSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetBackgroundColor(RGB(255, 255, 255));

	//SetDlgItemText(IDC_FONT_NAME_EDIT, m_font.c_str());
	//CString font_size_str;
	//font_size_str.Format(_T("%d"), m_line_space);
	//SetDlgItemText(IDC_FONT_SIZE_EDIT, font_size_str);

	m_transparency_slid.SetRange(20, 100);
	m_transparency_slid.SetPos(m_transparency);
	CString str;
	str.Format(_T("%d%%"), m_transparency);
	SetDlgItemText(IDC_TRANSPARENT_STATIC, str);

	m_spectrum_height_slid.SetRange(10, 300);
	m_spectrum_height_slid.SetPos(theApp.m_sprctrum_height);
	str.Format(_T("%d%%"), theApp.m_sprctrum_height);
	SetDlgItemText(IDC_SPECTRUM_HEIGHT_STATIC, str);

	//���������õ����������塱��ť��
	//GetDlgItem(IDC_SET_FONT_BUTTON)->SetFocus();

	//Ϊ�ؼ�����SS_NOTIFY���ԣ���������Ӧ��Ϣ
	DWORD dwStyle = m_color_static.GetStyle();
	::SetWindowLong(m_color_static.GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
	::SetWindowLong(m_color_static1.GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
	::SetWindowLong(m_color_static2.GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
	::SetWindowLong(m_color_static3.GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
	::SetWindowLong(m_color_static4.GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
	::SetWindowLong(m_color_static5.GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);
	::SetWindowLong(m_color_static6.GetSafeHwnd(), GWL_STYLE, dwStyle | SS_NOTIFY);

	m_toolTip.Create(this);
	m_toolTip.AddTool(&m_color_static, _T("��ǰ����ɫ"));
	m_toolTip.AddTool(&m_color_static1, _T("����ɫ"));
	m_toolTip.AddTool(&m_color_static2, _T("��ɫ"));
	m_toolTip.AddTool(&m_color_static3, _T("��ɫ"));
	m_toolTip.AddTool(&m_color_static4, _T("����ɫ"));
	m_toolTip.AddTool(&m_color_static5, _T("ǳ��ɫ"));
	m_toolTip.AddTool(&m_color_static6, _T("����ɫ"));

	////���á�������ɫ����ť�Ŀ���״̬
	//GetDlgItem(IDC_SET_PROGRESS_COLOR_BUTTON)->EnableWindow(!m_theme_color_follow_system);
	//�����Ƿ����ϵͳ���ø�ѡ��ť�ĳ�ʼ״̬
#ifdef COMPILE_IN_WIN_XP
	m_follow_system_color_check.EnableWindow(FALSE);
#else
	if (IsWindows8OrGreater())
		m_follow_system_color_check.SetCheck(m_theme_color_follow_system);
	else
		m_follow_system_color_check.EnableWindow(FALSE);		//Win8����ϵͳ���ô˸�ѡ��ť
#endif // !COMPILE_IN_WIN_XP


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CAppearanceSettingDlg::OnBnClickedSetFontButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFont font;
	font.CreatePointFont(m_font_size * 10, m_font.c_str());
	LOGFONT lf{};             //LOGFONT����
	font.GetLogFont(&lf);
	CFontDialog fontDlg(&lf);	//��������Ի��򣬳�ʼѡ������Ϊ֮ǰ����
	if (IDOK == fontDlg.DoModal())     // ��ʾ����Ի���
	{
		//��ȡ������Ϣ
		//m_font = fontDlg.m_cf.lpLogFont->lfFaceName;
		//m_font_size = fontDlg.m_cf.iPointSize / 10;
		m_font = fontDlg.GetFaceName();
		m_font_size = fontDlg.GetSize() / 10;
		//�������Ѹ���flag��Ϊtrue
		m_font_changed = true;
	}
}


//void CAppearanceSettingDlg::OnNMReleasedcaptureTransparentSlider(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	m_transparency = m_transparency_slid.GetPos();
//	CString str;
//	str.Format(_T("��͸���ȣ�%%%d"), m_transparency);
//	SetDlgItemText(IDC_TRANSPARENT_STATIC, str);
//	*pResult = 0;
//}


void CAppearanceSettingDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/
	//����ˮƽ������ʱ��Ӧ�˺���
	if ((pScrollBar->GetDlgCtrlID() == IDC_TRANSPARENT_SLIDER))
	{
		m_transparency = m_transparency_slid.GetPos();
		CString str;
		str.Format(_T("%d%%"), m_transparency);
		SetDlgItemText(IDC_TRANSPARENT_STATIC, str);

		SetTransparency();		//ʵʱ���ô��ڲ�͸����
	}
	if ((pScrollBar->GetDlgCtrlID() == IDC_SPECTRUM_HEIGHT_SLIDER))
	{
		theApp.m_sprctrum_height = m_spectrum_height_slid.GetPos();
		CString str;
		str.Format(_T("%d%%"), theApp.m_sprctrum_height);
		SetDlgItemText(IDC_SPECTRUM_HEIGHT_STATIC, str);
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CAppearanceSettingDlg::OnBnClickedSetThemeButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CColorDialog colorDlg(m_theme_color, 0, this);
	if (colorDlg.DoModal() == IDOK)
	{
		m_theme_color = colorDlg.GetColor();
		//if (m_theme_color == 0)
		//	MessageBox(_T("���棺��������ɫ���óɺ�ɫ��ʹ�����б������ڲ��ŵ���Ŀ��������"), NULL, MB_ICONWARNING);
		if(m_theme_color == RGB(255,255,255))
			MessageBox(_T("���棺��������ɫ���óɰ�ɫ��ʹ��������ȫ��������"), NULL, MB_ICONWARNING);
		CCommon::FillStaticColor(m_color_static, m_theme_color);
		//�����ˡ�������ɫ��֮��ȡ��������ϵͳ����ɫ����ѡ��ť��ѡ��
		m_theme_color_follow_system = false;
		m_follow_system_color_check.SetCheck(FALSE);
	}
}


void CAppearanceSettingDlg::OnStnClickedColorStatic2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_theme_color = m_color1;
	ClickColor();
}


void CAppearanceSettingDlg::OnStnClickedColorStatic3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_theme_color = m_color2;
	ClickColor();
}


void CAppearanceSettingDlg::OnStnClickedColorStatic4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_theme_color = m_color3;
	ClickColor();
}


void CAppearanceSettingDlg::OnStnClickedColorStatic5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_theme_color = m_color4;
	ClickColor();
}


void CAppearanceSettingDlg::OnStnClickedColorStatic6()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_theme_color = m_color5;
	ClickColor();
}


void CAppearanceSettingDlg::OnStnClickedColorStatic7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_theme_color = m_color6;
	ClickColor();
}


BOOL CAppearanceSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_MOUSEMOVE)
		m_toolTip.RelayEvent(pMsg);

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CAppearanceSettingDlg::OnBnClickedFollowSystemColorCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_theme_color_follow_system = (m_follow_system_color_check.GetCheck() != 0);
	//GetDlgItem(IDC_SET_PROGRESS_COLOR_BUTTON)->EnableWindow(!m_theme_color_follow_system);
}


void CAppearanceSettingDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnCancel();
}


void CAppearanceSettingDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}


//void CAppearanceSettingDlg::OnEnChangeFontNameEdit()
//{
//	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
//	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
//	// ���������� CRichEditCtrl().SetEventMask()��
//	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
//
//	// TODO:  �ڴ���ӿؼ�֪ͨ����������
//	CString font_name;
//	GetDlgItemText(IDC_FONT_NAME_EDIT, font_name);
//	m_font = font_name;
//	m_font_changed = true;
//}


void CAppearanceSettingDlg::OnEnChangeLineSpaceEdit()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CString font_size;
	//GetDlgItemText(IDC_FONT_SIZE_EDIT, font_size);
	//m_line_space = _wtoi(font_size);
	UpdateData(TRUE);
	if (m_line_space < 0) m_line_space = 0;
	if (m_line_space > MAX_LINE_SPACE) m_line_space = MAX_LINE_SPACE;
	UpdateData(FALSE);
	m_font_changed = true;
}


void CAppearanceSettingDlg::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pNMUpDown->iDelta == 1) // �����ֵΪ1 , ˵�������Spin�����¼�ͷ 
	{
		//���ٸ���м�����ֵ
		UpdateData(TRUE);
		if (m_line_space > 0)
			m_line_space--;
		UpdateData(FALSE);
	}
	else if (pNMUpDown->iDelta == -1) // �����ֵΪ-1 , ˵�������Spin�����ϼ�ͷ 
	{
		//���Ӹ���м�����ֵ
		UpdateData(TRUE);
		if (m_line_space < MAX_LINE_SPACE)
			m_line_space++;
		UpdateData(FALSE);
	}
	UpdateData(TRUE);
	m_font_changed = true;

	*pResult = 0;
}



HBRUSH CAppearanceSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd == &m_transparency_slid || pWnd == &m_spectrum_height_slid)		//���û������ؼ��ı���ɫΪ��ɫ
	{
		return (HBRUSH)::GetStockObject(WHITE_BRUSH);
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
