#include "flowchart_view.h"
#include <QApplication>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
FlowchartView::FlowchartView(QGraphicsScene *parent)
	: QGraphicsView(parent)
	, view_scale_(1.0)
{
    //QGraphicsView使用此属性来决定如何更新已重新导出或更改的场景区域。通常不需要修改此属性，但在某些情况下，这样做可以提高渲染性能。
    //QGraphicsView::AnchorUnderMouse:鼠标下方的点用作锚点(缩放中心点)。
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    /*
     * 调整视图大小时视图应如何定位场景。
        QGraphicsView使用此属性来决定当视口小部件的大小更改时如何在视口中定位场景。默认行为“NoAnchor”在调整大小期间保持场景的位置不变；调整大小时，视图的左上角将显示为已锚定。
        请注意，当只有场景的一部分可见时（即，当有滚动条时），此属性的效果是明显的。否则，如果整个场景适合视图，QGraphicsScene将使用视图对齐在视图中定位场景。
     */
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);
    //启用渲染提示提示；指示引擎应尽可能消除文本失真。若要强制禁用文本的抗锯齿，请不要使用此提示
	setRenderHint(QPainter::TextAntialiasing);
    //此属性保留使用橡皮筋选择矩形选择项目的行为。
    //Qt::ContainsItemBoundingRect:输出列表仅包含其边框完全包含在选择区域内的项目。与区域轮廓相交的项目不包括在内。
    setRubberBandSelectionMode(Qt::ContainsItemBoundingRect);
    /*
     *此属性保留在按下鼠标左键时在场景上拖动鼠标的行为。
      此属性定义了当用户单击场景背景并拖动鼠标（例如，使用指针滚动视口内容，或使用橡皮筋选择多个项目）时应执行的操作。默认值NoDrag不起任何作用。
      此行为仅影响未由任何项目处理的鼠标单击。您可以通过创建QGraphicsView的子类并重新实现mouseMoveEvent（）来定义自定义行为。
     *
    *QGraphicsView::RubberBandDrag将出现一条橡皮筋。拖动鼠标将设置橡皮筋的几何图形，并选中橡皮筋覆盖的所有项目。非交互式视图禁用此模式。
    */
     setDragMode(QGraphicsView::RubberBandDrag);
    //setRenderHint(QPainter::Antialiasing);//消除基本体的边的锯齿
    //setOptimizationFlag(QGraphicsView::DontSavePainterState);//在渲染背景或前景以及渲染每个项目时保护绘制者状态,允许更改画笔和画刷

     //禁用水平滚动条
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     //禁用垂直滚动条
	this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//设置记录光标信息
    //启用鼠标跟踪
	this->setMouseTracking(true);
}

FlowchartView::~FlowchartView()
{
}

void FlowchartView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::RightButton) {
		// ! [1] 鼠标拖动相关
		SetMouseModel(FlowchartCursor::OpenHandCursor);
        right_press_ = true;//点击右键标志
		mouse_press_point_ = event->pos();
	}

	QGraphicsView::mousePressEvent(event);
}

void FlowchartView::mouseReleaseEvent(QMouseEvent *event)
{
    //将应用程序覆盖光标设置为光标。如果setOverrideCursor（）已被调用两次，则调用restoreOverrideCuror（）将激活第一个游标集。第二次调用此函数将恢复原始小部件的游标。
	QApplication::restoreOverrideCursor();
	// 设置鼠标拖动模式
	/*	三种拖拽模式：
	1、NoDrag（没有任何反应，鼠标事件将被忽略）
	2、ScrollHandDrag(光标变为指向手，然后拖动鼠标将滚动滚动条，此模式在交互和非交互模式下均有效)
	3、RubberBandDrag(将出现矩形块，拖动鼠标将设置矩形的大小，并选中矩形覆盖的所有项目，非交互视图禁用此模式)*/
	setDragMode(QGraphicsView::RubberBandDrag);
	right_press_ = false;
	QGraphicsView::mouseReleaseEvent(event);
}

void FlowchartView::mouseMoveEvent(QMouseEvent *event)
{
    //将应用程序覆盖光标设置为光标。如果setOverrideCursor（）已被调用两次，则调用restoreOverrideCuror（）将激活第一个游标集。第二次调用此函数将恢复原始小部件的游标。
	QApplication::restoreOverrideCursor();
	if (right_press_) {
        is_move_ = true;//右键拖动时为true，但是这个标志位好像没啥用
		setDragMode(QGraphicsView::NoDrag);
        SetMouseModel(FlowchartCursor::ClosedHandCursor);//右键抓取模式

        QPointF disPointF = event->pos() - mouse_press_point_;//移动坐标
		mouse_press_point_ = event->pos();
        // 连续显示整个场景的移动
		this->scene()->setSceneRect(scene()->sceneRect().x() - disPointF.x(), scene()->sceneRect().y() - disPointF.y(),
			scene()->sceneRect().width(), scene()->sceneRect().height());
		this->scene()->update();
		/*QPoint move_pos = event->pos();
		QPointF dis = mapToScene(move_pos) - mapToScene(mouse_press_point_);
		QGraphicsView::translate(dis.x(), dis.y());*/
	}
	mouse_move_point_ = event->pos();
	QGraphicsView::mouseMoveEvent(event);
}

//右键菜单事件(功能异常未实现)
void FlowchartView::contextMenuEvent(QContextMenuEvent *event)
{
	if (is_move_) {
        is_move_ = false;//右键拖动时为true，但是这个标志位好像没啥用
		return;
	}

	// ! [1] 根据不同的选择 显示不同的菜单
    //返回位置pos处的项目，该位置在视口坐标中。如果此位置有多个项目，此函数将返回最上面的项目。
	auto * item = itemAt(event->pos());

	auto menu_exec = [=](ItemType _menu_type) {
        //键值对(Nomal代表全局常态： 菜单menu_)放入FlowchartView的menus_
		if (nullptr != menus_.value(_menu_type))
            //显示菜单，使action的操作位于指定的全局位置点
			menus_.value(_menu_type)->popup(event->globalPos() + QPoint(2, 2));
	};

	auto * d_item = qgraphicsitem_cast<FlowchartGraphicsRectItem*>(item);

	if (nullptr == d_item)
		menu_exec(ItemType::Nomal);
	else
	{
		switch (d_item->GetItemType()) {

		case ItemType::Rect:
		{
            //显示菜单，使action的操作位于指定的全局位置点
			menu_exec(ItemType::Rect);
			break;
		}
		case ItemType::Link:
		{
			menu_exec(ItemType::Link);
			break;
		}
		default:;
		}
	}

	QGraphicsView::contextMenuEvent(event);
}

void FlowchartView::wheelEvent(QWheelEvent* event)
{
	// ! [1] 获取当前的鼠标所在的view坐标;
	QPoint prev_viewPos = mouse_move_point_;
    // ! [2] 获取当前鼠标相对于scene的位置;相当于item->scenePos()
	QPointF prev_scenePos = mapToScene(prev_viewPos);

	// ! [3] 计算缩放比例 缩放界面
    //scale（x，y）：按（x，y）缩放当前视图变换。
	scale(1 / view_scale_, 1 / view_scale_);
    //返回轮子旋转的距离，单位为八分之一度。正值表示轮子向前旋转（远离用户），负值表示轮子向后旋转（朝向用户）
	if (event->angleDelta().y() > 0) {
        if (view_scale_ < 5)
			view_scale_ += 0.2;
		else
            view_scale_ = 5;
	}
    else
    {
        if (view_scale_ > 0.3)
			view_scale_ -= 0.2;
		else
            view_scale_ = 0.3;
	}
	//缩放
	scale(view_scale_, view_scale_);  
	//调整scene，使得scene和view一致，主要是为了排除掉scroll
    //scene()返回指向当前在视图中可视化的场景的指针。如果当前未显示任何场景，则返回0。
	scene()->setSceneRect(mapToScene(this->rect()).boundingRect());  
	//获取缩放后的scene坐标
	QPointF scenePos = mapToScene(prev_viewPos);
	//获取缩放前后的坐标差值，即为需要进行move的位移
	QPointF disPointF = scenePos - prev_scenePos;
	//调整位置
	scene()->setSceneRect(scene()->sceneRect().x() - disPointF.x(), scene()->sceneRect().y() - disPointF.y(),
		scene()->sceneRect().width(), scene()->sceneRect().height());
	scene()->update();

}

void FlowchartView::RegistryMenu(ItemType _menu_type, QMenu * _menu)
{
    // QMap<ItemType, QMenu*>		menus_
	menus_.insert(_menu_type, _menu);
}

void FlowchartView::SetMouseModel(FlowchartCursor _type)
{
    /*
     * QCursor类提供了一个具有任意形状的鼠标光标。
        这个类主要用于创建与特定小部件相关联的鼠标光标，以及获取和设置鼠标光标的位置。
        Qt有许多标准光标形状，但您也可以基于QBitmap、遮罩和热点来制作自定义光标形状。
        要将光标与小部件关联，请使用QWidget:：setCursor（）。要将光标与所有小部件关联（通常是短时间），请使用QGuiApplication:：setOverrideCursor（）。
        要设置光标形状，请使用QCursor:：setShape（）或使用将形状作为参数的QCursor构造函数，或者可以使用Qt:：CursorShape枚举中定义的预定义光标之一。
        如果您想用自己的位图创建光标，请使用以位图和掩码为参数的QCursor构造函数，或者使用以像素图为参数的构造函数。
        要设置或获取鼠标光标的位置，请使用静态方法QCursor:：pos（）和QCursor：：setPos（）。
     */
	QCursor cursor;
	if (_type == FlowchartCursor::ArrowCursor)
	{
        //标准箭头光标
		cursor = QCursor(Qt::ArrowCursor);
	}
	else if (_type == FlowchartCursor::DrawLinkCursor)
	{
        //十字光标，通常用于帮助用户准确地选择屏幕上的点
		cursor = QCursor(Qt::CrossCursor);
	}
	else if (_type == FlowchartCursor::SizeAllCurSor)
	{
        //用于元素的光标，这些元素用于在任何方向上调整顶层窗口的大小。
		cursor = QCursor(Qt::SizeAllCursor);
	}
	else if (_type == FlowchartCursor::OpenHandCursor)
    {
        //表示张开的手的光标，通常用于画布拖动该区域以滚动。
		cursor = QCursor(Qt::OpenHandCursor);
	}
	else if (_type == FlowchartCursor::ClosedHandCursor)
	{
        // 表示闭合的手的光标，通常用于指示正在进行涉及滚动的拖动操作
		cursor = QCursor(Qt::ClosedHandCursor);
	}
    // 返回视口小部件(设置鼠标光标形状)。使用QScrollArea:：widget（）函数检索视口小部件的内容。
	viewport()->setCursor(cursor);
}
