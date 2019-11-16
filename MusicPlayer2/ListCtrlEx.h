#pragma once
#include "afxcmn.h"
#include "ColorConvert.h"

//����϶�����򸸴��ڷ��ʹ���Ϣ��ͨ��wPara�����ϷŽ�����λ��
//ע�⣬ִ���϶��������б���Ŀ��λ�ò���ı䣬��Ҫ����Ӧ����Ϣ���ֶ�ˢ���б�
#define WM_LIST_ITEM_DRAGGED (WM_USER+121)

class CListCtrlEx :
	public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlEx)
public:
	CListCtrlEx();
	~CListCtrlEx();

	//void SetColor(const ColorTable& colors);
	void GetItemSelected(vector<int>& item_selected) const;
	int GetCurSel() const;		//��ȡ��ǰѡ�е���Ŀ
	void SetCurSel(int select);
    void SetCurSel(int first, int last);
	void SelectAll();
	void SelectNone();
	void SelectReverse();
	bool SetRowHeight(int height);		//���ñ���и�

	void SetHightItem(int item) { m_highlight_item = item; }			//���ø�������Ŀ�������б������ڲ��ŵ���Ŀ��

    void SetDragEnable(bool enable = true) { m_drag_enable = enable; }      //�Ƿ���������϶�

protected:
	ColorTable& m_theme_color;
	COLORREF m_background_color{ GRAY(255) };	//�б�ؼ��ı���ɫ
	int m_highlight_item{ -1 };			//��������Ŀ
    bool m_drag_enable{ false };
    bool m_dragging{ false };
    HCURSOR m_drag_cursor{};

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void PreSubclassWindow();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
    afx_msg void OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};

