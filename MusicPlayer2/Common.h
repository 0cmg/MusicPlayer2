//CCommon��Ϊȫ�ֺ����Ķ���
#pragma once

enum class Command
{
	OPEN,
	PLAY,
	CLOSE,
	PAUSE,
	STOP,
	PLAY_PAUSE,
	FF,	//���
	REW,		//�쵹
	VOLUME_UP,
	VOLUME_DOWN,
	SEEK
};

enum class CodeType
{
	ANSI,
	UTF8,
	UTF8_NO_BOM,
	UTF16,
	AUTO
};


class CCommon
{
public:
	CCommon();
	~CCommon();

	//�ж��ļ��Ƿ����
	static bool FileExist(const wstring& file);
	//�ж��ļ����Ƿ����
	static bool FolderExist(const wstring& file);
	//�淶���ַ�������ɾ���ַ���ǰ��ͺ���Ŀո������ַ�(ģ������ֻ����string��wstring)
	template<class T>
	static bool StringNormalize(T& str);
	//ɾ���ַ���ĩβ�Ŀո�
	template<class T>
	static bool DeleteEndSpace(T& str);
	//�Զ�����ַ�����������
	static void StringCopy(char* dest, int size, string source);
	//ת���ַ�����Сд�����upperΪtrue����ת���ɴ�д������ת����Сд
	template<class T>
	static bool StringTransform(T& str, bool upper);
	//�ַ����Ƚϣ����Դ�Сд
	static bool StringCompareNoCase(const wstring& str1, const wstring& str2);
	//�ַ������ң����Դ�Сд
	static size_t StringFindNoCase(const wstring& str, const wstring& find_str);

	static wstring TranslateToSimplifiedChinese(const wstring& str);
	static wstring TranslateToTranditionalChinese(const wstring& str);
	//�滻һ���ļ����е���Ч�ַ�
	static void FileNameNormalize(wstring& file_name);

	//�����ļ���С
	static size_t GetFileSize(const wstring& file_name);
	//��ȡ�ļ���չ��
	static wstring GetFileExtension(const wstring& file_name);
	//��ini�ļ�д��һ��int����
	static void WritePrivateProfileIntW(const wchar_t* AppName, const wchar_t* KeyName, int value, const wchar_t* Path);
	//��string���͵��ַ���ת����Unicode�����wstring�ַ���
	static wstring StrToUnicode(const string& str, CodeType code_type = CodeType::AUTO);
	//��Unicode�����wstring�ַ���ת����string���͵��ַ�����������ַ��޷�ת����������char_cannot_convertָ���bool������Ϊtrue
	static string UnicodeToStr(const wstring & wstr, CodeType code_type, bool* char_cannot_convert = nullptr);
	//�ж�һ���ַ����Ƿ�UTF8����
	static bool IsUTF8Bytes(const char* data);
	//��ȡ��ǰ����exe�ļ���·��
	static wstring GetExePath();
	//��ȡ�����·��
	static wstring GetDesktopPath();
	//��ȡ��ʱ�ļ���·��
	static wstring GetTemplatePath();
	//��ȡһ���б�ؼ���󳤶���Ŀ��ȵ�����ֵ
	static int GetListWidth(CListBox& list);
	////Ϊһ��Static�ؼ����ָ������ɫ����ɾ������Ϊʹ��CStaticEx���SetFillColor������
	//static void FillStaticColor(CStatic& static_ctr, COLORREF color);

	//�жϵ�ǰ����ϵͳ�Ƿ���Windows10�����ϰ汾
	//VersionHelper�еĺ����޷��ж�Windows8.1��Windows10��
	//�������������RtlGetNtVersionNumbers��ȡϵͳ�汾��Ϣ
	static bool IsWindows10OrLater();

	//ɾ��һ���ļ�
	static int DeleteAFile(HWND hwnd, _tstring file);

	//��һ���ַ������浽������
	static bool CopyStringToClipboard(const wstring& str);

	//
	static void WriteLog(const wchar_t* path, const wstring& content);

	//��ͨ�������в������ݹ����Ķ���ļ�·����֣������浽file���������������ݹ����ĵ�һ���ļ������ļ������ļ��У��򷵻��ļ���·�������򣬷��ؿ��ַ���
	static wstring DisposeCmdLine(const wstring& cmd_line, vector<wstring>& files);

	/*
	�������ܣ���ָ���ļ���ָ����Ŀ¼�´������ݷ�ʽ
	����������
	lpszLnkFileDir  ָ��Ŀ¼������ΪNULL��
	lpszFileName    ָ���ļ���ΪNULL��ʾ��ǰ���̵�EXE�ļ���
	lpszLnkFileName ��ݷ�ʽ���ƣ�ΪNULL��ʾEXE�ļ�����
	lpszWorkDir		��ݷ�ʽ����Ŀ¼��ΪNULL��ʾ��ݷ�ʽĿ������λ��
	wHotkey         Ϊ0��ʾ�����ÿ�ݼ�
	pszDescription  ��ע
	iShowCmd        ���з�ʽ��Ĭ��Ϊ���洰��
	*/
	static BOOL CreateFileShortcut(LPCTSTR lpszLnkFileDir, LPCTSTR lpszFileName = NULL, LPCTSTR lpszLnkFileName = NULL, LPCTSTR lpszWorkDir = NULL, WORD wHotkey = 0, LPCTSTR lpszDescription = NULL, int iShowCmd = SW_SHOWNORMAL);

	//��ȡһ��������ַ���
	static wstring GetRandomString(int length);
};

template<class T>
inline bool CCommon::StringNormalize(T & str)
{
	if (typeid(str) == typeid(string) || typeid(str) == typeid(wstring))
	{
		if (str.empty()) return false;

		int size = str.size();	//�ַ����ĳ���
		if (size < 0) return false;
		int index1 = 0 ;		//�ַ����е�1�����ǿո������ַ���λ��
		int index2 = size - 1;	//�ַ��������һ�����ǿո������ַ���λ��
		while (index1 < size && str[index1] >= 0 && str[index1] <=32)
			index1++;
		while (index2 >= 0 && str[index2] >= 0 && str[index2] <= 32)
			index2--;
		if (index1 > index2)	//���index1 > index2��˵���ַ���ȫ�ǿո������ַ�
			str.clear();
		else if (index1 == 0 && index2 == size - 1)	//���index1��index2��ֵ�ֱ�Ϊ0��size - 1��˵���ַ���ǰ��û�пո������ַ���ֱ�ӷ���
			return true;
		else
			str = str.substr(index1, index2 - index1 + 1);
		return true;
	}
	else
	{
		return false;
	}
}

template<class T>
inline bool CCommon::DeleteEndSpace(T & str)
{
	if (typeid(str) == typeid(string) || typeid(str) == typeid(wstring))
	{
		if (str.empty()) return false;
		while (!str.empty() && str.back() == L' ')
			str.pop_back();
		return true;
	}
	else
	{
		return false;
	}
}

template<class T>
inline bool CCommon::StringTransform(T & str, bool upper)
{
	if (typeid(str) == typeid(string) || typeid(str) == typeid(wstring))
	{
		if (str.empty()) return false;
		if (upper)
			std::transform(str.begin(), str.end(), str.begin(), toupper);
		else
			std::transform(str.begin(), str.end(), str.begin(), tolower);
		return true;
	}
	else
	{
		return false;
	}
}
