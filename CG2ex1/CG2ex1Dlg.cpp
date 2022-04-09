// CG2ex1Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "CG2ex1.h"
#include "CG2ex1Dlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCG2ex1Dlg dialog




CCG2ex1Dlg::CCG2ex1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCG2ex1Dlg::IDD, pParent)
	, dlgSegment(0)
	, mBshowYX(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCG2ex1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ESEGMENTS, dlgSegment);

	DDX_Check(pDX, IDC_SHOWYX, mBshowYX);
}

BEGIN_MESSAGE_MAP(CCG2ex1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPEN_BTN, &CCG2ex1Dlg::OnBnClickedOpenBtn)
	ON_BN_CLICKED(IDC_EXIT, &CCG2ex1Dlg::OnBnClickedExit)
	ON_BN_CLICKED(IDC_CHKSHWCTRLPOINTS, &CCG2ex1Dlg::OnBnClickedChkshwctrlpoints)
	ON_BN_CLICKED(IDC_CHKSHWKNOTVECTOR, &CCG2ex1Dlg::OnBnClickedChkshwknotvector)
	ON_BN_CLICKED(IDC_SHOWX, &CCG2ex1Dlg::OnBnClickedShowx)
	ON_BN_CLICKED(IDC_SHOWY, &CCG2ex1Dlg::OnBnClickedShowy)
	ON_BN_CLICKED(IDC_SHOWYX, &CCG2ex1Dlg::OnBnClickedShowyx)
	ON_BN_CLICKED(IDC_BBEZIER, &CCG2ex1Dlg::OnBnClickedBbezier)
	ON_BN_CLICKED(IDC_SHWCOORDS, &CCG2ex1Dlg::OnBnClickedShwcoords)
	ON_BN_CLICKED(IDC_BCENTER, &CCG2ex1Dlg::OnBnClickedBcenter)
END_MESSAGE_MAP()


// CCG2ex1Dlg message handlers

BOOL CCG2ex1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CRect rect;

	// Get size and position of the picture control
	GetDlgItem(IDC_OPENGL)->GetWindowRect(rect);

	// Convert screen coordinates to client coordinates
	ScreenToClient(rect);

	// Create OpenGL Control window
	m_oglWindow.oglCreate(rect, this);

	// Setup the OpenGL Window's timer to render
	m_oglWindow.m_unpTimer = m_oglWindow.SetTimer(1, 1, 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCG2ex1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCG2ex1Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCG2ex1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CCG2ex1Dlg::OnBnClickedOpenBtn()
{
	this->UpdateData();
	CStdioFile  f;
	CString str;
	CString resToken;
	int count=0;
	int curPos=0;
	int pos=0;
			
	char strFilter[] = { "TXT Files (*.txt)|*.txt|All Files (*.*)|*.*||" };
	CFileDialog FileDlg(TRUE, ".txt", NULL, 0, strFilter);
	if( FileDlg.DoModal() == IDOK )
	{
		if( f.Open(FileDlg.GetPathName(), CFile::modeRead) == FALSE )
			return;
		
		while(f.ReadString(str))
		{
			
			
			if(str[0]=='#') continue;
			switch(count)
			{
				case 0:
					m_oglWindow.numberOfCtrlPnts=atoi(str);
					m_oglWindow.numKnots=m_oglWindow.numberOfCtrlPnts+4;
					m_oglWindow.knotVector=new float[m_oglWindow.numKnots];
					m_oglWindow.ctrlPnts=new point3D[m_oglWindow.numberOfCtrlPnts];
					m_oglWindow.basis=new float**[m_oglWindow.numKnots];
					
					for(int i=0;i<m_oglWindow.numKnots;i++)
						m_oglWindow.basis[i]=new float*[4];
					for(int i=0;i<m_oglWindow.numKnots;i++)
						for(int j=0;j<4;j++)
						m_oglWindow.basis[i][j]=new float[LOD];
					m_oglWindow.bsplineX=new float[LOD];
					m_oglWindow.bsplineY=new float[LOD];
					
					count++;
					break;
				case 1:
					resToken= str.Tokenize(",",curPos);
					
					while (resToken != "")
					{
						m_oglWindow.knotVector[pos]=atof(resToken);
						resToken=str.Tokenize(",",curPos);
						pos++;
						
					};
					count++;
					curPos=0;
					pos=0;
					break;
				case 2:
					if(pos<m_oglWindow.numberOfCtrlPnts)
					{
						resToken=str.Tokenize(" ",curPos);
						m_oglWindow.ctrlPnts[pos][0]=atof(resToken);
						resToken=str.Tokenize(" ",curPos);
						m_oglWindow.ctrlPnts[pos][1]=atof(resToken);
						m_oglWindow.ctrlPnts[pos][2]=0.0;
						curPos=0;
						pos++;
					}
				default:
					continue;
			}
		}

	}
	else
		return;

	f.Close();
	m_oglWindow.indexOfKnotsInLODs=new int[m_oglWindow.numKnots];
	m_oglWindow.findMaxMin();
	m_oglWindow.bShowAxis=TRUE;
	//m_oglWindow.normalizeKnots();
	m_oglWindow.findBasis(0);
	m_oglWindow.calcBSpline();
	m_oglWindow.segments=m_oglWindow.numberOfCtrlPnts-3;
	for(int i=4;i<m_oglWindow.numKnots-3;i++)
		if(m_oglWindow.knotVector[i]==m_oglWindow.knotVector[i-1])
			m_oglWindow.segments--;

	dlgSegment=m_oglWindow.segments;
	
	m_oglWindow.bezierX=new float*[m_oglWindow.segments];
	m_oglWindow.bezierY=new float*[m_oglWindow.segments];
	m_oglWindow.bezCtrlPoints=new point2D*[m_oglWindow.segments];
	
	for(int i=0;i<m_oglWindow.segments;i++)
	{
		m_oglWindow.bezierX[i]=new float[LOD];
		m_oglWindow.bezierY[i]=new float[LOD];
		m_oglWindow.bezCtrlPoints[i]=new point2D[4];
	}
	m_oglWindow.BSplineToBezier();
	GetDlgItem(IDC_BBEZIER)->EnableWindow();

	m_oglWindow.m_fPosX = 0.0f;    // X position of model in camera view
	m_oglWindow.m_fPosY = 0.0f;    // Y position of model in camera view
	m_oglWindow.m_fZoom = 1.0f; 
	
	
	this->UpdateData(FALSE);
	
}

void CCG2ex1Dlg::OnBnClickedExit()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CCG2ex1Dlg::OnBnClickedChkshwctrlpoints()

{
	// TODO: Add your control notification handler code here
	if(m_oglWindow.m_bShowCtrlPoints==TRUE) m_oglWindow.m_bShowCtrlPoints=FALSE;
	else m_oglWindow.m_bShowCtrlPoints=TRUE;
}

void CCG2ex1Dlg::OnBnClickedChkshwknotvector()
{
	// TODO: Add your control notification handler code here
	if(m_oglWindow.m_bShowKnotVector==TRUE) m_oglWindow.m_bShowKnotVector=FALSE;
	else m_oglWindow.m_bShowKnotVector=TRUE;
}

void CCG2ex1Dlg::OnBnClickedShowx()
{
	// TODO: Add your control notification handler code here
	if(m_oglWindow.m_bShowXu==TRUE) m_oglWindow.m_bShowXu=FALSE;
	else m_oglWindow.m_bShowXu=TRUE;
}

void CCG2ex1Dlg::OnBnClickedShowy()
{
	// TODO: Add your control notification handler code here
	if(m_oglWindow.m_bShowYu==TRUE) m_oglWindow.m_bShowYu=FALSE;
	else m_oglWindow.m_bShowYu=TRUE;
}

void CCG2ex1Dlg::OnBnClickedShowyx()
{
	// TODO: Add your control notification handler code here
	if(m_oglWindow.m_bShowYX==TRUE) m_oglWindow.m_bShowYX=FALSE;
	else m_oglWindow.m_bShowYX=TRUE;
}

void CCG2ex1Dlg::OnBnClickedBbezier()
{
	// TODO: Add your control notification handler code here
	if(m_oglWindow.m_bBezier==TRUE)
	{
		m_oglWindow.m_bBezier=FALSE;
		GetDlgItem(IDC_BBEZIER)->SetWindowText("Convert to Bezier");
	}
	else {
		m_oglWindow.m_bBezier=TRUE;
		GetDlgItem(IDC_BBEZIER)->SetWindowText("Show B-Spline");
	}
	
}

void CCG2ex1Dlg::OnBnClickedShwcoords()
{
	// TODO: Add your control notification handler code here
	if(m_oglWindow.m_bShowCoords==TRUE) m_oglWindow.m_bShowCoords=FALSE;
	else m_oglWindow.m_bShowCoords=TRUE;
	
}

void CCG2ex1Dlg::OnBnClickedBcenter()
{
	// TODO: Add your control notification handler code here
	m_oglWindow.m_fPosX = 0.0f;    // X position of model in camera view
	m_oglWindow.m_fPosY = 0.0f;    // Y position of model in camera view
	m_oglWindow.m_fZoom = 1.0f;
	//this->UpdateData(FALSE);
}
