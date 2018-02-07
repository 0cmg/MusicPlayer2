//CStatic��������࣬����ʵ������Static�ؼ�������ɫ��
//�Լ���������˴��ڱ���ͼƬʱ�ؼ������ص������⡣
#pragma once
#include "afxwin.h"
class CStaticEx :
	public CStatic
{
public:
	CStaticEx();
	~CStaticEx();

	void DrawWindowText(LPCTSTR lpszString);	//�ڿؼ��ϻ�������ɫ���ı�����Ҫ���SetTextColorʹ�ã�
	void DrawWindowText(LPCTSTR lpszString, int split);	//�ڿؼ��ϻ��Ʒָ���ɫ���ı���splitΪ��ɫ�ָ��λ�ã�ȡֵΪ0~1000����������ģʽ�¸�ʶ�̬��ʾ��
	void DrawScrollText(LPCTSTR lpszString, int pixel, bool reset = false);	//�ڿؼ��ϻ��ƹ������ı��������Ȳ���ʱ����pixelָ���˺�������һ���ƶ�������ֵ�����resetΪtrue�����������ʼλ��
	void SetTextColor(COLORREF textColor);		//���ÿؼ��ı���ɫ
	void SetText2Color(COLORREF textColor);
	CString GetString() const;			//��ȡ�ؼ��ı�
	void SetTextCenter(bool center) { m_center = center; }	//�����ı�����

	void SetFillColor(COLORREF fill_color);		//����Ҫ���ı���ɫ

protected:
	COLORREF m_TextColor;	//�ؼ�������ɫ
	COLORREF m_TextBackColor;	//�ؼ��ı��ĵ�2����ɫ
	CString m_text;			//�ؼ��ϵ��ı�
	bool m_center{ false };		//�ı��Ƿ����

	COLORREF m_fill_color{};
	bool m_fill_color_enable{ false };

	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	afx_msg void OnPaint();
};

