// View.cpp : implementation of the CView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "View.h"

CView::CView() 
{
  bPaused = true;
  iFrame = 0;
  rate = 0.02;
  particleSize = 18;
  nParticles = 40;
  xShift = 30.0;
  xSpan = 1860;
  yTop = 10;
  yInterval = (int)(particleSize*1.25);
  nRows = 43;
  fixedSpeed = 1.0;
  particles.resize(nRows);

  redBrush.CreateSolidBrush(RGB(255, 0, 0));
  blackBrush.CreateSolidBrush(RGB(0, 0, 0));
  bluePen.CreatePen(PS_SOLID, 0, RGB(0, 0, 255));
  blackPen.CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
  thickPen.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));

  for (int j = 0; j < nRows; j++) {
    particles[j].resize(nParticles);
    double xRand = (rand() % (int)(xSpan*1.0)) / (double)nParticles;
    for (int i = 0; i < nParticles; i++) {
      //particles[j][i].x = (rand()%(int)(xSpan*0.5)) / (double)nParticles + i*xSpan / nParticles;
      particles[j][i].x = xRand + i*xSpan / nParticles;
      particles[j][i].x1 = particles[0][i].x;
      particles[j][i].y = yTop +j*yInterval;
      int rVal = rand();
      //particles[j][i].xSpeed = i % 2 ? 0.5+(rVal%1000)*0.001 : -(0.5+(rVal % 1000)*0.001);
      particles[j][i].xSpeed = i % 2 ? -fixedSpeed : fixedSpeed;
      particles[j][i].color = RGB(255, (int)(i*(255.0 / nParticles)), 255 - (int)(i*(255.0 / nParticles)));
    }
  }
  // Backup the particles
  particlesBackup = particles;
}

BOOL CView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

void CView::Init(void)
{
  particles = particlesBackup;
  iFrame = 0;
}

LRESULT CView::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
  if (wParam == 'P' || wParam == 'p')
  {
    bPaused = !bPaused;
    SetMsgHandled(true);
  }
  else if (wParam == 'N' || wParam == 'n')
  {
    Init();
    SetMsgHandled(true);
  }
  return 0;
}

LRESULT CView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);
	//TODO: Add your drawing code here
  CRect rc;
  GetClientRect(&rc);

  CMemoryDC mDC(dc, rc);
  mDC.SelectBrush(blackBrush); //(HBRUSH)GetStockObject(GRAY_BRUSH), BLACK_BRUSH, GRAY_BRUSH
  mDC.SelectPen(blackPen);
  CBrush hBkGnd;
  hBkGnd.CreateSysColorBrush(COLOR_WINDOW);
  mDC.FillRect(&rc, hBkGnd);
  mDC.SelectPen(bluePen);
  mDC.MoveTo(xShift, 0);
  mDC.LineTo(xShift, rc.Height());
  mDC.SelectPen(thickPen);
  for (int j = 0; j < rc.Height(); j += 3) {
    xHitAmp = -sin(iFrame*rate);
    int xVal = (int)(xShift*(1+ xHitAmp*sin(j/((double)rc.Height())*atan(1.0)*4.0)));
    if(j==0) mDC.MoveTo(xVal, j);
    else mDC.LineTo(xVal, j);
  }
  mDC.SelectPen(blackPen);
  if (!bPaused) {
    if (iFrame <3142) iFrame++;
    else iFrame = 0;
  }
  //std::vector<Particle>::iterator it;
  for (int j = 0; j < particles.size(); j++) {
    if (!bPaused) {
      for (int i = 0; i < particles[j].size(); i++) {
        Particle& p1 = particles[j][i];
        //p1.x1 = p1.x + p1.xSpeed;
        double xRef = xHitAmp*xShift;
        if (p1.x + p1.xSpeed < xRef) {
          double frac = (p1.x-xRef) / -p1.xSpeed;
          p1.xSpeed *= -1.0;
          p1.x = xRef + (1 - frac)*p1.xSpeed;
        }
        else if (p1.x + p1.xSpeed >= xSpan) {
          double frac = (xSpan-p1.x) / p1.xSpeed;
          p1.xSpeed *= -1.0;
          p1.x = xSpan + (1-frac)*p1.xSpeed;
        }
        else {
          p1.x += p1.xSpeed;
        }
      }
      Particle *p1 = &(particles[j][0]);
      for (int i = 1; i < particles[j].size(); i++) {
        Particle *p2 = &(particles[j][i]);
        if (p1->x + particleSize+1 >= p2->x) {
          double val = p1->xSpeed;
          p1->xSpeed = p2->xSpeed;
          p2->xSpeed = val;
        }
        p1 = p2;
      }
    }
    for (int i = 0; i < particles[j].size(); i++) {
      Particle& p1 = particles[j][i];
      CBrush parBrush = CreateSolidBrush(p1.color);
      mDC.SelectBrush(parBrush);
      mDC.Ellipse(xShift+p1.x, p1.y, xShift + p1.x + particleSize, p1.y + particleSize);
      mDC.SelectBrush(NULL);
      parBrush.DeleteObject();
    }
  }
  dc.BitBlt(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, mDC, 0, 0, SRCCOPY);
	return 0;
}
