/*
 * BezierCurve.cpp
 * Author: Zex <top_zlynch@yahoo.com>
 */
#include "BezierCurve.h"
#include <iostream>

typedef std::vector<CvPoint> Points;
typedef std::map<int, int> PointsMap;

// p=p+q 
CvPoint BezierCurve::pointAdd(CvPoint p, CvPoint q)
{
    p.x += q.x;
    p.y += q.y;
    return p;
}

// p=c*p 
CvPoint BezierCurve::pointTimes(float c, CvPoint p)
{
    p.x *= c;
    p.y *= c;
    return p;
}

// u = [0-1]
//P1*t^3 + P2*3*t^2*(1-t) + P3*3*t*(1-t)^2 + P4*(1-t)^3 = Pnew  
CvPoint BezierCurve::Bernstein(float u, Points p)
{
    CvPoint a, b, c, d, r;

    a = pointTimes(pow(u,3), p[0]);
    b = pointTimes(3*pow(u,2)*(1-u), p[1]);
    c = pointTimes(3*u*pow((1-u),2), p[2]);
    d = pointTimes(pow((1-u),3), p[3]);

    r = pointAdd(pointAdd(a, b), pointAdd(c, d));

    return r;
}

void BezierCurve::CtrlPoint(int index, CvPoint p)
{
    ctrl_pts_[index] = p;
}

void BezierCurve::Add(int x, int y)
{
    CvPoint pt;
    pt.x = x;
    pt.y = y;

    ctrl_pts_.push_back(pt);
}

void BezierCurve::Add(CvPoint b)
{
    ctrl_pts_.push_back(b);
}

void BezierCurve::Start()
{
    if (4 > ctrl_pts_.size() || ((ctrl_pts_.size()-1) % 3))
        return;

    curve_pts_.clear();

    for (Points::iterator it = ctrl_pts_.begin();
        3 <= std::distance(it, ctrl_pts_.end()); it += 3)
        {
            for (int i = 0; i < divs_; i++)
            {
                float u  = (float)i/divs_;

                Points buf(4);
                std::copy(it, it+4, buf.begin());

                CvPoint p = Bernstein(u, buf);
                curve_pts_[p.x] = p.y;

            }
        }
}

void SimpleBezierCurve::Init(CvPoint start, CvPoint end)
{
    Add((end.x-start.x)/2, start.y+10);
    Add((end.x-start.x)/2, start.y+10);
    Add(end.x, start.y);
    Divs(end.x-start.x);
}

void SimpleBezierCurve::Start(int x, int y)
{
    CtrlPoint(1, cvPoint(x, y));
    CtrlPoint(2, cvPoint(x, y));
    base::Start();
}

void SimpleBezierCurve::Start()
{
    base::Start();
}

CvPoint SimpleBezierCurve::AnglePoint(CvPoint center)
{
    PointsMap pts = CurvePoints();
    CvPoint start = CtrlPoints().at(0);
    CvPoint end = CtrlPoints().at(3);
    CvPoint ret = start;

    if (center.x < start.x)
    {
        for (PointsMap::iterator it = pts.begin(); it != pts.end(); it++)
        {
            ret = (start.x - it->first > start.x-ret.x )? cvPoint(it->first, it->second) : ret;
        }
    }
    else if (center.x > end.x)
    {
        for (PointsMap::iterator it = pts.begin(); it != pts.end(); it++)
        {
            ret = (it->first - end.x > ret.x - end.x ) ? cvPoint(it->first, it->second) : ret;
        }
    }
    else if (2 > abs(center.y - start.y))
    {
        ret = cvPoint((start.x+end.x)/2, start.y);
    }
    else
    {
        for (PointsMap::iterator it = pts.begin(); it != pts.end(); it++)
        {
            ret = (abs(ret.y-start.y) < abs(it->second-start.y) )? cvPoint(it->first, it->second) : ret;
        }
    }

    return ret;
}


////-------------------test---------------->
//IplImage *image = 0;
//
//void on_mouse(int event, int x, int y, int flags, void *param) 
//{ 
//  SimpleBezierCurve *bc = (SimpleBezierCurve*)param;
//
//  if (event == CV_EVENT_MOUSEMOVE)
//  {
//      cvZero(image);
//      cvLine(image, cvPoint(200, 200), cvPoint(500, 200), CV_RGB(150,130,100), 1, 8, 0); 
//      bc->Start(x, y);
//      PointsMap pts = bc->CurvePoints();
////        int step = 10;
//
//      for (PointsMap::iterator it = pts.begin(); it != pts.end(); it++)
//           cvCircle(image, cvPoint(it->first, it->second), 0, CV_RGB(150,100,100), 1, 8, 1); 
//  }  
//  else if (event == CV_EVENT_LBUTTONDOWN)
//  {
//      cvZero(image);
//      bc->Add(cvPoint(x, y));
//      bc->Start();
//      PointsMap pts = bc->CurvePoints();
//
//      for (PointsMap::iterator it = pts.begin(); it != pts.end(); it++)
//           cvCircle(image, cvPoint(it->first, it->second), 0, CV_RGB(150,100,100), 1, 8, 1); 
//  }
//
//  cvShowImage("Tinny Tiger", image); 
//} 
// 
//int main(int argc, char* argv[]) 
//{ 
//  SimpleBezierCurve bc(cvPoint(200, 200), cvPoint(500, 200));
//
//  CvSize image_sz = cvSize(1000,1000);  
//  image = cvCreateImage(image_sz , 8, 3); 
//  cvNamedWindow("Tinny Tiger", CV_WINDOW_AUTOSIZE); 
//  cvSetMouseCallback("Tinny Tiger", on_mouse, &bc); 
//  //cvResizeWindow("Tinny Tiger",500,500); 
// 
//  cvShowImage("Tinny Tiger", image); 
//  cvWaitKey(0); 
//
//  return 0; 
//} 
////-------------------test----------------|
