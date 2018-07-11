
// VolumeRendererDoc.cpp: CVolumeRendererDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "VolumeRenderer.h"
#endif

#include "VolumeRendererDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CVolumeRendererDoc

IMPLEMENT_DYNCREATE(CVolumeRendererDoc, CDocument)

BEGIN_MESSAGE_MAP(CVolumeRendererDoc, CDocument)
END_MESSAGE_MAP()


// CVolumeRendererDoc 생성/소멸

CVolumeRendererDoc::CVolumeRendererDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CVolumeRendererDoc::~CVolumeRendererDoc()
{
}

BOOL CVolumeRendererDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.
	// 새 창이 뜨면 data를 불러와 버퍼에 저장한다.
	FILE* fp;
	fopen_s(&fp, "data//Bighead.den", "rb");

	// volume 사이즈를 미리 계산해 둔 것이다.
	int width = 256;
	int height = 256;
	int depth = 256;
	unsigned char* temp_vol = new unsigned char[width*height*depth];
	fread(temp_vol, 1, sizeof(unsigned char)*width*height*depth, fp);

	fclose(fp);

	printf("volume load complete\n");
	return TRUE;
}




// CVolumeRendererDoc serialization

void CVolumeRendererDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CVolumeRendererDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CVolumeRendererDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CVolumeRendererDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CVolumeRendererDoc 진단

#ifdef _DEBUG
void CVolumeRendererDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CVolumeRendererDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CVolumeRendererDoc 명령
