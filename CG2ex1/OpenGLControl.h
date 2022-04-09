#pragma once
#include "afxwin.h"
#include <GL/glut.h>



typedef float point2D[2];
typedef float point3D[3];

template <class T> std::string toString( T value );
CString FloatToText(float n, int nNumberOfDecimalPlaces);

class COpenGLControl :
	public CWnd
{
public:
	COpenGLControl(void);
	~COpenGLControl(void);
	// Timer
    UINT_PTR m_unpTimer;
	void oglCreate(CRect rect, CWnd *parent);
	void oglInitialize(void);
	void oglDrawScene(void);
	void findMaxMin(void);
	void normalizeKnots(void);
	void findBasis(int d);
	void calcBSpline(void);
	void BSplineToBezier(void);
	void doCircle(double x, double y, double radius);

private:
	CWnd    *hWnd;
    HDC     hdc;
    HGLRC   hrc;
    int     m_nPixelFormat;
    CRect   m_rect;
    CRect   m_oldWindow;
    CRect   m_originalRect;
	BOOL	m_bIsMaximized;
	

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDraw(CDC *pDC);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	float	m_fPosX;
	float	m_fPosY;
	float	m_fZoom;
	float	m_fRotX;
	float	m_fRotY;
	float	m_fLastX;
	float	m_fLastY;
	int		numberOfCtrlPnts;
	float*	knotVector;
	int		numKnots;
	point3D* ctrlPnts;
	point2D maxCoords;
	point2D minCoords;
	double lengthX;
	double lengthY;
	double maxLength;
	BOOL	 bShowAxis;
	BOOL	 m_bShowCtrlPoints;
	BOOL	 m_bShowKnotVector;
	BOOL	 m_bShowXu;
	BOOL	 m_bShowYu;
	BOOL	 m_bShowYX;
	BOOL	 m_bBezier;
	BOOL	 m_bShowCoords;
	//GLUnurbsObj *NurbId;
	float*** basis;
	int* indexOfKnotsInLODs;
	float* bsplineX;
	float* bsplineY;
	float** bezierX;
	float** bezierY;
	point2D** bezCtrlPoints;
	int segments;



	
	

};


