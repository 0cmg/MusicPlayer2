#pragma once

//ʹ�ö�̬��ʽ����Dll
class CDllLib
{
public:
    CDllLib();
    ~CDllLib();
    void Init(const wstring& dll_path);		//����DLL�ļ�����ȡ�������
    void UnInit();
    bool IsSuccessed();		//�ж�DLL�еĺ����Ƿ��ȡ�ɹ�

protected:
    virtual bool GetFunction() = 0;     //��DLL�л�ȡ������ڵ�ַ�������������ɹ��򷵻�true

protected:
    HMODULE m_dll_module;
    bool m_success{ false };

};

