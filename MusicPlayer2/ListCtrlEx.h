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
    void ShowPopupMenu(CMenu* pMenu, int item_index, CWnd* pWnd);
    void FillLeftSpaceAfterPaint(bool fill);        //���Ϊtrue������ÿ�л���֮��������հף������ڻ���֮ǰ��䣨������û��ͼ���ѡ����Ӧ����Ϊtrue����������Ϊfalse��

    typedef map<int, wstring> RowData;      //�б�������ÿһ�е����ݣ�map��keyΪ����ţ�valueΪ��ʾ���ı�
    typedef vector<RowData> ListData;       //�б����ݣ�����vectorΪÿһ�е�����

    //�����б�����
	//��ע��ʹ�ô˺��������б�����ʹ���������б�ķ�ʽ���������������������ܶ������µļ����ٶȣ�
	//ʹ�ô˺���ʱ����ȷ���б���� LVS_OWNERDATA ��ʽ��
    void SetListData(ListData* pListData);

	//�����б�����
	//��ʹ�ô�ͳ�ķ�ʽ��
	void SetListData(const ListData& list_data);

	void SetCtrlAEnable(bool enable) { m_enable_ctrl_a = enable; }		//�Ƿ�����Ctrl + Aȫѡ

protected:
	const ColorTable& m_theme_color;
	COLORREF m_background_color{ GRAY(255) };	//�б�ؼ��ı���ɫ
	int m_highlight_item{ -1 };			//��������Ŀ
    bool m_drag_enable{ false };
    bool m_dragging{ false };
    HCURSOR m_drag_cursor{};
    bool m_fill_left_space_after_paint{ true };
	ListData* m_pListData{};
	bool m_enable_ctrl_a{ false };	//�Ƿ�����Ctrl+Aȫѡ

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
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLvnGetdispinfo(NMHDR *pNMHDR, LRESULT *pResult);
};

