//�����б�ר�õĿؼ��࣬��CListCtrl����������ʵ������CListCtrl�ؼ�������ɫ
//�Լ������ʾ��Ϣ
#pragma once
#include "AudioCommon.h"

// CPlayListCtrl

class CPlayListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CPlayListCtrl)

public:
	CPlayListCtrl(const vector<SongInfo>& all_song_info);
	virtual ~CPlayListCtrl();

	void SetHightItem(int item) { m_hight_item = item; }					//���ø�������Ŀ���������б������ڲ��ŵ���Ŀ
	void SetColor(COLORREF TextColor, COLORREF TextBkColor, COLORREF selected_color, COLORREF other_text_color);	//���ò����б����ɫ

	void EnableTip(bool enable = true) { m_bEnableTips = enable; }		//�����Ƿ�����ʾ

	static wstring GetDisplayStr(const SongInfo& song_info, DisplayFormat display_format);		//����display_formatָ������ʾ��ʽ������һ����Ŀ��ʾ���ַ���
	void ShowPlaylist(DisplayFormat display_format);		//��ʾ�����б�

protected:
	COLORREF m_background_color{ RGB(255,255,255) };	//�б�ؼ��ı���ɫ
	COLORREF m_text_color;		//������Ŀ���ı���ɫ
	COLORREF m_back_color;		//������Ŀ�ı�����ɫ
	COLORREF m_selected_color;	//ѡ����Ŀ����ɫ
	COLORREF m_other_text_color;	//�Ǹ�����Ŀ���ı���ɫ
	int m_hight_item;			//��������Ŀ

	CToolTipCtrl m_toolTip;		//�ı���ʾ��
	int m_nItem;				//����к�
	//int m_nSubItem;			//����к�
	bool m_bEnableTips{ false };	//�Ƿ����ı���ʾ

	const vector<SongInfo>& m_all_song_info;		//���沥���б������и�������Ϣ

	afx_msg void CPlayListCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};


