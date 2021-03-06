
// VolumeRendererView.h: CVolumeRendererView 클래스의 인터페이스
//

#pragma once
#include <memory>
#include "MainFrm.h"
#include "GlobalDefine.h"
using namespace std;

class CVolumeRendererView : public CView
{
protected: // serialization에서만 만들어집니다.
	CVolumeRendererView();
	DECLARE_DYNCREATE(CVolumeRendererView)

// 특성입니다.
public:
	CVolumeRendererDoc* GetDocument() const;
	void SetDrawImage(unsigned char* image,
		const int width, const int height, const int byte);

private:
	int m_ImgWidth;
	int m_ImgHeight;
	shared_ptr<unsigned char> m_Image;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CVolumeRendererView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // VolumeRendererView.cpp의 디버그 버전
inline CVolumeRendererDoc* CVolumeRendererView::GetDocument() const
   { return reinterpret_cast<CVolumeRendererDoc*>(m_pDocument); }
#endif

