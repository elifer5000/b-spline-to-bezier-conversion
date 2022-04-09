#include "StdAfx.h"
#include "OpenGLControl.h"

COpenGLControl::COpenGLControl(void)
{
	bShowAxis = FALSE;
	m_bShowCtrlPoints = FALSE;
	m_bShowKnotVector = FALSE;
	m_bShowXu=FALSE;
	m_bShowYu=FALSE;
	m_bShowYX=TRUE;
	m_bShowCoords=FALSE;
	m_bBezier=FALSE;
	numberOfCtrlPnts = 0;
	knotVector = NULL;
	ctrlPnts = NULL;
	m_fPosX = 0.0f;    // X position of model in camera view
	m_fPosY = 0.0f;    // Y position of model in camera view
	m_fZoom = 1.0f;   // Zoom on model in camera view
	m_fRotX = 0.0f;    // Rotation on model in camera view
	m_fRotY = 0.0f;    // Rotation on model in camera view
	maxCoords[0]=0.0;
	maxCoords[1]=0.0;
	minCoords[0]=0.0;
	minCoords[1]=0.0;

	//NurbId = gluNewNurbsRenderer();
}

COpenGLControl::~COpenGLControl(void)
{
}

void COpenGLControl::oglCreate(CRect rect, CWnd *parent)
{
   CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_OWNDC, NULL,(HBRUSH)GetStockObject(BLACK_BRUSH), NULL);
   CreateEx(0, className, "OpenGL", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);

   // Set initial variables' values
   m_oldWindow    = rect;
   m_originalRect = rect;

   hWnd = parent;
}


BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void COpenGLControl::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	ValidateRect(NULL);
}

int COpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	oglInitialize();
	return 0;
}

void COpenGLControl::oglInitialize(void)
{
   // Initial Setup:
   //
   static PIXELFORMATDESCRIPTOR pfd =
   {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,
      PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
      PFD_TYPE_RGBA,
      32,    // bit depth
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      16,    // z-buffer depth
      0, 0, 0, 0, 0, 0, 0,
   };

   // Get device context only once.
   hdc = GetDC()->m_hDC;

   // Pixel format.
   m_nPixelFormat = ChoosePixelFormat(hdc, &pfd);
   SetPixelFormat(hdc, m_nPixelFormat, &pfd);

   // Create the OpenGL Rendering Context.
   hrc = wglCreateContext(hdc);
   wglMakeCurrent(hdc, hrc);

   // Basic Setup:
   //
   // Set color to use when clearing the background.
   glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
   glClearDepth(1.0f);

   glEnable(GL_POINT_SMOOTH);
   //glEnable(GL_LINE_SMOOTH);
   glHint(GL_NICEST,GL_POINT_SMOOTH_HINT);
   //glHint(GL_NICEST,GL_LINE_SMOOTH_HINT);
   //gluNurbsProperty(NurbId, GLU_SAMPLING_TOLERANCE, 5.0);


   // Turn on backface culling
   //glFrontFace(GL_CCW);
   //glCullFace(GL_BACK);

   // Turn on depth testing
   //glEnable(GL_DEPTH_TEST);
   //glDepthFunc(GL_LEQUAL);

   // Send draw request
   OnDraw(NULL);
}

void COpenGLControl::OnDraw(CDC *pDC)
{
	//glLoadIdentity();
	//glScalef(m_fZoom,m_fZoom,1.0);
	//glTranslatef(0.0f, 0.0f, -m_fZoom);
	//glTranslatef(m_fPosX, m_fPosY, 0.0f);
	//glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
	//glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);
}

void COpenGLControl::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
   {
      case 1:
      {
         // Clear color and depth buffer bits
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

         // Draw OpenGL scene
         oglDrawScene();

         // Swap buffers
         SwapBuffers(hdc);

         break;
      }

      default:
         break;
   }

   CWnd::OnTimer(nIDEvent);
}

void COpenGLControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy || nType == SIZE_MINIMIZED) return;

   // Map the OpenGL coordinates.
   glViewport(0, 0, cx, cy);

   // Projection view
   glMatrixMode(GL_PROJECTION);

   glLoadIdentity();

   // Set our current view perspective
   gluOrtho2D(minCoords[0]-0.2*lengthX,maxCoords[0]+0.2*lengthX,minCoords[1]-0.4*lengthY,maxCoords[1]+0.2*lengthY);
   // Model view
   glMatrixMode(GL_MODELVIEW);

	
}

void COpenGLControl::oglDrawScene(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(minCoords[0]-0.2*lengthX,maxCoords[0]+0.2*lengthX,minCoords[1]-0.4*lengthY,maxCoords[1]+0.2*lengthY);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(m_fZoom,m_fZoom,1.0);
	//glTranslatef(0.0f, 0.0f, -m_fZoom);
	glTranslatef(m_fPosX, m_fPosY, 0.0f);
	//glPushMatrix();
	//glLoadIdentity();

	//Draw axis
	if(bShowAxis)
	{
		glBegin(GL_LINES);
			glColor3f(1.0,1.0,1.0);
			glVertex2f(minCoords[0]-2*lengthX,0);
			glVertex2f(maxCoords[0]+2*lengthX,0);
		
			glVertex2f(0,minCoords[1]-2*lengthY);
			glVertex2f(0,maxCoords[1]+2*lengthY);
		glEnd();
	}
	//glPopMatrix();
	//B-SPLINE DRAWING
	if(m_bBezier==FALSE)
	{
		//Show control points
		if(m_bShowCtrlPoints)
		{
			for(int i=0; i<numberOfCtrlPnts;i++)
			{
				glPointSize(3.0);
				glBegin(GL_POINTS);
					glColor3f(1.0,0.0,0.0);
					glVertex2fv(ctrlPnts[i]);
				glEnd();
				
				if(m_bShowCoords)
					{
					CString Cbuffer;
					Cbuffer=FloatToText(ctrlPnts[i][0],2) + "," + FloatToText(ctrlPnts[i][1],2);
					std::string buffer( (const char*)Cbuffer );
					glColor3f(0.9,0.9,0.9);
					glRasterPos3f(ctrlPnts[i][0]+0.01*lengthX,ctrlPnts[i][1]+0.01*lengthY,0.0);
					for(std::string::iterator j=buffer.begin();j!=buffer.end();++j)
						glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, (int) *j);
				}
 			}
			
			for(int i=0; i<numberOfCtrlPnts-1;i++)
			{
				glBegin(GL_LINES);
					glColor3f(0.0,0.30,0.8);
					glVertex2fv(ctrlPnts[i]);
					glVertex2fv(ctrlPnts[i+1]);
				glEnd();
			}
		}

		//Show B-Spline
		if(numberOfCtrlPnts!=0 && m_bShowYX)
		{
			glBegin(GL_LINE_STRIP);
			for(int i=indexOfKnotsInLODs[3];i<indexOfKnotsInLODs[numKnots-4];i++)
			{
				
					glColor3f(1.0,0.1,0.05);
					glVertex2f(bsplineX[i],bsplineY[i]);
			}
			glEnd();
			// Show segmentation
			for(int j=3;j<=numKnots-4;j++)
				{
					glPointSize(5.0);
					glBegin(GL_POINTS);
					glColor3f(0.9,0.9,0.9);
					glVertex2f(bsplineX[indexOfKnotsInLODs[j]],bsplineY[indexOfKnotsInLODs[j]]);
					glEnd();
				}
		}
	}

	//BEZIER DRAWING
	if(m_bBezier==TRUE)
	{
		if(numberOfCtrlPnts!=0 && m_bShowYX)
		{
			//Show control points
			if(m_bShowCtrlPoints)
			{
			for(int i=0; i<segments;i++)
			{
				for(int j=0; j<4;j++)
				{
					glPointSize(3.0);
					glBegin(GL_POINTS);
						glColor3f(1.0,0.0,0.0);
						glVertex2fv(bezCtrlPoints[i][j]);
					glEnd();
					
					if(m_bShowCoords)
					{
						CString Cbuffer;
						Cbuffer=FloatToText(bezCtrlPoints[i][j][0], 2)+ "," + FloatToText(bezCtrlPoints[i][j][1], 2);
						std::string buffer( (const char*)Cbuffer );
						
						glColor3f(0.9,0.9,0.9);
						glRasterPos3f(bezCtrlPoints[i][j][0]+0.01*lengthX,bezCtrlPoints[i][j][1]+0.01*lengthY,0.0);
						for(std::string::iterator j=buffer.begin();j!=buffer.end();++j)
							glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, (int) *j);
					}
 				}
				for(int k=0; k<3;k++)
				{
					glBegin(GL_LINES);
						glColor3f(0.0,0.30,0.8);
						glVertex2fv(bezCtrlPoints[i][k]);
						glVertex2fv(bezCtrlPoints[i][k+1]);
					glEnd();
				}
			}
			}
			//Draw Bezier
			glBegin(GL_LINE_STRIP);
			for(int v=0;v<segments;v++)
				for(int i=0;i<LOD;i++)
				{
					glColor3f(1.0,0.5,0.25);
					glVertex2f(bezierX[v][i],bezierY[v][i]);
				}
			glEnd();
		}
		/*
		//Show X(u) and Y(u)
		if((m_bShowXu || m_bShowYu) && numberOfCtrlPnts!=0)
		{
			float min;
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			if(minCoords[1]>=minCoords[0]) min=minCoords[0]; else min=minCoords[1];

			gluOrtho2D(-0.1*(knotVector[numKnots-1]-knotVector[0])+knotVector[0],0.1*(knotVector[numKnots-1]-knotVector[0])+knotVector[numKnots-1],min,1.2*maxLength);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}
		if(m_bShowXu && numberOfCtrlPnts!=0)
		{
			glColor3f(0.5,0.8,0.0);
			glBegin(GL_LINE_STRIP);
			
			float factor=(knotVector[numKnots-1]-knotVector[0])/(segments*(LOD-1));
			
			
			for(int v=0;v<segments;v++)
				for(int i=0;i<LOD;i++)
				{
				glVertex2f(factor*(i+v*(LOD-1)),bezierX[v][i]);
				
				}
			glEnd();
		}

		if(m_bShowYu && numberOfCtrlPnts!=0)
		{
			glColor3f(0.8,0.2,0.8);
			glBegin(GL_LINE_STRIP);
			
			float factor=(knotVector[numKnots-1]-knotVector[0])/(segments*(LOD-1));

			for(int v=0;v<segments;v++)
				for(int i=0;i<LOD;i++)
				{
				glVertex2f(factor*(i+v*(LOD-1)),bezierY[v][i]);
				}
			glEnd();
		}*/
	}

	//Show X(u) and Y(u)
	if((m_bShowXu || m_bShowYu) && numberOfCtrlPnts!=0)
	{
		float min;
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(minCoords[1]>=minCoords[0]) min=minCoords[0]; else min=minCoords[1];

		gluOrtho2D(-0.1*(knotVector[numKnots-1]-knotVector[0])+knotVector[0],0.1*(knotVector[numKnots-1]-knotVector[0])+knotVector[numKnots-1],min,1.2*maxLength);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
			if(m_bShowXu && numberOfCtrlPnts!=0)
			{
				glColor3f(0.5,0.8,0.0);
				glBegin(GL_LINE_STRIP);
				
				float factor=(knotVector[numKnots-1]-knotVector[0])/(LOD-1);
				
				for(int i=indexOfKnotsInLODs[3];i<indexOfKnotsInLODs[numKnots-4];i++)
				{
					glVertex2f(i*factor,bsplineX[i]);
					
				}
				glEnd();
			}

			if(m_bShowYu && numberOfCtrlPnts!=0)
			{
				glColor3f(0.8,0.2,0.8);
				glBegin(GL_LINE_STRIP);
				
				float factor=(knotVector[numKnots-1]-knotVector[0])/(LOD-1);
				for(int i=indexOfKnotsInLODs[3];i<indexOfKnotsInLODs[numKnots-4];i++)
				{
					glVertex2f(i*factor,bsplineY[i]);
				}
				glEnd();
			}
		glPopMatrix();
	}

	//Show knot vector
	float knotY=0.15;
	if(m_bShowKnotVector && numberOfCtrlPnts!=0)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-0.1*(knotVector[numKnots-1]-knotVector[0])+knotVector[0],0.1*(knotVector[numKnots-1]-knotVector[0])+knotVector[numKnots-1],0.0,10);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		
		for(int i=0; i<(numKnots); i++)
		{
			glColor3f(0.7,0.65,0.9);
			if(knotVector[i]==knotVector[i-1]) knotY+=0.2;
			else knotY=0.15;
			glPointSize(10.0);
					glBegin(GL_POINTS);
					glColor3f(0.1,0.1,0.9);
					glVertex2f(knotVector[i],knotY);
			glEnd();
		}
		glEnable(GL_LINE_SMOOTH);
		for(int i=4;i<numKnots-3;i++)
			if(knotVector[i]!=knotVector[i-1])
			{
				glColor3f(0.7,0.8,0.4);
				glBegin(GL_LINES);
					glVertex2f(knotVector[i-1],0.15);
					glVertex2f(knotVector[i],0.15);
				glEnd();
			}
		glDisable(GL_LINE_SMOOTH);

		//Draw Basis
		srand(347);  //Seed for random colors
		
		glBegin(GL_LINE_STRIP);
		for(int k=0;k<numKnots-4;k++)
		{
			glColor3f((rand() % 1000) / 1000.0, (rand() % 1000) / 1000.0, (rand() % 1000) / 1000.0);
			for(int u=0;u<LOD;u++)
				glVertex2f(knotVector[0]+((float)u/(float)(LOD-1))*knotVector[numKnots-1], basis[k][3][u]+0.5);
		}
		glEnd();
		glPopMatrix();

	}

}

void COpenGLControl::OnMouseMove(UINT nFlags, CPoint point)
{
   int diffX = (int)(point.x - m_fLastX);
   int diffY = (int)(point.y - m_fLastY);
   m_fLastX  = (float)point.x;
   m_fLastY  = (float)point.y;

   // Left mouse button
   if (nFlags & MK_LBUTTON)
   {
      m_fRotX += (float)0.5f * diffY;

      if ((m_fRotX > 360.0f) || (m_fRotX < -360.0f))
      {
         m_fRotX = 0.0f;
      }

      m_fRotY += (float)0.5f * diffX;

      if ((m_fRotY > 360.0f) || (m_fRotY < -360.0f))
      {
         m_fRotY = 0.0f;
      }
   }

   // Right mouse button - zoom
   else if (nFlags & MK_RBUTTON)
   {
	  m_fZoom += (float)0.001f*maxLength*diffY;
	  if(m_fZoom < 0.1) m_fZoom = 0.1;
	  if(m_fZoom > 20) m_fZoom = 20;
	 
   }

   // Middle mouse button - translate
   else if (nFlags & MK_MBUTTON)
   {
      m_fPosX += (float)0.0025f * lengthX / m_fZoom *diffX;
      m_fPosY -= (float)0.0025f * lengthY / m_fZoom *diffY;
   }

   OnDraw(NULL);

	CWnd::OnMouseMove(nFlags, point);
}

void COpenGLControl::findMaxMin(void)
{
	maxCoords[0]=ctrlPnts[0][0];
	maxCoords[1]=ctrlPnts[0][1];
	minCoords[0]=ctrlPnts[0][0];
	minCoords[1]=ctrlPnts[0][1];
	for(int i=1;i<numberOfCtrlPnts;i++)
	{
		if(ctrlPnts[i][0]>maxCoords[0])
			maxCoords[0]=ctrlPnts[i][0];
		if(ctrlPnts[i][1]>maxCoords[1])
			maxCoords[1]=ctrlPnts[i][1];
		if(ctrlPnts[i][0]<minCoords[0])
			minCoords[0]=ctrlPnts[i][0];
		if(ctrlPnts[i][1]<minCoords[1])
			minCoords[1]=ctrlPnts[i][1];
	}
	lengthX=maxCoords[0]-minCoords[0];
	lengthY=maxCoords[1]-minCoords[1];
	if(lengthX>lengthY) maxLength=lengthX;
	else maxLength=lengthY;

}

void COpenGLControl::normalizeKnots()
{
	for(int i=0;i<numKnots;i++)
		knotVector[i]/=knotVector[numberOfCtrlPnts+3];
}

void COpenGLControl::doCircle(double x, double y, double radius)
	{
		double y1=y+radius;
		double x1=x;
		glBegin(GL_LINE_STRIP);
		for(double angle=0.0f;angle<=(2.0f*3.14159);angle+=0.01f)
		{
			double x2=x+(radius*(float)sin((double)angle));
			double y2=y+(radius*(float)cos((double)angle));
			glVertex2d(x1,y1);
			y1=y2;
			x1=x2;
		}
		glEnd();
	}

void COpenGLControl::findBasis(int d)
{
	float num1=0;
	float num2=0;
	float den1=0;
	float den2=0;
	//float normed_u=0;
	float u=0;
	float u1=0;

	switch(d)
	{
	case 0:
		for(int k=0;k<numKnots;k++)
			for(int i=0;i!=LOD;i++)
			{
				u=(knotVector[numKnots-1]-knotVector[0])*i/(float)(LOD-1);
				u1=(knotVector[numKnots-1]-knotVector[0])*(i+1)/(float)(LOD-1);
				
				if(i==LOD-1) u-=0.001f;
				if(i+1==LOD-1) u1-=0.001f;
				if(knotVector[k]>=u && knotVector[k]<u1)
					indexOfKnotsInLODs[k]=i;
				//normed_u=u/((float)(LOD));
				if((knotVector[k]<=u && u<=knotVector[k+1]))
					basis[k][0][i]=1.0;
				else basis[k][0][i]=0.0;
			}
		findBasis(1);
		break;
	default:
		if(d>3) break;
		for(int k=0;k<numKnots-d;k++)
		{
			
			den1=knotVector[k+d]-knotVector[k];
			den2=knotVector[k+d+1]-knotVector[k+1];
			
			if(fabs(den1)<EPSILON) den1=1.0;
			if(fabs(den2)<EPSILON) den2=1.0;
			
			for(int i=0;i<LOD;i++)
			{
				u=(knotVector[numKnots-1]-knotVector[0])*i/(float)(LOD-1);
				if(i==LOD-1) u-=0.001f;
				num1=u-knotVector[k];
				num2=knotVector[k+d+1]-u;
				basis[k][d][i]=(num1/den1)*basis[k][d-1][i]+(num2/den2)*basis[k+1][d-1][i];
			}
		}
		findBasis(++d);
	}
				
}

void COpenGLControl::calcBSpline()
{
	for(int i=0;i<LOD;i++)
	{
		bsplineX[i]=0.0;
		bsplineY[i]=0.0;
		for(int k=0;k<numberOfCtrlPnts;k++)
		{
			bsplineX[i]+=basis[k][3][i]*ctrlPnts[k][0];
			bsplineY[i]+=basis[k][3][i]*ctrlPnts[k][1];
		}
	}
}

void COpenGLControl::BSplineToBezier()
{
	float a1,b1,c1,d1;
	float a2,b2,c2,d2;
	point2D A,B;
	int u1=0;
	int u2=0;
	float t,invt;
	
	for(int v=3;v<numKnots-4;v++)
		if(knotVector[v]!=knotVector[v+1])
		{
			bezCtrlPoints[v-3][0][0]=bsplineX[indexOfKnotsInLODs[v]];
			bezCtrlPoints[v-3][0][1]=bsplineY[indexOfKnotsInLODs[v]];

			bezCtrlPoints[v-3][3][0]=bsplineX[indexOfKnotsInLODs[v+1]];
			bezCtrlPoints[v-3][3][1]=bsplineY[indexOfKnotsInLODs[v+1]];

			a1=0.343;  //t=0.3
			b1=0.441;
			c1=0.189;
			d1=0.027;

			a2=0.064;  //t=0.6
			b2=0.288;
			c2=0.432;
			d2=0.216;
			
			u1=(0.7*indexOfKnotsInLODs[v]+0.3*indexOfKnotsInLODs[v+1]);
			
			A[0]=bsplineX[u1]-a1*bezCtrlPoints[v-3][0][0]-d1*bezCtrlPoints[v-3][3][0];
			A[1]=bsplineY[u1]-a1*bezCtrlPoints[v-3][0][1]-d1*bezCtrlPoints[v-3][3][1];

			u2=(0.4*indexOfKnotsInLODs[v]+0.6*indexOfKnotsInLODs[v+1]);
			
			B[0]=bsplineX[u2]-a2*bezCtrlPoints[v-3][0][0]-d2*bezCtrlPoints[v-3][3][0];
			B[1]=bsplineY[u2]-a2*bezCtrlPoints[v-3][0][1]-d2*bezCtrlPoints[v-3][3][1];
			
			bezCtrlPoints[v-3][2][0]=(b1*B[0]-b2*A[0])/(c2*b1-c1*b2);
			bezCtrlPoints[v-3][2][1]=(b1*B[1]-b2*A[1])/(c2*b1-c1*b2);

			bezCtrlPoints[v-3][1][0]=(A[0]-c1*bezCtrlPoints[v-3][2][0])/b1;
			bezCtrlPoints[v-3][1][1]=(A[1]-c1*bezCtrlPoints[v-3][2][1])/b1;

			for(int j=0;j<LOD;j++)
			{
				t=(float)j/(float)(LOD-1);
				if(j==LOD-1) t-=0.001f;
				invt=1.0-t;

				bezierX[v-3][j]=bezCtrlPoints[v-3][0][0]*invt*invt*invt+bezCtrlPoints[v-3][1][0]*3.0*t*invt*invt+bezCtrlPoints[v-3][2][0]*3.0*t*t*invt+bezCtrlPoints[v-3][3][0]*t*t*t;
				bezierY[v-3][j]=bezCtrlPoints[v-3][0][1]*invt*invt*invt+bezCtrlPoints[v-3][1][1]*3.0*t*invt*invt+bezCtrlPoints[v-3][2][1]*3.0*t*t*invt+bezCtrlPoints[v-3][3][1]*t*t*t;
			}
			
		}
}

template <class T> std::string toString( T value )
{
   std::ostringstream oss;

   oss << value;

   return oss.str();
}

// Converts a floating point number to ascii (without the appended 0's)
// Rounds the value if nNumberOfDecimalPlaces >= 0
CString FloatToText(float n, int nNumberOfDecimalPlaces)
{
    CString str;

    if(nNumberOfDecimalPlaces >= 0)
    {
        int decimal, sign;
        char *buffer = _fcvt((double)n, nNumberOfDecimalPlaces, &decimal, &sign);

        CString temp(buffer);
        
        // Sign for +ve or -ve
        if(sign != 0)
            str = "-";

        // Copy digits up to decimal point
        if(decimal <= 0)
        {
            str += "0.";
            for(; decimal < 0; decimal++)
                str += "0";
            str += temp;
        } else {
            str += temp.Left(decimal);
            str += ".";
            str += temp.Right(temp.GetLength() - decimal);
        }
    } else {
        str.Format("%-g", n);
    }

    // Remove appended zero's. "123.45000" become "123.45"
    int nFind = str.Find(".");
    if(nFind >= 0)
    {
        int nFinde = str.Find("e");    // 1.0e-010 Don't strip the ending zero
        if(nFinde < 0)
        {
            while(str.GetLength() > 1 && str.Right(1) == "0")
                str = str.Left(str.GetLength() - 1);
        }
    }

    // Remove decimal point if nothing after it. "1234." becomes "1234"
    if(str.Right(1) == ".")
        str = str.Left(str.GetLength() - 1);
    
    return str;
}