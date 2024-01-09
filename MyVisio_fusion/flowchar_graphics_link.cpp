/**
*****************************************************************************
*  Copyright (C) 2024 湖南大学机器人学院 All rights reserved
*  @file    flowchar_graphics_link.cpp
*  @brief   图元连接线类的方法定义
*  @author  刘鹏
*  @date    2024.01.09
*  @version V0.1
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改日期    修改内容
*  @note
*****************************************************************************
*/

#include "flowchar_graphics_link.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "Curve.h"


FlowcharGraphicsLink::FlowcharGraphicsLink(FlowchartGraphicsItem* _start_item, FlowchartGraphicsItem* _end_item, FlowcharGraphicsLinkInfo* _infor, QObject *parent)
	: QObject(parent)
	, FlowchartGraphicsItem(_infor, ItemType::Link)
    , link_infor_(_infor)
	, start_item_(_start_item)
	, end_item_(_end_item)
	, mouse_pressed_flag_(false)
	, angle_start_(0)
	, angle_end_(0)
{
	// ! [1] 图元相关设置
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
	setZValue(3);
    //设置部件的工具提示文本
    this->setToolTip(link_infor_->item_content_.tooltip_);
	// 初始化中点位置
	angle_start_ = link_infor_->angle_start_;
	angle_end_ = link_infor_->angle_end_;
}


FlowcharGraphicsLink::~FlowcharGraphicsLink()
{
}

// 获取开始图元
FlowchartGraphicsItem* FlowcharGraphicsLink::GetStartItem()
{
	if (start_item_)
		return start_item_;
	return nullptr;
}

// 获取结束图元
FlowchartGraphicsItem* FlowcharGraphicsLink::GetEndItem()
{
	if (end_item_)
		return end_item_;
	return nullptr;
}

// 获取文本区域
QRectF FlowcharGraphicsLink::GetTextRect()
{
	return text_rect_;
}

// 更新箭头位置
void FlowcharGraphicsLink::UpdateLineArrow()
{
	if (start_item_ == nullptr)
		return;

	// ! [1] 区分两图元间连线		单图元连线
	if (end_item_ == nullptr) {
		//	单图元连线
		DrawOneItemLine();
	}
	else {
		//	两图元连线
		DrawTwoItemLine();
	}

	// ! [2] 获取弧线
    //QPainterPath类提供了一个用于绘制操作的容器(画线框)
	arrow_path_ = QPainterPath();
    //将当前点移动到给定点，隐式启动新的子路径并关闭上一个子路径
	arrow_path_.moveTo(start_point_);
	std::vector<Q_PointF>ctrl_points;
	ctrl_points.push_back(Q_PointF(start_point_.x(), start_point_.y()));
	ctrl_points.push_back(Q_PointF(mid_point_.x(), mid_point_.y()));
	ctrl_points.push_back(Q_PointF(end_point_.x(), end_point_.y()));
	Curve curve;
    // 获取曲线点集
	std::vector<Q_PointF>curve_points = curve.Generate(ctrl_points);
	for (const auto& point : curve_points)
        //添加一条从当前位置到给定端点的直线。绘制直线后，当前位置将更新为直线的终点。
        //struct pair结构体中两个成员变量first，second
		arrow_path_.lineTo(QPointF(point.first, point.second));
	// 掏空，防止首尾相连
    //QPainterPathStroker类用于为给定的绘制器路径生成可填充的轮廓。
    //通过调用createStroke（）函数，传递给定的QPainterPath作为参数，将创建一个表示给定路径轮廓的新绘制器路径。然后可以填充新创建的画家路径以绘制原始画家路径的轮廓。
	QPainterPathStroker stroker;
    //生成一个新路径，该路径是表示给定路径轮廓的可填充区域。
	arrow_path_ = stroker.createStroke(arrow_path_);

	// ! [3] 获取箭头
	QPointF arrow_start_point = QPointF(curve_points[curve_points.size() - 2].first, curve_points[curve_points.size() - 2].second);
	QPointF arrow_end_point = QPointF(curve_points[curve_points.size() - 1].first, curve_points[curve_points.size() - 1].second);
    //最后一段直线，用于计算箭头夹角
    last_line_.setPoints(arrow_start_point, arrow_end_point);
	qreal arrowSize = 16;
    //atan2函数在C语言里返回的是指方位角，返回以弧度表示的 y/x 的反正切，C 语言中atan2的函数原型为 double atan2(double y, double x)
	double angle = std::atan2(-last_line_.dy(), last_line_.dx());
	QPointF arrowP1 = end_point_ - QPointF(sin(angle + M_PI / 3) * arrowSize, cos(angle + M_PI / 3) * arrowSize);
	QPointF arrowP2 = end_point_ - QPointF(sin(angle + M_PI - M_PI / 3) * arrowSize, cos(angle + M_PI - M_PI / 3) * arrowSize);
    // 箭头（多个点连接成的图形）QPolygonF类型 QPolygonF是一个QVector＜QPointF＞。
    arrow_head_.clear();
	arrow_head_ << end_point_ << arrowP1 << arrowP2 << end_point_;
    //将给定的多边形作为（未闭合的）子路径添加到路径中。
    //请注意，添加多边形后的当前位置是多边形中的最后一个点。
    arrow_path_.addPolygon(arrow_head_);
	setPath(arrow_path_);

	// ! [3] 计算线段的中间点 用于文本时显示
	text_point_.setX(curve_points[curve_points.size() / 2].first);
	text_point_.setY(curve_points[curve_points.size() / 2].second);
}

// 设置文本
void FlowcharGraphicsLink::SetText(QString _title, QString _tooltip)
{
	link_infor_->item_content_.content_ = _title;
	if (_tooltip.compare("") == 0)
		return;
	link_infor_->item_content_.tooltip_ = _tooltip;
	this->setToolTip(link_infor_->item_content_.tooltip_);
	this->update();
}

// 获取图元信息
FlowchartInforBase* FlowcharGraphicsLink::GetItemInformation()
{
	link_infor_->angle_start_ = angle_start_;
	link_infor_->angle_end_ = angle_end_;
	if (start_item_)
		link_infor_->start_item_id_ = start_item_->GetItemId();
	if (end_item_)
		link_infor_->end_item_id_ = end_item_->GetItemId();
	item_base_info_ = (FlowchartInforBase*)link_infor_;
	return item_base_info_;
}

// 绘制两图元连线
void FlowcharGraphicsLink::DrawTwoItemLine()
{
	// ! [1] 获取起始点
	switch (CompareDrawPoint(start_item_->GetCenterPoint(), end_item_->GetCenterPoint()))
	{
	case DrawLineAlignment::DrawLeft:
		start_point_ = start_item_->GetLeftPoint();
		end_point_ = end_item_->GetRightPoint();
		break;
	case DrawLineAlignment::DrawRight:
		start_point_ = start_item_->GetRightPoint();
		end_point_ = end_item_->GetLeftPoint();
		break;
	case DrawLineAlignment::DrawTop:
		start_point_ = start_item_->GetTopPoint();
		end_point_ = end_item_->GetBottomPoint();
		break;
	case DrawLineAlignment::DrawBottom:
		start_point_ = start_item_->GetBottomPoint();
		end_point_ = end_item_->GetTopPoint();
		break;
	default:
		break;
	}

	// ! [2] 获取默认中点
	if (mid_point_.isNull())
	{
		mid_point_.setX(start_point_.x() / 2 + end_point_.x() / 2 - 10);
		mid_point_.setY(start_point_.y() / 2 + end_point_.y() / 2 - 10);
	}
	else
		mid_point_ = GetLineCenterPoint();
}

// 绘制单图元连线（用于自循环画线）
void FlowcharGraphicsLink::DrawOneItemLine()
{
	// ! [1] 获取起始点
    //获取起始图元右上点的xy坐标值和左上点的xy坐标值
	start_point_.setX(start_item_->GetRightPoint().x());
	start_point_.setY(start_item_->GetTopPoint().y());
	end_point_.setX(start_item_->GetLeftPoint().x());
	end_point_.setY(start_item_->GetTopPoint().y());
	// ! [2] 获取默认中点
	mid_point_.setX(start_point_.x() / 2 + end_point_.x() / 2);
	mid_point_.setY(start_point_.y() / 2 + end_point_.y() / 2 - 30);
}

// 判断箭头连线起始点类型
DrawLineAlignment FlowcharGraphicsLink::CompareDrawPoint(QPointF _start_point, QPointF _end_point)
{
	// 直线斜率
    //-1 <= k <= 1 : 倾斜角【0，45】度,【135,180】度
    //k > 1 : 倾斜角【45,90】度
    //K < -1 : 倾斜角【90,135】度
	double k_num = (double)(_end_point.y() - _start_point.y()) / (_end_point.x() - _start_point.x());
	if (k_num >= -1 && k_num <= 1)
	{
		if (_start_point.x() >= _end_point.x())
		{
			return DrawLineAlignment::DrawLeft;
		}
		else
		{
			return DrawLineAlignment::DrawRight;
		}
	}
	else
	{
		if (_start_point.y() >= _end_point.y())
		{
			return DrawLineAlignment::DrawTop;
		}
		else
		{
			return DrawLineAlignment::DrawBottom;
		}
	}
}

// 绘制箭头连线
void FlowcharGraphicsLink::DrawLineArrow(QPainter *_painter)
{
	// ! [1] 更新箭头位置
	UpdateLineArrow();

	// ! [2] 绘制图形
	_painter->save();
	QPen line_pen = link_infor_->item_style_.pen_;
	_painter->setPen(line_pen);
	_painter->setBrush(link_infor_->item_style_.brush_);
	_painter->drawPath(this->path());
	_painter->restore();
}

// 绘制文本
void FlowcharGraphicsLink::DrawLineText(QPainter *_painter)
{
	_painter->save();

	/* ! [1] 计算字体宽度 根据item大小*/
	_painter->setFont(link_infor_->item_style_.font_);
	QFontMetrics font_metrics = _painter->fontMetrics();
	QString text = link_infor_->item_content_.content_;
	int text_width = font_metrics.boundingRect(text).width() + 10;
	int text_height = font_metrics.height();

	QString tremporart_text;
	QRectF resize_rect = QRectF(text_point_ - QPointF(text_width / 2, text_height / 2), text_point_+ QPointF(text_width / 2, text_height / 2));
	text_rect_ = resize_rect.adjusted(-3, -3, 3, 3);
	_painter->setBrush(QColor(255, 255, 255));
	_painter->setPen(QColor(255,255,255));
	_painter->drawRect(text_rect_);
	_painter->setPen(link_infor_->item_style_.text_pen_);
	_painter->drawText(resize_rect, Qt::AlignCenter | Qt::TextWrapAnywhere, text);

	_painter->restore();
}

// 获取箭头
void FlowcharGraphicsLink::UpdataArrow(QPointF _point)
{
	
	// ! [1] 更新箭头
	qreal arrowSize = 20;

	double angle = std::atan2(-last_line_.dy(), last_line_.dx());

	QPointF arrowP1 = _point - QPointF(sin(angle + M_PI / 3), cos(angle + M_PI / 3))* arrowSize;
	QPointF arrowP2 = _point - QPointF(sin(angle + M_PI - M_PI / 3), cos(angle + M_PI - M_PI / 3))* arrowSize;

	arrow_head_.clear();
	arrow_head_ << _point << arrowP1 << arrowP2;
}

// 获取中位点位置
QPointF FlowcharGraphicsLink::GetLineCenterPoint()
{
	// 计算过起始点斜率连线
	double temp_number = 100;
	QLineF mid_line_start = caculateThirdPtFromTwoPoint(start_point_, end_point_, temp_number, angle_start_);
	// 计算过终点斜率连线
	QLineF mid_line_end = caculateThirdPtFromTwoPoint(end_point_, start_point_, temp_number, angle_end_);
	QPointF mid_point(0, 0);
    mid_line_start.intersect(mid_line_end, &mid_point);
	return mid_point;
}

// 获取该角度连线
QLineF FlowcharGraphicsLink::caculateThirdPtFromTwoPoint(const QPointF& _point1, const QPointF& _point2, const double _lenght, const double _angle)
{
	double x1 = _point1.x();
	double y1 = _point1.y();
	double x2 = _point2.x();
	double y2 = _point2.y();
	QLineF old_line = QLineF(_point1, _point2);

	QTransform translateTopoint1;
	translateTopoint1.translate(-x1, -y1);
	QLineF translate_line;
	translate_line = translateTopoint1.map(old_line);
	translate_line = translate_line.unitVector();
	translate_line.setP2(translate_line.p2() * _lenght);

	QTransform translateToold;
	translateToold.translate(x1, y1);

	QTransform rotationTopoint1;
	rotationTopoint1.rotateRadians(2 * M_PI - _angle * M_PI / 180);

	QTransform transform = rotationTopoint1 * translateToold;
	QLineF result_line = transform.map(translate_line);
	return result_line;
}

QPainterPath FlowcharGraphicsLink::shape() const
{
	QPainterPath path;
	path.addPolygon(arrow_head_);
	path.addPath(this->path());

	const qreal penWidthZero = qreal(0.00000001);
	QPainterPathStroker ps;
	ps.setWidth(10);
	QPainterPath p = ps.createStroke(path);
	p.addPath(path);

	return p;
}

QRectF FlowcharGraphicsLink::boundingRect() const
{
	qreal extra = pen().width() + 20;

	return this->path().boundingRect().adjusted(-extra, -extra, extra, extra);
}

void FlowcharGraphicsLink::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= Q_NULLPTR*/)
{
	painter->setRenderHint(QPainter::Antialiasing, true);

	// ! [1] 绘制箭头连线
	DrawLineArrow(painter);

	// ! [2] 绘制文本
	if (link_infor_->item_content_.content_.compare("") != 0)
		DrawLineText(painter);
	else
		text_rect_ = QRectF(text_point_ - QPointF(15, 13), text_point_ + QPointF(15, 13));

	// ! [3] 选中态
	if (isSelected()) {
		painter->setPen(QPen(Qt::black));
		painter->setBrush(Qt::green);

		QRectF select_rect = QRectF((mid_point_ - QPointF(3, 3)), QSizeF(6, 6));
		QRectF start_rect = QRectF((last_line_.p1() - QPointF(3, 3)), QSizeF(6, 6));
		QRectF end_rect = QRectF((last_line_.p2() - QPointF(3, 3)), QSizeF(6, 6));

		painter->drawRect(select_rect);
		painter->drawRect(start_rect);
		painter->drawRect(end_rect);
	}


	// 
}

void FlowcharGraphicsLink::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	
	QRectF select_rect = QRectF((mid_point_ - QPointF(5, 5)), QSizeF(10, 10));
	if (event->button() == Qt::LeftButton && select_rect.contains(event->pos()))
	{
		mouse_pressed_flag_ = true;
	}
	this->update();
	return QGraphicsPathItem::mousePressEvent(event);
}

void FlowcharGraphicsLink::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	if (mouse_pressed_flag_)
	{
		// 计算中位点斜率
		QPointF mouse_point = event->pos();
		mid_point_ = mouse_point;
		QLineF line_temp = QLineF(start_point_, end_point_);
		QLineF line_start = QLineF(start_point_, mouse_point);
		QLineF line_end = QLineF(end_point_, mouse_point);

		// 计算两线夹角，逆时针旋转
		angle_start_ = line_temp.angleTo(line_start);
		angle_end_ = line_temp.angleTo(line_end);
	}
	this->update();
	return QGraphicsPathItem::mouseMoveEvent(event);
}

void FlowcharGraphicsLink::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if (mouse_pressed_flag_)
	{
		mouse_pressed_flag_ = false;
	}
	this->update();
	return QGraphicsPathItem::mouseReleaseEvent(event);
}

