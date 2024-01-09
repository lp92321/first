/**
*****************************************************************************
*  Copyright (C) 2024 湖南大学机器人学院 All rights reserved
*  @file    flowchart_view.h
*  @brief   模型编辑窗口视图类
*  @author  刘鹏
*  @date    2024.01.09
*  @version V0.1
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改日期    修改内容
*  @note
*****************************************************************************
*/

#ifndef FLOWCHART_VIEW_H
#define FLOWCHART_VIEW_H

#include <QGraphicsView>
#include "flowchart_graphics_item.h"
#include "flowchar_graphics_link.h"
#include <QObject>
#include "flowchart_global.h"
#include <QMenu>

class FlowchartView : public QGraphicsView
{
	Q_OBJECT

public:
	FlowchartView(QGraphicsScene *parent = nullptr);
	~FlowchartView();
	
	void RegistryMenu(ItemType _menu_type, QMenu * _menu);

	void SetMouseModel(FlowchartCursor _type);

protected:
    virtual void mousePressEvent(QMouseEvent *event);//右击设置鼠标模式
    virtual void mouseReleaseEvent(QMouseEvent *event);//取消右击模式
    virtual void mouseMoveEvent(QMouseEvent *event);//拖动整个场景
    virtual void contextMenuEvent(QContextMenuEvent *event);//右键菜单事件
    virtual void wheelEvent(QWheelEvent* event);//滚轮事件scale

private:


private:
	QCursor						hand_cursor_;
    QPoint						mouse_press_point_;//右键按下的坐标
    QPoint						mouse_move_point_;//鼠标移动后的位置
	bool						is_move_ = false;
    bool						right_press_ = false;//右键是否按下
    QMap<ItemType, QMenu*>		menus_;//用于部件右键事件菜单选择
    double						view_scale_;//视图缩放比例，构造函数初始化为1
};

#endif // FLOWCHART_VIEW_H
