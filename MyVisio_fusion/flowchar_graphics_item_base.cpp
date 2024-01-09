/**
*****************************************************************************
*  Copyright (C) 2024 湖南大学机器人学院 All rights reserved
*  @file    flowchar_graphics_item_base.cpp
*  @brief   图元基类的方法定义
*  @author  刘鹏
*  @date    2024.01.09
*  @version V0.1
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改日期    修改内容
*  @note
*****************************************************************************
*/

#include "flowchar_graphics_item_base.h"
#include <QGraphicsScene>
#include "flowchar_graphics_link.h"
//*************************************图元基类***************************************
FlowchartGraphicsItem::FlowchartGraphicsItem(FlowchartInforBase* _infor, ItemType _type)
	: item_base_info_(_infor)
{
	// 设置图元类型
	item_base_info_->item_type_.type = _type;
}

FlowchartGraphicsItem::~FlowchartGraphicsItem()
{
	// 释放结构体对象
	//delete item_base_info_;
	//item_base_info_ = nullptr;
}

QString FlowchartGraphicsItem::GetItemId()
{
	if (item_base_info_==nullptr)
		return "";
	return item_base_info_->item_content_.id_;
}


ItemType FlowchartGraphicsItem::GetItemType()
{
	if (item_base_info_ == nullptr)
		return ItemType::Null;
	return item_base_info_->item_type_.type;
}

void FlowchartGraphicsItem::AddChild(FlowchartGraphicsItem* _item, FlowcharGraphicsLink* _link_item)
{
	QString child_id = _item->GetItemId();
    //map类型数据：QMap<QString, FlowchartGraphicsItem*>		children_item_;
    //map类型数据：QMap<QString, FlowcharGraphicsLink*>		children_item_link_;
	children_item_.insert(child_id, _item);
	children_item_link_.insert(child_id, _link_item);
	//item_base_info_->item_structural_.children_ids_.push_back(child_id);
}

void FlowchartGraphicsItem::DelChild(QString _id)
{
	if (children_item_link_.contains(_id)){
		children_item_link_.remove(_id);
	}
	if (children_item_.contains(_id)){
		children_item_.remove(_id);
		//item_base_info_->item_structural_.children_ids_.removeOne(_id);
	}
}

void FlowchartGraphicsItem::AddFather(FlowchartGraphicsItem* _item, FlowcharGraphicsLink* _link_item)
{
	QString father_id = _item->GetItemId();
	father_item_.insert(father_id, _item);
	father_item_link_.insert(father_id, _link_item);
	//item_base_info_->item_structural_.father_ids_.push_back(father_id);
}

void FlowchartGraphicsItem::DelFather(QString _id)
{
	if (father_item_link_.contains(_id)){
		father_item_link_.remove(_id);
	}
	if (father_item_.contains(_id)){
		father_item_.remove(_id);
		//item_base_info_->item_structural_.father_ids_.removeOne(_id);
	}
}

void FlowchartGraphicsItem::ItemClear()
{
	// ! [1] 获取该图元id
	QString item_id = item_base_info_->item_content_.id_;

	// ! [2] 遍历所有父类
	for (QMap<QString, FlowchartGraphicsItem*>::iterator iter = father_item_.begin(); iter != father_item_.end(); iter++)
	{
		FlowchartGraphicsItem* father_item = iter.value();
		// 清空 父类->该类 节点及其连线的容器
		father_item->DelChild(item_id);
	}
	// 清空父类对象容器
	father_item_.clear();
	// 遍历所有 父类->该类 连线
	for (QMap<QString, FlowcharGraphicsLink*>::iterator iter = father_item_link_.begin(); iter != father_item_link_.end(); iter++)
	{
		FlowcharGraphicsLink* father_link_item = iter.value();
		// 场景删除连线
		QGraphicsScene *scence_widget = father_link_item->scene();
		scence_widget->removeItem(father_link_item);
		// 释放连线内存
		delete father_link_item;
		father_link_item = nullptr;
	}
	// 清空 父类->该类 连线对象容器
	father_item_link_.clear();

	// ! [3] 遍历所有子类
	for (QMap<QString, FlowchartGraphicsItem*>::iterator iter = children_item_.begin(); iter != children_item_.end(); iter++)
	{
		FlowchartGraphicsItem* children_item = iter.value();
		// 删除 该类->子类 节点连线  清空容器
		children_item->DelFather(item_id);
	}
	// 清空子类对象容器
	children_item_.clear();
	// 遍历所有 该类->子类 连线
	for (QMap<QString, FlowcharGraphicsLink*>::iterator iter = children_item_link_.begin(); iter != children_item_link_.end(); iter++)
	{
		FlowcharGraphicsLink* children_link_item = iter.value();
		QGraphicsScene *scence_widget = children_link_item->scene();
		scence_widget->removeItem(children_link_item);
		// 释放连线内存
		delete children_link_item;
		children_link_item = nullptr;
	}
	children_item_link_.clear();
}

QMap<QString, FlowchartGraphicsItem*> FlowchartGraphicsItem::GetChildrenItems()
{
	return children_item_;
}

QMap<QString, FlowchartGraphicsItem*> FlowchartGraphicsItem::GetFatherItems()
{
	return father_item_;
}

