#pragma once

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