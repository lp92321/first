#pragma once

#include <QGraphicsScene>
#include <QObject>
#include <QLineEdit>
#include <QTextEdit>
#include <QAction>

#include "flowchart_view.h"
#include "flowchart_graphics_item.h"
#include "flowchar_graphics_link.h"
#include "flowchar_arrow_button.h"

enum class SceneMode
{
    DrawLineItem, //划线十字光标
    MoveItem //标准箭头光标
};
class CutomTextWdit;
class FlowchartScene : public QGraphicsScene
{
	Q_OBJECT

public:
	FlowchartScene(QObject *parent = nullptr);
	~FlowchartScene();

	// 获取视口对象
	FlowchartView* GetFlowcharView();

	// 设置鼠标绘制方式
	void SetSceneMode(SceneMode _model);

	// 流程图生成结构体集合
	FlowchartInforBases FlowChart2Data();

	// 结构体集合生成流程图
	void Data2FlowChart(FlowchartInforBases _datas);


private:

	// 添加一个图元
	FlowchartGraphicsItem* AddChildItem(FlowchartInforBase* _item_infor, ItemType _type);

	// 添加图元连接线
	FlowcharGraphicsLink* AddStartToEndItem(FlowchartGraphicsItem* _start_item, FlowchartGraphicsItem* _end_item);

	// 对象转化	qt原生对象->图元基类对象
	FlowchartGraphicsItem* QGraphToFlow(QGraphicsItem* _item);

	// 对象转化	图元基类对象->qt原生对象
	QGraphicsItem* FlowToQGraph(FlowchartGraphicsItem* _item);

	// 创建菜单
	void InitMenu();

	// 初始化编辑框
	void InitLineEdit();

	// 绘制磁吸线 水平
	void MagneticHLine();

	// 绘制磁吸线 垂直
	void MagneticVLine();

	// 删除图元
	void DeleteItem();

private slots:

	// ! [todo..] 菜单槽函数



	// 编辑框相关
	void FocusOutSlot(QString _text);

protected:

	virtual void drawBackground(QPainter *painter, const QRectF &rect);

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);//鼠标双击事件：图元文本编辑
    virtual void keyPressEvent(QKeyEvent *event);//键盘ESC和del键事件处理
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);//当拖放操作进入窗体时发送给它的事件
    virtual void dragMoveEvent(QGraphicsSceneDragDropEvent *event);//在进行拖放操作时发送的事件
    virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent *event);//当拖放操作离开窗体时发送给它的事件
    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);//当一个拖放操作完成时的事件
//    bool eventFilter(QObject *obj,QEvent *e);//重写事件筛选器
private:

    FlowchartView	  *widget_;	 // 视口
    QMenu	  *menu_;	 // 右键菜单
    CutomTextWdit  *input_widget_;	 // 图元编辑框（修改图元名字的TextEdit框）
    SceneMode	 scene_mode_ = SceneMode::MoveItem;	 // 鼠标编辑模式
    QPointF	 mouse_move_point_, mouse_pressed_point_;		// 鼠标按下与鼠标移动位置
    QMap<QString, FlowchartGraphicsItem*>  temp_items_;	 // 临时当前所有图元（不包含连线）	主要用来读取文件时连线使用

    FlowchartGraphicsItem		 *flow_item_temp_;	 // 临时图元对象，拖动创建
    QGraphicsLineItem	 *temp_line_ = nullptr;		 // 绘制连线临时对象 过程中删除  析构中无需删除
    QGraphicsItem	 *item_temp_;		 // 鼠标当前拖动对象 磁吸线会用到该对象
    QGraphicsLineItem	 *item_hline_,*item_vline_;	 // 水平磁吸线	垂直磁吸线

};


