#pragma once
#include "DrawCommon.h"
#include "CPlayerUIBase.h"


class CPlayerUI : public CPlayerUIBase
{
private:
	struct DrawData
	{
		CRect draw_rect;					//��ͼ����

		CRect cover_rect;					//��ʾר������ľ��������Ի�ͼ�������Ͻ�Ϊԭ�㣩
		CRect repetemode_rect;				//��ʾ��ѭ��ģʽ���ľ�������
		bool repetemode_hover{ false };		//���ָ���ˡ�ѭ��ģʽ���ľ�������
		UIButton volume_btn;				//������ť
		CRect volume_up_rect, volume_down_rect;	//�������������Ӻͼ��������ľ�������
		bool show_volume_adj{ false };		//��ʾ����������ť
		UIButton translate_btn;				//��ʷ��밴ť
		UIButton skin_btn;
	};

public:
	CPlayerUI(UIData& ui_data);
	~CPlayerUI();

	virtual void Init(CDC* pDC) override;
	virtual void DrawInfo(bool narrow_mode, bool reset = false) override;		//������Ϣ

	virtual void RButtonUp(CPoint point, bool narrow_mode) override;
	virtual void MouseMove(CPoint point) override;
	virtual void LButtonUp(CPoint point) override;
	virtual void OnSizeRedraw(int cx, int cy, bool narrow_mode) override;


	virtual CRect GetThumbnailClipArea(bool narrow_mode) override;

protected:
	void DrawLyricsSingleLine(CRect lyric_rect);			//���Ƹ�ʣ�խ����ģʽ�µ�����ʾ��������Ϊ��ʾ��ʵľ�������
	void DrawLyricsMulityLine(CRect lyric_rect, CDC* pDC);			//���Ƹ�ʣ���ͨģʽ�¶�����ʾ��������Ϊ��ʾ��ʵľ�������

protected:
	DrawData m_draw_data;
	UIData& m_ui_data;
	std::shared_ptr<SLayoutData> m_pLayout{ nullptr };
};

