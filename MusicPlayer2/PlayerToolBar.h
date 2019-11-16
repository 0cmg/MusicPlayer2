#pragma once
#include "CommonData.h"

// CPlayerToolBar
// �Ի�Ĺ�����

class CPlayerToolBar : public CStatic
{
	DECLARE_DYNAMIC(CPlayerToolBar)
public:
	CPlayerToolBar();
	virtual ~CPlayerToolBar();

    void AddToolButton(IconRes icon, LPCTSTR strText, LPCTSTR strToolTip, UINT cmdId, bool showTest = false);        //���һ����ť���������cmdId����
    void AddToolButton(IconRes icon, LPCTSTR strText, LPCTSTR strToolTip, CMenu* pMenu, bool showTest = false);      //���һ����ť������󵯳��˵�
    void SetIconSize(int size);

	CToolTipCtrl* GetToolTip() { return &m_tool_tip; }

protected:
    struct ToolBtn
    {
        IconRes icon;           //ͼ��
        CString text;           //��ʾ�ı�
        bool show_text = false; //�Ƿ���ʾ�ı�
        UINT cmd_id;            //������͵�����ID
        CMenu* pMenu = nullptr; //����󵯳��Ĳ˵�
        bool is_cmd = true;     //�����������ǵ����˵�
        CString tooltip_text;   //�����ʾ�ı�
        CRect rect;				//��ť�ľ�������
        bool hover{ false };	//����Ƿ�ָ��ť
        bool pressed{ false };	//��ť�Ƿ���
        bool enable{ true };	//��ť�Ƿ�����
    };

    void AddToolTips();

protected:
	CToolTipCtrl m_tool_tip;		//�ı���ʾ��
    std::vector<ToolBtn> m_buttons;
    ColorTable& m_theme_color;
    int m_icon_size = 20;
    int m_cur_btn_id = 100;
    bool m_first_draw = true;

	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void PreSubclassWindow();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnMouseLeave();
    afx_msg void OnMouseHover(UINT nFlags, CPoint point);
protected:
    afx_msg LRESULT OnInitmenu(WPARAM wParam, LPARAM lParam);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};


