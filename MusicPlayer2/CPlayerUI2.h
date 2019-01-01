#pragma once
#include "DrawCommon.h"
#include "CPlayerUIBase.h"


class CPlayerUI2 : public CPlayerUIBase
{
private:
	struct DrawData
	{
		CRect cover_rect;
		CRect info_rect;
		CRect lyric_rect;
	};

public:
	CPlayerUI2(UIData& ui_data);
	~CPlayerUI2();

	virtual void DrawInfo(bool reset = false) override;		//������Ϣ

	virtual void RButtonUp(CPoint point) override;
	virtual void MouseMove(CPoint point) override;
	virtual void LButtonUp(CPoint point) override;

	virtual CRect GetThumbnailClipArea() override;

	virtual void AddMouseToolTip(BtnKey btn, LPCTSTR str) override;		//Ϊһ����ť��������ʾ
	virtual void UpdateMouseToolTip(BtnKey btn, LPCTSTR str) override;
	virtual void UpdateToolTipPosition() override;

protected:
	//void DrawControlBar(bool draw_background, CRect rect);

protected:
	DrawData m_draw_data;

	CFont m_title_font;
	CFont m_artist_font;
};

