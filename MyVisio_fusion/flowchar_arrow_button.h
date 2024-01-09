/**
*****************************************************************************
*  Copyright (C) 2024 湖南大学机器人学院 All rights reserved
*  @file    flowchar_arrow_button.h
*  @brief   连接线箭头类和图元编辑框类
*  @author  刘鹏
*  @date    2024.01.09
*  @version V0.1
*----------------------------------------------------------------------------
*  @note 历史版本  修改人员    修改日期    修改内容
*  @note
*****************************************************************************
*/

#ifndef FLOWCHAR_ARROW_BUTTON_H
#define FLOWCHAR_ARROW_BUTTON_H


#include <QPainter>
#include <QPushButton>
#include <QStackedWidget>
#include <QTextEdit>

// 自定义箭头按钮
class FlowCharArrowButton : public QPushButton
{
	Q_OBJECT
public:
	explicit FlowCharArrowButton(QWidget *parent = Q_NULLPTR);
	~FlowCharArrowButton(){};

protected:
	virtual QSize sizeHint() const;

	virtual void paintEvent(QPaintEvent * event);
};


// 自定义编辑框
class CutomTextWdit
	:public QTextEdit
{
	Q_OBJECT

public:

	CutomTextWdit(QWidget *parent = nullptr);
	~CutomTextWdit();

signals:

	void FocusOutSignal(QString _text);

protected:

	virtual void focusInEvent(QFocusEvent *e);


	virtual void focusOutEvent(QFocusEvent *e);

};

#endif // FLOWCHAR_ARROW_BUTTON_H
