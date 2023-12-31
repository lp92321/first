#pragma once

#include <QtWidgets/QMainWindow>
#include <QUuid>
#include <QHBoxLayout>
#include <QToolBar>
#include <QWidget>
#include <QLabel>
#include <QStackedWidget>
#include <QDockWidget>
#include <QToolButton>
#include <QMap>
#include <QDrag>
#include <QMimeData>
#include "flowchart_scene.h"
#include "flowchart_view.h"
#include "flowchar_arrow_button.h"

// 流程图编辑框
class FlowCharToolButtonBar;
class FlowCharToolSideBar;
class FlowCharWidget : public QMainWindow
{
	Q_OBJECT

public:

	FlowCharWidget(QWidget *parent = Q_NULLPTR);

	// 获取流程图数据
	FlowchartInforBases GetFlowCharData();

	// 设置流程图数据
	void SetFlowChartData(FlowchartInforBases _datas);

private:

	// 初始化窗体
	void InitWindow();

	void DataTest();


private:

	FlowchartScene*					scene_;							// 流程图编辑界面
	FlowCharToolSideBar*			tool_side_bar_;					// 左侧形状菜单
	FlowCharToolButtonBar*			tool_button_bar_;				// 顶层工具栏
};


//*******************************工具栏************************************
class FlowCharToolButtonBar : public QToolBar
{
	Q_OBJECT

public:

	FlowCharToolButtonBar(QWidget *parent = Q_NULLPTR);

	~FlowCharToolButtonBar();

	// 传入场景对象
	void SetFlowchartScene(FlowchartScene* _scene);

	// 传入形状栏
	void SetToolSideBar(FlowCharToolSideBar* _side_bar);

private:

	// 初始化界面
	void InitWidget();

	// 文件保存
	void FileSave();

	// 文件读取
	void FileRead();

private slots:

	// action通用槽函数
	void FlowActionSlot();


private:

	// 设置鼠标状态		指针工具	连接线
	QAction *mouse_status_nomal_, *mouse_status_link_;
	// 形状
	QAction *item_type_;
	// 保存为文件	读取文件
	QAction* file_save_, * file_read_;

    //
    FlowchartScene* scene_;
	FlowCharToolSideBar* side_bar_;

	// 测试数据
	FlowchartInforBases datas;
};


//*******************************左侧工具栏************************************
class  FlowCharToolSideBar : public QDockWidget
{
	Q_OBJECT

public:
	FlowCharToolSideBar(QWidget *parent = Q_NULLPTR);
	~FlowCharToolSideBar();

	void SetFlowchartScene(FlowchartScene* _scene);

private:

	void InitWidget();

	void InitData();

private slots:

	void FlowButtonSlot();

private:

	QStackedWidget* stacked_widget_;

	FlowchartScene* scene_;
	QMap<std::string, QPushButton*> vec_btn_;
};
