#pragma once
#include "afxcmn.h"
#include "ColorConvert.h"
class CListCtrlEx :
	public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlEx)
public:
	CListCtrlEx();
	~CListCtrlEx();

	void SetColor(const ColorTable& colors);
	void SetHightItem(int item) { m_highlight_item = item; }					//���ø�������Ŀ���������б������ڲ��ŵ���Ŀ

protected:
	ColorTable m_theme_color;
	COLORREF m_background_color{ GRAY(255) };	//�б�ؼ��ı���ɫ
	int m_highlight_item{};			//��������Ŀ

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void PreSubclassWindow();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

