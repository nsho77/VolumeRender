
// VolumeRendererDoc.cpp: CVolumeRendererDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#include "VolumeRenderer.h"

#include "VolumeRendererDoc.h"
#include "VolumeRendererView.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LARGE_INTEGER Frequency;
LARGE_INTEGER BeginTime;
LARGE_INTEGER Endtime;

// CVolumeRendererDoc

IMPLEMENT_DYNCREATE(CVolumeRendererDoc, CDocument)

BEGIN_MESSAGE_MAP(CVolumeRendererDoc, CDocument)
	ON_COMMAND(ID_SLICERENDERING_ZDIRECTION, &CVolumeRendererDoc::OnSlicerenderingZdirection)
	ON_COMMAND(ID_SLICERENDERING_YDIRECTION, &CVolumeRendererDoc::OnSlicerenderingYdirection)
	ON_COMMAND(ID_SLICERENDERING_XDIRECTION, &CVolumeRendererDoc::OnSlicerenderingXdirection)
	ON_COMMAND(ID_MIPRENDERING_XDIRECTION, &CVolumeRendererDoc::OnMiprenderingXdirection)
	ON_COMMAND(ID_MIPRENDERING_YDIRECTION, &CVolumeRendererDoc::OnMiprenderingYdirection)
	ON_COMMAND(ID_MIPRENDERING_ZDIRECTION, &CVolumeRendererDoc::OnMiprenderingZdirection)
	ON_COMMAND(ID_MIPRENDERING_ANYDIRECTION, &CVolumeRendererDoc::OnMiprenderingAnydirection)
	ON_COMMAND(ID_VOLUMERENDERING_XDIRECTION, &CVolumeRendererDoc::OnVolumerenderingXdirection)
	ON_COMMAND(ID_VOLUMERENDERING_YDIRECTION, &CVolumeRendererDoc::OnVolumerenderingYdirection)
	ON_COMMAND(ID_VOLUMERENDERING_ZDIRECTION, &CVolumeRendererDoc::OnVolumerenderingZdirection)
	ON_COMMAND(ID_VOLUMERENDERING_ANYDIRECTION, &CVolumeRendererDoc::OnVolumerenderingAnydirection)
	ON_COMMAND(ID_SLICERENDERING_ANYDIRECTION, &CVolumeRendererDoc::OnSlicerenderingAnydirection)
END_MESSAGE_MAP()


// CVolumeRendererDoc 생성/소멸

CVolumeRendererDoc::CVolumeRendererDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_DirKey = -1;

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
	FILE* fp;
	fopen_s(&fp, "data//Bighead.den", "rb");

	int width = 256;
	int height = 256;
	int depth = 225;
	unsigned char* temp_vol = new unsigned char[width*height*depth];
	fread(temp_vol, 1, sizeof(unsigned char)*width*height*depth, fp);

	fclose(fp);

	m_pRenderer = shared_ptr<Renderer>(new Renderer(temp_vol, width, height, depth));
	delete[] temp_vol;

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

void CVolumeRendererDoc::DirKeyDownRendering(int DirKey)
{
	m_DirKey = DirKey;

	int CurMode = m_pRenderer->GetCurMode();
	switch (CurMode)
	{
	case SLICE:
		OnSlicerenderingAnydirection();
		break;
	case MIP:
		OnMiprenderingAnydirection();
		break;
	case VR:
		OnVolumerenderingAnydirection();
		break;
	default:
		break;
	}

}


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

void CVolumeRendererDoc::OnSlicerenderingZdirection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	const int img_width = 256;
	const int img_height = 256;
	shared_ptr<unsigned char> image =
		shared_ptr<unsigned char>(new unsigned char[img_width*img_height]);

	m_pRenderer->SetCurMode(SLICE);
	m_pRenderer->RenderSliceZDirection(image.get(), img_width, img_height, 120);

	CVolumeRendererView* pView =
		(CVolumeRendererView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	pView->SetDrawImage(image.get(), img_width, img_height, 1);

	pView->OnInitialUpdate();
}


void CVolumeRendererDoc::OnSlicerenderingYdirection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	const int img_width = 256;
	const int img_height = 256;
	shared_ptr<unsigned char> image =
		shared_ptr<unsigned char>(new unsigned char[img_width*img_height]);

	m_pRenderer->SetCurMode(SLICE);
	m_pRenderer->RenderSliceYDirection(image.get(), img_width, img_height, 120);

	CVolumeRendererView* pView =
		(CVolumeRendererView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	pView->SetDrawImage(image.get(), img_width, img_height, 1);

	pView->OnInitialUpdate();
}


void CVolumeRendererDoc::OnSlicerenderingXdirection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	const int img_width = 256;
	const int img_height = 256;
	shared_ptr<unsigned char> image =
		shared_ptr<unsigned char>(new unsigned char[img_width*img_height]);

	m_pRenderer->SetCurMode(SLICE);
	m_pRenderer->RenderSliceXDirection(image.get(), img_width, img_height, 120);

	CVolumeRendererView* pView =
		(CVolumeRendererView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	pView->SetDrawImage(image.get(), img_width, img_height, 1);

	pView->OnInitialUpdate();
}


void CVolumeRendererDoc::OnSlicerenderingAnydirection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	const int img_width = 256;
	const int img_height = 256;
	shared_ptr<unsigned char> image =
		shared_ptr<unsigned char>(new unsigned char[img_width*img_height]);

	m_pRenderer->SetCurMode(SLICE);
	m_pRenderer->RenderSliceAnyDirection(image.get(), img_width, img_height,
		120, m_DirKey);

	CVolumeRendererView* pView =
		(CVolumeRendererView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	pView->SetDrawImage(image.get(), img_width, img_height, 1);

	pView->OnInitialUpdate();
}


void CVolumeRendererDoc::OnMiprenderingXdirection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int img_width = 256;
	int img_height = 256;

	shared_ptr<unsigned char> image =
		shared_ptr<unsigned char>(new unsigned char[img_width*img_height]);
	memset(image.get(), 0, sizeof(unsigned char)*img_width*img_height);

	m_pRenderer->SetCurMode(MIP);
	m_pRenderer->RenderMIPXDirection(image.get(), img_width, img_height);

	CVolumeRendererView* pView =
		(CVolumeRendererView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	pView->SetDrawImage(image.get(), img_width, img_height, 1);

	pView->OnInitialUpdate();
}


void CVolumeRendererDoc::OnMiprenderingYdirection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int img_width = 256;
	int img_height = 256;

	shared_ptr<unsigned char> image =
		shared_ptr<unsigned char>(new unsigned char[img_width*img_height]);
	memset(image.get(), 0, sizeof(unsigned char)*img_width*img_height);

	m_pRenderer->SetCurMode(MIP);
	m_pRenderer->RenderMIPYDirection(image.get(), img_width, img_height);

	CVolumeRendererView* pView =
		(CVolumeRendererView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	pView->SetDrawImage(image.get(), img_width, img_height, 1);

	pView->OnInitialUpdate();
}


void CVolumeRendererDoc::OnMiprenderingZdirection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int img_width = 256;
	int img_height = 256;

	shared_ptr<unsigned char> image =
		shared_ptr<unsigned char>(new unsigned char[img_width*img_height]);
	memset(image.get(), 0, sizeof(unsigned char)*img_width*img_height);

	m_pRenderer->SetCurMode(MIP);
	m_pRenderer->RenderMIPZDirection(image.get(), img_width, img_height);

	CVolumeRendererView* pView =
		(CVolumeRendererView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	pView->SetDrawImage(image.get(), img_width, img_height, 1);

	pView->OnInitialUpdate();
	
}


void CVolumeRendererDoc::OnMiprenderingAnydirection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int img_width = 256;
	int img_height = 256;

	printf("MIP AnyDirection \n");

	shared_ptr<unsigned char> image =
		shared_ptr<unsigned char>(new unsigned char[img_width*img_height]);
	memset(image.get(), 0, sizeof(unsigned char)*img_width*img_height);

	QueryPerformanceFrequency(&Frequency);
	QueryPerformanceCounter(&BeginTime);

	m_pRenderer->SetCurMode(MIP);
	m_pRenderer->RenderMIPAnyDirection(image.get(), img_width, img_height,m_DirKey);
	

	QueryPerformanceCounter(&Endtime);
	int elapsed = Endtime.QuadPart - BeginTime.QuadPart;
	double duringtime = (double)elapsed / (double)Frequency.QuadPart;

	printf("MIP AnyDirection time : %f\n", duringtime);

	CVolumeRendererView* pView =
		(CVolumeRendererView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	pView->SetDrawImage(image.get(), img_width, img_height, 1);

	pView->OnInitialUpdate();

	printf("MIP AnyDirection End \n");
}

void CVolumeRendererDoc::OnVolumerenderingXdirection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	int img_width = 256;
	int img_height = 256;

	shared_ptr<unsigned char> image =
		shared_ptr<unsigned char>(new unsigned char[img_width*img_height*3]);
	memset(image.get(), 0, sizeof(unsigned char)*img_width*img_height*3);

	m_pRenderer->SetCurMode(VR);
	m_pRenderer->RenderVRXDirection(image.get(), img_width, img_height);

	CVolumeRendererView* pView =
		(CVolumeRendererView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	pView->SetDrawImage(image.get(), img_width, img_height, 3);

	pView->OnInitialUpdate();
}


void CVolumeRendererDoc::OnVolumerenderingYdirection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	int img_width = 256;
	int img_height = 256;

	shared_ptr<unsigned char> image =
		shared_ptr<unsigned char>(new unsigned char[img_width*img_height * 3]);
	memset(image.get(), 0, sizeof(unsigned char)*img_width*img_height * 3);

	m_pRenderer->SetCurMode(VR);
	m_pRenderer->RenderVRYDirection(image.get(), img_width, img_height);

	CVolumeRendererView* pView =
		(CVolumeRendererView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	pView->SetDrawImage(image.get(), img_width, img_height, 3);

	pView->OnInitialUpdate();
}


void CVolumeRendererDoc::OnVolumerenderingZdirection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	int img_width = 256;
	int img_height = 256;

	shared_ptr<unsigned char> image =
		shared_ptr<unsigned char>(new unsigned char[img_width*img_height * 3]);
	memset(image.get(), 0, sizeof(unsigned char)*img_width*img_height * 3);

	m_pRenderer->SetCurMode(VR);
	m_pRenderer->RenderVRZDirection(image.get(), img_width, img_height);

	CVolumeRendererView* pView =
		(CVolumeRendererView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	pView->SetDrawImage(image.get(), img_width, img_height, 3);

	pView->OnInitialUpdate();
}


void CVolumeRendererDoc::OnVolumerenderingAnydirection()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	int img_width = 256;
	int img_height = 256;

	shared_ptr<unsigned char> image =
		shared_ptr<unsigned char>(new unsigned char[img_width*img_height * 3]);
	memset(image.get(), 0, sizeof(unsigned char)*img_width*img_height * 3);

	m_pRenderer->SetCurMode(VR);
	m_pRenderer->RenderVRAnyDirection(image.get(), img_width, img_height, m_DirKey);

	CVolumeRendererView* pView =
		(CVolumeRendererView*)((CMainFrame*)(AfxGetApp()->m_pMainWnd))->GetActiveView();

	pView->SetDrawImage(image.get(), img_width, img_height, 3);

	pView->OnInitialUpdate();
}



