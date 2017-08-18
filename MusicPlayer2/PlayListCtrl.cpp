// ListCtrlEx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MusicPlayer2.h"
#include "PlayListCtrl.h"


// CPlayListCtrl

IMPLEMENT_DYNAMIC(CPlayListCtrl, CListCtrl)

//ͨ�����캯�����������б��������ļ�����Ϣ������
CPlayListCtrl::CPlayListCtrl(const vector<SongInfo>& all_song_info) :m_all_song_info{ all_song_info }
{
	m_toolTip.Create(this, TTS_ALWAYSTIP | TTS_NOPREFIX);
}

CPlayListCtrl::~CPlayListCtrl()
{
}

void CPlayListCtrl::SetColor(COLORREF TextColor, COLORREF TextBkColor, COLORREF selected_color, COLORREF other_text_color)
{
	m_text_color = TextColor;
	m_back_color = TextBkColor;
	m_selected_color = selected_color;
	m_other_text_color = other_text_color;
	DWORD itemCount = GetItemCount();
	this->RedrawItems(0, itemCount - 1);
}

wstring CPlayListCtrl::GetDisplayStr(const SongInfo & song_info, DisplayFormat display_format)
{
	switch (display_format)
	{
	case DF_FILE_NAME:		//��ʾΪ�ļ���
		return song_info.file_name;
	case DF_TITLE:			//��ʾΪ��������
		if (song_info.title == DEFAULT_TITLE)	//�����ȡ�����������⣬����ʾ�ļ���
			return song_info.file_name;
		else
			return song_info.title;
	case DF_ARTIST_TITLE:	//��ʾΪ������ - ����
		if (song_info.title == DEFAULT_TITLE && song_info.artist == DEFAULT_ARTIST)		//�������������Ҷ���ȡ����������ʾ�ļ���
			return song_info.file_name;
		else
			return (song_info.artist + _T(" - ") + song_info.title);
	case DF_TITLE_ARTIST:	//��ʾΪ���� - ������
		if (song_info.title == DEFAULT_TITLE && song_info.artist == DEFAULT_ARTIST)		//�������������Ҷ���ȡ����������ʾ�ļ���
			return song_info.file_name;
		else
			return (song_info.title + _T(" - ") + song_info.artist);
	default:
		return song_info.file_name;
	}
}

void CPlayListCtrl::ShowPlaylist(DisplayFormat display_format)
{
	int item_num_before = GetItemCount();
	int item_num_after = theApp.m_player.GetSongNum();
	//�����ǰ�б�����Ŀ������С��ԭ���ģ���ֱ�����ԭ���б������е���Ŀ���������
	if (item_num_after < item_num_before)
	{
		DeleteAllItems();
		item_num_before = 0;
	}
	CString str;
	for (int i{}; i < item_num_after; i++)
	{
		if (i >= item_num_before)	//�����ǰ�б��е���Ŀ��������֮ǰ������������Ҫ�ڲ���ʱ�����µ���Ŀ
		{
			str.Format(_T("%u"), i + 1);
			InsertItem(i, str);
		}
		SetItemText(i, 1, (GetDisplayStr(m_all_song_info[i], display_format)).c_str());

		SetItemText(i, 2, theApp.m_player.GetAllSongLength(i).time2str().c_str());
	}
}


BEGIN_MESSAGE_MAP(CPlayListCtrl, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, CPlayListCtrl::OnNMCustomdraw)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CPlayListCtrl ��Ϣ�������

void CPlayListCtrl::OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = CDRF_DODEFAULT;
	LPNMLVCUSTOMDRAW lplvdr = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	NMCUSTOMDRAW& nmcd = lplvdr->nmcd;
	switch (lplvdr->nmcd.dwDrawStage)	//�ж�״̬   
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:			//���Ϊ��ITEM֮ǰ��Ҫ������ɫ�ĸı�
		//��ѡ�������Ǹ�����ʱ
		if (GetItemState(nmcd.dwItemSpec, LVIS_SELECTED) == LVIS_SELECTED && nmcd.dwItemSpec == m_hight_item)
		{
			SetItemState(nmcd.dwItemSpec, 0, LVIS_SELECTED);
			lplvdr->clrText = m_back_color;
			lplvdr->clrTextBk = m_selected_color;
		}
		//ѡ������ɫ�ı�
		else if (GetItemState(nmcd.dwItemSpec, LVIS_SELECTED) == LVIS_SELECTED/*pLVCD->nmcd.uItemState & CDIS_SELECTED*/)
		{
			SetItemState(nmcd.dwItemSpec, 0, LVIS_SELECTED);
			lplvdr->clrText = RGB(255,255,255);
			lplvdr->clrTextBk = m_other_text_color;
		}
		//else if (MapItemColor.Lookup(nmcd.dwItemSpec, tb))
			// ������ SetItemColor(DWORD iItem, COLORREF color) ���õ�   
			// ITEM�ź�COLORREF �������в��ң�Ȼ�������ɫ��ֵ��
		else if(nmcd.dwItemSpec==m_hight_item)
		{
			lplvdr->clrText = m_text_color;
			lplvdr->clrTextBk = m_back_color;
		}
		else
		{
			lplvdr->clrText = GetSysColor(COLOR_WINDOWTEXT);
			lplvdr->clrTextBk = m_background_color;
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}




void CPlayListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//��������ı���ʾ
	if (m_bEnableTips)
	{
		CString str_tip;
		LVHITTESTINFO lvhti;

		// �ж���굱ǰ���ڵ�λ��(��, ��)
		lvhti.pt = point;
		SubItemHitTest(&lvhti);

		//�������ƶ�����һ��, ����д���; ����, ��������
		if (lvhti.iItem != m_nItem)
		{
			// ���浱ǰ������ڵ���
			m_nItem = lvhti.iItem;
			if (m_nItem >= m_all_song_info.size()) return;

			// �������ƶ���һ���Ϸ����У�����ʾ�µ���ʾ��Ϣ��������ʾ��ʾ
			if (m_nItem != -1)
			{
				CString dis_str = GetItemText(m_nItem, 1);
				int strWidth = GetStringWidth(dis_str) + DPI(10);		//��ȡҪ��ʾ��ǰ�ַ�������С���
				int columnWidth = GetColumnWidth(1);	//��ȡ���ָ���еĿ��
				if (columnWidth < strWidth)		//����Ԫ���ڵĵ��ַ��޷���ʾ��ȫʱ����ʾ�ĵ�1����ʾ��Ԫ�����ı�
				{
					str_tip += dis_str;
					str_tip += _T("\r\n");
				}

				str_tip += _T("�ļ�����");
				str_tip += m_all_song_info[m_nItem].file_name.c_str();
				str_tip += _T("\r\n");

				str_tip += _T("���⣺");
				str_tip += m_all_song_info[m_nItem].title.c_str();
				str_tip += _T("\r\n");

				str_tip += _T("�����ң�");
				str_tip += m_all_song_info[m_nItem].artist.c_str();
				str_tip += _T("\r\n");

				str_tip += _T("��Ƭ����");
				str_tip += m_all_song_info[m_nItem].album.c_str();
				str_tip += _T("\r\n");

				CString str_bitrate;
				str_bitrate.Format(_T("�����ʣ�%dkbps"), m_all_song_info[m_nItem].bitrate);
				str_tip += str_bitrate;

				m_toolTip.SetMaxTipWidth(DPI(400));		//������ʾ��Ϣ�Ŀ�ȣ���֧����ʾ����

				m_toolTip.AddTool(this, str_tip);
				m_toolTip.Pop();			// ��ʾ��ʾ��
			}
			else
			{
				m_toolTip.AddTool(this, _T(""));
				m_toolTip.Pop();
			}
		}
	}
	CListCtrl::OnMouseMove(nFlags, point);
}


BOOL CPlayListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (m_toolTip.GetSafeHwnd())
	{
		m_toolTip.RelayEvent(pMsg);
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}


void CPlayListCtrl::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	//CWindowDC dc(this);
	//HDC hDC = dc.GetSafeHdc();
	//m_dpi = GetDeviceCaps(hDC, LOGPIXELSY);

	//����ʾ��Ϣ��Ϊ�ö�
	m_toolTip.SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	//��ʼ�������б�
	SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	int width0, width1, width2;
	width0 = DPI(40);
	width2 = DPI(50);
	CRect rect;
	GetWindowRect(rect);
	width1 = rect.Width() - width0 - width2 - DPI(21);
	InsertColumn(0, _T("���"), LVCFMT_LEFT, width0);		//�����1��
	InsertColumn(1, _T("��Ŀ"), LVCFMT_LEFT, width1);		//�����2��
	InsertColumn(2, _T("����"), LVCFMT_LEFT, width2);		//�����3��
	EnableTip();

	this->SetBkColor(m_background_color);

	CListCtrl::PreSubclassWindow();
}


void CPlayListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	this->SetFocus();
	CListCtrl::OnLButtonDown(nFlags, point);
}


void CPlayListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	this->SetFocus();
	CListCtrl::OnRButtonDown(nFlags, point);
}
