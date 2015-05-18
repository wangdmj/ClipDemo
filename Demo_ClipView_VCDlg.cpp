
// Demo_ClipView_VCDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "Demo_ClipView_VC.h"
#include "Demo_ClipView_VCDlg.h"
#include "afxdialogex.h"
#include <TlHelp32.h>
#include "../clipView/clipView.h"
#pragma comment(lib,"..\\Release\\clipView.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDemo_ClipView_VCDlg 对话框

#define CANVAS_WIDTH	800
#define CANVAS_HEIGHT	600
#define INFO_HEIGHT		50
#define TESTDATA_XML1  "TestData1.xml"
#define TESTDATA_XML2  "TestData2.xml"


CDemo_ClipView_VCDlg::CDemo_ClipView_VCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDemo_ClipView_VCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemo_ClipView_VCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CLIP, m_btn_clip);
	DDX_Control(pDX, IDC_STATIC_DRAWING, m_stc_drawing);
	DDX_Control(pDX, IDC_STATIC_INFO_1, m_stc_info1);
	DDX_Control(pDX, IDC_STATIC_INFO_2, m_stc_info2);
}

BEGIN_MESSAGE_MAP(CDemo_ClipView_VCDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CLIP, &CDemo_ClipView_VCDlg::OnBnClickedBtnClip)
	ON_WM_NCLBUTTONDOWN()
END_MESSAGE_MAP()


BOOL CDemo_ClipView_VCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	CRect clientRect;
	GetClientRect(&clientRect);
	CRect windowRect;
	GetWindowRect(&windowRect);
	int width = CANVAS_WIDTH + windowRect.Width() - clientRect.Width();
	int height = CANVAS_HEIGHT + INFO_HEIGHT + windowRect.Height() - clientRect.Height();
	SetWindowPos(NULL, 0, 0, width, height, SWP_NOMOVE|SWP_NOZORDER);

	m_btn_clip.SetWindowPos(NULL, 50, CANVAS_HEIGHT + 10, 80, 30, SWP_NOZORDER);
	m_stc_drawing.SetWindowPos(NULL, 140, CANVAS_HEIGHT + 15, 200, 20, SWP_NOZORDER);
	m_stc_info1.SetWindowPos(NULL, 350, CANVAS_HEIGHT + 5, 450, 20, SWP_NOZORDER);
	m_stc_info2.SetWindowPos(NULL, 350, CANVAS_HEIGHT + 25, 450, 20, SWP_NOZORDER);

	hasOutCanvasData = FALSE;
	GetModuleFileName(NULL,curPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	curPath.ReleaseBuffer();
	curPath = curPath.Left(curPath.ReverseFind('\\') + 1);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDemo_ClipView_VCDlg::OnPaint()
{

	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CDemo_ClipView_VCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CDemo_ClipView_VCDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UINT uMsg=LOWORD(wParam);
	switch(uMsg)
	{
	case ID_TESTCASE1:
		{
			CString xmlPath = curPath + TESTDATA_XML1;
			DrawTestCase(xmlPath, "1");
		}
		break;
	case ID_TESTCASE2:
		{
			CString xmlPath = curPath + TESTDATA_XML1;
			DrawTestCase(xmlPath, "2");
		}
		break;
	case ID_TESTCASE3:
		{
			CString xmlPath = curPath + TESTDATA_XML1;
			DrawTestCase(xmlPath, "3");
		}
		break;
	case ID_TESTCASE4:
		{
			CString xmlPath = curPath + TESTDATA_XML1;
			DrawTestCase(xmlPath, "4");
		}
		break;
	case ID_TESTCASE5:
		{
			CString xmlPath = curPath + TESTDATA_XML2;
			DrawTestCase(xmlPath, "5");
		}
		break;
	default:
		break;
	}
	return CDialogEx::OnCommand(wParam, lParam);
}

void CDemo_ClipView_VCDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	if (HTCAPTION == nHitTest) {
		return;
	}
	CDialogEx::OnNcLButtonDown(nHitTest, point);
}

BOOL CDemo_ClipView_VCDlg::XmlNodeToPoint(pugi::xml_node node, CPoint& piont)
{
	pugi::xml_node pointNode = node.first_child();
	CString strValue(pointNode.value());
	int curPos = 0;
	int count = 0;
	while(TRUE)
	{
		CString resToken = strValue.Tokenize(",", curPos);
		if (resToken.IsEmpty())
			break;
		int coord = _ttoi(resToken);
		count++;
		if (count == 1)
		{
			piont.x = coord;
		}
		else if (count == 2)
		{
			piont.y = CANVAS_HEIGHT - coord; //调整CAD与窗口显示的坐标系一致
		}
	}
	if (count != 2)
	{
		return FALSE;
	}
	return TRUE;
}
int  CDemo_ClipView_VCDlg::SplitCStringToArray(CString str,CStringArray& strArray, CString split)
{
	int curPos = 0;
	while(TRUE)
	{
		CString resToken = str.Tokenize(split, curPos);
		if (resToken.IsEmpty())
			break;
		strArray.Add(resToken);
	}
	return strArray.GetSize();
}
BOOL CDemo_ClipView_VCDlg::IsPointOutCanvas(CPoint point)
{
	if (point.x < 0 || point.y < 0 || point.x > CANVAS_WIDTH || point.y > CANVAS_HEIGHT)
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CDemo_ClipView_VCDlg::IsLineOutCanvas(Line line)
{
	if (IsPointOutCanvas(line.startpoint) || IsPointOutCanvas(line.endpoint))
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CDemo_ClipView_VCDlg::IsCircleOutCanvas(Circle circle)
{
	if (IsPointOutCanvas(circle.center))
	{
		return TRUE;
	}
	if ((circle.center.x - circle.radius) < 0 || (circle.center.x + circle.radius) > CANVAS_WIDTH
		|| (circle.center.y - circle.radius) < 0 || (circle.center.y + circle.radius) > CANVAS_HEIGHT)
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CDemo_ClipView_VCDlg::IsBoundaryOutCanvas(Boundary boundary)
{
	vector<CPoint>::iterator iter = boundary.vertexs.begin();
	for (;iter != boundary.vertexs.end(); iter++)
	{
		if (IsPointOutCanvas(*iter))
		{
			return TRUE;
		}
	}
	return FALSE;
}


BOOL CDemo_ClipView_VCDlg::LoadTestCaseData(CString xmlPath, CString caseID)
{
	pugi::xml_document doc;
	if (!doc.load_file(xmlPath)) return FALSE;
	pugi::xml_node root = doc.child("TestRoot");
	pugi::xml_node testCase = root.find_child_by_attribute("ID",caseID);
	if (!testCase) return FALSE;
	pugi::xml_attribute complex = testCase.attribute("complex");
	if (!complex) return FALSE;
	CString strComplex = complex.value();
	if (SplitCStringToArray(strComplex, complexArray, ",") != 6)
	{
		complexArray.RemoveAll();
		complexArray.InsertAt(0, " ");
		complexArray.InsertAt(0, " ");
		complexArray.InsertAt(0, " ");
		complexArray.InsertAt(0, " ");
		complexArray.InsertAt(0, " ");
		complexArray.InsertAt(0, " ");
	}
	for (pugi::xml_node entity = testCase.first_child(); entity; entity = entity.next_sibling())
	{
		pugi::xml_attribute type = entity.attribute("Type");
		CString typeValue(type.value());
		if (typeValue.CompareNoCase("Line") == 0)
		{
			Line line;
			pugi::xml_node startNode = entity.child("StartPoint");
			if(!XmlNodeToPoint(startNode, line.startpoint)) return FALSE;
			pugi::xml_node endNode = entity.child("EndPoint");
			if(!XmlNodeToPoint(endNode, line.endpoint)) return FALSE;
			if (IsLineOutCanvas(line))
			{
				hasOutCanvasData = TRUE;
				continue;
			}
			lines.push_back(line);
		} 
		else if(typeValue.CompareNoCase("Circle") == 0)
		{
			Circle circle;
			pugi::xml_node centerNode = entity.child("CenterPoint");
			if(!XmlNodeToPoint(centerNode, circle.center)) return FALSE;
			pugi::xml_node radiusNode = entity.child("Radius");
			pugi::xml_node pointNode = radiusNode.first_child();
			circle.radius = _ttoi(pointNode.value());
			if (IsCircleOutCanvas(circle))
			{
				hasOutCanvasData = TRUE;
				continue;
			}
			circles.push_back(circle);
		}
		else if (typeValue.CompareNoCase("Convex Polygon") == 0)
		{
			boundary.isConvex = true;
			for (pugi::xml_node vertex = entity.first_child(); vertex; vertex = vertex.next_sibling())
			{
				CPoint point;
				if(!XmlNodeToPoint(vertex, point)) return FALSE;
				boundary.vertexs.push_back(point);
			}
			if (IsBoundaryOutCanvas(boundary))
			{
				hasOutCanvasData = TRUE;
				boundary.vertexs.clear();
			}
		}
		else if (typeValue.CompareNoCase("Concave Polygon") == 0)
		{
			boundary.isConvex = false;
			for (pugi::xml_node vertex = entity.first_child(); vertex; vertex  = vertex.next_sibling())
			{
				CPoint point;
				if(!XmlNodeToPoint(vertex, point)) return FALSE;
				boundary.vertexs.push_back(point);
			}
			if (IsBoundaryOutCanvas(boundary))
			{
				hasOutCanvasData = TRUE;
				boundary.vertexs.clear();
			}
		}
	}
	return TRUE;
}
void CDemo_ClipView_VCDlg::ClearTestCaseData()
{
	hasOutCanvasData = FALSE;
	complexArray.RemoveAll();
	boundary.vertexs.clear();
	circles.clear();
	lines.clear();
	CClientDC dc(this);
	dc.FillSolidRect(0,0,CANVAS_WIDTH,CANVAS_HEIGHT, RGB(0,0,0));
}

double CDemo_ClipView_VCDlg::maxMemory = 0;
bool CDemo_ClipView_VCDlg::m_bStopTimer = false;
int CDemo_ClipView_VCDlg::timerId = 0;
void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime);
void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime)
{
	HANDLE handle=GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;    
	GetProcessMemoryInfo(handle,&pmc,sizeof(pmc));
	double curMemory = pmc.PeakPagefileUsage;
	if (curMemory > CDemo_ClipView_VCDlg::maxMemory)
	{
		CDemo_ClipView_VCDlg::maxMemory = curMemory;
	}
}
UINT ThreadProc(LPVOID lParam);
UINT ThreadProc(LPVOID lParam)
{
	SetTimer(NULL, ++CDemo_ClipView_VCDlg::timerId, /*频率*/10/*毫秒*/, TimerProc);
	int itemp;
	MSG msg;
	while((itemp=GetMessage(&msg, NULL, NULL, NULL)) && (-1 != itemp))
	{
		if (CDemo_ClipView_VCDlg::m_bStopTimer)
		{
			KillTimer(NULL, CDemo_ClipView_VCDlg::timerId);
			return 0;
		}
		if (msg.message == WM_TIMER)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return 0;
}

void CDemo_ClipView_VCDlg::BeginTimeAndMemoryMonitor()
{
	m_btn_clip.EnableWindow(FALSE);
	m_stc_drawing.SetWindowText("裁剪中...");
	m_stc_info1.ShowWindow(SW_HIDE);
	//AfxBeginThread(ThreadProc, NULL);
	startTime = clock();
}
void CDemo_ClipView_VCDlg::EndTimeAndMemoryMonitor()
{
	endTime = clock();
	m_bStopTimer = true;
	double useTimeS = (endTime - startTime)/1000;
	m_stc_drawing.SetWindowText("裁剪完毕！");
	m_stc_info1.ShowWindow(SW_SHOW);
	m_stc_info2.SetWindowTextA("");
	m_stc_info2.ShowWindow(SW_SHOW);
	CString strTime;
	strTime.Format("耗时:  %lfs", useTimeS);
	m_stc_info1.SetWindowText(strTime);
	
}
void CDemo_ClipView_VCDlg::DrawTestCase(CString xmlPath, CString caseID)
{
	m_stc_drawing.ShowWindow(SW_SHOW);
	m_stc_drawing.SetWindowText("图形绘制中...");
	m_stc_info1.ShowWindow(SW_HIDE);
	m_stc_info2.ShowWindow(SW_HIDE);
	ClearTestCaseData();
	if (!LoadTestCaseData(xmlPath, caseID))
	{
		MessageBox("读取数据失败!","Demo_CliView_VC",MB_OK);
		m_stc_drawing.SetWindowText("");
		return;
	}
	COLORREF clrLine = RGB(0,255,0);
	vector<Line>::iterator iterLine = lines.begin();
	for (;iterLine != lines.end(); iterLine++)
	{
		DrawLine(*iterLine, clrLine);
	}

	COLORREF clrCircle = RGB(0,0,255);
	vector<Circle>::iterator iterCircle = circles.begin();
	for (;iterCircle != circles.end(); iterCircle++)
	{
		DrawCircle(*iterCircle, clrCircle);
	}

	COLORREF clrBoundary = RGB(255,0,0);
	DrawBoundary(boundary, clrBoundary);

	if (hasOutCanvasData)
	{
		m_stc_drawing.SetWindowText("存在超出边界数据！！！");
	}
	else
	{
		m_btn_clip.EnableWindow(TRUE);
		m_stc_drawing.SetWindowText("图形绘制完成：");
		m_stc_info1.ShowWindow(SW_SHOW);
		CString strInfo1;
		strInfo1.Format("共有%s个图形和%s个边界，其中%s个图形与边界相交。",complexArray.GetAt(0),complexArray.GetAt(1),complexArray.GetAt(2));
		m_stc_info1.SetWindowText(strInfo1);
		m_stc_info2.ShowWindow(SW_SHOW);
		CString strInfo2;
		strInfo2.Format("共有%s个图形与边界无交点，其中%s个位于边界内，%s个位于边界外。",complexArray.GetAt(3),complexArray.GetAt(4),complexArray.GetAt(5));
		m_stc_info2.SetWindowText(strInfo2);
	}
	boundarySize=boundary.vertexs.size()-1;
}


void CDemo_ClipView_VCDlg::DrawLine(Line line, COLORREF clr)
{
	CClientDC dc(this);
	CPen penUse;
	penUse.CreatePen(PS_SOLID, 1, clr);
	CPen* penOld = dc.SelectObject(&penUse);

	dc.MoveTo(line.startpoint);
	dc.LineTo(line.endpoint);

	dc.SelectObject(penOld);
}
void CDemo_ClipView_VCDlg::DrawCircle(Circle circle, COLORREF clr)
{
	CClientDC dc(this);
	CPen penUse;
	penUse.CreatePen(PS_SOLID, 1, clr);
	CPen* penOld = dc.SelectObject(&penUse);

	dc.Arc(circle.center.x - circle.radius, circle.center.y - circle.radius, circle.center.x + circle.radius, circle.center.y + circle.radius, 0, 0, 0, 0);

	dc.SelectObject(penOld);
}
void CDemo_ClipView_VCDlg::DrawBoundary(Boundary boundary, COLORREF clr)
{
	CClientDC dc(this);
	CPen penUse;
	penUse.CreatePen(PS_SOLID, 1, clr);
	CPen* penOld = dc.SelectObject(&penUse);


	vector<CPoint>::iterator iter = boundary.vertexs.begin();
	if (iter == boundary.vertexs.end()) return;
	CPoint startPoint = *iter;
	dc.MoveTo(startPoint);
	for (;iter != boundary.vertexs.end(); iter++)
	{
		dc.LineTo(*iter);
	}
	dc.LineTo(startPoint);

	dc.SelectObject(penOld);
}

BOOL CDemo_ClipView_VCDlg::GetThreadIdList(CList<int>& tIdList)
{
	int pID = _getpid();
	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32;
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pID);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	te32.dwSize = sizeof(THREADENTRY32);
	BOOL bGetThread = Thread32First(hThreadSnap, &te32);
	while(bGetThread)
	{
		if (te32.th32OwnerProcessID == pID)
		{
			tIdList.AddTail(te32.th32ThreadID);
		}
		bGetThread = Thread32Next(hThreadSnap, &te32);
	}
	CloseHandle(hThreadSnap);
	return TRUE;
}

vector<CPoint> CDemo_ClipView_VCDlg::GetIntersectionsOfLine(UINT index)
{
	vector<CPoint> pts;
	Line l=lines[index];
	for (UINT i=0;i<boundarySize;i++)
	{
		double x0,y0;
		CPoint temp;
		GetIntersection(l.startpoint.x,l.startpoint.y,l.endpoint.x,l.endpoint.y,
			boundary.vertexs[i].x,boundary.vertexs[i].y,
			boundary.vertexs[(i+1)%boundarySize].x,boundary.vertexs[(i+1)%boundarySize].y,x0,y0);
		temp.x=x0;
		temp.y=y0;
		if(!(temp.x==0))
		pts.push_back(temp);
	}
	return pts;
}

vector<CPoint> CDemo_ClipView_VCDlg::GetIntersectionsOfCircle(UINT index)
{
	vector<CPoint> pts;
	CClientDC dc(this);
	double k2,b2,x1,y1,x2,y2;
	double c0=circles[index].center.x;
	double d0=circles[index].center.y;
	int r0=circles[index].radius;
	for (UINT i=0;i<boundarySize;i++)
	{
		CPoint a =boundary.vertexs[i+1];
		getLineFunction(boundary.vertexs[i].x,boundary.vertexs[i].y,a.x,a.y,k2,b2);
		double temp=isIntersections(k2,b2,c0,d0);
		if(r0>temp)
		{
			getCircleIntersections(c0,d0,r0,k2,b2,x1,y1,x2,y2,boundary.vertexs[i].x);
			if (((x1<=a.x&&x1>=boundary.vertexs[i].x)||(x1>=a.x&&x1<=boundary.vertexs[i].x))
				&&((y1<=a.y&&y1>=boundary.vertexs[i].y)||(y1>=a.y&&y1<=boundary.vertexs[i].y)))
			{
				dc.Ellipse(x1-5,y1-5,x1+5,y1+5);
				pts.push_back(CPoint(x1,y1));
			}
			if (((x2<=a.x&&x2>=boundary.vertexs[i].x)||(x2>=a.x&&x2<=boundary.vertexs[i].x))
				&&((y2<=a.y&&y2>=boundary.vertexs[i].y)||(y2>=a.y&&y2<=boundary.vertexs[i].y)))
			{
				dc.Ellipse(x2-5,y2-5,x2+5,y2+5);
				pts.push_back(CPoint(x2,y2));
			}
		}	
	}	
	return pts;
}

bool CDemo_ClipView_VCDlg::PointInPolygon(double x,double y)
{
	int    polySides=0; 
	vector<double> polyX;    
	vector<double> polyY;  
	UINT size=boundary.vertexs.size();
	for (int index=0;index<size;index++)
	{
		polySides++;
		polyX.push_back(boundary.vertexs[index].x);
		polyY.push_back(boundary.vertexs[index].y);
	}
	int   i,j=polySides-1 ;
	bool  oddNodes=false;
	for (i=0;i<polySides; i++)
	{
		if((polyY[i]< y && polyY[j]>=y || polyY[j]<y && polyY[i]>=y) && (polyX[i]<=x || polyX[j]<=x)) 
		{
			oddNodes^=(polyX[i]+(y-polyY[i])/(polyY[j]-polyY[i])*(polyX[j]-polyX[i])<x);
		}
			j=i;
	}
	return oddNodes;
}
void CDemo_ClipView_VCDlg::OnBnClickedBtnClip()
{
	BeginTimeAndMemoryMonitor();	
	vector<CPoint> pts;
	CClientDC dc(this);
	CPen greenPen,bluePen;
	greenPen.CreatePen(PS_SOLID,1,RGB(0,255,0));
	bluePen.CreatePen(PS_SOLID,1,RGB(0,0,255));
	
	
	dc.FillSolidRect(0,0,CANVAS_WIDTH,CANVAS_HEIGHT, RGB(0,0,0));
	UINT lineSize=lines.size();
	dc.SelectObject(&greenPen);
	for (UINT index=0;index<lineSize;index++)
	{
		pts=this->GetIntersectionsOfLine(index);
		int size=pts.size();
			for (int i=0;i<size;i+=2)
			{
				dc.MoveTo(pts[i]);
				dc.LineTo(pts[i+1]);
			}
		pts.clear();
	}
	dc.SelectObject(&bluePen);
	UINT circleSize=circles.size();
	DrawBoundary(boundary,RGB(255,0,0));
	for (UINT index=0;index<circleSize;index++)
	{
		pts=this->GetIntersectionsOfCircle(index);
		int size=pts.size();
		Circle c=circles[index];
		double centerX,centerY;
		
		if (size>1)
		{
			centerPoint(pts[0].x,pts[0].y,pts[1].x,pts[1].y,c.center.x,
			c.center.y,c.radius,centerX,centerY);
			bool isPointInPolygon=PointInPolygon(centerX,centerY);
			if (isPointInPolygon)
			{
				for (int index1=size-1;index1>=0;index1-=2)
				{
					if (pts[index1].x==pts[index1-1].x && pts[index1].y==pts[index1-1].y)
					{
						continue;
					}
					dc.Arc(c.center.x-c.radius,c.center.y-c.radius,
						c.center.x+c.radius,c.center.y+c.radius,
						pts[index1].x,pts[index1].y,pts[(index1-1+size)%size].x,pts[(index1-1+size)%size].y);
				}
			}
			else
			{
				for (int index1=size-2;index1>=0;index1-=2)
				{
					if (pts[index1].x==pts[index1-1].x && pts[index1].y==pts[index1-1].y)
					{
						continue;
					}
					dc.Arc(c.center.x-c.radius,c.center.y-c.radius,
					c.center.x+c.radius,c.center.y+c.radius,
					pts[index1].x,pts[index1].y,pts[(index1-1+size)%size].x,pts[(index1-1+size)%size].y);
				}
			}
		}	
		else if (size==1)
		{
			if(PointInPolygon(c.center.x,c.center.y))
			{
				dc.Arc(c.center.x-c.radius,c.center.y-c.radius,c.center.x+c.radius,c.center.y+c.radius,0,0,0,0);
			}
		}
		pts.clear();
	/*	CString str;
		str.Format("XML点坐标 x:%d,y:%d",c.center.x,CANVAS_HEIGHT-c.center.y);
		MessageBox(str);*/
	}
	DrawBoundary(boundary,RGB(255,0,0));
	bluePen.DeleteObject();
	greenPen.DeleteObject();
	EndTimeAndMemoryMonitor();

}
