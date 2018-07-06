#include "stdafx.h"
#include "CoverDownloadCommon.h"


CCoverDownloadCommon::CCoverDownloadCommon()
{
}


CCoverDownloadCommon::~CCoverDownloadCommon()
{
}

wstring CCoverDownloadCommon::GetAlbumCoverURL(const wstring & song_id)
{
	if(song_id.empty())
		return wstring();
	//��ȡר������ӿڵ�URL
	wchar_t buff[256];
	swprintf_s(buff, L"http://music.163.com/api/song/detail/?id=%s&ids=%%5B%s%%5D&csrf_token=", song_id.c_str(), song_id.c_str());
	wstring contents;
	//��URL���ݱ��浽�ڴ�
	if(!CInternetCommon::GetURL(wstring(buff), contents))
		return wstring();
	size_t index;
	index = contents.find(L"\"album\"");
	if (index == wstring::npos)
		return wstring();
	index = contents.find(L"\"picUrl\"", index + 7);
	if (index == wstring::npos)
		return wstring();
	wstring url;
	size_t index1;
	index1 = contents.find(L'\"', index + 10);
	url = contents.substr(index + 10, index1 - index - 10);

	return url;
}
