//��������ڶ������ڻ�ͼ�ĺ���
#pragma once
class CDrawCommon
{
public:

	//����ģʽ
	enum class StretchMode
	{
		STRETCH,		//���죬��ı����
		FILL,			//��䣬���ı��������ü�����
		FIT			//��Ӧ������ı���������ü�
	};

	//������DrawScrollText��������ʱʹ�õ�һЩ��Ҫ�ں���������Ϻ�������ڵı���
	struct ScrollInfo
	{
		int shift_cnt{};		//�ƶ��Ĵ���
		bool shift_dir{};		//�ƶ��ķ�������Ϊfalse������Ϊtrue
		int freez{};			//���ñ�������0ʱ���ı���������ֱ��С�ڵ���0Ϊֹ
		bool dir_changed{ false };	//����������˱仯����Ϊtrue
	};

	CDrawCommon();
	~CDrawCommon();

	void Create(CDC* pDC, CWnd* pMainWnd);
	//void SetBackColor(COLORREF back_color);		//���û����ı�ʱ���ı�����ɫ
	//COLORREF GetBackColor() const { return m_backColor; }
	void SetFont(CFont* pfont);		//���û����ı�������
	void SetDC(CDC* pDC);		//���û�ͼ��DC
	CDC* GetDC() { return m_pDC; }

	//�����ǻ����ı��ĺ��������ܺ�CStaticEx���е�ͬ��������ͬ����ͬ��������Щ�������������ض��Ŀؼ�
	void DrawWindowText(CRect rect, LPCTSTR lpszString, COLORREF color, bool center, bool no_clip_area = false);	//��ָ���ľ��������ڻ�������ɫ���ı������no_clip_areaΪtrue�������������ʱ���ƻ�ͼ����
	void DrawWindowText(CRect rect, LPCTSTR lpszString, COLORREF color1, COLORREF color2, int split, bool center, bool no_clip_area = false);	//��ָ���ľ��������ڻ��Ʒָ���ɫ���ı���splitΪ��ɫ�ָ��λ�ã�ȡֵΪ0~1000�����ڸ�ʶ�̬��ʾ�������no_clip_areaΪtrue�������������ʱ���ƻ�ͼ����
	void DrawScrollText(CRect rect, LPCTSTR lpszString, COLORREF color, int pixel, bool center, ScrollInfo& scroll_info, bool reset = false);	//�ڿؼ��ϻ��ƹ������ı��������Ȳ���ʱ����pixelָ���˺�������һ���ƶ�������ֵ�����resetΪtrue�����������ʼλ��
	void DrawScrollText2(CRect rect, LPCTSTR lpszString, COLORREF color, int pixel, bool center, ScrollInfo& scroll_info, bool reset = false);	//�������ܺ�DrawScrollTextһ����ֻ���������ֻ������ҹ����������������

	static void SetDrawArea(CDC* pDC, CRect rect);

	void DrawBitmap(CBitmap& bitmap, CPoint start_point, CSize size, StretchMode stretch_mode);
	void DrawBitmap(UINT bitmap_id, CPoint start_point, CSize size, StretchMode stretch_mode);
	void DrawBitmap(HBITMAP hbitmap, CPoint start_point, CSize size, StretchMode stretch_mode);

	void FillRect(CRect rect, COLORREF color);
	void FillAlphaRect(CRect rect, COLORREF color, BYTE alpha);		//���һ����͸���ľ��Σ�����http://blog.csdn.net/lee353086/article/details/38311421��

	void DrawRectTopFrame(CRect rect, COLORREF color, int pilex = 1);
private:
	CDC* m_pDC{};		//���ڻ�ͼ��CDC���ָ��
	CWnd* m_pMainWnd{};	//��ͼ���ڵľ��
	//COLORREF m_backColor{ RGB(255,255,255) };
	CFont* m_pfont{};
};

