#include "stdafx.h"
#include "DllLib.h"


CDllLib::CDllLib()
{
}


CDllLib::~CDllLib()
{
}

void CDllLib::Init(const wstring & dll_path)
{
    //����DLL
    m_dll_module = ::LoadLibrary(dll_path.c_str());
    //��ȡ�������
    bool rtn = GetFunction();
    //�ж��Ƿ�ɹ�
    m_success = (m_dll_module != NULL && rtn);
}

void CDllLib::UnInit()
{
    if (m_dll_module != NULL)
    {
        FreeLibrary(m_dll_module);
        m_dll_module = NULL;
    }
}

bool CDllLib::IsSuccessed()
{
    return m_success;
}
