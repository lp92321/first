/**
*****************************************************************************
*  Copyright (C) 2024 湖南大学机器人学院 All rights reserved
*  @file    flowchart_graphics_item.cpp
*  @brief   各种算子的图元类的方法定义
*  @author  刘鹏
*  @date    2024.01.09
*  @version V0.1
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改日期    修改内容
*  @note
*****************************************************************************
*/

#include "flowchart_graphics_item.h"
#include <QPainter>

#pragma region 流程矩形
//QGraphicsRecctItem类提供了一个矩形项，您可以将其添加到QGraphicsScene中。
//要设置项的矩形，请将QRectF传递给QGraphicsRectItem的构造函数，或调用setRect（）函数。rect（）函数返回当前矩形。

FlowchartGraphicsRectItem::FlowchartGraphicsRectItem(FlowchartItemRectInfo* _infor, QObject *parent)
	: QObject(parent)
	, FlowchartGraphicsItem(_infor, ItemType::Rect)
{
	// ! [1] 初始化图元位置大小
    //item_infor_矩形数据结构体
	item_infor_ = _infor;
    //QRectF类使用浮点精度在平面中定义矩形;QPointF类使用浮点精度定义平面中的点;QSizeF类使用浮点精度定义二维对象的大小。
	setRect({ QPointF(-item_infor_->width_ / 2, -item_infor_->height_ / 2), QSizeF(item_infor_->width_, item_infor_->height_) });
    // 将部件的位置设置为（0,0），是为了设置默认位置，对之后的通过代码设置相对位置有很大作用
    setPos(0,0);
    //返回部件相对父对象的坐标
	center_point_ = pos();
    //坐标系右下对应XY正方向
    //左边中间点=center-半宽-笔宽
    left_point_ = pos() + QPointF(-item_infor_->width_ / 2 - PEN_WIDTH, 0);
    //右边中间点=center+半宽+笔宽
    right_point_ = pos() + QPointF(item_infor_->width_ / 2 + PEN_WIDTH, 0);
    //上边中间点=center-半高-笔宽
    top_point_ = pos() + QPointF(0, -item_infor_->height_ / 2 - PEN_WIDTH);
    //下边中间点=center+半高+笔宽
    bottom_point_ = pos() + QPointF(0, item_infor_->height_ / 2 + PEN_WIDTH);
    // 将部件的位置设置为（position_x_,position_y_）
	setPos(item_infor_->position_x_, item_infor_->position_y_);
	// ! [2] 图元相关设置
    //启用项目标志
    /*
    *该项目支持使用鼠标进行交互式移动。
    *单击项目然后拖动，项目将与鼠标光标一起移动。
    *如果项目有子项，则所有子项也会被移动。
    *如果项目是选择的一部分，则所有选定的项目也会移动。
    *这个特性是通过QGraphicsItem的鼠标事件处理程序的基本实现提供的。
    */
	setFlag(QGraphicsItem::ItemIsMovable, true);
    /*
     *该项目支持选择。
     *启用此功能将使setSelected（）能够切换项目的选择。
     *它还允许通过调用QGraphicsScene::setSelectionArea（）、
     *单击项目或在QGraphicsView中使用橡皮筋选择来自动选择项目。
     */
	setFlag(QGraphicsItem::ItemIsSelectable, true);
    // 接收部件位置和形状更改的通知
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	// 设置图元层级
    //层级决定同级（相邻）项目的堆叠顺序。层级较高的同级项将始终绘制在层级较低的另一同级项之上。
	setZValue(2);

	// ! [3] 图元信息设置
    //设置工具提示
	this->setToolTip(item_infor_->item_content_.tooltip_);
//    SetText("新建的模型","这个模型的提示");
	
}

FlowchartGraphicsRectItem::~FlowchartGraphicsRectItem()
{
}

void FlowchartGraphicsRectItem::DrawItemText(QPainter *_painter)
{
	_painter->save();

	/*[] 计算字体宽度 根据item大小*/
	_painter->setFont(item_infor_->item_style_.font_);
	QFontMetrics font_metrics = _painter->fontMetrics();

	QRectF rect = this->rect();
	QRectF resize_rect = rect;
	QString text = item_infor_->item_content_.content_;

	QString tremporart_text;

	resize_rect.setHeight(rect.height());
	resize_rect.setWidth(rect.width());

	tremporart_text = font_metrics.elidedText(text, Qt::ElideRight, resize_rect.width());

	_painter->setPen(item_infor_->item_style_.text_pen_);

	tremporart_text.replace(text, "");
	if (!tremporart_text.isEmpty())
		resize_rect.setWidth(rect.width());

	resize_rect.moveCenter(rect.center());
	_painter->drawText(resize_rect, Qt::AlignCenter | Qt::TextWrapAnywhere, text);

	_painter->restore();
}

void FlowchartGraphicsRectItem::SetText(QString _content, QString _tooltip)
{
	item_infor_->item_content_.content_ = _content;
	if (_tooltip.compare("") == 0)
		return;
	item_infor_->item_content_.tooltip_ = _tooltip;
    //将结构体中的工具提示信息设置到部件
	this->setToolTip(item_infor_->item_content_.tooltip_);
    //刷新并生效
	this->update();
}


FlowchartInforBase* FlowchartGraphicsRectItem::GetItemInformation()
{
	// 位置信息更新
    //scenePos返回项目在场景坐标中的位置
	item_infor_->position_x_ = this->scenePos().x();
	item_infor_->position_y_ = this->scenePos().y();
	item_infor_->width_ = this->boundingRect().width();
	item_infor_->height_ = this->boundingRect().height();

	// 父子关系更新
	FlowchartStructuralData	structural_data;
	QMap<QString, FlowchartGraphicsItem*> father_items = GetFatherItems();
	for (QMap<QString, FlowchartGraphicsItem*>::iterator iter = father_items.begin(); iter != father_items.end(); iter++){
		structural_data.father_ids_.push_back(iter.key());
	}
	QMap<QString, FlowchartGraphicsItem*> children_items = GetChildrenItems();
	for (QMap<QString, FlowchartGraphicsItem*>::iterator iter = children_items.begin(); iter != children_items.end(); iter++) {
		structural_data.children_ids_.push_back(iter.key());
	}
	item_infor_->item_structural_ = structural_data;

	item_base_info_ = (FlowchartInforBase*)item_infor_;
	return item_base_info_;
}

//用来确定绘制图元的形状生效范围
QRectF FlowchartGraphicsRectItem::boundingRect() const
{
	QRectF rect = QGraphicsRectItem::boundingRect().adjusted(-3, -3, 3, 3);
	return rect;
}

//用来绘制图元的形状、内容、交互状态
void FlowchartGraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= Q_NULLPTR*/)
{
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	painter->save();
	painter->setPen(item_infor_->item_style_.pen_);
	painter->setBrush(item_infor_->item_style_.brush_);
	painter->drawRect(this->boundingRect());
	painter->restore();

	/*[] 绘制选中外框*/
	if (this->isSelected()) {
		/*[1]: 绘制外围的矩形框 */
		QPen selected_pen(QColor(147, 147, 147));
		selected_pen.setWidth(0.5);
		selected_pen.setStyle(Qt::DashLine);

		painter->setPen(selected_pen);
		painter->setBrush(Qt::NoBrush);
		painter->drawRect(this->boundingRect());

		const auto left_center_rect = QRectF((left_point_ - QPointF(2, 2)), QSizeF(5, 5));
		const auto right_center_rect = QRectF((right_point_ - QPointF(3, 3)), QSizeF(5, 5));
		const auto top_center_rect = QRectF((top_point_ - QPointF(2, 2)), QSizeF(5, 5));
		const auto bottom_center_rect = QRectF((bottom_point_ - QPointF(3, 3)), QSizeF(5, 5));

		/*[2]: 绘制四个方向的连接点 */
		painter->setPen(QColor(147, 147, 147));
		painter->setBrush(QColor(255, 255, 255));
		painter->drawRect(left_center_rect);
		painter->drawRect(right_center_rect);
		painter->drawRect(top_center_rect);
		painter->drawRect(bottom_center_rect);
	}

	DrawItemText(painter);
}

#pragma endregion 流程矩形

#pragma region 判定

FlowchartGraphicsConditionItem::FlowchartGraphicsConditionItem(FlowchartItemConditionInfo* _infor, QObject *parent /*= nullptr*/)
	: QObject(parent)
	, FlowchartGraphicsItem(_infor, ItemType::Condition)
{
	// ! [1] 初始化图元位置大小
	item_infor_ = _infor;
	this->item_infor_->height_ = 80;
	setPos(0,0);
	center_point_ = pos();
	left_point_ = pos() + QPointF(-item_infor_->width_ / 2 - 3, 0);
	right_point_ = pos() + QPointF(item_infor_->width_ / 2 + 3, 0);
	top_point_ = pos() + QPointF(0, -item_infor_->height_ / 2 - 3);
	bottom_point_ = pos() + QPointF(0, item_infor_->height_ / 2 + 3);
	setPos(item_infor_->position_x_, item_infor_->position_y_);
	QPainterPath path;
	path.moveTo(left_point_);
	path.lineTo(top_point_);
	path.lineTo(right_point_);
	path.lineTo(bottom_point_);
	setPath(path);

	// ! [2] 图元相关设置
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	// 设置图元层级
	setZValue(2);

	// ! [3] 图元信息设置
	this->setToolTip(item_infor_->item_content_.tooltip_);

}

FlowchartGraphicsConditionItem::~FlowchartGraphicsConditionItem()
{

}


void FlowchartGraphicsConditionItem::DrawItemText(QPainter *_painter)
{
	_painter->save();

	/*[] 计算字体宽度 根据item大小*/
	_painter->setFont(item_infor_->item_style_.font_);
	QFontMetrics font_metrics = _painter->fontMetrics();

	QRectF rect = this->boundingRect();
	QRectF resize_rect = rect;
	QString text = item_infor_->item_content_.content_;

	QString tremporart_text;

	resize_rect.setHeight(rect.height());
	resize_rect.setWidth(rect.width());

	tremporart_text = font_metrics.elidedText(text, Qt::ElideRight, resize_rect.width());

	_painter->setPen(item_infor_->item_style_.text_pen_);

	tremporart_text.replace(text, "");
	if (!tremporart_text.isEmpty())
		resize_rect.setWidth(rect.width());

	resize_rect.moveCenter(rect.center());
	_painter->drawText(resize_rect, Qt::AlignCenter | Qt::TextWrapAnywhere, text);

	_painter->restore();
}


void FlowchartGraphicsConditionItem::SetText(QString _content, QString _tooltip)
{
	item_infor_->item_content_.content_ = _content;
	if (_tooltip.compare("") == 0)
		return;
	item_infor_->item_content_.tooltip_ = _tooltip;
	this->setToolTip(item_infor_->item_content_.tooltip_);
	this->update();
}


FlowchartInforBase* FlowchartGraphicsConditionItem::GetItemInformation()
{
	item_infor_->position_x_ = this->scenePos().x();
	item_infor_->position_y_ = this->scenePos().y();
	item_infor_->width_ = this->boundingRect().width();
	item_infor_->height_ = this->boundingRect().height();

	// 父子关系更新
	FlowchartStructuralData	structural_data;
	QMap<QString, FlowchartGraphicsItem*> father_items = GetFatherItems();
	for (QMap<QString, FlowchartGraphicsItem*>::iterator iter = father_items.begin(); iter != father_items.end(); iter++) {
		structural_data.father_ids_.push_back(iter.key());
	}
	QMap<QString, FlowchartGraphicsItem*> children_items = GetChildrenItems();
	for (QMap<QString, FlowchartGraphicsItem*>::iterator iter = children_items.begin(); iter != children_items.end(); iter++) {
		structural_data.children_ids_.push_back(iter.key());
	}
	item_infor_->item_structural_ = structural_data;

	item_base_info_ = (FlowchartInforBase*)item_infor_;
	return item_base_info_;
}

QRectF FlowchartGraphicsConditionItem::boundingRect() const
{
	return this->path().boundingRect().adjusted(-3, -3, 3, 3);
}

void FlowchartGraphicsConditionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= Q_NULLPTR*/)
{
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	painter->save();
	painter->setPen(item_infor_->item_style_.pen_);
	painter->setBrush(item_infor_->item_style_.brush_);
	painter->drawPath(this->path());
	painter->restore();

	/*[] 绘制选中外框*/
	if (this->isSelected()) {
		/*[1]: 绘制外围的矩形框 */
		QPen selected_pen(QColor(147, 147, 147));
		selected_pen.setWidth(1);
		selected_pen.setStyle(Qt::DashLine);

		painter->setPen(selected_pen);
		painter->setBrush(Qt::NoBrush);
		painter->drawRect(this->boundingRect());

		const auto left_center_rect = QRectF((left_point_ - QPointF(3, 3)), QSizeF(5, 5));
		const auto right_center_rect = QRectF((right_point_ - QPointF(3, 3)), QSizeF(5, 5));
		const auto top_center_rect = QRectF((top_point_ - QPointF(3, 3)), QSizeF(5, 5));
		const auto bottom_center_rect = QRectF((bottom_point_ - QPointF(3, 3)), QSizeF(5, 5));

		/*[2]: 绘制四个方向的连接点 */
		painter->setPen(QColor(147, 147, 147));
		painter->setBrush(QColor(255, 255, 255));
		painter->drawRect(left_center_rect);
		painter->drawRect(right_center_rect);
		painter->drawRect(top_center_rect);
		painter->drawRect(bottom_center_rect);
	}

	DrawItemText(painter);
}

#pragma endregion 判定

#pragma region 自循环

FlowchartGraphicsCirculationItem::FlowchartGraphicsCirculationItem(FlowchartItemCirculationInfo* _infor, QObject *parent /*= nullptr*/)
	: QObject(parent)
	, FlowchartGraphicsItem(_infor, ItemType::Circulation)
{
	// ! [1] 初始化图元位置大小
	item_infor_ = _infor;
	this->item_infor_->width_ = 200;
	setPos(0, 0);
	center_point_ = pos();
	left_point_ = pos() + QPointF(-item_infor_->width_ / 2 - 3, 0);
	right_point_ = pos() + QPointF(item_infor_->width_ / 2 + 3, 0);
	top_point_ = pos() + QPointF(0, -item_infor_->height_ / 2 - 3);
	bottom_point_ = pos() + QPointF(0, item_infor_->height_ / 2 + 3);
	setPos(item_infor_->position_x_, item_infor_->position_y_);
	QPainterPath path;
	path.moveTo(QPointF(left_point_.x(),top_point_.y()));
	path.lineTo(QPointF(right_point_.x(), top_point_.y()));
	path.lineTo(QPointF(right_point_.x(), bottom_point_.y()));
	path.lineTo(QPointF(left_point_.x(), bottom_point_.y()));
	setPath(path);

	// ! [2] 图元相关设置
	setFlag(QGraphicsItem::ItemIsMovable, true);
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	// 设置图元层级
	setZValue(2);

	// ! [3] 图元信息设置
	this->setToolTip(item_infor_->item_content_.tooltip_);
}

FlowchartGraphicsCirculationItem::~FlowchartGraphicsCirculationItem()
{
	delete graphics_link_;
	graphics_link_ = nullptr;
}

void FlowchartGraphicsCirculationItem::SetText(QString _content, QString _tooltip /*= ""*/)
{
	item_infor_->item_content_.content_ = _content;
	if (_tooltip.compare("") == 0)
		return;
	item_infor_->item_content_.tooltip_ = _tooltip;
	this->setToolTip(item_infor_->item_content_.tooltip_);
	this->update();
}

void FlowchartGraphicsCirculationItem::SetCirculationLink(FlowcharGraphicsLink* _item)
{
	// 自循环连线不可选中不可移动
	graphics_link_ = _item;
	//graphics_link_->setFlags(flags()&~ItemIsSelectable | ItemIsMovable);
}

FlowchartInforBase* FlowchartGraphicsCirculationItem::GetItemInformation()
{
	item_infor_->position_x_ = this->scenePos().x();
	item_infor_->position_y_ = this->scenePos().y();
	item_infor_->width_ = this->boundingRect().width();
	item_infor_->height_ = this->boundingRect().height();

	// 父子关系更新
	FlowchartStructuralData	structural_data;
	QMap<QString, FlowchartGraphicsItem*> father_items = GetFatherItems();
	for (QMap<QString, FlowchartGraphicsItem*>::iterator iter = father_items.begin(); iter != father_items.end(); iter++) {
		structural_data.father_ids_.push_back(iter.key());
	}
	QMap<QString, FlowchartGraphicsItem*> children_items = GetChildrenItems();
	for (QMap<QString, FlowchartGraphicsItem*>::iterator iter = children_items.begin(); iter != children_items.end(); iter++) {
		structural_data.children_ids_.push_back(iter.key());
	}
	item_infor_->item_structural_ = structural_data;

	item_base_info_ = (FlowchartInforBase*)item_infor_;
	return item_base_info_;
}

void FlowchartGraphicsCirculationItem::DrawItemText(QPainter *_painter)
{
	_painter->save();

	/*[] 计算字体宽度 根据item大小*/
	_painter->setFont(item_infor_->item_style_.font_);
	QFontMetrics font_metrics = _painter->fontMetrics();

	QRectF rect = this->boundingRect();
	QRectF resize_rect = rect;
	QString text = item_infor_->item_content_.content_;

	QString tremporart_text;

	resize_rect.setHeight(rect.height());
	resize_rect.setWidth(rect.width());

	tremporart_text = font_metrics.elidedText(text, Qt::ElideRight, resize_rect.width());

	_painter->setPen(item_infor_->item_style_.text_pen_);

	tremporart_text.replace(text, "");
	if (!tremporart_text.isEmpty())
		resize_rect.setWidth(rect.width());

	resize_rect.moveCenter(rect.center());
	_painter->drawText(resize_rect, Qt::AlignCenter | Qt::TextWrapAnywhere, text);

	_painter->restore();
}

QRectF FlowchartGraphicsCirculationItem::boundingRect() const
{
	return this->path().boundingRect().adjusted(-3, -3, 3, 3);
}

void FlowchartGraphicsCirculationItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= Q_NULLPTR*/)
{
	painter->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	painter->save();
	painter->setPen(item_infor_->item_style_.pen_);
	painter->setBrush(item_infor_->item_style_.brush_);
	painter->drawPath(this->path());
	painter->restore();

	/*[] 绘制选中外框*/
	if (this->isSelected()) {
		/*[1]: 绘制外围的矩形框 */
		QPen selected_pen(QColor(147, 147, 147));
		selected_pen.setWidth(1);
		selected_pen.setStyle(Qt::DashLine);

		painter->setPen(selected_pen);
		painter->setBrush(Qt::NoBrush);
		painter->drawRect(this->boundingRect());

		const auto left_center_rect = QRectF((left_point_ - QPointF(3, 3)), QSizeF(5, 5));
		const auto right_center_rect = QRectF((right_point_ - QPointF(3, 3)), QSizeF(5, 5));
		const auto top_center_rect = QRectF((top_point_ - QPointF(3, 3)), QSizeF(5, 5));
		const auto bottom_center_rect = QRectF((bottom_point_ - QPointF(3, 3)), QSizeF(5, 5));

		/*[2]: 绘制四个方向的连接点 */
		painter->setPen(QColor(147, 147, 147));
		painter->setBrush(QColor(255, 255, 255));
		painter->drawRect(left_center_rect);
		painter->drawRect(right_center_rect);
		painter->drawRect(top_center_rect);
		painter->drawRect(bottom_center_rect);
	}

	DrawItemText(painter);
}

#pragma endregion 自循环
