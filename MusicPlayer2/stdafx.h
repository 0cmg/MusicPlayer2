
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

//#define COMPILE_IN_WIN_XP

//�Զ�������ļ�
#include <conio.h >
#include <vector>
#include <io.h>
#include <string>
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <tuple>
#include <deque>
#include <algorithm>
//#ifndef COMPILE_IN_WIN_XP
//#include <VersionHelpers.h>
//#endif
#include <iomanip>
#include <map>
#include <cmath>
using std::ofstream;
using std::ifstream;
using std::string;
using std::wstring;
using std::vector;
using std::deque;
using std::map;

//����BASS��Ƶ���֧��
#include"bass.h"
#pragma comment(lib,"bass.lib")

//����֧��ʹ��������ص���
#include <afxinet.h>
#include <afxwin.h>
#include <afxcontrolbars.h>




#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


//�Զ���ĺ�

#ifdef UNICODE
#define _tstring  std::wstring
#else
#define _tstring  std::string
#endif // !UNICODE

//����DPI���Ŵ�С
//#define theApp.DPI(x) (theApp.m_dpi*(x)/96)

//CPlayer�����õ��ĳ���
#define NEXT (-999)		//���塰��һ��������
#define PREVIOUS (-998)		//���塰��һ��������

#define MAX_NUM_LENGTH 5000		//�����ȡ��Ƶ�ļ���Ϣ���������ֵ
#define MAX_SONG_NUM 9999		//�����б����ļ����������ֵ

#define VERSION L"2.61"	//����汾

//������ͼ��ť
#define IDT_PLAY_PAUSE 1321		//����������ͼ������/��ͣ����ť��ID
#define IDT_PREVIOUS 1322		//����������ͼ����һ������ť��ID
#define IDT_NEXT 1323			//����������ͼ����һ������ť��ID

//ȫ���ȼ�
#define HK_PLAY_PAUSE 1031		//ȫ���ȼ�������/��ͣ����ID
#define HK_PLAY_PAUSE2 1032		//ȫ���ȼ�������/��ͣ����ID
#define HK_PREVIOUS 1033		//ȫ���ȼ�����һ������ID
#define HK_NEXT 1034			//ȫ���ȼ�����һ������ID
#define HK_VOLUME_UP 1035		//ȫ���ȼ��������ӡ���ID
#define HK_VOLUME_DOWN 1036		//ȫ���ȼ�������������ID

//��ʱ��
#define TIMER_ID 1234			//���嶨ʱ����ID
#define TIMER_ELAPSE 80		//���嶨ʱ����Ӧ��ʱ���������룩
#define TIMER_ID_MINI 1235		//����ģʽ�еĶ�ʱ��ID
#define TIMER_ID_MINI2 1236		//����ģʽ�еĶ�ʱ��ID��������ʾƵ�׷���
#define TIMER_ELAPSE_MINI 200	//��������ģʽ�еĶ�ʱ����Ӧ��ʱ���������룩
//#define TIMER_ID_SPECTRAL 
//#define TIMER_ID_EXIT 1237

//Ƶ�׷���
#define FFT_SAMPLE 128			//Ƶ�׷�����������
//#define FFT_NUM 128			//Ҫʹ�õ�Ƶ�׵ĵ���
#define SPECTRUM_ROW 32			//Ƶ�׷������ε�����������Ϊ2�������η���С�ڻ����FFT_SAMPLE��

//��ȡ����������ǩʱ��Ĭ���ַ���
#define DEFAULT_TITLE L"<δ֪����>"
#define DEFAULT_ARTIST L"<δ֪������>"
#define DEFAULT_ALBUM L"<δ֪��Ƭ��>"
#define DEFAULT_YEAR L"<δ֪���>"
#define DEFAULT_GENRE L"<δ֪����>"

//һ����Ϊ�հ�ʱ��Ĭ����ʾ�ı�
#define DEFAULT_LYRIC_TEXT L"����"
#define DEFAULT_LYRIC_TEXT_CORTANA L"Music ��"

#define IDC_SAVE_COMBO_BOX 1990		//������ӵ������Ϊ���Ի����е���Ͽ��ID

//��ӵ�ϵͳ�˵��еġ�����ģʽ���˵����ID
#define IDM_MINIMODE 112

//������
#define EQU_CH_NUM 10		//������ͨ��������
const int EQU_STYLE_TABLE[9][EQU_CH_NUM]		//������Ԥ��
{
	{ 0,0,0,0,0,0,0,0,0,0 } ,			//��
	{ 4,3,3,2,2,1,0,-1,-2,-2 },		//�ŵ�
	{ 3,2,0,-1,-2,-2,-1,0,2,3 },		//����
	{ 2,1,0,-1,-1,1,3,5,3 },		//��ʿ
	{ -2,0,2,4,-1,-1,0,0,2,3 },		//ҡ��
	{ 1,0,0,1,2,1,-1,-2,-2,-2 },		//���
	{ 4,6,6,-2,-1,0,0,0,0,0 },		//�ص���
	{ -5,-5,-3,-2,-2,0,0,0,0,0 },		//��������
	{ 0,0,0,0,0,-1,-3,-5,-5,-4 }		//��������
};

#define GRAY(x) RGB(x,x,x)

#define ALBUM_COVER_NAME L"CurrentAlbumCover-MusicPlayer2-jMZB7TMf"
//#define DEFAULT_ALBUM_NAME L"cover"

//#define BACKGROUND_ALPHA 200	//���汳����͸����0~255
#define ALPHA_CHG(alpha) ((alpha)*255/100)		//��͸���ȴ�0~100ӳ�䵽0~255

#define LOCAL_DIR_STR L"%localdir%"

#define WM_OPEN_FILE_COMMAND_LINE (WM_USER+110)		//ͨ�������д��ļ�ʱ������ʵ�����ڷ��͵���Ϣ

#define WM_NEXT_USER_MSG (WM_USER+115)