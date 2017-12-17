//�����б�ר�õĿؼ��࣬��CListCtrlEx����������ʵ������CListCtrl�ؼ�������ɫ
//�Լ������ʾ��Ϣ
#pragma once
#include "AudioCommon.h"
#include "ColorConvert.h"
#include "ListCtrlEx.h"

// CPlayListCtrl

class CPlayListCtrl : public CListCtrlEx
{
	DECLARE_DYNAMIC(CPlayListCtrl)

public:
	CPlayListCtrl(const vector<SongInfo>& all_song_info);
	virtual ~CPlayListCtrl();

	void EnableTip(bool enable = true) { m_bEnableTips = enable; }		//�����Ƿ�����ʾ

	static wstring GetDisplayStr(const SongInfo& song_info, DisplayFormat display_format);		//����display_formatָ������ʾ��ʽ������һ����Ŀ��ʾ���ַ���
	void ShowPlaylist(DisplayFormat display_format);		//��ʾ�����б�

protected:

	CToolTipCtrl m_toolTip;		//�ı���ʾ��
	int m_nItem;				//����к�
	//int m_nSubItem;			//����к�
	bool m_bEnableTips{ false };	//�Ƿ����ı���ʾ

	const vector<SongInfo>& m_all_song_info;		//���沥���б������и�������Ϣ

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
};


