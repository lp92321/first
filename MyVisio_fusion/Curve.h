/**
*****************************************************************************
*  Copyright (C) 2024 湖南大学机器人学院 All rights reserved
*  @file    Curve.h
*  @brief   图元连接线（曲线）类
*  @author  刘鹏
*  @date    2024.01.09
*  @version V0.1
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改日期    修改内容
*  @note
*****************************************************************************
*/

#ifndef CURVE_H
#define CURVE_H

#include "utils.h"

class Curve
{
public:
	vector<Q_PointF>Generate(const vector<Q_PointF>& _points)
	{
		if (_points.size() < 2)
			return vector<Q_PointF>();
		else if (_points.size() == 2)
			return _points;
		else
			return getCurvePoints(t, _points);
	}

private:
	double t = 0.3;
};

#endif // CURVE_H
