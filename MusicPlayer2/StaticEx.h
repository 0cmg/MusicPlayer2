//CStatic��������࣬������Static�ؼ��ϻ����ı�
#pragma once
#include "afxwin.h"
class CStaticEx :
	public CStatic
{
public:
	CStaticEx();
	~CStaticEx();

	enum class TextType
	{
		NORMAL,
		SPLIT,
		SCROLL
	};

	void DrawWindowText(LPCTSTR lpszString);	//�ڿؼ��ϻ�������ɫ���ı�����Ҫ���SetTextColorʹ�ã�
	void DrawWindowText(LPCTSTR lpszString, int split);	//�ڿؼ��ϻ��Ʒָ���ɫ���ı���splitΪ��ɫ�ָ��λ�ã�ȡֵΪ0~1000����������ģʽ�¸�ʶ�̬��ʾ��
	void DrawScrollText(LPCTSTR lpszString, int pixel, bool reset = false);	//�ڿؼ��ϻ��ƹ������ı��������Ȳ���ʱ����pixelָ���˺�������һ���ƶ�������ֵ�����resetΪtrue�����������ʼλ��
	void SetTextColor(COLORREF textColor);		//���ÿؼ��ı���ɫ
	void SetText2Color(COLORREF textColor);
	CString GetString() const;			//��ȡ�ؼ��ı�
	void SetTextCenter(bool center) { m_center = center; }	//�����ı�����

protected:
	COLORREF m_TextColor;	//�ؼ�������ɫ
	COLORREF m_TextBackColor;	//�ؼ��ı��ĵ�2����ɫ
	CString m_text;			//�ؼ��ϵ��ı�
	bool m_center{ false };		//�ı��Ƿ����

	TextType m_text_type;
	int m_split;
	int m_scroll_pixel;
	bool m_scroll_reset;

	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
	afx_msg void OnPaint();
};

