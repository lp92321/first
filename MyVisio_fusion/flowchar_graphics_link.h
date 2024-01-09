/**
*****************************************************************************
*  Copyright (C) 2024 湖南大学机器人学院 All rights reserved
*  @file    flowchar_graphics_link.h
*  @brief   图元连接线类
*  @author  刘鹏
*  @date    2024.01.09
*  @version V0.1
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改日期    修改内容
*  @note
*****************************************************************************
*/

#ifndef FLOWCHAR_GRAPHICS_LINK_H
#define FLOWCHAR_GRAPHICS_LINK_H

#include <QObject>
#include <QGraphicsPathItem>
#include "flowchart_graphics_item.h"
#include "flowchart_global.h"



enum class DrawLineAlignment
{
	DrawLeft = 0,
	DrawRight,
	DrawTop,
	DrawBottom,
};

struct FlowcharGraphicsLinkInfo : FlowchartInforBase
{
    double			angle_start_;//起点角度
    double			angle_end_;//终点角度
    QString			start_item_id_;//起点图元ID
    QString			end_item_id_;//终点图元ID
	FlowcharGraphicsLinkInfo() : FlowchartInforBase()
	{
//		item_style_.pen_.setColor(QColor(89, 152, 209));
//		item_style_.pen_.setWidth(2);
//		item_style_.brush_ = QBrush(QColor(89, 152, 209));
//		item_style_.text_pen_.setColor(QColor(89, 152, 209));
//        item_content_.content_ = "";
//        angle_start_ = 10;
//        angle_end_ = 350;
        item_style_.pen_.setColor(QColor(0, 255, 0));
        item_style_.pen_.setWidth(2);
        item_style_.brush_ = QBrush(QColor(0, 0, 255));
        item_style_.text_pen_.setColor(QColor(255, 0, 0));
        item_content_.content_ = "连接线";
        angle_start_ = 1;
        angle_end_ = 359;

		start_item_id_ = "";
		end_item_id_ = "";
	}
};

class FlowcharGraphicsLink 
	: public QObject
	, public QGraphicsPathItem
	, public FlowchartGraphicsItem
{
	Q_OBJECT
    /*
     *QGraphicsPathItem类提供了一个可以添加到QGraphicsScene的路径项。
     *要设置项的路径，请将QPainterPath传递给QGraphicsPathItem的构造函数，
     *或调用setPath（）函数。path（）函数返回当前路径。
     */

public:
	// 自循环只需要开始图元
	FlowcharGraphicsLink(FlowchartGraphicsItem* _start_item, FlowchartGraphicsItem* _end_item = nullptr, FlowcharGraphicsLinkInfo* _infor = new FlowcharGraphicsLinkInfo(), QObject *parent = nullptr);
	~FlowcharGraphicsLink();

	// 获取开始图元
	FlowchartGraphicsItem* GetStartItem();

	// 获取结束图元
	FlowchartGraphicsItem* GetEndItem();

	// 获取开始点
	QPointF GetStartPointF() {return start_point_;};

	// 获取结束点
	QPointF GetEndPointF() { return end_point_; };

	// 获取文本区域
	QRectF GetTextRect();

	// 更新箭头位置
	void UpdateLineArrow();

	// 设置文本
	virtual void SetText(QString _content, QString _tooltip = "") override;

	// 获取图元信息
	virtual FlowchartInforBase* GetItemInformation() override;


private:

	// 绘制两图元连线
	void DrawTwoItemLine();

	// 绘制单图元连线
	void DrawOneItemLine();

	// 判断箭头连线起始点类型
	DrawLineAlignment CompareDrawPoint(QPointF _start_point, QPointF _end_point);

	// 绘制箭头连线
	void DrawLineArrow(QPainter *_painter);

	// 绘制文本
	void DrawLineText(QPainter *_painter);

	// 获取箭头
	void UpdataArrow(QPointF _point);

	// 获取中位点位置
	QPointF GetLineCenterPoint();

	// 获取该角度连线
	QLineF caculateThirdPtFromTwoPoint(const QPointF& _point1, const QPointF& _point2, const double _lenght, const double _angle);

protected:

	virtual QPainterPath shape() const;


	virtual QRectF boundingRect() const;


	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);


	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);


	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);


	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


private:
	FlowcharGraphicsLinkInfo*	link_infor_;						// 图元信息

    FlowchartGraphicsItem*		start_item_; ///<起点图元对象,用于拖动图元时重绘连接线
    FlowchartGraphicsItem*		end_item_; ///<终点图元对象,用于拖动图元时重绘连接线


	QPointF						text_point_;						// 文本绘制位置
    QRectF						text_rect_; //连线的文本编辑框
	QPointF						mouse_pressed_point_;				// 鼠标按下位置
	bool						mouse_pressed_flag_;				// 鼠标是否按下

	// ! 绘制曲线箭头
	QPolygonF                   arrow_head_;						// 箭头
    QLineF						last_line_;							// 最后一段线用于计算箭头夹角  直接连线是表示最后一段线  直角连线时表示整段线
	QPointF						start_point_;						// 起点
	QPointF						end_point_;							// 终点
	QPointF						mid_point_;							// 中位点
	double						angle_start_, angle_end_;			// 计算中位点所需角度
	QPainterPath				arrow_path_;						// 箭头曲线

};

#endif // FLOWCHAR_GRAPHICS_LINK_H
