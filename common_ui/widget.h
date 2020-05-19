#ifndef WIDGET_H
#define WIDGET_H

#include<initializer_list>
#include<memory>
#include <QWidget>
#include"flutteringwings.h"
#include"setting.h"
#include"common.h"
#include"alarm.h"
#include<QPointer>

namespace Util {
    inline constexpr __factor_int factor=1; //10%
}
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class QScrollArea;
class QMenu;
class Simulator;
class QStackedWidget;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);

    ~Widget();
    struct WidgetMappingInfo : Util::ObjFD
    {
        QString showName{};     //控件显示的名字
        int menuListIndex{0};       //显示的菜单列表索引
        int dataInputPaneIndex{0};       //在被容器插入时,由插入时顺序决定
        bool isAnimationComplete{false};    //动画是否结束
    };
    void addMapping(std::initializer_list<WidgetMappingInfo> list_);

    void init();
private:
    void loadCnf();
    void initUI();
    void prepareNewSimulation();
    void initAction();
    void prepare();
    void changeSimulator(int objFd);
    void showMsg();
    int getObjFdFromMenuList(int index_);
    int getObjFdFromDataInputPane(int index_);

private:
    Ui::Widget *ui;
    QPointer<QMenu> menu;
    Configuration settings;
    QPointer<SettingPane> globalSetting;
    QPointer<QStackedWidget> dataInputPane;     //容纳各个simulator各自的输入和控制面板

    std::shared_ptr<Simulator> currentSimulator;

    int currentobjFd=0;
    QHash<int,WidgetMappingInfo> simMappingContainer;

    Alarm * alarm{};

    QVector<std::tuple<int,QAction *>> canvasActionMapping{};

    bool isctl;
    Util::__factor_int factor{Util::factor};
    Mode mode;

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual bool eventFilter(QObject *object, QEvent *event) override;
    virtual void contextMenuEvent(QContextMenuEvent *event) override;
    virtual void closeEvent(QCloseEvent *event) override;
private slots:
    void on_startBtn_clicked();
    void on_horizontalSlider_valueChanged(int value);

signals:
    void changeElementsSize(int);
    void switchClickd();

};

#endif // WIDGET_H
