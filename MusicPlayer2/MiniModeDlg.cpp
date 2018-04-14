// MiniModeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "MiniModeDlg.h"
#include "afxdialogex.h"


// CMiniModeDlg �Ի���

IMPLEMENT_DYNAMIC(CMiniModeDlg, CDialogEx)

CMiniModeDlg::CMiniModeDlg(int& item_selected, CMenu& popup_menu, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MINI_DIALOG, pParent), m_item_selected{ item_selected }, m_popup_menu{ popup_menu }
{
	m_screen_width = GetSystemMetrics(SM_CXFULLSCREEN);
	m_screen_height = GetSystemMetrics(SM_CYFULLSCREEN) + 23;
}

CMiniModeDlg::~CMiniModeDlg()
{
}

void CMiniModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LYRIC_STATIC, m_lyric_static);
	DDX_Control(pDX, IDC_TIME_STATIC1, m_time_static);
	DDX_Control(pDX, ID_M_PREVIOUS, m_previous_button);
	DDX_Control(pDX, ID_M_PLAY_PAUSE, m_play_pause_button);
	DDX_Control(pDX, ID_M_NEXT, m_next_button);
	DDX_Control(pDX, IDC_SPECTRAL_STATIC, m_spectral_static);
	DDX_Control(pDX, IDC_LIST2, m_playlist_ctrl);
	DDX_Control(pDX, IDC_SHOW_LIST_BUTTON, m_show_list_button);
	DDX_Control(pDX, IDOK, m_return_button);
	DDX_Control(pDX, ID_MINI_MODE_EXIT, m_exit_button);
	DDX_Control(pDX, IDC_MINI_PROGRESS_STATIC, m_progress_bar);
}

void CMiniModeDlg::SaveConfig() const
{
	CCommon::WritePrivateProfileIntW(L"mini_mode", L"position_x", m_position_x, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"mini_mode", L"position_y", m_position_y, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"mini_mode", L"text_color", m_theme_color.original_color, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"mini_mode", L"dark_mode", m_dark_mode, theApp.m_config_path.c_str());
	CCommon::WritePrivateProfileIntW(L"mini_mode", L"follow_main_color", m_follow_main_color, theApp.m_config_path.c_str());
}

void CMiniModeDlg::LoadConfig()
{
	m_position_x = GetPrivateProfileInt(_T("mini_mode"), _T("position_x"), -1, theApp.m_config_path.c_str());
	m_position_y = GetPrivateProfileInt(_T("mini_mode"), _T("position_y"), -1, theApp.m_config_path.c_str());
	m_theme_color.original_color = GetPrivateProfileInt(_T("mini_mode"), _T("text_color"), 13526784, theApp.m_config_path.c_str());
	m_dark_mode = (GetPrivateProfileInt(_T("mini_mode"), _T("dark_mode"), 0, theApp.m_config_path.c_str()) != 0);
	m_follow_main_color = (GetPrivateProfileInt(_T("mini_mode"), _T("follow_main_color"), 1, theApp.m_config_path.c_str()) != 0);
}

void CMiniModeDlg::CheckWindowPos()
{
	CRect rect;
	GetWindowRect(rect);
	if (rect.left < 0)
	{
		rect.MoveToX(0);
		MoveWindow(rect);
	}
	if (rect.top < 0)
	{
		rect.MoveToY(0);
		MoveWindow(rect);
	}
	if (rect.right > m_screen_width)
	{
		rect.MoveToX(m_screen_width - rect.Width());
		MoveWindow(rect);
	}
	if (rect.bottom > m_screen_height)
	{
		rect.MoveToY(m_screen_height - rect.Height());
		MoveWindow(rect);
	}
}

void CMiniModeDlg::UpdateSongTipInfo()
{
	m_song_tip_info = _T("");
	m_song_tip_info += _T("��ǰ���ţ�");
	m_song_tip_info += theApp.m_player.GetFileName().c_str();
	m_song_tip_info += _T("\r\n");
	m_song_tip_info += _T("���⣺");
	m_song_tip_info += theApp.m_player.GetPlayList()[theApp.m_player.GetIndex()].title.c_str();
	m_song_tip_info += _T("\r\n");
	m_song_tip_info += _T("�����ң�");
	m_song_tip_info += theApp.m_player.GetPlayList()[theApp.m_player.GetIndex()].artist.c_str();
	m_song_tip_info += _T("\r\n");
	m_song_tip_info += _T("��Ƭ����");
	m_song_tip_info += theApp.m_player.GetPlayList()[theApp.m_player.GetIndex()].album.c_str();
	m_song_tip_info += _T("\r\n");
}

void CMiniModeDlg::DrawSpectral()
{
	if (m_spectrum_pDC == nullptr)
	{
		m_spectrum_pDC = m_spectral_static.GetDC();
	}
	//pDC->SetBkMode(TRANSPARENT);
	CBrush BGBrush, *pOldBrush;
	BGBrush.CreateSolidBrush(m_theme_color.original_color);
	pOldBrush = m_spectrum_pDC->SelectObject(&BGBrush);
	DrawThemeParentBackground(m_hWnd, m_spectrum_pDC->GetSafeHdc(), &m_spectral_client_rect);	//�ػ�ؼ������Խ�������ص�������
	//��CPlayer�����õ���Ƶ�����ݱ任��ROW�е�����
	float spectral_data[ROW];
	memset(spectral_data, 0, sizeof(spectral_data));
	for (int i{}; i < 16; i++)
	{
		spectral_data[i / (16 / ROW)] += theApp.m_player.GetSpectralData()[i];
	}
	for (int i{}; i < ROW; i++)
	{
		CRect rect_tmp{ m_spectral_rect[i] };
		int spetral_height = static_cast<int>(spectral_data[i] * m_spectral_client_rect.Height() / 120 * theApp.m_app_setting_data.sprctrum_height / 100);
		if (spetral_height <= 0 || theApp.m_player.IsError()) spetral_height = 1;		//Ƶ�׸߶�����Ϊ1�����أ�������ų���Ҳ����ʾƵ��
		rect_tmp.top = rect_tmp.bottom - spetral_height;
		if (rect_tmp.top < 0) rect_tmp.top = 0;
		m_spectrum_pDC->FillRect(&rect_tmp, &BGBrush);
	}
	m_spectrum_pDC->SelectObject(pOldBrush);
	BGBrush.DeleteObject();

}

void CMiniModeDlg::DrawBorder()
{
	//CDC* pDC = GetDC();
	//int height = (m_show_playlist ? m_rect.Height() : m_rect_s.Height());
	//CPen aPen;
	//aPen.CreatePen(PS_SOLID, 2 * theApp.m_dpi / 96 , m_item_text_color);
	//CPen* pOldPen{ pDC->SelectObject(&aPen) };
	//pDC->MoveTo(0, 0);
	//pDC->LineTo(m_rect.Width(), 0);
	//pDC->LineTo(m_rect.Width(), height);
	//pDC->LineTo(0, height);
	//pDC->LineTo(0, 0);
	//pDC->SelectObject(pOldPen);

}


BEGIN_MESSAGE_MAP(CMiniModeDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
//	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_MINI_MODE_EXIT, &CMiniModeDlg::OnMiniModeExit)
	//ON_COMMAND(ID_SONG_INFO, &CMiniModeDlg::OnSongInfo)
	ON_WM_INITMENU()
	ON_COMMAND(ID_SET_TEXT_COLOR, &CMiniModeDlg::OnSetTextColor)
	ON_BN_CLICKED(ID_M_PLAY_PAUSE, &CMiniModeDlg::OnBnClickedMPlayPause)
	ON_BN_CLICKED(ID_M_PREVIOUS, &CMiniModeDlg::OnBnClickedMPrevious)
	ON_BN_CLICKED(ID_M_NEXT, &CMiniModeDlg::OnBnClickedMNext)
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CMiniModeDlg::OnNMDblclkList2)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CMiniModeDlg::OnNMRClickList2)
	ON_BN_CLICKED(IDC_SHOW_LIST_BUTTON, &CMiniModeDlg::OnBnClickedShowListButton)
	ON_WM_PAINT()
	ON_COMMAND(ID_DARK_MODE, &CMiniModeDlg::OnDarkMode)
	ON_COMMAND(ID_FOLLOW_MAIN_COLOR, &CMiniModeDlg::OnFollowMainColor)
	ON_STN_CLICKED(IDC_MINI_PROGRESS_STATIC, &CMiniModeDlg::OnStnClickedMiniProgressStatic)
END_MESSAGE_MAP()


// CMiniModeDlg ��Ϣ�������


void CMiniModeDlg::UpdatePlayPauseButton()
{
	if (theApp.m_player.IsPlaying() && !theApp.m_player.IsError())
	{
		m_play_pause_button.SetImage(m_hPauseIcon_s, FALSE);
		m_play_pause_button.SetWindowText(_T(""));		//���ܰ�ť��ֻ��ʾͼ�������ʾ�ı��������������SetWindowText�������Խ�����°�ťͼ���ˢ�µ�����
		m_Mytip.UpdateTipText(_T("��ͣ"), &m_play_pause_button);
	}
	else
	{
		m_play_pause_button.SetImage(m_hPlayIcon_s, FALSE);
		m_play_pause_button.SetWindowText(_T(""));
		m_Mytip.UpdateTipText(_T("����"), &m_play_pause_button);
	}
}

void CMiniModeDlg::ShowPlaylist()
{
	m_playlist_ctrl.ShowPlaylist(m_display_format);
	SetPlayListColor();
}

void CMiniModeDlg::SetPlayListColor()
{
	//m_playlist_ctrl.ResetAllItemColor();
	//m_playlist_ctrl.SetItemColor(theApp.m_player.GetIndex(), m_item_text_color, m_item_back_color);
	m_playlist_ctrl.SetHightItem(theApp.m_player.GetIndex());
	m_playlist_ctrl.SetColor(m_theme_color);
	m_playlist_ctrl.EnsureVisible(theApp.m_player.GetIndex(), FALSE);
}

BOOL CMiniModeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	LoadConfig();

	SetWindowText(_T("MiniDlg_ByH87M"));		//Ϊ�Ի�������һ�����⣬����ͨ��FindWindow��������

	GetWindowRect(m_rect);
	CRect rect;
	m_lyric_static.GetWindowRect(rect);
	m_rect_s = m_rect;
	m_rect_s.bottom = rect.bottom + DPI(2);

	//��ȡר������ľ�������
	m_album_rect = m_rect_s;
	CRect previous_btn_tect;
	m_previous_button.GetWindowRect(previous_btn_tect);
	m_album_rect.right = previous_btn_tect.left;
	m_album_rect.MoveToXY(0, 0);
	m_album_rect.DeflateRect(DPI(2), DPI(2));


	//���ô��ڱ���ͼƬ
	//m_back_img = (HBITMAP)LoadImage(NULL, (CCommon::GetExePath() + L"minimode_background.bmp").c_str(), IMAGE_BITMAP, m_rect_s.Width(), m_rect_s.Height(), LR_LOADFROMFILE);
	//SetBackgroundImage(m_back_img);

	m_lyric_static.SetTextCenter(true);

	//���밴ťСͼ�꣨16*16��
	m_hPreviousIcon_s = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PREVIOUS), IMAGE_ICON, DPI(16), DPI(16), LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
	m_hNextIcon_s = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_NEXT1), IMAGE_ICON, DPI(16), DPI(16), LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
	m_hPlayIcon_s = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PLAY), IMAGE_ICON, DPI(16), DPI(16), LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);
	m_hPauseIcon_s = (HICON)LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_PAUSE), IMAGE_ICON, DPI(16), DPI(16), LR_DEFAULTCOLOR | LR_CREATEDIBSECTION);

	//���ð�ťͼ��
	m_previous_button.SetImage(m_hPreviousIcon_s);
	if (theApp.m_player.IsPlaying())
		m_play_pause_button.SetImage(m_hPauseIcon_s, FALSE);
	else
		m_play_pause_button.SetImage(m_hPlayIcon_s, FALSE);
	m_next_button.SetImage(m_hNextIcon_s);

	//���ð�ť�ϵ��ı�Ϊ��
	m_previous_button.SetWindowText(_T(""));
	m_play_pause_button.SetWindowText(_T(""));
	m_next_button.SetWindowText(_T(""));

	m_show_list_button.SetFont(GetFont());
	m_return_button.SetFont(GetFont());
	m_exit_button.SetFont(GetFont());

	//��ʼ�������ʾ
	m_Mytip.Create(this, TTS_ALWAYSTIP);
	m_Mytip.AddTool(&m_previous_button, _T("��һ��"));
	if (theApp.m_player.IsPlaying())
		m_Mytip.AddTool(&m_play_pause_button, _T("��ͣ"));
	else
		m_Mytip.AddTool(&m_play_pause_button, _T("����"));
	m_Mytip.AddTool(&m_next_button, _T("��һ��"));
	m_Mytip.AddTool(GetDlgItem(IDOK), _T("��������ģʽ"));
	m_Mytip.AddTool(GetDlgItem(ID_MINI_MODE_EXIT), _T("�˳�����"));
	m_Mytip.AddTool(GetDlgItem(IDC_SHOW_LIST_BUTTON), _T("��ʾ/���ز����б�"));
	m_Mytip.SetMaxTipWidth(800);

	UpdateSongTipInfo();
	m_Mytip.AddTool(this, m_song_tip_info);

	//��ʼ������λ��
	if (m_position_x != -1 && m_position_y != -1)
		SetWindowPos(nullptr , m_position_x, m_position_y, m_rect_s.Width(), m_rect_s.Height(), SWP_NOZORDER);
	CheckWindowPos();

	//װ���Ҽ��˵�
	m_menu.LoadMenu(IDR_MINI_MODE_MENU);
	//m_list_popup_menu.LoadMenu(IDR_POPUP_MENU);		//װ���Ҽ��˵�

	//����Ƶ�׷�����ÿ�����ε�λ��
	m_spectral_static.GetClientRect(&m_spectral_client_rect);
	int width{ m_spectral_client_rect.Width()/ROW };	//ÿ�����εĿ��
	m_spectral_rect[0] = m_spectral_client_rect;
	m_spectral_rect[0].right = m_spectral_rect[0].left + width - 1;
	for (int i{ 1 }; i < ROW; i++)
	{
		m_spectral_rect[i] = m_spectral_rect[0];
		m_spectral_rect[i].left += (i * width);
		m_spectral_rect[i].right += (i * width);
	}

	//��ȡƵ�׷����ؼ���CDC�����ڻ���Ƶ������
	m_spectrum_pDC = m_spectral_static.GetDC();

	m_draw.Create(GetDC(), this);

	//Ϊ��ȷ��ÿ�δ����㴰��ʱһ������ʾ��ǰ��ʣ������ｫ�˱�����Ϊ-1
	m_last_lyric_index = -1;

	m_last_index = -1;

	////��ʾ���Ÿ�����
	//m_lyric_static.SetWindowText(theApp.m_player.GetFileName().c_str());

	//��ʼ���������ؼ�
	m_time_static.GetWindowRect(rect);
	ScreenToClient(rect);
	rect.MoveToY(rect.bottom);
	rect.bottom = rect.top + DPI(8);
	m_progress_bar.MoveWindow(rect);
	m_progress_bar.GetToolTip()->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);	//������Ϣ��ʾ�ö�
	m_progress_bar.SetProgressBarHeight(DPI(2));
	m_progress_bar.SetRange(1000);		//���ý������ķ�Χ
	
	m_progress_bar.SetSongLength(theApp.m_player.GetSongLength());

	//���ö�ʱ��
	SetTimer(TIMER_ID_MINI, TIMER_ELAPSE_MINI, NULL);	//��������ʱ��
	SetTimer(TIMER_ID_MINI2, TIMER_ELAPSE, NULL);		//��������Ƶ�׷����Ķ�ʱ��

	//��ʾ�����б�
	ShowPlaylist();

	//��������ɫ
	if (m_follow_main_color)
		m_theme_color = theApp.m_app_setting_data.theme_color;

	m_first_start = true;
	m_show_playlist = false;
	m_show_volume = false;
	m_first_show_time = true;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CMiniModeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMiniModeDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == TIMER_ID_MINI)
	{
		//���������ʾ
		static int index{};
		static wstring song_name{};
		//�����ǰ���ŵĸ��������仯���͸��������ʾ��Ϣ
		if (index != theApp.m_player.GetIndex() || song_name != theApp.m_player.GetFileName())
		{
			UpdateSongTipInfo();
			m_Mytip.UpdateTipText(m_song_tip_info, this);
			index = theApp.m_player.GetIndex();
			song_name = theApp.m_player.GetFileName();
		}
		
	}
	if (nIDEvent == TIMER_ID_MINI2)
	{
		if (m_first_start)
		{
			//���ڸոմ�ʱִ��
			ColorChanged();
			RePaint();
			m_first_start = false;
		}
		if (theApp.m_player.IsPlaying())
		{
			DrawSpectral();
		}
		ShowInfo(false);
	}
	if (nIDEvent == 11)
	{
		m_show_volume = false;
		KillTimer(11);
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CMiniModeDlg::ShowVolume(int volume)
{
	CString volume_str;
	volume_str.Format(_T("����:%d%%"), volume);
	m_time_static.DrawWindowText(volume_str);
}

void CMiniModeDlg::ShowInfo(bool force_refresh)
{
	if (theApp.m_player.m_Lyrics.IsEmpty())	//û�и��ʱ��ʾ���ŵ��ļ���
	{
		//���ڲ��ŵ��ļ����Թ�������ʽ��ʾ������ÿ����һ�ι���DPI/72�����أ�150%����ʱΪ2�����أ����������Ҫ��ǿ��ˢ�£������ù���λ��
		m_lyric_static.DrawScrollText(CPlayListCtrl::GetDisplayStr(theApp.m_player.GetCurrentSongInfo(), m_display_format).c_str(), theApp.m_dpi / 72, force_refresh);
	}
	else		//��ʾ���
	{
		if (theApp.m_play_setting_data.lyric_karaoke_disp)		//����Կ���OK��ʽ��ʾʱ
		{
			wstring current_lyric{ theApp.m_player.m_Lyrics.GetLyric(Time(theApp.m_player.GetCurrentPosition()), 0) };	//��ȡ�����
			int progress{ theApp.m_player.m_Lyrics.GetLyricProgress(Time(theApp.m_player.GetCurrentPosition())) };		//��ȡ��ǰ��ʽ��ȣ���ΧΪ0~1000��
			if (current_lyric.empty())		//�����ǰ���Ϊ�հף�����ʾΪʡ�Ժ�
				current_lyric = DEFAULT_LYRIC_TEXT;
			m_lyric_static.DrawWindowText(current_lyric.c_str(), progress);
		}
		else				//��ʲ��Կ���OK��ʽ��ʾʱ
		{
			int lyric_index = theApp.m_player.m_Lyrics.GetLyricIndex(Time(theApp.m_player.GetCurrentPosition()));
			//�ж���ʾ�ĸ���Ƿ����仯����������˱仯�������Ҫ��ǿ��ˢ�£��͸��¸����ʾ
			if (lyric_index != m_last_lyric_index || force_refresh)
			{
				wstring current_lyric{ theApp.m_player.m_Lyrics.GetLyric(Time(theApp.m_player.GetCurrentPosition()), 0) };
				if (current_lyric.empty())		//�����ǰ���Ϊ�հף�����ʾΪʡ�Ժ�
					current_lyric = DEFAULT_LYRIC_TEXT;
				m_lyric_static.DrawWindowText(current_lyric.c_str());
				m_last_lyric_index = lyric_index;
			}
		}
	}
	//��ʾ����ʱ��
	if (!m_show_volume)
	{
		static int last_second;
		int second = theApp.m_player.GetCurrentSecond();
		if (second != last_second || m_first_show_time)
		{
			if (theApp.m_player.IsError())		//���ִ���ʱ��ʾ������Ϣ
			{
				m_time_static.DrawWindowText(_T("���ų���"));
			}
			else
			{
				m_time_static.DrawWindowText(theApp.m_player.GetTimeString().c_str());
			}
			last_second = second;
			m_first_show_time = false;
		}
	}

	//����ר������
	if (theApp.m_player.AlbumCoverExist())
		m_draw.DrawBitmap(theApp.m_player.GetAlbumCover(), m_album_rect.TopLeft(), m_album_rect.Size(), CDrawCommon::StretchMode::FILL);
	else
		m_draw.DrawBitmap(IDB_DEFAULT_COVER, m_album_rect.TopLeft(), m_album_rect.Size(), CDrawCommon::StretchMode::FILL);
}

void CMiniModeDlg::ColorChanged()
{
	//m_item_text_color = CColorConvert::ConvertToItemColor(theme_color);
	//m_item_back_color = CColorConvert::ConvertToBackColor(theme_color);
	//m_lyric_text_color = CColorConvert::ConvertToLightColor(theme_color);
	CColorConvert::ConvertColor(m_theme_color);
	if (!m_dark_mode)
	{
		//���ô����ı���ɫ
		m_lyric_static.SetTextColor(m_theme_color.dark2);
		m_lyric_static.SetText2Color(m_theme_color.light1);
		m_time_static.SetTextColor(m_theme_color.dark2);
		//���ñ�����ɫ
		SetBackgroundColor(m_theme_color.light3);
		//���ý�������ɫ
		m_progress_bar.SetColor(m_theme_color.original_color);
		m_progress_bar.SetBackColor(RGB(255, 255, 255));
	}
	else
	{
		//���ô����ı���ɫ
		m_lyric_static.SetTextColor(m_theme_color.light3);
		m_lyric_static.SetText2Color(m_theme_color.light1);
		m_time_static.SetTextColor(m_theme_color.light3);
		//���ñ�����ɫ
		SetBackgroundColor(m_theme_color.dark2);
		//���ý�������ɫ
		m_progress_bar.SetColor(RGB(255, 255, 255));
		m_progress_bar.SetBackColor(m_theme_color.dark1);
	}
	SetPlayListColor();
	//m_progress_bar.Invalidate();
}

void CMiniModeDlg::SetVolume(bool up)
{
	if (up)
		theApp.m_player.MusicControl(Command::VOLUME_UP);
	else
		theApp.m_player.MusicControl(Command::VOLUME_DOWN);
	ShowVolume(theApp.m_player.GetVolume());
	KillTimer(11);
	SetTimer(11, 1500, NULL);		//��ʾ����������һ��1500����Ķ�ʱ����������ʾ����1.5�룩
	m_show_volume = true;
}

void CMiniModeDlg::RePaint()
{
	ShowInfo(true);
	DrawSpectral();
	DrawBorder();
}


BOOL CMiniModeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		OnOK();		//��ESC������������
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;

	//�����·������������
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_UP)
	{
		SetVolume(true);
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DOWN)
	{
		SetVolume(false);
		return TRUE;
	}

	//��Ctrl+X�˳�
	if (pMsg->message == WM_KEYDOWN && (GetKeyState(VK_CONTROL) & 0x80) && pMsg->wParam == 'X')
	{
		OnCancel();
		return TRUE;
	}
	//��Ctrl+M�ص�������
	if (pMsg->message == WM_KEYDOWN && (GetKeyState(VK_CONTROL) & 0x80) && pMsg->wParam == 'M')
	{
		OnOK();
		return TRUE;
	}

	//���˴��ڵ�����������Ϣת����������
	if (pMsg->message == WM_KEYDOWN)
	{
		::PostMessage(theApp.m_pMainWnd->m_hWnd, WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
		return TRUE;
	}

	if (pMsg->message == WM_MOUSEMOVE)
		m_Mytip.RelayEvent(pMsg);

	return CDialogEx::PreTranslateMessage(pMsg);
}


//void CMiniModeDlg::OnClose()
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//	SaveConfig();
//	CDialogEx::OnClose();
//}


void CMiniModeDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	GetWindowRect(rect);
	m_position_x = rect.left;
	m_position_y = rect.top;
	SaveConfig();
	KillTimer(TIMER_ID_MINI);
	m_menu.DestroyMenu();
	ReleaseDC(m_spectrum_pDC);
}


void CMiniModeDlg::OnMove(int x, int y)
{
	CDialogEx::OnMove(x, y);

	// TODO: �ڴ˴������Ϣ����������
	CheckWindowPos();
}


void CMiniModeDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//���õ������Ҽ������˵�
	CMenu* pContextMenu = m_menu.GetSubMenu(0); //��ȡ��һ�������˵������Ե�һ���˵��������Ӳ˵� 
	CPoint point1;	//����һ������ȷ�����λ�õ�λ��  
	GetCursorPos(&point1);	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����  
	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point1.x, point1.y, this); //��ָ��λ����ʾ�����˵�

	CDialogEx::OnRButtonUp(nFlags, point);
}


void CMiniModeDlg::OnMiniModeExit()
{
	// TODO: �ڴ���������������
	//ִ���˳�����ʱ�öԻ��򷵻�IDCANCEL�������ڼ�⵽�������ֵʱ���˳�����
	OnCancel();
}


//void CMiniModeDlg::OnSongInfo()
//{
//	// TODO: �ڴ���������������
//	CPropertyDlg propertyDlg(theApp.m_player.GetPlayList());
//	propertyDlg.m_path = theApp.m_player.GetCurrentPath();
//	propertyDlg.m_index = theApp.m_player.GetIndex();
//	propertyDlg.m_song_num = theApp.m_player.GetSongNum();
//	propertyDlg.DoModal();
//}


void CMiniModeDlg::OnInitMenu(CMenu* pMenu)
{
	CDialogEx::OnInitMenu(pMenu);

	// TODO: �ڴ˴������Ϣ����������

	//��������ģʽ�����Ҽ��˵�
	//����ѭ��ģʽ�˵��ĵ�ѡ���
	RepeatMode repeat_mode{ theApp.m_player.GetRepeatMode() };
	switch (repeat_mode)
	{
	case RM_PLAY_ORDER: pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_PLAY_ORDER, MF_BYCOMMAND | MF_CHECKED); break;
	case RM_PLAY_SHUFFLE: pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_PLAY_SHUFFLE, MF_BYCOMMAND | MF_CHECKED); break;
	case RM_LOOP_PLAYLIST: pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_LOOP_PLAYLIST, MF_BYCOMMAND | MF_CHECKED); break;
	case RM_LOOP_TRACK: pMenu->CheckMenuRadioItem(ID_PLAY_ORDER, ID_LOOP_TRACK, ID_LOOP_TRACK, MF_BYCOMMAND | MF_CHECKED); break;
	default: break;
	}

	//���ø�ѡ�˵���Ĺ�ѡ
	pMenu->CheckMenuItem(ID_DARK_MODE, MF_BYCOMMAND | (m_dark_mode ? MF_CHECKED : MF_UNCHECKED));
	pMenu->CheckMenuItem(ID_FOLLOW_MAIN_COLOR, MF_BYCOMMAND | (m_follow_main_color ? MF_CHECKED : MF_UNCHECKED));

	//���ò����б��Ҽ��˵�
	//���������б��Ҽ��˵�ʱ�����û��ѡ�в����б��е���Ŀ��������Ҽ��˵��С����š��������б���ɾ�����������ԡ������Ӵ���ɾ������Ŀ��
	if (m_item_selected < 0 || m_item_selected >= theApp.m_player.GetSongNum())
	{
		pMenu->EnableMenuItem(ID_PLAY_ITEM, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_REMOVE_FROM_PLAYLIST, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_ITEM_PROPERTY, MF_BYCOMMAND | MF_GRAYED);
		pMenu->EnableMenuItem(ID_DELETE_FROM_DISK, MF_BYCOMMAND | MF_GRAYED);
	}
	else
	{
		pMenu->EnableMenuItem(ID_PLAY_ITEM, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_REMOVE_FROM_PLAYLIST, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_ITEM_PROPERTY, MF_BYCOMMAND | MF_ENABLED);
		pMenu->EnableMenuItem(ID_DELETE_FROM_DISK, MF_BYCOMMAND | MF_ENABLED);
	}
	//���ò����б��Ҽ��˵���Ĭ�ϲ˵���
	pMenu->SetDefaultItem(ID_PLAY_ITEM);

}


void CMiniModeDlg::OnSetTextColor()
{
	// TODO: �ڴ���������������
	CColorDialog colorDlg(m_theme_color.original_color, 0, this);
	if (colorDlg.DoModal() == IDOK)
	{
		m_theme_color.original_color = colorDlg.GetColor();
		ColorChanged();
		RePaint();
		//m_lyric_static.SetTextColor(m_item_text_color);
		//m_time_static.SetTextColor(m_item_text_color);
		//DrawBorder();
		SaveConfig();
		m_follow_main_color = false;
	}
}


void CMiniModeDlg::OnBnClickedMPlayPause()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetFocus();
	SendMessage(WM_COMMAND, ID_PLAY_PAUSE);
}


void CMiniModeDlg::OnBnClickedMPrevious()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetFocus();
	SendMessage(WM_COMMAND, ID_PREVIOUS);
}


void CMiniModeDlg::OnBnClickedMNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetFocus();
	SendMessage(WM_COMMAND, ID_NEXT);
}


BOOL CMiniModeDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (zDelta > 0)
	{
		SetVolume(true);
	}
	if (zDelta < 0)
	{
		SetVolume(false);
	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CMiniModeDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	OnOK();

	CDialogEx::OnLButtonDblClk(nFlags, point);
}


void CMiniModeDlg::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int row = pNMItemActivate->iItem;
	theApp.m_player.PlayTrack(row);
	//SwitchTrack();
	SetPlayListColor();
	RePaint();
	UpdatePlayPauseButton();
	*pResult = 0;
}


void CMiniModeDlg::OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_item_selected = pNMItemActivate->iItem;	//��ȡ���ѡ�е���Ŀ

	CMenu* pContextMenu = m_popup_menu.GetSubMenu(0); //��ȡ��һ�������˵�
	CPoint point;			//����һ������ȷ�����λ�õ�λ��  
	GetCursorPos(&point);	//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����

	pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this); //��ָ��λ����ʾ�����˵�
	*pResult = 0;
}


void CMiniModeDlg::OnBnClickedShowListButton()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SetFocus();
	//�л���ʾ�����б�
	if (m_show_playlist)
	{
		//MoveWindow(m_rect_s);
		SetWindowPos(nullptr, 0, 0, m_rect_s.Width(), m_rect_s.Height(), SWP_NOMOVE | SWP_NOZORDER);
		CheckWindowPos();
		m_show_playlist = false;
	}
	else
	{
		//MoveWindow(m_rect);
		SetWindowPos(nullptr, 0, 0, m_rect.Width(), m_rect.Height(), SWP_NOMOVE | SWP_NOZORDER);
		CheckWindowPos();
		m_show_playlist = true;
	}
	//SetBackgroundImage(m_back_img);
	//ShowInfo(true);
	RePaint();
	//DrawSpectral();
	//DrawBorder();
}


void CMiniModeDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


void CMiniModeDlg::OnDarkMode()
{
	// TODO: �ڴ���������������
	m_dark_mode = !m_dark_mode;
	ColorChanged();
	RePaint();
	//DrawBorder();
}


void CMiniModeDlg::OnFollowMainColor()
{
	// TODO: �ڴ���������������
	m_follow_main_color = !m_follow_main_color;
	if (m_follow_main_color)
		m_theme_color = theApp.m_app_setting_data.theme_color;
	ColorChanged();
	RePaint();
}


void CMiniModeDlg::OnStnClickedMiniProgressStatic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int pos = m_progress_bar.GetPos();
	int song_pos = static_cast<__int64>(pos) * theApp.m_player.GetSongLength() / 1000;
	theApp.m_player.SeekTo(song_pos);
}
