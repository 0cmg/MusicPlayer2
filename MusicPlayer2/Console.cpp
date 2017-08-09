#include "stdafx.h"
#include "Console.h"
#include "MusicPlayer2.h"

CConsole::CConsole()
{
}


CConsole::~CConsole()
{
}

void CConsole::IniColsole()
{
	m_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	//char buff[32];
	//sprintf_s(buff, sizeof(buff), "mode con:cols=%d lines=%d", GetWindowWidth(), GetWindowHight());
	//system(buff);		//���ô��ڵĿ�Ⱥ͸߶�

	//ϵͳ��windows10����ʱ���ڳ�ʼ��ʱ����һ���̶��Ļ�������С��
	//��windows10���Ը��ݴ��ڴ�С�Զ�������������С����
	if (!theApp.m_is_windows10)
	{
		COORD size{ 300, 80 };
		SetConsoleScreenBufferSize(m_handle, size);		//���ô��ڻ�������С
	}

	setlocale(LC_ALL, "");		//�����������趨Ϊ�Ӳ���ϵͳ��õ��û�Ĭ�ϵ�ANSI����ҳ

	CursorVisible(false);		//���ع��
}

size_t CConsole::WcharStrHalfWidthLen(const wchar_t * str)
{
	size_t size{ 0 };
	const size_t length{ wcslen(str) };
	for (int i{ 0 }; i < length; i++)
	{
		if (str[i] >= 0 && str[i] < 128)
			size++;		//���һ��Unicode�ַ�������0~127��Χ�ڣ���ռһ������ַ����
		else
			size += 2;		//������ռ��������ַ����
	}
	return size;
}

size_t CConsole::WcharStrLen(const wchar_t * str, size_t max_len)
{
	size_t count{ 0 };
	size_t half_width_count{ 0 };
	const size_t length{ wcslen(str) };
	for (; count < length; count++)
	{
		if (str[count] >= 0 && str[count] < 128)
			half_width_count++;
		else
			half_width_count += 2;
		if (half_width_count > max_len) break;
	}
	return count;
}

size_t CConsole::FullWidthCount(const wchar_t * str, size_t end)
{
	int half_width_cnt{ 0 }, full_width_count{ 0 };
	size_t length{ wcslen(str) };
	for (int i{ 0 }; i < length; i++)
	{
		if (str[i] < 0 || str[i] >= 128)
		{
			half_width_cnt += 2;
			full_width_count++;
		}
		else
		{
			half_width_cnt++;
		}
		if (half_width_cnt >= end) break;
	}
	return full_width_count;
}

void CConsole::PrintInt(int num, short x, short y, CONSOLECOLOR color) const
{
	COORD pos;
	ULONG unuse;
	pos.X = x;
	pos.Y = y;
	char str[20];
	_itoa_s(num, str, 10);
	size_t len{ strlen(str) };
	WriteConsoleOutputCharacterA(m_handle, str, len, pos, &unuse);		//����ַ�
	FillConsoleOutputAttribute(m_handle, color, len, pos, &unuse);		//������ɫ
}

void CConsole::PrintWString(const wchar_t * str, int x, int y, CONSOLECOLOR color) const
{
	COORD pos;
	ULONG unuse;
	pos.X = x;
	pos.Y = y;
	size_t len{ wcslen(str) };	//�ַ�������
	size_t len_halfwidth{ WcharStrHalfWidthLen(str) };	//�ַ���ռ�ð���ַ���
	WriteConsoleOutputCharacterW(m_handle, str, len, pos, &unuse);			//����ַ�
	FillConsoleOutputAttribute(m_handle, color, len_halfwidth, pos, &unuse);	//������ɫ
}

void CConsole::PrintWString(const wchar_t * str, short x, short y, int length, CONSOLECOLOR color) const
{
	if (length <= 0) return;
	COORD pos;
	ULONG unuse;
	pos.X = x;
	pos.Y = y;
	size_t len{ WcharStrLen(str, length) };		//�ַ�������
	size_t len_halfwidth{ WcharStrHalfWidthLen(str) };	//�ַ���ռ�ð���ַ���
	if (len_halfwidth > length) len_halfwidth = length;
	WriteConsoleOutputCharacterW(m_handle, str, len, pos, &unuse);			//����ַ�
	FillConsoleOutputAttribute(m_handle, color, len_halfwidth, pos, &unuse);	//������ɫ
}

void CConsole::PrintWString(const wchar_t * str, short x, short y, int split, CONSOLECOLOR color1, CONSOLECOLOR color2) const
{
	COORD pos;
	ULONG unuse;
	pos.X = x;
	pos.Y = y;
	size_t len{ wcslen(str) };	//�ַ�������
	size_t len_halfwidth{ WcharStrHalfWidthLen(str) };	//�ַ���ռ�ð���ַ���
	if (split > len_halfwidth) split = len_halfwidth;
	WriteConsoleOutputCharacterW(m_handle, str, len, pos, &unuse);		//����ַ�
	FillConsoleOutputAttribute(m_handle, color1, split, pos, &unuse);		//������ɫ1
	pos.X += split;
	FillConsoleOutputAttribute(m_handle, color2, len_halfwidth - split, pos, &unuse);	//������ɫ2
}

void CConsole::PrintWString(const wchar_t * str, short x, short y, int length, int split, CONSOLECOLOR color1, CONSOLECOLOR color2) const
{
	if (length <= 0) return;
	COORD pos;
	ULONG unuse;
	pos.X = x;
	pos.Y = y;
	size_t len{ WcharStrLen(str, length) };	//�ַ�������
	size_t len_halfwidth{ WcharStrHalfWidthLen(str) };	//�ַ���ռ�ð���ַ���
	if (len_halfwidth > length) len_halfwidth = length;
	if (split > len_halfwidth) split = len_halfwidth;
	WriteConsoleOutputCharacterW(m_handle, str, len, pos, &unuse);		//����ַ�
	FillConsoleOutputAttribute(m_handle, color1, split, pos, &unuse);		//������ɫ1
	pos.X += split;
	FillConsoleOutputAttribute(m_handle, color2, len_halfwidth - split, pos, &unuse);	//������ɫ2
}

void CConsole::PrintWStringScroll(const wchar_t * str, short x, short y, int length, CONSOLECOLOR color, int& str_start, bool scrolling, bool reset) const
{
	if (length <= 0) return;
	ClearString(x, y, length);
	wstring temp;
	if (reset)		//���resetΪtrue����Ҫ��str_start����
	{
		str_start = 0;
	}
	if (CConsole::WcharStrHalfWidthLen(str) <= length)		//���ַ����ĳ��Ȳ�����length���ַ�ʱ��ֱ����ʾ����
	{
		PrintWString(str, x, y, length, color);		//����ַ��������ֻ���length���ַ���
	}
	else		//�ַ������ȳ���length���ַ�ʱ������ʾ���˺���ÿ����һ�ι���һ��
	{
		if (str_start > static_cast<int>(CConsole::WcharStrHalfWidthLen(str) - length - CConsole::FullWidthCount(str, str_start)) || str_start >= wcslen(str))
			str_start = 0;
		temp = wstring(str).substr(str_start, length);		//ȡ�õ�str_start����ʼ��length���ַ�
		if (scrolling)		//ֻ��scrollingΪtrueʱ�Ź���
			str_start++;
		PrintWString(temp.c_str(), x, y, length, color);
	}
}

void CConsole::ClearString(short x, short y, int length) const
{
	if (length <= 0) return;
	COORD pos;
	ULONG unuse;
	pos.X = x;
	pos.Y = y;
	//FillConsoleOutputAttribute(m_handle, BLACK, length, pos, &unuse);		//��Ҫ������ַ�����������Ϊ��ɫ
	string mask(length, ' ');		//����length���ȵĿո�
	WriteConsoleOutputCharacterA(m_handle, mask.c_str(), length, pos, &unuse);	//��Ҫ����������ÿո����
}

void CConsole::GotoXY(short x, short y) const
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(m_handle, pos);
}

void CConsole::CursorVisible(bool visible)
{
	m_cursor_info.bVisible = visible;
	m_cursor_info.dwSize = 25;		//����ȣ�ȡֵΪ1~100
	SetConsoleCursorInfo(m_handle, &m_cursor_info);
}

int CConsole::GetWindowWidth()
{
	GetConsoleScreenBufferInfo(m_handle, &m_buff_info);
	return m_buff_info.srWindow.Right + 1;
}

int CConsole::GetWindowHight()
{
	GetConsoleScreenBufferInfo(m_handle, &m_buff_info);
	return m_buff_info.srWindow.Bottom + 1;
}

void CConsole::SetFont(const wchar_t * font_name, int font_size)
{
#ifndef COMPILE_IN_WIN_XP
	m_font_info.cbSize = sizeof(m_font_info);
	m_font_info.dwFontSize.Y = font_size; 		//����XΪ0  
	m_font_info.FontWeight = FW_NORMAL;
	wcscpy_s(m_font_info.FaceName, font_name);
	SetCurrentConsoleFontEx(m_handle, NULL, &m_font_info);
#endif
}

wstring CConsole::GetSpectralChar(int spectral)
{
	const wchar_t spectral_charactors[]{ L"�x�y�z�{�|�}�~��" };
	if (spectral < 0) return wstring(L"  ");
	else if(spectral>=8) return wstring(1, spectral_charactors[7]);
	else return wstring(1, spectral_charactors[spectral]);
}

void CConsole::PrintSpectral(float spectral, short x, short y, CONSOLECOLOR color) const
{
	ULONG unuse;
	int spectral_int{ static_cast<int>(spectral) };
	int spectral1, spectral2, spectral3;		//��һ��Ƶ�����ηֳ�3�����ֺ�ÿ�����ֵ�Ƶ��ֵ����ΧΪ0~7���ܹ�����ʾ�ķ�ΧΪ0~23
	spectral3 = spectral_int;
	spectral2 = spectral_int - 8;
	spectral1 = spectral_int - 16;

	WriteConsoleOutputCharacterW(m_handle, GetSpectralChar(spectral1).c_str(), GetSpectralChar(spectral1).size(), COORD{ x,y }, &unuse);
	WriteConsoleOutputCharacterW(m_handle, GetSpectralChar(spectral2).c_str(), GetSpectralChar(spectral2).size(), COORD{ x,y + 1 }, &unuse);
	WriteConsoleOutputCharacterW(m_handle, GetSpectralChar(spectral3).c_str(), GetSpectralChar(spectral3).size(), COORD{ x,y + 2 }, &unuse);

	FillConsoleOutputAttribute(m_handle, color, 2, COORD{ x,y }, &unuse);	//������ɫ
	FillConsoleOutputAttribute(m_handle, color, 2, COORD{ x,y + 1 }, &unuse);	//������ɫ
	FillConsoleOutputAttribute(m_handle, color, 2, COORD{ x,y + 2 }, &unuse);	//������ɫ
}
