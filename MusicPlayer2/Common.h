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
	//�淶���ַ�������ɾ���ַ���ǰ��ͺ���Ŀո������ַ�(ģ������ֻ����string��wstring)
	template<class T>
	static bool StringNormalize(T& str);
	//ɾ���ַ���ĩβ�Ŀո�
	template<class T>
	static bool DeleteEndSpace(T& str);
	//�����ļ���С
	static size_t GetFileSize(const wstring& file_name);
	//��ini�ļ�д��һ��int����
	static void WritePrivateProfileIntW(const wchar_t* AppName, const wchar_t* KeyName, int value, const wchar_t* Path);
	//��string���͵��ַ���ת����Unicode�����wstring�ַ���
	static wstring StrToUnicode(const string& str, CodeType code_type = CodeType::AUTO);
	//��Unicode�����wstring�ַ���ת����string���͵��ַ�����������ַ��޷�ת����������char_cannot_convertָ���bool������Ϊtrue
	static string UnicodeToStr(const wstring & wstr, CodeType code_type, bool* char_cannot_convert = nullptr);
	//�ж�һ���ַ����Ƿ�UTF8����
	static bool IsUTF8Bytes(const char* data);
	static wstring GetExePath();
	//��ȡһ���б�ؼ���󳤶���Ŀ��ȵ�����ֵ
	static int GetListWidth(CListBox& list);
	//Ϊһ��Static�ؼ����ָ������ɫ
	static void FillStaticColor(CStatic& static_ctr, COLORREF color);

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
