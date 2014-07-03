/*
 * BezierCurve.h
 * Author: Zex <top_zlynch@yahoo.com>
 */
#ifndef __BEZIER_CURVE_H_
#define __BEZIER_CURVE_H_

#include <opencv/cxcore.h> 
#include <opencv/cv.h> 
#include <opencv/highgui.h>
 
typedef std::vector<CvPoint> Points;
typedef std::map<int, int> PointsMap;

class BezierCurve
{
    Points ctrl_pts_;
    PointsMap curve_pts_;
    int divs_;

protected:
    // p=p+q 
    CvPoint pointAdd(CvPoint p, CvPoint q); 
    
    // p=c*p 
    CvPoint pointTimes(float c, CvPoint p); 
    
    // u = [0-1]
    //P1*t^3 + P2*3*t^2*(1-t) + P3*3*t*(1-t)^2 + P4*(1-t)^3 = Pnew  
    CvPoint Bernstein(float u, Points p); 

public:

    BezierCurve()
    {   
        BezierCurve(200, 200);
    }   

    BezierCurve(int x, int y)
    {   
        Add(x, y); 
    }   

    BezierCurve(CvPoint p)
    {   
        Add(p);
    }   

    ~BezierCurve()
    {}  

    Points CtrlPoints() { return ctrl_pts_; }
    PointsMap CurvePoints() { return curve_pts_; }

    CvPoint CtrlPoint(int index) { return ctrl_pts_.at(index); }
    void CtrlPoint(int index, CvPoint p); 

    void Divs(int divs) { divs_ = divs; }
    int Divs() { return divs_; }

    friend BezierCurve operator+= (BezierCurve a, CvPoint b)
    {
        a.Add(b);
        return a;
    }

    void Add(int x, int y);

    void Add(CvPoint b);

    void Start();

};

class SimpleBezierCurve : public BezierCurve
{
    typedef BezierCurve base;

public:
    SimpleBezierCurve(int start_x, int start_y, int end_x, int end_y)
    : BezierCurve(start_x, start_y)
    {
        Init(cvPoint(start_x, start_y), cvPoint(end_x, end_y));
    }

    SimpleBezierCurve(CvPoint start, CvPoint end)
    : BezierCurve(start)
    {
        Init(start, end);
    }

    void Init(CvPoint start, CvPoint end);

    ~SimpleBezierCurve()
    {}

    void Start(int x, int y);

    void Start();

    CvPoint AnglePoint(CvPoint center);
};

#endif /* __BEZIER_CURVE_H_ */
