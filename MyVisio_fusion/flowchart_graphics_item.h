/**
*****************************************************************************
*  Copyright (C) 2024 湖南大学机器人学院 All rights reserved
*  @file    flowchart_graphics_item.h
*  @brief   各种算子的图元类
*  @author  刘鹏
*  @date    2024.01.09
*  @version V0.1
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改日期    修改内容
*  @note
*****************************************************************************
*/

#ifndef FLOWCHART_GRAPHICS_ITEM_H
#define FLOWCHART_GRAPHICS_ITEM_H

#include <QTextEdit>
#include <QLineEdit>

#include "flowchar_graphics_item_base.h"
#include "flowchar_graphics_link.h"


// 流程矩形结构体
struct FlowchartItemRectInfo : FlowchartInforBase
{

	FlowchartItemRectInfo() :FlowchartInforBase()
	{
        item_content_.tooltip_ = "流程矩形结构体";
	};
	FlowchartItemRectInfo(double _x, double _y, double _width = 160.0, double _height = 40.0)
		:FlowchartInforBase(_x, _y, _width, _height)
	{
        item_content_.tooltip_ = "流程矩形结构体";
	};   

};

// 判定结构体
struct FlowchartItemConditionInfo : FlowchartInforBase
{
	FlowchartItemConditionInfo() :FlowchartInforBase()
	{
	};
	FlowchartItemConditionInfo(double _x, double _y, double _width = 160.0, double _height = 40.0)
		:FlowchartInforBase(_x, _y, _width, _height)
	{
	};
};

// 自循环结构体
struct FlowchartItemCirculationInfo : FlowchartInforBase
{
	FlowchartItemCirculationInfo() :FlowchartInforBase()
	{
	};
	FlowchartItemCirculationInfo(double _x, double _y, double _width = 160.0, double _height = 40.0)
		:FlowchartInforBase(_x, _y, _width, _height)
	{
	};
};

#pragma region 流程矩形

// 流程矩形
class FlowchartGraphicsRectItem
	: public QObject
	, public QGraphicsRectItem
	, public FlowchartGraphicsItem
{
	Q_OBJECT

public:

	explicit FlowchartGraphicsRectItem(FlowchartItemRectInfo* _infor, QObject *parent = nullptr);
	~FlowchartGraphicsRectItem();

	// 修改文本内容
	virtual void SetText(QString _content, QString _tooltip = "") override;

	// 获取界面信息
	virtual FlowchartInforBase* GetItemInformation() override;

	/****************获取图元点--中心、左、右、上、下*******************/
	virtual QPointF GetCenterPoint() override { return mapToScene(center_point_); };
	virtual QPointF GetLeftPoint() override{ return mapToScene(left_point_); };
	virtual QPointF GetRightPoint() override{ return mapToScene(right_point_); };
    //mapToScene:将位于该项目坐标系中的点映射到场景的坐标系，并返回映射的坐标
	virtual QPointF GetTopPoint() override{ return mapToScene(top_point_); };
	virtual QPointF GetBottomPoint() override{ return mapToScene(bottom_point_); };

	

private:

	// 绘制文本内容
	void DrawItemText(QPainter *_painter);

public:

	virtual QRectF boundingRect() const;

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

private:
	// 流程矩形结构体方便取用
	FlowchartItemRectInfo*	item_infor_;

};

#pragma endregion 流程矩形

#pragma region 判定

// 判定
class FlowchartGraphicsConditionItem
	: public QObject
	, public QGraphicsPathItem
	, public FlowchartGraphicsItem
{
	Q_OBJECT

public:
	explicit FlowchartGraphicsConditionItem(FlowchartItemConditionInfo* _infor, QObject *parent = nullptr);
	~FlowchartGraphicsConditionItem();


	// 修改文本内容
	virtual void SetText(QString _content, QString _tooltip = "") override;

	// 获取界面信息
	virtual FlowchartInforBase* GetItemInformation() override;

	/****************获取图元点--中心、左、右、上、下*******************/
	virtual QPointF GetCenterPoint() override { return mapToScene(center_point_); };
	virtual QPointF GetLeftPoint() override{ return mapToScene(left_point_); };
	virtual QPointF GetRightPoint() override{ return mapToScene(right_point_); };
	virtual QPointF GetTopPoint() override{ return mapToScene(top_point_); };
	virtual QPointF GetBottomPoint() override{ return mapToScene(bottom_point_); };

private:

	// 绘制文本内容
	void DrawItemText(QPainter *_painter);

public:

	virtual QRectF boundingRect() const;

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

private:

	FlowchartItemConditionInfo*			item_infor_;
};


#pragma endregion 判定

#pragma region 自循环

// 自循环
class FlowchartGraphicsCirculationItem
	: public QObject
	, public QGraphicsPathItem
	, public FlowchartGraphicsItem
{
	Q_OBJECT

public:
	explicit FlowchartGraphicsCirculationItem(FlowchartItemCirculationInfo* _infor, QObject *parent = nullptr);
	~FlowchartGraphicsCirculationItem();


	// 修改文本内容
	virtual void SetText(QString _content, QString _tooltip = "") override;

	// 添加自循环连线
	void SetCirculationLink(FlowcharGraphicsLink* _item);


	// 获取界面信息
	virtual FlowchartInforBase* GetItemInformation() override;

	/****************获取图元点--中心、左、右、上、下*******************/
	virtual QPointF GetCenterPoint() override { return mapToScene(center_point_); };
	virtual QPointF GetLeftPoint() override{ return mapToScene(left_point_); };
	virtual QPointF GetRightPoint() override{ return mapToScene(right_point_); };
	virtual QPointF GetTopPoint() override{ return mapToScene(top_point_); };
	virtual QPointF GetBottomPoint() override{ return mapToScene(bottom_point_); };

private:

	// 绘制文本内容
	void DrawItemText(QPainter *_painter);

public:

	virtual QRectF boundingRect() const;

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

private:

	FlowcharGraphicsLink						*graphics_link_;
	FlowchartItemCirculationInfo*				item_infor_;

};

#pragma endregion 自循环

# endif // FLOWCHART_GRAPHICS_ITEM_H

