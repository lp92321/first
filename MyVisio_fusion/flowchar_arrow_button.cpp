/**
*****************************************************************************
*  Copyright (C) 2024 湖南大学机器人学院 All rights reserved
*  @file    flowchar_arrow_button.cpp
*  @brief   连接线箭头类、图元编辑框类的方法定义
*  @author  刘鹏
*  @date    2024.01.09
*  @version V0.1
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改日期    修改内容
*  @note
*****************************************************************************
*/

#include "flowchar_arrow_button.h"
#include <QStyleOptionButton>

FlowCharArrowButton::FlowCharArrowButton(QWidget *parent /*= Q_NULLPTR*/)
	: QPushButton(parent)
{
	setFixedSize(20, 40);
}

QSize FlowCharArrowButton::sizeHint() const
{
	return QSize(20,40);
}

void FlowCharArrowButton::paintEvent(QPaintEvent * event)
{
	QPainter p(this);
	QStyleOptionButton option;
	initStyleOption(&option);
	const bool enabled = option.state & QStyle::State_Enabled;		//！ \可用
	const bool checked = option.state & QStyle::State_On;			//!  \选中
	const bool selected = option.state & QStyle::State_MouseOver;	//!  \滑过
	const bool pressed = option.state & QStyle::State_Sunken;		//!	 \按下

	p.setPen(Qt::NoPen);
	p.setRenderHints(QPainter::Antialiasing);
	QColor brush = QColor(14,47,88, 220);
	if (selected || checked) {
		brush = QColor(14, 47, 88, 194);
	}
	p.fillRect(rect(), brush);

	QRect rect = option.rect;
	QRect icon_rect = option.rect;
	QPixmap pixmap = option.icon.pixmap(option.iconSize, QIcon::Normal, QIcon::On);
	int pixmap_width = pixmap.width() / pixmap.devicePixelRatio();
	int pixmap_heidth = pixmap.height() / pixmap.devicePixelRatio();

	icon_rect = QRect(rect.x() + (rect.width() - pixmap_width) / 2,
		rect.y() + (rect.height() - pixmap_heidth) / 2,
		pixmap_width, pixmap_heidth);
	p.save();
	p.setRenderHint(QPainter::SmoothPixmapTransform);
	p.drawPixmap(icon_rect, pixmap);
	p.restore();
}



CutomTextWdit::CutomTextWdit(QWidget *parent /*= nullptr*/)
	:QTextEdit(parent)
{
	QStringList qss;
	qss.append("QTextEdit{background:rgba(255,255,255,200); border:1px solid rgb(0,0,0);}");
	this->setStyleSheet(qss.join(""));
    // 此属性保存垂直滚动条的策略(关闭滚动条)
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

CutomTextWdit::~CutomTextWdit()
{

}

void CutomTextWdit::focusInEvent(QFocusEvent *e)
{
	return QTextEdit::focusInEvent(e);
}

void CutomTextWdit::focusOutEvent(QFocusEvent *e)
{
    // toPlainText:以纯文本形式返回文本编辑的文本
	emit FocusOutSignal(this->toPlainText());
	return QTextEdit::focusOutEvent(e);
}
