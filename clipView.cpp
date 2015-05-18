#define DLL_clip _declspec(dllexport)
#include <math.h>
#include "clipView.h"
void getLineFunction(double x1,double y1,double x2,double y2,double &k,double &b )
{
	double c,d,e;
	c=(x2-x1);
	d=y1*x2;
	e=y2*x1;
	k=(y2-y1)/c;
	b=(d-e)/c;
}
void getCircleIntersections
	(double c,double d,double r,double k,double b,double &x1,double &y1,double &x2,double &y2,double startPoint)
{
	double a,e,f,g,h,i,j,l,m,n,o,p,q,s,t,u,tx1,ty1,tx2,ty2;
	t=k*k;
	a=t+1;
	e=a*r*r;
	f=c*k;
	g=b-d;
	h=f+g;
	i=sqrt(e-h*h);
	j=i-c;
	u=g*k;
	l=j+u;
	m=j*k;
	n=d*t+b;
	o=n-m;
	p=i+c;
	q=p-u;
	s=p*k+n;
	tx1=-l/a;
	ty1=o/a;
	tx2=q/a;
	ty2=s/a;
	if (fabs(tx1-startPoint)<fabs(tx2-startPoint))
	{
		x1=tx1;
		y1=ty1;
		x2=tx2;
		y2=ty2;
	}
	else
	{
		x1=tx2;
		y1=ty2;
		x2=tx1;
		y2=ty1;
	}
}
double isIntersections(double k,double b,double c,double d)
{
	double a,e,l;
	a=k*c-d+b;
	e=sqrt(1+k*k);
	if (a<0)
		a=-a;
	l=a/e;
	return l;
}
void GetIntersection
	(double x1,double y1,double x2,double y2,double x3,double y3,double x4,double y4,double &x,double &y)
{
	double k1,b1,k2,b2,x0,y0,c,d,e,f;
	getLineFunction(x1,y1,x2,y2,k1,b1);
	getLineFunction(x3,y3,x4,y4,k2,b2);
	c=b2-b1;
	d=k1-k2;
	e=b2*k1;
	f=b1*k2;
	x0=c/d;
	y0=(e-f)/d;
	if ((x0>x1&&x0<x2 || x0<x1&&x0>x2)&&(x0>x3&&x0<x4 || x0<x3&&x0>x4))
	{
		x=x0;
		y=y0;
	}
	else
	{
		x=0;
		y=0;
	}
}
void centerPoint(double x1,double y1,double x2,double y2,double c,double d,double r,double&cx,double&cy)
{
	static double pi=3.1415926;
	double a,a1,a2;
	getAngle(x1,y1,c,d,a1);
	getAngle(x2,y2,c,d,a2);
	if (a1<a2)
		a=(a1+a2)/2+pi;
	else if(a2<a1) a=(a1+a2)/2;
	cx=r*cos(a)+c;
	cy=d-r*sin(a);
}
void getAngle(double x,double y,double c,double d,double&a)
{
	static double pi=3.1415926;
	double k,m,n;
	m=d-y;
	n=x-c;
	k=m/n;
	if (k>0)
	{
		if (m>0)
			a=atan(k);
		else a=atan(k)+pi;
	}
	else if (k<0)
	{
		if(m>0) a=atan(k)+pi;
		else a=atan(k)+pi*2;
	}
	else
	{
		if(n>0) a=0;
		else a=pi;
	}
}