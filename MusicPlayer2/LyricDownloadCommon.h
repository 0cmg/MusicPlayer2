//�������ڶ���ͨ���������ظ����ص�ȫ�ֺ���
//��ʵ����ص��������������ֵ�API
#pragma once
#include "Common.h"

#define SUCCESS 0	// �����ɹ�
#define FAILURE 1	// ����ʧ��
#define OUTTIME 2	// ������ʱ

class CLyricDownloadCommon
{
#define  NORMAL_CONNECT INTERNET_FLAG_KEEP_CONNECTION
#define  SECURE_CONNECT NORMAL_CONNECT | INTERNET_FLAG_SECURE
#define  NORMAL_REQUEST INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE 
#define  SECURE_REQUEST NORMAL_REQUEST | INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID

public:
	//һ�������������Ϣ
	struct ItemInfo
	{
		wstring id;		//������ID
		wstring title;		//�����ı���
		wstring artist;		//������������
		wstring album;		//�����ĳ�Ƭ��
	};

	CLyricDownloadCommon();
	~CLyricDownloadCommon();


	//��һ���ַ���ת����URL���루��UTF8�����ʽ��
	static wstring URLEncode(const wstring& wstr);

	//���������������и�����id���ظ�ʣ����������result�У�download_translate����ָ���Ƿ����ش�����ĸ��
	static bool DownloadLyric(const wstring& song_id, wstring& result, bool download_translate = true);

	//��ָ����url����http post���󣬽��������result��
	static int HttpPost(const wstring& str_url, wstring& result);

	static bool DisposeLryic(wstring& lyric_str);	//�Դ��������������صĸ�ʽ��д���ת������ȷ�ĸ���ı�
	static void DeleteStrSlash(wstring& str);		//����ַ����еġ�\"����ɾ���ַ����еķ�б��
	static void DisposeSearchResult(vector<ItemInfo>& down_list, const wstring& search_result, int result_count = 30);		//���������search_result����ȡ����������Ϣ����������down_list������

	/// <summary>
	/// �ַ������ƶ��㷨-�༭���뷨
	/// </summary>
	/// <returns>���ص�ֵΪ0~1��Խ�����ƶ�Խ��</returns>
	static double StringSimilarDegree_LD(const wstring& srcString, const wstring& matchString);

	//���ݲ����ṩ�ĸ������⡢�����ҡ���Ƭ�����ļ�������down_list�����в�����ƥ���һ�������������ֵ
	static int SelectMatchedItem(const vector<ItemInfo>& down_list, const wstring& title, const wstring& artist, const wstring& album, const wstring& filename, bool write_log = false);
};

