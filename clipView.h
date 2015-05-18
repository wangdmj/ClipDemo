#ifdef DLL_clip
#else
#define DLL_clip _declspec(dllimport)
#endif
DLL_clip void getLineFunction(double x1,double y1,double x2,double y2,double &k,double &b );
DLL_clip void getCircleIntersections(double c,double d,double r,double k,double b,double &x1,double &y1,double &x2,double &y2,double startPoint);
DLL_clip double isIntersections(double k,double b,double c,double d);
DLL_clip void GetIntersection(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,double &x,double &y);
DLL_clip void centerPoint(double x1,double y1,double x2,double y2,double c,double d,double r,double&cx,double&cy);
DLL_clip void getAngle(double x,double y,double c,double d,double&a);