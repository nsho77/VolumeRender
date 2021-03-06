
// VolumeRendererDoc.h: CVolumeRendererDoc 클래스의 인터페이스
//


#pragma once
#include "Renderer.h"
#include <memory>

using namespace std;

class CVolumeRendererDoc : public CDocument
{
private:
	shared_ptr<Renderer> m_pRenderer;
	int m_DirKey;

protected: // serialization에서만 만들어집니다.
	CVolumeRendererDoc();
	DECLARE_DYNCREATE(CVolumeRendererDoc)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CVolumeRendererDoc();
	void DirKeyDownRendering(int DirKey);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnSlicerenderingZdirection();
	afx_msg void OnSlicerenderingYdirection();
	afx_msg void OnSlicerenderingXdirection();
	afx_msg void OnSlicerenderingAnydirection();
	afx_msg void OnMiprenderingXdirection();
	afx_msg void OnMiprenderingYdirection();
	afx_msg void OnMiprenderingZdirection();
	afx_msg void OnMiprenderingAnydirection();
	afx_msg void OnVolumerenderingXdirection();
	afx_msg void OnVolumerenderingYdirection();
	afx_msg void OnVolumerenderingZdirection();
	afx_msg void OnVolumerenderingAnydirection();
};
