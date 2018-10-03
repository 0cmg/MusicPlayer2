#pragma once

struct COLORIZATIONPARAMS
{
	COLORREF         clrColor;          //ColorizationColor
	COLORREF         clrAftGlow;	   //ColorizationAfterglow
	UINT             nIntensity;	   //ColorizationColorBalance -> 0-100
	UINT             clrAftGlowBal;    //ColorizationAfterglowBalance
	UINT		 clrBlurBal;       //ColorizationBlurBalance
	UINT		 clrGlassReflInt;  //ColorizationGlassReflectionIntensity
	BOOL             fOpaque;
};

class CDwmColorLibrary
{
	typedef HRESULT(WINAPI *_DwmIsCompositionEnabled)(BOOL *pfEnabled);
	typedef HRESULT(WINAPI *_DwmSetColorizationParameters) (COLORIZATIONPARAMS *colorparam, UINT unknown);
	typedef HRESULT(WINAPI *_DwmGetColorizationParameters) (COLORIZATIONPARAMS *colorparam);

public:
	CDwmColorLibrary();
	~CDwmColorLibrary();
	bool IsSuccessed() const;		//�ж�DLL�еĺ����Ƿ��ȡ�ɹ�
	COLORREF GetWindowsThemeColor() const;

protected:
	void Init(const std::wstring& dll_path);		//����DLL�ļ�����ȡ�������
	void UnInit();

	_DwmIsCompositionEnabled DwmIsCompositionEnabled;
	_DwmGetColorizationParameters DwmGetColorizationParameters;
	_DwmSetColorizationParameters DwmSetColorizationParameters;

protected:
	HMODULE m_dll_module;
	bool m_successed{ false };
};

