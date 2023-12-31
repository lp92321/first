#pragma once
#include <iostream>
#include <vector>
#include "PlottingMath.h"

#ifndef Q_PointF
#define Q_PointF std::pair<double,double>
#endif

using namespace std;

//	计算两个屏幕坐标点的距离
double MathDistance(Q_PointF _pnt1, Q_PointF _pnt2)
{
	return sqrtf(pow((_pnt1.first - _pnt2.first), 2) + pow((_pnt1.second - _pnt2.second), 2));
}

//	计算点集合的总距离
double wholeDistance(const vector<Q_PointF>& _points)
{
	double distance = 0.0;
	for (int i = 0; i < _points.size() - 1; i++)
		distance += MathDistance(_points[i], _points[i + 1]);
	return distance;
}

//	获取基础长度
double getBaseLength(const vector<Q_PointF>& _points)
{
	return pow(wholeDistance(_points), 0.99);
}

//	求取两个屏幕点坐标的中间值
Q_PointF Mid(Q_PointF _pnt1, Q_PointF _pnt2)
{
	return Q_PointF((_pnt1.first + _pnt2.first) / 2.0, (_pnt1.second + _pnt2.second) / 2.0);
}

//	获取交集的点
Q_PointF getIntersectPoint(Q_PointF _pnt1, Q_PointF _pnt2, Q_PointF _pnt3, Q_PointF _pnt4)
{
	if (_pnt1.first == _pnt2.first)
	{
		double f = (_pnt4.first - _pnt3.first) / (_pnt4.second - _pnt3.second);
		double x = f * (_pnt1.second - _pnt3.second) + _pnt3.first;
		double y = _pnt1.second;
		return Q_PointF(x, y);
	}
	if (_pnt3.second == _pnt4.second)
	{
		double e = (_pnt2.first - _pnt1.first) / (_pnt2.second - _pnt1.second);
		double x = e * (_pnt3.second - _pnt1.second) + _pnt1.first;
		double y = _pnt3.second;
		return Q_PointF(x, y);
	}
	double e = (_pnt2.first - _pnt1.first) / (_pnt2.second - _pnt1.second);
	double f = (_pnt4.first - _pnt3.first) / (_pnt4.second - _pnt3.second);
	double y = (e * _pnt1.second - _pnt1.first - f * _pnt3.second + _pnt3.first) / (e - f);
	double x = e * y - e * _pnt1.second + _pnt1.first;
	return Q_PointF(x, y);
}

//	通过三个点确定一个圆的中心点
Q_PointF getCircleCenterOfThreePoints(Q_PointF _pnt1, Q_PointF _pnt2, Q_PointF _pnt3)
{
	Q_PointF pnt1 = Q_PointF((_pnt1.first + _pnt2.first) / 2, (_pnt1.second + _pnt2.second) / 2);
	Q_PointF pnt2 = Q_PointF((pnt1.first - _pnt1.second + _pnt2.second), pnt1.second + _pnt1.first - _pnt2.first);
	Q_PointF pnt3 = Q_PointF((_pnt1.first + _pnt3.first) / 2, (_pnt1.second + _pnt3.second) / 2);
	Q_PointF pnt4 = Q_PointF(pnt3.first - _pnt1.second + _pnt3.second, pnt3.second + _pnt1.first - _pnt3.first);
	return getIntersectPoint(pnt1, pnt2, pnt3, pnt4);
}

//	获取方位角(地平经度)
double getAzimuth(Q_PointF _start_point, Q_PointF _end_point)
{
	double azimuth = 0.0;
	double angle = asinf(abs(_end_point.second - _start_point.second) / MathDistance(_start_point, _end_point));
	if (_end_point.second >= _start_point.second&&_end_point.first >= _start_point.first)
		azimuth = angle + PI;
	else if (_end_point.second >= _start_point.second&&_end_point.first < _start_point.first)
		azimuth = PI * 2 - angle;
	else if (_end_point.second < _start_point.second&&_end_point.first < _start_point.first)
		azimuth = angle;
	else if (_end_point.second < _start_point.second&&_end_point.first >= _start_point.first)
		azimuth = PI - angle;
	return azimuth;
}

//	通过三个点获取方位角
double getAngleOfThreePoints(Q_PointF _pnt1, Q_PointF _pnt2, Q_PointF _pnt3)
{
	double angle = getAzimuth(_pnt2, _pnt1) - getAzimuth(_pnt2, _pnt3);
	return ((angle < 0) ? (angle + PI * 2) : angle);
}

//	判断是否是顺时针
bool isClockWise(Q_PointF _pnt1, Q_PointF _pnt2, Q_PointF _pnt3)
{
	return ((_pnt3.second - _pnt1.second)*(_pnt2.first - _pnt1.first)>(_pnt2.second - _pnt1.second)*(_pnt3.first - _pnt1.first));
}

//	获取线上的点
Q_PointF getPointOnLine(double t, Q_PointF _start_point, Q_PointF _end_point)
{
	double x = _start_point.first + (t*(_end_point.first - _start_point.first));
	double y = _start_point.second + (t*(_end_point.second - _start_point.second));
	return Q_PointF(x, y);
}

//	获取立方值
Q_PointF getCubicValue(double t, Q_PointF _start_point, Q_PointF _cPnt1, Q_PointF _cPnt2, Q_PointF _end_point)
{
	//t = max(min(t, 1), 0);
	t = (t < 1 ? t : 1)>0 ? (t < 1 ? t : 1) : 0;
	double tp = (1 - t);
	double t2 = t * t;
	double t3 = t2 * t;
	double tp2 = tp * tp;
	double tp3 = tp2 * tp;
	double x = (tp3 * _start_point.first) + (3 * tp2*t*_cPnt1.first) + (3 * tp*t2*_cPnt2.first) + (t3*_end_point.first);
	double y = (tp3 * _start_point.second) + (3 * tp2*t*_cPnt1.second) + (3 * tp*t2*_cPnt2.second) + (t3*_end_point.second);
	return Q_PointF(x, y);
}

//	根据起始点和旋转方向求取第三个点
Q_PointF getThirdPoint(Q_PointF _start_point, Q_PointF _end_point, double _angle, double _distance, bool _clockwise)
{
	double azimuth = getAzimuth(_start_point, _end_point);
	double alpha = _clockwise ? (azimuth + _angle) : (azimuth - _angle);
	double dx = _distance*cos(alpha);
	double dy = _distance*sin(alpha);
	return Q_PointF(_end_point.first + dx, _end_point.second + dy);
}

//	插值弓形线段点
vector<Q_PointF>getArcPoints(Q_PointF _center, double _radius, double _start_angle, double _end_angle)
{
	double x = 0.0, y = 0.0;
	vector<Q_PointF>pnts = vector<Q_PointF>();
	double angleDiff = _end_angle - _start_angle;
	angleDiff = ((angleDiff < 0) ? (angleDiff + PI * 2) : angleDiff);
	for (int i = 0; i <= 100; i++)
	{
		double angle = _start_angle + angleDiff*i / 100;
		x = _center.first + _radius*cos(angle);
		y = _center.second + _radius*sin(angle);
		pnts.push_back(Q_PointF(x, y));
	}
	return pnts;
}

//	获取默认三点的内切圆
Q_PointF getNormal(Q_PointF _pnt1, Q_PointF _pnt2, Q_PointF _pnt3)
{
	double dx1 = _pnt1.first - _pnt2.first;
	double dy1 = _pnt1.second - _pnt2.second;
	double d1 = sqrtf(dx1*dx1 + dy1*dy1);
	dx1 /= d1;
	dy1 /= d1;
	double dx2 = _pnt3.first - _pnt2.first;
	double dy2 = _pnt3.second - _pnt2.second;
	double d2 = sqrtf(dx2*dx2 + dy2*dy2);
	dx2 /= d2;
	dy2 /= d2;
	double ux = dx1 + dx2;
	double uy = dy1 + dy2;
	return Q_PointF(ux, uy);
}

pair<Q_PointF, Q_PointF>getBisectorNormals(double t, Q_PointF _pnt1, Q_PointF _pnt2, Q_PointF _pnt3)
{
	Q_PointF normal = getNormal(_pnt1, _pnt2, _pnt3);
	Q_PointF bisectorNormalRight = Q_PointF();
	Q_PointF bisectorNormalLeft = Q_PointF();
	double dt = 0.0, x = 0.0, y = 0.0;
	double dist = sqrtf(normal.first*normal.first + normal.second*normal.second);
	double ux = normal.first / dist;
	double uy = normal.second / dist;
	double d1 = MathDistance(_pnt1, _pnt2);
	double d2 = MathDistance(_pnt2, _pnt3);
	if (dist > 0.0001)
	{
		if (isClockWise(_pnt1, _pnt2, _pnt3))
		{
			dt = t*d1;
			x = _pnt2.first - dt*uy;
			y = _pnt2.second + dt*ux;
			bisectorNormalRight = Q_PointF(x, y);
			dt = t*d2;
			x = _pnt2.first + dt*uy;
			y = _pnt2.second - dt*ux;
			bisectorNormalLeft = Q_PointF(x, y);
		}
		else
		{
			dt = t*d1;
			x = _pnt2.first + dt*uy;
			y = _pnt2.second - dt*ux;
			bisectorNormalRight = Q_PointF(x, y);
			dt = t*d2;
			x = _pnt2.first - dt*uy;
			y = _pnt2.second + dt*ux;
			bisectorNormalLeft = Q_PointF(x, y);
		}
	}
	else
	{
		x = _pnt2.first + t*(_pnt1.first - _pnt2.first);
		y = _pnt2.second + t*(_pnt1.second - _pnt2.second);
		bisectorNormalRight = Q_PointF(x, y);
		x = _pnt2.first + t*(_pnt3.first - _pnt2.first);
		y = _pnt2.second + t*(_pnt3.second - _pnt2.second);
		bisectorNormalLeft = Q_PointF(x, y);
	}
	return pair<Q_PointF, Q_PointF>(bisectorNormalRight, bisectorNormalLeft);
}

//	获取左边控制点
Q_PointF getLeftMostControlPoint(const std::vector<Q_PointF>& _controlPoints, double t)
{
	if (_controlPoints.size() < 3)
		return Q_PointF();
	Q_PointF pnt1 = _controlPoints[0], pnt2 = _controlPoints[1], pnt3 = _controlPoints[2];
	double control_x = 0.0, control_y = 0.0;
	pair<Q_PointF, Q_PointF>pnts = getBisectorNormals(0, pnt1, pnt2, pnt3);
	Q_PointF normalRight = pnts.first;
	Q_PointF normal = getNormal(pnt1, pnt2, pnt3);
	double dist = sqrtf(normal.first*normal.first + normal.second*normal.second);
	if (dist > 0.0001)
	{
		Q_PointF mid = Mid(pnt1, pnt2);
		double px = pnt1.first - mid.first;
		double py = pnt1.second - mid.second;
		double d1 = MathDistance(pnt1, pnt2);
		double n = 2.0 / d1;
		double nx = -n*py;
		double ny = n*px;
		double a11 = nx*nx - ny*ny;
		double a12 = 2 * nx*ny;
		double a22 = ny*ny - nx*nx;
		double dx = normalRight.first - mid.first;
		double dy = normalRight.second - mid.second;
		control_x = mid.first + a11*dx + a12*dy;
		control_y = mid.second + a12*dx + a22*dy;
	}
	else
	{
		control_x = pnt1.first + t*(pnt2.first - pnt1.first);
		control_y = pnt1.second + t*(pnt2.second - pnt1.second);
	}
	return Q_PointF(control_x, control_y);
}

//	获取右边控制点
Q_PointF getRightMostControlPoint(const vector<Q_PointF>& _controlPoints, double t)
{
	int count = _controlPoints.size();
	if (count < 3)
		return Q_PointF();
	Q_PointF pnt1 = _controlPoints[count - 3];
	Q_PointF pnt2 = _controlPoints[count - 2];
	Q_PointF pnt3 = _controlPoints[count - 1];
	pair<Q_PointF, Q_PointF>pnts = getBisectorNormals(0, pnt1, pnt2, pnt3);
	Q_PointF normalLeft = pnts.second;
	Q_PointF normal = getNormal(pnt1, pnt2, pnt3);
	double dist = sqrtf(normal.first*normal.first + normal.second*normal.second);
	double control_x = 0.0, control_y = 0.0;
	if (dist > 0.0001)
	{
		Q_PointF mid = Mid(pnt2, pnt3);
		double px = pnt3.first - mid.first;
		double py = pnt3.second - mid.second;
		double d1 = MathDistance(pnt2, pnt3);
		double n = 2.0 / d1;
		double nx = -n*py;
		double ny = n*px;
		double a11 = nx*nx - ny*ny;
		double a12 = 2 * nx*ny;
		double a22 = ny*ny - nx*nx;
		double dx = normalLeft.first - mid.first;
		double dy = normalLeft.second - mid.second;
		control_x = mid.first + a11*dx + a12*dy;
		control_y = mid.second + a12*dx + a22*dy;
	}
	else
	{
		control_x = pnt3.first + t*(pnt2.first - pnt3.first);
		control_y = pnt3.second + t*(pnt2.second - pnt3.second);
	}
	return Q_PointF(control_x, control_y);
}

//	插值曲线点
vector<Q_PointF>getCurvePoints(double t, const vector<Q_PointF>& _controlPoints)
{
	Q_PointF leftControl = getLeftMostControlPoint(_controlPoints, t);
	Q_PointF pnt1 = Q_PointF(), pnt2 = Q_PointF(), pnt3 = Q_PointF();
	vector<Q_PointF>normals = vector<Q_PointF>(), points = vector<Q_PointF>();
	normals.push_back(leftControl);
	for (int i = 0; i < _controlPoints.size() - 2; i++)
	{
		pnt1 = _controlPoints[i];
		pnt2 = _controlPoints[i + 1];
		pnt3 = _controlPoints[i + 2];
		pair<Q_PointF, Q_PointF>normalPoints = getBisectorNormals(t, pnt1, pnt2, pnt3);
		normals.push_back(normalPoints.first);
		normals.push_back(normalPoints.second);
	}
	Q_PointF rightControl = getRightMostControlPoint(_controlPoints, t);
	if (rightControl != Q_PointF())
		normals.push_back(rightControl);
	for (int i = 0; i < _controlPoints.size() - 1; i++)
	{
		pnt1 = _controlPoints[i];
		pnt2 = _controlPoints[i + 1];
		points.push_back(pnt1);
		for (int j = 0; j < 100; j++)
		{
			if (i*2 < normals.size() && i*2+1<normals.size())
			{
				Q_PointF pnt = getCubicValue(j / 100.0, pnt1, normals[i * 2], normals[i * 2 + 1], pnt2);
				points.push_back(pnt);
			}
		}
		points.push_back(pnt2);
	}
	return points;
}

//	获取阶乘数据
int getFactorial(int n)
{
	if (n <= 1)
		return 1;
	else if (n == 2)
		return 2;
	else if (n == 3)
		return 6;
	else if (n == 4)
		return 24;
	else if (n == 5)
		return 120;
	else
	{
		int result = 1;
		for (int i = 1; i <= n; i++)
			result *= i;
		return result;
	}
}

//	获取二项分布
double getBinomialFactory(int n, int _index)
{
	return (getFactorial(n) / (getFactorial(_index)*getFactorial(n - _index)));
}

//	贝塞尔曲线
vector<Q_PointF>getBezierPoints(const vector<Q_PointF>&_points)
{
	if (_points.size() <= 2)
		return _points;
	else
	{
		vector<Q_PointF>bezierPoints = vector<Q_PointF>();
		int n = _points.size() - 1;
		for (double i = 0; i <= 1; i += 0.01)
		{
			double x = 0.0, y = 0.0;
			for (int j = 0; j <= n; j++)
			{
				double factor = getBinomialFactory(n, j);
				double a = pow(i, j);
				double b = pow(1 - i, n - j);
				x += factor*a*b*_points[j].first;
				y += factor*a*b*_points[j].second;
			}
			bezierPoints.push_back(Q_PointF(x, y));
		}
		bezierPoints.push_back(_points[n]);
		return bezierPoints;
	}
}

//	得到线性二次因子
double getQuadricBSplinFactor(int k, double t)
{
	double res = 0.0;
	if (k == 0)
		res = pow(t - 1, 2) / 2;
	else if (k == 1)
		res = (-2 * pow(t, 2) + 2 * t + 1) / 2;
	else if (k == 2)
		res = pow(t, 2) / 2;
	return res;
}

//	插值线性点
vector<Q_PointF>getQBSpilinePoints(const vector<Q_PointF>& _points)
{
	if (_points.size() <= 2)
		return _points;
	else
	{
		int n = 2;
		vector<Q_PointF>bSplinePoints = vector<Q_PointF>();
		int m = _points.size() - n - 1;
		bSplinePoints.push_back(_points[0]);
		for (int i = 0; i <= m; i++)
		{
			for (double j = 0; j <= 1; j += 0.05)
			{
				double x = 0.0, y = 0.0;
				for (int k = 0; k <= n; k++)
				{
					double factor = getQuadricBSplinFactor(k, j);
					x += factor*_points[i + k].first;
					y += factor*_points[i + k].second;
				}
				bSplinePoints.push_back(Q_PointF(x, y));
			}
		}
		bSplinePoints.push_back(_points[_points.size() - 1]);
		return bSplinePoints;
	}
}

