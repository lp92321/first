#include "flowchart_scene.h"
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QMetaEnum>
#include <QKeyEvent>
#include <QMimeData>
#include <QDebug>
FlowchartScene::FlowchartScene(QObject* parent)
	: QGraphicsScene(parent)
	, widget_(new FlowchartView(this))
	, item_temp_(nullptr)
	, item_hline_(nullptr)
	, item_vline_(nullptr)
{
    // 初始化编辑框（焦点事件改名事件处理）
    InitLineEdit();
    // 创建菜单
    InitMenu();

    // 此属性设置是否为widget_启用了丢弃事件
    //场景中的FlowchartView     *widget_;
	widget_->setAcceptDrops(true);
    // widget_安装事件筛选器this，但是类中没有重写事件筛选器，安装了也没用
//	widget_->installEventFilter(this);


	// ! [todo..] 防止连线移动到图外不刷新的bug
	/*connect(this, &QGraphicsScene::sceneRectChanged, this, [this](const QRectF area) {

		if (sceneRect().width() != itemsBoundingRect().width() ||
			sceneRect().height() != itemsBoundingRect().height())
		{
			auto area_items = items();
			for (auto area_item : area_items)
			{
				if (QGraphToFlow(area_item)->GetItemType() == ItemType::Link) {
					FlowcharGraphicsLink* update_link = (FlowcharGraphicsLink*)area_item;
					update_link->UpdateLineArrow();
					qDebug() << "line updata";
				}
			}
			setSceneRect(itemsBoundingRect());
		}
		});*/
}

FlowchartScene::~FlowchartScene()
{
}

// 获取视口对象
FlowchartView* FlowchartScene::GetFlowcharView()
{
	if (widget_ == nullptr) {
		widget_ = new FlowchartView(this);
		return widget_;
	}
	return widget_;
}

// 设置鼠标绘制方式
void FlowchartScene::SetSceneMode(SceneMode _model)
{
	scene_mode_ = _model;
	if (_model == SceneMode::MoveItem) {
		widget_->SetMouseModel(FlowchartCursor::ArrowCursor);
	}
	else if (_model == SceneMode::DrawLineItem) {
		widget_->SetMouseModel(FlowchartCursor::DrawLinkCursor);
	}
	widget_->update();
}

// 流程图生成结构体集合
FlowchartInforBases FlowchartScene::FlowChart2Data()
{
	// ! [1] 获取流程图所有图元对象
	FlowchartInforBases datas;
	auto area_items = items();

	// ! [2] 获取所有流程图信息
	for (auto area_item : area_items) {
		datas.push_back(QGraphToFlow(area_item)->GetItemInformation());
	}
	return datas;
}

// 结构体集合生成流程图
void FlowchartScene::Data2FlowChart(FlowchartInforBases _datas)
{
	// ! [1] 绘制图元
    for (auto data_one : _datas)
    {
        // 判断结构体数据类型
        ItemType type = data_one->item_type_.type;
        if (type != ItemType::Link) {
            FlowchartGraphicsItem* item_oen = AddChildItem(data_one, type);
            if (item_oen==nullptr)
                continue;
            //临时当前所有图元QMap<QString, FlowchartGraphicsItem*> temp_items_;
            temp_items_.insert(item_oen->GetItemId(), item_oen);
        }
    }

	// ! [2] 绘制连线
    for (auto data_one : _datas)
    {
        // 判断结构体数据类型
        ItemType type = data_one->item_type_.type;
        if (type == ItemType::Link) {
            FlowchartGraphicsItem* item_oen = AddChildItem(data_one, type);
            if (item_oen == nullptr)
                continue;
            temp_items_.insert(item_oen->GetItemId(), item_oen);
        }
    }

	temp_items_.clear();
}

// 添加一个图元
FlowchartGraphicsItem* FlowchartScene::AddChildItem(FlowchartInforBase* _item_infor, ItemType _type)
{
	FlowchartGraphicsItem* flowchar_item = nullptr;
	if (_type == ItemType::Null)
	{
	}
	else if (_type == ItemType::Nomal)
    {// 代表全局常态
	}
	else if (_type == ItemType::Link)
	{
		// 此处逻辑只允许在读取文件生成流程图时使用，其余均为界面拖动绘制连线
		// ! [1] 获取父子节点
		FlowcharGraphicsLinkInfo* item_infor = (FlowcharGraphicsLinkInfo*)_item_infor;
        //临时当前所有图元QMap<QString, FlowchartGraphicsItem*> temp_items_;
        FlowchartGraphicsItem* start_item = temp_items_[item_infor->start_item_id_];
		FlowchartGraphicsItem* end_item = temp_items_[item_infor->end_item_id_];
		if (end_item == nullptr)
			return flowchar_item;
		// ! [2] 添加连线
		FlowcharGraphicsLink* item_link = new FlowcharGraphicsLink(start_item, end_item, item_infor);
		this->addItem(item_link);
		// ! [3] 连线两端父子节点相互添加
		start_item->AddChild(end_item, item_link);
		end_item->AddFather(start_item, item_link);
		// ! [3] 更新连线位置
		item_link->UpdateLineArrow();
		flowchar_item = (FlowchartGraphicsItem*)item_link;
	}
	else if (_type == ItemType::Rect)
	{
		// 流程
//        FlowchartItemRectInfo* aa = (FlowchartItemRectInfo*)_item_infor;//好像无用
		FlowchartGraphicsRectItem* item_rect = new FlowchartGraphicsRectItem((FlowchartItemRectInfo*)_item_infor);
		this->addItem(item_rect);
		flowchar_item = (FlowchartGraphicsItem*)item_rect;
	}
	else if (_type == ItemType::Condition)
	{
		// 判定
		FlowchartGraphicsConditionItem* item_condition = new FlowchartGraphicsConditionItem((FlowchartItemConditionInfo*)_item_infor);
		this->addItem(item_condition);
		flowchar_item = (FlowchartGraphicsItem*)item_condition;
	}
	else if (_type == ItemType::Circulation)
	{
		// 自循环
		FlowchartGraphicsCirculationItem* item_circulation = new FlowchartGraphicsCirculationItem((FlowchartItemCirculationInfo*)_item_infor);
		FlowcharGraphicsLink* item_link = new FlowcharGraphicsLink(item_circulation);
		this->addItem(item_link);
		item_circulation->SetCirculationLink(item_link);
		this->addItem(item_circulation);
		flowchar_item = (FlowchartGraphicsItem*)item_circulation;
	}
	return flowchar_item;
}

// 添加图元连接线
FlowcharGraphicsLink* FlowchartScene::AddStartToEndItem(FlowchartGraphicsItem* _start_item, FlowchartGraphicsItem* _end_item)
{
	// ! [1] 添加连线
	FlowcharGraphicsLink* item = new FlowcharGraphicsLink(_start_item, _end_item);
	this->addItem(item);

	// ! [2] 连线两端父子节点相互添加
	_start_item->AddChild(_end_item, item);
	_end_item->AddFather(_start_item, item);

	// ! [3] 更新连线位置
	item->UpdateLineArrow();
	return item;
}

// 对象转化	qt原生对象->图元基类对象
FlowchartGraphicsItem* FlowchartScene::QGraphToFlow(QGraphicsItem* _item)
{
	if (_item == nullptr)
		return nullptr;
	FlowchartGraphicsRectItem* item = (FlowchartGraphicsRectItem*)_item;
	return (FlowchartGraphicsItem*)item;
}

// 对象转化	图元基类对象->qt原生对象
QGraphicsItem* FlowchartScene::FlowToQGraph(FlowchartGraphicsItem* _item)
{
	if (_item == nullptr)
		return nullptr;
	FlowchartGraphicsRectItem* item = (FlowchartGraphicsRectItem*)_item;
	return (QGraphicsItem*)item;
}

// 创建菜单
void FlowchartScene::InitMenu()
{
    // ! [1] 一级菜单，用于部件右键事件菜单选择
	menu_ = new QMenu(widget_);
#if 0

	QAction* add_item_action = new QAction(QStringLiteral("添加图元"));
	QAction* del_item_action = new QAction(QStringLiteral("删除图元"));
	menu_->addAction(add_item_action);
	menu_->addAction(del_item_action);

	// ! [2] 二级菜单
	// 添加图元
	QMenu* add_item_menu = new QMenu();
	add_item_action->setMenu(add_item_menu);
	QAction* add_item_rect = new QAction(QStringLiteral("流程"));
	QAction* add_item_condition = new QAction(QStringLiteral("判定"));
	QAction* add_item_circulation = new QAction(QStringLiteral("自循环"));
	// ! [todo..] 其他图形
	add_item_menu->addAction(add_item_rect);
	add_item_menu->addAction(add_item_condition);
	add_item_menu->addAction(add_item_circulation);

	// ! [3] 菜单槽函数绑定
	// 添加矩形
	connect(del_item_action, SIGNAL(triggered()), this, SLOT(DeleteItem()));
	connect(add_item_rect, SIGNAL(triggered()), this, SLOT(AddItemSlot()));
	connect(add_item_condition, SIGNAL(triggered()), this, SLOT(AddItemSlot()));
	connect(add_item_circulation, SIGNAL(triggered()), this, SLOT(AddItemSlot()));
#endif

	// ! [4] 添加菜单
    // 键值对(Nomal代表全局常态： 菜单menu_)放入FlowchartView的menus_，用于部件右键事件菜单选择
    //flowchart_scene中的FlowchartView	  *widget_;
	widget_->RegistryMenu(ItemType::Nomal, menu_);
	return;
}

// 初始化编辑框
void FlowchartScene::InitLineEdit()
{
    // 图元名字编辑框
	input_widget_ = new CutomTextWdit(widget_);
	QStringList qss;
//    qss.append("QTextEdit{background:rgba(255,255,255,200); border:1px solid rgb(0,0,0);}");
    qss.append("QTextEdit{background:rgba(255,0,0,200); border:1px solid rgb(0,255,0);}");
	input_widget_->setStyleSheet(qss.join(""));
//    input_widget_->hide();// 为什么隐藏
    //焦点离开事件触发信号和槽
	connect(input_widget_, SIGNAL(FocusOutSignal(QString)), this, SLOT(FocusOutSlot(QString)));
}

// 绘制磁吸线 水平
void FlowchartScene::MagneticHLine()
{
	// 删除上次磁吸线 水平
	if (item_hline_) { removeItem(item_hline_); delete item_hline_; item_hline_ = nullptr; }

	// 水平磁吸线
	// 磁吸线判断区域 判断区域越小越好，最好所有在区域内的图元都在同一水平线上，这样相近的两个图元不会互相干扰
	QRectF magarea(QPointF(this->sceneRect().left(), mouse_move_point_.y() - 1), QSizeF(this->width(), 1));
	auto area_items = items(magarea);
	// 去除掉自己
	if (area_items.contains(item_temp_)) {
		area_items.removeOne(item_temp_);
	}
	// 去除垂直磁吸线
	if (area_items.contains(item_vline_)) {
		area_items.removeOne(item_vline_);
	}
	// 去掉连线
	for (auto area_item : area_items)
	{
		if (QGraphToFlow(area_item)->GetItemType() == ItemType::Link) {
			area_items.removeOne(area_item);
		}

	}
	// 判断区域内有无图元
	if (!area_items.isEmpty())
	{
		for (auto i : area_items)
		{
			// 根据鼠标y坐标和另一个图元判断，水平对齐
			float y = mouse_move_point_.y();
			float ny = i->sceneBoundingRect().center().y();
			if (y > ny - 25 && y < ny + 25)
			{
				if (item_hline_) { removeItem(item_hline_); delete item_hline_; item_hline_ = nullptr; }
				QPointF temp_point = QPointF(item_temp_->sceneBoundingRect().center().x(), ny) - item_temp_->boundingRect().center();
				item_temp_->setPos(temp_point);
				//画线
				item_hline_ = new QGraphicsLineItem(QLineF(item_temp_->sceneBoundingRect().center(), i->sceneBoundingRect().center()));
				item_hline_->setPen(QPen(Qt::DashLine));
				addItem(item_hline_);
			}
		}
	}
}

// 绘制磁吸线 垂直
void FlowchartScene::MagneticVLine()
{
	// 删除上次磁吸线 垂直
	if (item_vline_) { removeItem(item_vline_); delete item_vline_; item_vline_ = nullptr; }

	// 水平磁吸线
	// 磁吸线判断区域 判断区域越小越好，最好所有在区域内的图元都在同一水平线上，这样相近的两个图元不会互相干扰
	QRectF magarea(QPointF(mouse_move_point_.x() - 1, this->sceneRect().top()), QSizeF(1, this->height()));
	auto area_items = items(magarea);
	// 去除掉自己
	if (area_items.contains(item_temp_)) {
		area_items.removeOne(item_temp_);
	}
	// 去除水平磁吸线
	if (area_items.contains(item_hline_)) {
		area_items.removeOne(item_hline_);
	}
	// 去掉连线
	for (auto area_item : area_items)
	{
		if (QGraphToFlow(area_item)->GetItemType() == ItemType::Link) {
			area_items.removeOne(area_item);
		}

	}
	// 判断区域内有无图元
	if (!area_items.isEmpty())
	{
		for (auto i : area_items)
		{
			// 根据鼠标y坐标和另一个图元判断，水平对齐
			float x = mouse_move_point_.x();
			float nx = i->sceneBoundingRect().center().x();
			if (x > nx - 25 && x < nx + 25)
			{
				if (item_vline_) { removeItem(item_vline_); delete item_vline_; item_vline_ = nullptr; }
				QPointF temp_point = QPointF(nx, item_temp_->sceneBoundingRect().center().y()) - item_temp_->boundingRect().center();
				item_temp_->setPos(temp_point);
				//画线
				item_vline_ = new QGraphicsLineItem(QLineF(item_temp_->sceneBoundingRect().center(), i->sceneBoundingRect().center()));
				item_vline_->setPen(QPen(Qt::DashLine));
				addItem(item_vline_);
			}
		}
	}
}

// 删除图元
void FlowchartScene::DeleteItem()
{
	// ! [1] 首次遍历删除图元及其相关连线
	auto graphics_items = this->selectedItems();
	// 获取非连线图元
	QList<QGraphicsItem*> flow_items;
	for (QGraphicsItem* item : graphics_items) {
		if (QGraphToFlow(item)->GetItemType() != ItemType::Link)
            //添加选中的非连线图元
			flow_items.push_back(item);
	}
	for (QGraphicsItem* item : flow_items)
	{
		if (item != nullptr)
		{
			// 从场景移除该图元
			this->removeItem(item);
			// 清理该图元依赖关系
			QGraphToFlow(item)->ItemClear();
			// 释放该图元内存
			delete item;
			item = nullptr;
		}
	}
	// ! [2] 遍历剩余独立连线
	graphics_items.clear();
	graphics_items = this->selectedItems();
	for (QGraphicsItem* item : graphics_items)
	{
		FlowcharGraphicsLink* item_link = (FlowcharGraphicsLink*)item;
		// （筛选条件）自循环连线不可单独单独删除
		if (item_link != nullptr && item_link->GetEndItem() != nullptr)
		{
			this->removeItem(item_link);
			item_link->ItemClear();
			delete item_link;
			item_link = nullptr;
		}
	}
}

void FlowchartScene::FocusOutSlot(QString _text)
{
    //此函数功能：修改部件的名字
    // 当前选中部件
//    qDebug()<<"textedit焦点离开事件槽函数触发====";
	QList<QGraphicsItem*> graphics_items = this->selectedItems();
	if (graphics_items.size() == 0) {
		input_widget_->hide();
		return;
	}
    //qt原生对象 转化为 图元基类对象，并赋值给FlowchartGraphicsItem
	FlowchartGraphicsItem* select_item = QGraphToFlow(graphics_items.at(0));
    //设置图元内容（工具提示不设置）
	select_item->SetText(_text);
//    qDebug()<<"textedit焦点离开事件设置图元内容完毕====";
    input_widget_->hide();
}

void FlowchartScene::drawBackground(QPainter* painter, const QRectF& rect)
{
	// ! [1] 绘制背景色
	painter->save();
	painter->fillRect(rect, QColor(255, 255, 255));
	painter->restore();
}

void FlowchartScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
	// ! [1] 获取点击对象
	QList<QGraphicsItem*> graphics_items = this->selectedItems();
	if (graphics_items.size() != 1)
		return;
	FlowchartGraphicsItem* select_item = QGraphToFlow(graphics_items.at(0));
	if (select_item->GetItemType() == ItemType::Rect)
	{
		// 获取流程矩形
		FlowchartGraphicsRectItem* rect_item = (FlowchartGraphicsRectItem*)select_item;
		// 获取视口中item矩形
		QPointF item_left_top_scene = QPointF(rect_item->GetLeftPoint().x(), rect_item->GetTopPoint().y());
		QPointF item_right_bottom_scene = QPointF(rect_item->GetRightPoint().x(), rect_item->GetBottomPoint().y());
        //场景类中的FlowchartView	  *widget_;	视口
        //将场景坐标点返回到视口坐标。
        QPointF item_left_top = widget_->mapFromScene(item_left_top_scene);
		QPointF item_right_bottom = widget_->mapFromScene(item_right_bottom_scene);
		QRectF item_rect = QRectF(item_left_top, item_right_bottom);
        //CutomTextWdit  *input_widget_;	 // 图元编辑框（修改图元名字的TextEdit框）
		input_widget_->setFixedSize(item_rect.width(), item_rect.height());
		input_widget_->move(item_left_top.toPoint());
//		QRectF rect_temp = rect_item->boundingRect();
		input_widget_->setFixedSize(item_rect.width(), item_rect.height());
		input_widget_->move(item_left_top.toPoint());

		input_widget_->show();

		QString item_text = rect_item->GetItemInformation()->item_content_.content_;
		rect_item->SetText("");
        //文本编辑器的文本设置
		input_widget_->setText(item_text);
	}
	if (select_item->GetItemType() == ItemType::Link)
	{
		// 获取连线对象
		FlowcharGraphicsLink* link_item = (FlowcharGraphicsLink*)select_item;
		// 获取视口中item矩形
		QPointF item_left_top_scene = link_item->GetTextRect().topLeft();
		QPointF item_right_bottom_scene = link_item->GetTextRect().bottomRight();
		QPointF item_left_top = widget_->mapFromScene(item_left_top_scene);
		QPointF item_right_bottom = widget_->mapFromScene(item_right_bottom_scene);
		QRectF item_rect = QRectF(item_left_top, item_right_bottom);
		input_widget_->setFixedSize(item_rect.width() + 5, item_rect.height());
		input_widget_->move(item_left_top.toPoint());
		input_widget_->show();

		QString item_text = link_item->GetItemInformation()->item_content_.content_;
		link_item->SetText("");
		input_widget_->setText(item_text);
	}
	if (select_item->GetItemType() == ItemType::Condition)
	{
		// 获取流程矩形
		FlowchartGraphicsConditionItem* codition_item = (FlowchartGraphicsConditionItem*)select_item;
		// 获取视口中item矩形
		QPointF item_left_top_scene = QPointF(codition_item->GetLeftPoint().x(), codition_item->GetTopPoint().y());
		QPointF item_right_bottom_scene = QPointF(codition_item->GetRightPoint().x(), codition_item->GetBottomPoint().y());
		QPointF item_left_top = widget_->mapFromScene(item_left_top_scene);
		QPointF item_right_bottom = widget_->mapFromScene(item_right_bottom_scene);
		QRectF item_rect = QRectF(item_left_top, item_right_bottom);
		input_widget_->setFixedSize(item_rect.width(), item_rect.height());
		input_widget_->move(item_left_top.toPoint());
		input_widget_->show();

		QString item_text = codition_item->GetItemInformation()->item_content_.content_;
		codition_item->SetText("");
		input_widget_->setText(item_text);
	}
	if (select_item->GetItemType() == ItemType::Circulation)
	{
		// 获取流程矩形
		FlowchartGraphicsCirculationItem* codition_item = (FlowchartGraphicsCirculationItem*)select_item;
		// 获取视口中item矩形
		QPointF item_left_top_scene = QPointF(codition_item->GetLeftPoint().x(), codition_item->GetTopPoint().y());
		QPointF item_right_bottom_scene = QPointF(codition_item->GetRightPoint().x(), codition_item->GetBottomPoint().y());
		QPointF item_left_top = widget_->mapFromScene(item_left_top_scene);
		QPointF item_right_bottom = widget_->mapFromScene(item_right_bottom_scene);
		QRectF item_rect = QRectF(item_left_top, item_right_bottom);
		input_widget_->setFixedSize(item_rect.width(), item_rect.height());
		input_widget_->move(item_left_top.toPoint());
		input_widget_->show();

		QString item_text = codition_item->GetItemInformation()->item_content_.content_;
		codition_item->SetText("");
		input_widget_->setText(item_text);
	}
	input_widget_->setFocus();

}

void FlowchartScene::keyPressEvent(QKeyEvent* event)
{
    //ESC按键：设置鼠标标准状态，取消选中
	if (event->key() == Qt::Key_Escape) {
		SetSceneMode(SceneMode::MoveItem);
        //清除当前选中状态。
        this->clearSelection();
	}
    //Delete键：删除选中的部件
	if (event->key() == Qt::Key_Delete) {
		DeleteItem();
	}
	return QGraphicsScene::keyPressEvent(event);
}

void FlowchartScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) {
		switch (scene_mode_) {
		case SceneMode::DrawLineItem:
		{
			temp_line_ = new QGraphicsLineItem(QLineF(event->scenePos(), event->scenePos()));
			temp_line_->setPen(QPen(QColor(89, 152, 209), 3));
			addItem(temp_line_);
			break;
		}
		case SceneMode::MoveItem:
		{
			// 降序获取，第一个为顶层图元
			auto select_items = this->items(event->scenePos(), Qt::IntersectsItemShape, Qt::DescendingOrder);
			if (select_items.isEmpty())
				break;
			// 获取当前点击图元
			item_temp_ = select_items.at(0);
			// 区分是否为连线
			if (QGraphToFlow(item_temp_)->GetItemType() == ItemType::Link) {
				item_temp_ = nullptr;
				break;
			}
			// 移动图元时，保证鼠标位于图元中心点
			item_temp_->setPos(mouse_move_point_ - item_temp_->boundingRect().center());
			break;
		}
		default:;
		}
	}
	else if (event->button() == Qt::RightButton) {
		mouse_pressed_point_ = event->scenePos();
	}
	return QGraphicsScene::mousePressEvent(event);
}

void FlowchartScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
	// ! [1] 实时获取鼠标位置
	mouse_move_point_ = event->scenePos();

	// ! [2] 绘制临时线段 连接两图元的动态效果
	if (scene_mode_ == SceneMode::DrawLineItem
		&& nullptr != temp_line_) {
		// 如果临时线段不为空的时候，将press事件中的点和和移动的位置点进行连接，更新临时线
		QLineF temp_line(temp_line_->line().p1(), event->scenePos());
		temp_line_->setLine(temp_line);
	}

	// ! [3] 鼠标拖动图元移动
	else if (scene_mode_ == SceneMode::MoveItem)
	{
		// ! [4] 更新连线
		auto area_items = items();
		// 去除垂直磁吸线
		if (area_items.contains(item_vline_)) {
			area_items.removeOne(item_vline_);
		}
		// 去除水平磁吸线
		if (area_items.contains(item_hline_)) {
			area_items.removeOne(item_hline_);
		}
		for (auto area_item : area_items)
		{
			if (QGraphToFlow(area_item)->GetItemType() == ItemType::Link) {
				FlowcharGraphicsLink* update_link = (FlowcharGraphicsLink*)area_item;
				// 判断连线位置是否正确，位置异常再刷新，减少内存消耗
				QPointF point_s = update_link->GetStartPointF();
				QPointF point_e = update_link->GetEndPointF();
				QPointF point_item_s = update_link->GetStartItem()->GetCenterPoint();
				if ((point_s.x() != point_item_s.x() && point_s.y() != point_item_s.y()) || 
					(point_e.x() != point_item_s.x() && point_e.y() != point_item_s.y()))
					update_link->UpdateLineArrow();
			}
		}
		QGraphicsScene::mouseMoveEvent(event);

		// ! [5] 绘制磁吸线
		// 判断选中多个不走磁吸线逻辑
		if (selectedItems().size() > 1) {
			return;
		}
		if (item_temp_ == nullptr) {
			return;
		}
		// 移动图元时，保证鼠标位于图元中心点
		item_temp_->setPos(mouse_move_point_ - item_temp_->boundingRect().center());
		// 水平磁吸线
		MagneticHLine();
		// 垂直磁吸线
		MagneticVLine();

	}

	

}

void FlowchartScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{

	if (temp_line_ != nullptr && scene_mode_ == SceneMode::DrawLineItem) {

		// ! [1] 移除线段的开始点p1和结束点p2处的线段item，QGraphicsItem
		QList<QGraphicsItem*> startItems = items(temp_line_->line().p1());
		if (!startItems.isEmpty() && startItems.first() == temp_line_)
			startItems.removeFirst();
		QList<QGraphicsItem*> endItems = items(temp_line_->line().p2());
		if (!endItems.isEmpty() && endItems.first() == temp_line_)
			endItems.removeFirst();

		// ! [2] 将临时线段删除，以获取开始和结束Item，临时线段已经无用
		removeItem(temp_line_);
		delete temp_line_;
		temp_line_ = nullptr;

		// ! [3] 根据开始和结束item重新绘制带箭头的连接线
		if (!startItems.isEmpty() && !endItems.isEmpty() && startItems.first() != endItems.first()) {
			FlowcharGraphicsLink* link_item = AddStartToEndItem(QGraphToFlow(startItems.first()), QGraphToFlow(endItems.first()));
		}
	}
	if (item_temp_ != nullptr && scene_mode_ == SceneMode::MoveItem) {

		// ! [1]  临时图元置空
		item_temp_ = nullptr;

		// ! [2] 磁吸线删除
		if (item_hline_) { removeItem(item_hline_); delete item_hline_; item_hline_ = nullptr; }
		if (item_vline_) { removeItem(item_vline_); delete item_vline_; item_vline_ = nullptr; }
	}
	return QGraphicsScene::mouseReleaseEvent(event);
}
// 鼠标拖拽进入控件时，会触发该事件
// 判断是否接收数据
void FlowchartScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
	// ! [1] 判断是否接收数据
	if (event->source() == widget_) {
		event->ignore();
		return QGraphicsScene::dragEnterEvent(event);
	}
	flow_item_temp_ = nullptr;
	event->accept();

}

void FlowchartScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	if (flow_item_temp_ == nullptr) {
		// ! [1] 创建对应图元类型
		QString item_type = event->mimeData()->text();
		FlowchartInforBase* item_infor = new FlowchartInforBase();
		if (item_type.compare(QStringLiteral("流程")) == 0)
		{
			item_infor->item_content_.content_ = QStringLiteral("算法流程");
			flow_item_temp_ = AddChildItem(item_infor, ItemType::Rect);
		}
		if (item_type.compare(QStringLiteral("判定")) == 0)
		{
			item_infor->item_content_.content_ = QStringLiteral("判定");
			flow_item_temp_ = AddChildItem(item_infor, ItemType::Condition);
		}
		if (item_type.compare(QStringLiteral("自循环")) == 0)
		{
			item_infor->item_content_.content_ = QStringLiteral("自循环");
			flow_item_temp_ = AddChildItem(item_infor, ItemType::Circulation);
		}
	}
	if (flow_item_temp_ == nullptr)
		return;
	FlowToQGraph(flow_item_temp_)->setPos(event->scenePos().toPoint());
}

void FlowchartScene::dragLeaveEvent(QGraphicsSceneDragDropEvent* event)
{
	if (flow_item_temp_ != nullptr)
	{
		QGraphicsItem* item = FlowToQGraph(flow_item_temp_);
		this->removeItem(item);
		delete item;
		item = nullptr;
	}
}

void FlowchartScene::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	if (event->source() == widget_) {
		event->ignore();
		return QGraphicsScene::dragEnterEvent(event);
	}
	flow_item_temp_ = nullptr;
	event->acceptProposedAction();
}

