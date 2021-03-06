
// VolumeRendererView.cpp: CVolumeRendererView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "VolumeRenderer.h"
#endif

#include "VolumeRendererDoc.h"
#include "VolumeRendererView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVolumeRendererView

IMPLEMENT_DYNCREATE(CVolumeRendererView, CView)

BEGIN_MESSAGE_MAP(CVolumeRendererView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CVolumeRendererView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CVolumeRendererView 생성/소멸

CVolumeRendererView::CVolumeRendererView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// 화면 사이즈와 그려야 할 데이터를 정의한다.
	m_Image = nullptr;
	m_ImgWidth = 768;
	m_ImgHeight = 768;
	m_Image = 
		shared_ptr<unsigned char>(new unsigned char[m_ImgWidth*m_ImgHeight*4]);
	memset(m_Image.get(), 0, sizeof(unsigned char)*m_ImgWidth*m_ImgHeight * 4);
}

CVolumeRendererView::~CVolumeRendererView()
{
}

// 인자로 받은 image를 화면 사이즈에 맞춰 그린다.
void CVolumeRendererView::SetDrawImage(unsigned char* image,
	const int width, const int height, const int byte)
{
	if (byte <= 0) return;

	float rate[2] = { 0.f };
	rate[0] = static_cast<float>(width) / static_cast<float>(m_ImgWidth);
	rate[1] = static_cast<float>(height) / static_cast<float>(m_ImgHeight);
	
	if (byte > 1)
	{
		for (int i = 0; i < m_ImgWidth; i++)
		{
			for (int j = 0; j < m_ImgHeight; j++)
			{
				int mod[2] = { i*rate[0], j*rate[1] };
				if (mod[0] >= width || mod[1] >= height) continue;

				m_Image.get()[(j*m_ImgWidth + i) * 4 + 0] =
					image[(mod[1]*width + mod[0]) * byte + 0];
				m_Image.get()[(j*m_ImgWidth + i) * 4 + 1] =
					image[(mod[1] *width + mod[0]) * byte + 1];
				m_Image.get()[(j*m_ImgWidth + i) * 4 + 2] =
					image[(mod[1] *width + mod[0]) * byte + 2];
			}
		}
	}
	else
	{
		for (int i = 0; i < m_ImgWidth; i++)
		{
			for (int j = 0; j < m_ImgHeight; j++)
			{
				int mod[2] = { i*rate[0], j*rate[1] };
				if (mod[0] >= width || mod[1] >= height) continue;

				m_Image.get()[(j*m_ImgWidth + i) * 4 + 0] =
											image[mod[1] * width + mod[0]];
				m_Image.get()[(j*m_ImgWidth + i) * 4 + 1] =
											image[mod[1] * width + mod[0]];
				m_Image.get()[(j*m_ImgWidth + i) * 4 + 2] =
											image[mod[1] * width + mod[0]];
			}
		}
	}
	
}

BOOL CVolumeRendererView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CVolumeRendererView 그리기

void CVolumeRendererView::OnDraw(CDC* pDC)
{
	CVolumeRendererDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	if (m_Image)
	{
		CDC MemDC;
		BITMAP bmpInfo;

		// 화면 DC와 호환되는 메모리 DC를 생성
		MemDC.CreateCompatibleDC(pDC);

		// 비트맵 리소스 로딩
		CBitmap cBitmap;
		cBitmap.CreateBitmap(m_ImgWidth, m_ImgHeight, 1, 32, m_Image.get());
		CBitmap* pOldBmp = NULL;

		// 로딩된 비트맵 정보 확인
		cBitmap.GetBitmap(&bmpInfo);

		//printf("view image width %d, height %d\n", bmpInfo.bmWidth, bmpInfo.bmHeight);

		// 메모리 DC에 선택
		pOldBmp = MemDC.SelectObject(&cBitmap);

		// 메모리 DC에 들어 있는 비트맵을 화면 DC로 복사하여 출력
		pDC->BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &MemDC, 0, 0, SRCCOPY);
	}
}


// CVolumeRendererView 인쇄


void CVolumeRendererView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CVolumeRendererView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CVolumeRendererView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CVolumeRendererView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CVolumeRendererView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CVolumeRendererView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CVolumeRendererView 진단

#ifdef _DEBUG
void CVolumeRendererView::AssertValid() const
{
	CView::AssertValid();
}

void CVolumeRendererView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVolumeRendererDoc* CVolumeRendererView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	CMainFrame *pMain = (CMainFrame*)AfxGetMainWnd(); // MainFrame에 대한 포인터 얻기
	pMain->SetWindowPos(NULL, 0, 0, 800, 900, SWP_SHOWWINDOW);

	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVolumeRendererDoc)));
	return (CVolumeRendererDoc*)m_pDocument;
}
#endif //_DEBUG


// CVolumeRendererView 메시지 처리기


void CVolumeRendererView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CVolumeRendererDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	printf("MIP AnyDirection \n");

	switch (nChar)
	{
	case VK_LEFT: // 왼쪽 화살표키 눌러짐.
		printf("Hello Left Key\n");
		//pDoc->OnMiprenderingAnydirection();
		pDoc->DirKeyDownRendering(LEFT);

		printf("Hello Left Key end\n");
		break;
	case VK_RIGHT: // 오른쪽 화살표 키 눌러짐.
		printf("Hello Right Key\n");
		//pDoc->OnMiprenderingAnydirection();
		pDoc->DirKeyDownRendering(RIGHT);
		printf("Hello Right Key end\n");
		break;
	}
}
