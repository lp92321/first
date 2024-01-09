/**
*****************************************************************************
*  Copyright (C) 2024 湖南大学机器人学院 All rights reserved
*  @file    flowchar_graphics_item_base.h
*  @brief   图元基类，包含和父对象、子对象连接线，增删节点，获取父、子图元等
*  @author  刘鹏
*  @date    2024.01.09
*  @version V0.1
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改日期    修改内容
*  @note
*****************************************************************************
*/

#ifndef FLOWCHAR_GRAPHICS_ITEM_BASE_H
#define FLOWCHAR_GRAPHICS_ITEM_BASE_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QUuid>
#include <QPen>
#include <QMap>
#include <QBrush>
#include <QFont>
#include "flowchart_global.h"

//*************************************图元基类***************************************
class FlowcharGraphicsLink;
class FlowchartGraphicsItem
{

public:

	FlowchartGraphicsItem(FlowchartInforBase* _infor, ItemType _type = ItemType::Null);
	~FlowchartGraphicsItem();


	// 获取图元id
	QString GetItemId();

	// 获取图元类型
	ItemType GetItemType();

	// 添加子节点
	void AddChild(FlowchartGraphicsItem* _item, FlowcharGraphicsLink* _link_item);

	// 删除子节点及其连线
	void DelChild(QString _id);

	// 添加父节点
	void AddFather(FlowchartGraphicsItem* _item, FlowcharGraphicsLink* _link_item);

	// 删除父节点连线及其连线
	void DelFather(QString _id);

	// 清空图元依赖关系
	void ItemClear();

	// 获取子对象集合
	QMap<QString, FlowchartGraphicsItem*> GetChildrenItems();

	// 获取父对象集合
	QMap<QString, FlowchartGraphicsItem*> GetFatherItems();

	


public:

	virtual QPointF GetCenterPoint(){ return center_point_; };
	virtual QPointF GetLeftPoint(){ return left_point_; };
	virtual QPointF GetRightPoint(){ return right_point_; };
	virtual QPointF GetTopPoint(){ return top_point_; };
	virtual QPointF GetBottomPoint(){ return bottom_point_; };

	// 获取图元信息
	virtual FlowchartInforBase* GetItemInformation() { return item_base_info_; };

	// 设置文本内容
	virtual void SetText(QString _content, QString _tooltip = "") = 0;


private:

    // 子对象维护
	QMap<QString, FlowchartGraphicsItem*>		children_item_;
	QMap<QString, FlowcharGraphicsLink*>		children_item_link_;
    // 父对象维护
	QMap<QString, FlowchartGraphicsItem*>		father_item_;
	QMap<QString, FlowcharGraphicsLink*>		father_item_link_;

public:

	FlowchartInforBase*				item_base_info_;
	QPointF                         center_point_, left_point_, right_point_, top_point_, bottom_point_;
};

#endif // FLOWCHAR_GRAPHICS_ITEM_BASE_H
