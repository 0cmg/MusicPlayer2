#pragma once
#include "DrawCommon.h"
#include "CPlayerUIBase.h"


class CPlayerUI2 : public CPlayerUIBase
{
public:
	CPlayerUI2(UIData& ui_data);
	~CPlayerUI2();

	virtual void DrawInfo(bool reset = false) override;		//������Ϣ

	virtual void AddMouseToolTip(BtnKey btn, LPCTSTR str) override;		//Ϊһ����ť��������ʾ
	virtual void UpdateMouseToolTip(BtnKey btn, LPCTSTR str) override;
	virtual void UpdateToolTipPosition() override;

protected:
	//CFont m_title_font;
	//CFont m_artist_font;
};

