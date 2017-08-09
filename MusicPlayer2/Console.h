#pragma once

//����̨��ɫ
enum CONSOLECOLOR
{
	BLACK = 0,
	DARK_BLUE,
	DARK_GREEN,
	DARK_CYAN,
	DARK_RED,
	DARK_PURPLE,
	DARK_YELLOW,
	DARK_WHITE,
	GRAY,
	BLUE,
	GREEN,
	CYAN,
	RED,
	PURPLE,
	YELLOW,
	WHITE
};

class CConsole
{
private:
	HANDLE m_handle;		//��׼������
	CONSOLE_SCREEN_BUFFER_INFO m_buff_info;	//����̨��������Ϣ
	CONSOLE_FONT_INFOEX m_font_info;		//����̨������Ϣ
	CONSOLE_CURSOR_INFO m_cursor_info;		//�����Ϣ

public:
	CConsole();
	~CConsole();

	void IniColsole();		//��ʼ������̨

	//����һ�����ֽ��ַ����ĳ���ռ�õİ���ַ���
	static size_t WcharStrHalfWidthLen(const wchar_t* str);
	//����һ�����ֽ��ַ����ĳ��ȣ������޶����max_len������ַ���
	static size_t WcharStrLen(const wchar_t* str, size_t max_len);
	//����һ�����ֽ��ַ�������end������ַ�����ʱһ���ж��ٸ�ȫ���ַ�
	static size_t FullWidthCount(const wchar_t* str, size_t end);

	//�ڽ����x,y���괦���һ������
	void PrintInt(int num, short x, short y, CONSOLECOLOR color) const;
	//�ڿ���̨��x,y���괦���һ�����ַ���
	void PrintWString(const wchar_t *str, int x, int y, CONSOLECOLOR color) const;
	//�ڿ���̨��x,y���괦���һ��ָ����󳤶ȵĿ��ַ���
	void PrintWString(const wchar_t* str, short x, short y, int length, CONSOLECOLOR color) const;
	//�ڿ���̨��x,y�����һ�����ַ�����ǰ����ַ���ʾΪcolor1����ɫ���ӵ�split���ַ���ʼ��ʾΪcolor2����ɫ
	void PrintWString(const wchar_t* str, short x, short y, int split, CONSOLECOLOR color1, CONSOLECOLOR color2) const;
	//�ڿ���̨��x,y�����һ�����ַ�����ǰ����ַ���ʾΪcolor1����ɫ���ӵ�split���ַ���ʼ��ʾΪcolor2����ɫ��ͬʱָ����󳤶�Ϊlength
	void PrintWString(const wchar_t* str, short x, short y, int length, int split, CONSOLECOLOR color1, CONSOLECOLOR color2) const;
	//�ڿ���̨��x,y���Թ�������ʽ��ʾһ�����ַ�����ָ����󳤶�Ϊlength����scrollingΪfalseʱ��ͣ��������resetΪtrueʱ��ͷ��ʼ������������ÿ����һ�ι���һ�Σ�
	void PrintWStringScroll(const wchar_t* str, short x, short y, int length, CONSOLECOLOR color, int& str_start, bool scrolling = true, bool reset = false) const;
	//�������̨��x,y����ʼ��length���ַ�
	void ClearString(short x, short y, int length) const;
	//����ƶ���x,y����
	void GotoXY(short x, short y) const;
	//��ʾ�����ع��
	void CursorVisible(bool visible);
	//��ȡ��ǰ����̨���ڵĿ��
	int GetWindowWidth();
	//��ȡ��ǰ����̨���ڵĸ߶�
	int GetWindowHight();
	//���ÿ���̨����
	void SetFont(const wchar_t* font_name, int font_size);
	//���ݲ�������һ��Ƶ�׷�����ʾ��һ���ַ���һ���ַ���ʾ�ķ�ΧΪ0~7
	static wstring GetSpectralChar(int spectral);
	//�ڿ���̨��x,y�����һ��Ƶ�׷���������
	void PrintSpectral(float spectral, short x, short y, CONSOLECOLOR color) const;
};

