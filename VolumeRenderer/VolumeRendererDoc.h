
// VolumeRendererDoc.h: CVolumeRendererDoc 클래스의 인터페이스
//


#pragma once
//shared_ptr 사용하려면 아래와 같이 선언
#include <memory>
// Volume 클래스를 사용하기 위해 선언
#include "Volume.h"

using namespace std;

class CVolumeRendererDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CVolumeRendererDoc();
	DECLARE_DYNCREATE(CVolumeRendererDoc)

private:
	//shared_ptr을 이용하여 Volume 클래스 포인터를 선언한다. 
	shared_ptr<Volume> m_pVolume;
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
};
