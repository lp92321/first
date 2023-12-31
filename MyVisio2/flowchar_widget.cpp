#include "flowchar_widget.h"
#include <QDebug>
FlowCharWidget::FlowCharWidget(QWidget *parent)
	: QMainWindow(parent)
{
	InitWindow();
//	DataTest();//好像没用
	
}

FlowchartInforBases FlowCharWidget::GetFlowCharData()
{
	FlowchartInforBases datas;
    // 流程图编辑界面
	if (scene_ == nullptr)
		return datas;
	datas = scene_->FlowChart2Data();
	return datas;
}

void FlowCharWidget::SetFlowChartData(FlowchartInforBases _datas)
{

}

void FlowCharWidget::InitWindow()
{
    // 创建流程图编辑界面场景，创建FlowchartView *widget_，将场景附在widget_上
	scene_ = new FlowchartScene();
    // 中心窗口为widget_
    this->setCentralWidget(scene_->GetFlowcharView());
	// 左侧形状菜单
    tool_side_bar_ = new FlowCharToolSideBar(this);
    // 将给定的dockwidget(tool_side_bar_)添加到指定区域LeftDockWidgetArea
    this->addDockWidget(Qt::LeftDockWidgetArea, tool_side_bar_);
    // 顶层工具栏(控场):(其中的场景指针、左工具指针都指向了实际界面的指针)
    tool_button_bar_ = new FlowCharToolButtonBar(this);
    // 顶层工具栏中的FlowchartScene* scene_  = 编辑界面场景scene_（注释后点击工具栏按钮崩溃）
    tool_button_bar_->SetFlowchartScene(scene_);
    // 顶层工具栏的FlowCharToolSideBar* side_bar_ = 左侧形状菜单tool_side_bar_（注释后点击工具栏按钮崩溃）
    tool_button_bar_->SetToolSideBar(tool_side_bar_);
    // 添加toolbar到主窗口
    this->addToolBar(tool_button_bar_);

    // （main_widget好像注释后无影响，待确认）
    QWidget *main_widget = new QWidget();
    QHBoxLayout *main_layout = new QHBoxLayout();
    // 窗口垂直布局
    main_widget->setLayout(main_layout);
    main_widget->setStyleSheet("QWidget{background:yellow}");
	this->resize(1400, 800);
}

void FlowCharWidget::DataTest()
{
}

//*******************************工具栏************************************
FlowCharToolButtonBar::FlowCharToolButtonBar(QWidget *parent /*= Q_NULLPTR*/)
	: QToolBar(parent)
{
	InitWidget();
}

FlowCharToolButtonBar::~FlowCharToolButtonBar()
{

}

void FlowCharToolButtonBar::SetFlowchartScene(FlowchartScene* _scene)
{
    // 将ToolButtonBar内部元素指向流程图编辑界面
	scene_ = _scene;
}

void FlowCharToolButtonBar::SetToolSideBar(FlowCharToolSideBar* _side_bar)
{
	side_bar_ = _side_bar;
}

void FlowCharToolButtonBar::InitWidget()
{
    // 是否将工具栏作为独立窗口进行拖放
    setFloatable(false);
    // 是否可以在工具栏区域内或在工具栏区域之间移动工具栏
    setMovable(false);
    // 拖动顶部工具栏能放置的位置，未设置时可放任意位置
	setAllowedAreas(Qt::TopToolBarArea | Qt::RightToolBarArea);
    //工具按钮的样式，描述如何显示按钮的文本和图标:文本显示在图标旁边
    this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);//Qt::ToolButtonTextUnderIcon

    file_save_ = new QAction(QIcon(QPixmap(":/picture/4.png")), QStringLiteral("保存"));
//	file_save_ = new QAction(QPixmap(":/new/prefix1/Resources/FileSave.png"), QStringLiteral("保存"));
    file_read_ = new QAction(QPixmap(":/picture/3.jpg"), QStringLiteral("打开"));
    item_type_ = new QAction(QPixmap(":/picture/1.jpg"), QStringLiteral("形状(显示图元)"));
    mouse_status_nomal_ = new QAction(QPixmap(":/picture/2.jpg"), QStringLiteral("指针工具(箭头)"));
    mouse_status_link_ = new QAction(QPixmap(":/picture/6.jpg"),QStringLiteral("连接线（十字）"));

	addAction(file_save_);
	addAction(file_read_);
	addAction(item_type_);
	addAction(mouse_status_nomal_);
	addAction(mouse_status_link_);

	connect(file_save_, SIGNAL(triggered()), this, SLOT(FlowActionSlot()));
	connect(file_read_, SIGNAL(triggered()), this, SLOT(FlowActionSlot()));
	connect(item_type_, SIGNAL(triggered()), this, SLOT(FlowActionSlot()));
	connect(mouse_status_nomal_, SIGNAL(triggered()), this, SLOT(FlowActionSlot()));
	connect(mouse_status_link_, SIGNAL(triggered()), this, SLOT(FlowActionSlot()));
}

void FlowCharToolButtonBar::FileSave()
{
    // ToolButtonBar内部元素
	if (scene_ == nullptr)
		return;
    qDebug()<<"save file ing==";
    //图元结构体向量FlowchartInforBases datas;
	datas = scene_->FlowChart2Data();
//	for (auto data_one : datas)
//	{
//	}
}

void FlowCharToolButtonBar::FileRead()
{
    // ToolButtonBar内部元素
	if (scene_ == nullptr)
		return;
	scene_->Data2FlowChart(datas);
}

void FlowCharToolButtonBar::FlowActionSlot()
{
	QAction *signal_action = (QAction*)sender();
    // 指针工具
	if (signal_action == mouse_status_nomal_){
        // 场控的场景变量
		if (scene_ == nullptr)
			return;
        // 设置鼠标绘制方式：标准箭头光标
		scene_->SetSceneMode(SceneMode::MoveItem);
    }//连接线
	else if (signal_action == mouse_status_link_){
		if (scene_ == nullptr)
			return;
        // 设置鼠标绘制方式：划线十字光标
		scene_->SetSceneMode(SceneMode::DrawLineItem);
    }//形状作用：显示左侧步骤栏
	else if (signal_action == item_type_){
		if (side_bar_ == nullptr)
			return;
        // 场控的左侧形状菜单变量
		side_bar_->show();
    }// 文件保存
	else if (signal_action == file_save_) {
		FileSave();
    }// 打开文件
	else if (signal_action == file_read_) {
		FileRead();
	}
	return;
}

//*******************************左侧工具栏************************************
FlowCharToolSideBar::FlowCharToolSideBar(QWidget *parent /*= Q_NULLPTR*/)
	: QDockWidget(parent)
{
	InitWidget();
}

FlowCharToolSideBar::~FlowCharToolSideBar()
{

}

void FlowCharToolSideBar::SetFlowchartScene(FlowchartScene* _scene)
{
    // ToolSideBar内部元素指向流程图编辑界面
	scene_ = _scene;
}

void FlowCharToolSideBar::InitWidget()
{
    // 拖动左工具时：推窗时能放置的位置，不设置时可放任意位置
    //不可拖动时这里设置的位置无效
    setAllowedAreas(Qt::RightDockWidgetArea);
    setWindowTitle(QStringLiteral("形状"));
	setMinimumWidth(270);

    QWidget *main_widget = new QWidget();
    QVBoxLayout *main_layout = new QVBoxLayout();
    main_widget->setLayout(main_layout);
    this->setWidget(main_widget);
//    main_widget->setStyleSheet("QWidget{background:yellow}");

    //左推窗类中map：QMap<std::string, QPushButton*> vec_btn_;
	vec_btn_.insert("流程", new QPushButton(QStringLiteral("流程")));
	vec_btn_.insert("判定", new QPushButton(QStringLiteral("判定")));
	vec_btn_.insert("自循环", new QPushButton(QStringLiteral("自循环"))); 
	for (QMap<std::string, QPushButton*>::iterator iter = vec_btn_.begin(); iter != vec_btn_.end(); iter++){
		main_layout->addWidget(iter.value());
		connect(iter.value(), SIGNAL(pressed()), this, SLOT(FlowButtonSlot()));
	}
    //将最小大小和拉伸因子拉伸为零的可拉伸空间（QSpacerItem）添加到此方框布局的末尾
    main_layout->addStretch();
	/*main_layout->addWidget(new QAction(""));*/
}

void FlowCharToolSideBar::InitData()
{

}

void FlowCharToolSideBar::FlowButtonSlot()
{
    // QDrag类提供了对基于MIME的拖放数据传输的支持
	QDrag *drag = new QDrag(this);
    /*
    QMimeData类为记录其MIME类型信息的数据提供了一个容器。
    QMimeData用于描述可以存储在剪贴板中并通过拖放机制传输的信息。
    QMimeData对象将它们所保存的数据与相应的MIME类型相关联，
    以确保信息可以在应用程序之间安全地传输，并在同一应用程序中四处复制。
    QMimeData对象通常使用新的创建，并提供给QDrag或QClipboard对象。
    这是为了使Qt能够管理他们使用的内存。
    单个QMimeData对象可以同时使用几种不同的格式存储相同的数据。
    formats（）函数按首选项的顺序返回可用格式的列表。
    data（）函数返回与MIME类型相关联的原始数据，
    setData（）允许您设置MIME类型的数据。
     */
	QMimeData *data = new QMimeData();
	data->clear();
	QPushButton *clicked_btn = (QPushButton*)sender();
	if (clicked_btn == vec_btn_["流程"]){
        data->setText(QStringLiteral("流程"));//在编译阶段生成字符数据,运行时不会发生转换或分配,提升运行效率
//        data->setText("流程");//此方法涉及到内存分配和将数据复制/转换为QString的内部编码,低效率
	}
	else if (clicked_btn == vec_btn_["判定"]){
		data->setText(QStringLiteral("判定"));
	}
	else if (clicked_btn == vec_btn_["自循环"]){
		data->setText(QStringLiteral("自循环"));
	}
	drag->setMimeData(data);
    /*
     * 启动拖放操作，并在拖放操作完成时返回一个值，指示请求的拖放操作。用户可以从supportedActions中指定选择的操作。默认的建议操作将按以下顺序从允许的操作中选择：移动、复制和链接。
        注意：在Linux和macOS上，拖放操作可能需要一些时间，但此功能不会阻止事件循环。在执行操作时，其他事件仍会传递到应用程序。在Windows上，Qt事件循环在操作过程中被阻止。
     *
     */
	drag->exec(Qt::MoveAction);
}
