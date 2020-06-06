#include "widget.h"
#include "ui_widget.h"
#include<QTimer>
#include<QDebug>
#include<QPainter>
#include<QAction>
#include"register_type.h"
#include"simulator.h"
#include<QStackedWidget>
#include<QKeyEvent>
#include<QMenu>
#include<QActionGroup>
#include<QScreen>
#include"freeze_painter.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget),alarm(new Alarm)
{
    ui->setupUi(this);
//    setContextMenuPolicy(Qt::ActionsContextMenu);
    initUI();
    initAction();
    loadCnf();
    prepare();
}

void Widget::addMapping(std::initializer_list<WidgetMappingInfo> list_)
{
    ui->menuList->disconnect();//先断开信号连接
    for(auto x_:list_){
        simMappingContainer.insert(x_.fd,x_);   //注意,QHash是先创建对象,然后调用=操作符复制.
        int index_=dataInputPane->addWidget(new QWidget);
        simMappingContainer[x_.fd].dataInputPaneIndex=index_;
        ui->menuList->addItem(x_.showName);//第一次也将触发currentIndexChanged信号
        simMappingContainer[x_.fd].menuListIndex=ui->menuList->count()-1;
    }
    //恢复信号连接
    connect(ui->menuList,QOverload<int>::of(&QComboBox::currentIndexChanged),[this](int x){
        auto objFd=getObjFdFromMenuList(x);
        changeSimulator(objFd);
    });
    changeSimulator((list_.begin())->fd);
}

void Widget::init()
{

}

Widget::~Widget()
{
    delete ui;
    delete globalSetting;
    delete dataInputPane;
}

void Widget::loadCnf()
{
    settings.animationInterval=500;
    settings.canvasWidth=800;
    settings.canvasHeight=500;
}

void Widget::initUI()
{
    globalSetting=new SettingPane(settings,this);
    globalSetting->installEventFilter(this);
    connect(globalSetting,&SettingPane::changeAnimationSpeed,[this](int x_){
//        timeLine->setInterval(x_);
        ui->rightContainerWidget->setInterval(x_);
    });

    dataInputPane=new QStackedWidget();
    dataInputPane->installEventFilter(this);

    ui->rightContainerWidget->setFocus();
    ui->rightContainerWidget->setDefaultCanva(qApp->primaryScreen()->size());
    ui->rightContainerWidget->addCanvas({{qApp->primaryScreen()->size()*2},{2000,1500},{3000,2000},{4000,3000}});

//    ui->rightContainerWidget->resize(qApp->primaryScreen()->size());
}

void Widget::prepareNewSimulation()
{

}

void Widget::initAction()
{
    menu=new QMenu(this);

    // =fuc
    auto startAct=menu->addAction(tr("生成模拟数据"));
    connect(startAct,&QAction::triggered,[this](){      
        ui->textBrowser->append(tr("正在生成模拟数据"));
        currentSimulator->setInputData({});
        alarm->startD();        //发送信号,生成原始数据
    });

    auto restart_Act=menu->addAction(tr("重新放映"));
    connect(restart_Act,&QAction::triggered,[this](){
        if(currentSimulator->currentStatus()!=Simulator::Status::Empty){
            ui->rightContainerWidget->prepareReplayAnimation();
            simMappingContainer[currentobjFd].isAnimationComplete=false;
            ui->rightContainerWidget->initMesgOnPix(tr("请开始重新进行放映"));
        }else
            ui->textBrowser->append(tr("无数据,请先生成"));
//        if(mode==Automatic)
//            animationTimer->start();
    });

    ui->rigthBottom->hide();
//    auto ac2_=new QAction("显示底栏");
//    connect(ac2_,&QAction::triggered,[=](bool){
//        if(ui->rigthBottom->isHidden()){
//            ui->rigthBottom->show();
//            ac2_->setText("隐藏底栏");
//        } else{
//            ui->rigthBottom->hide();
//            ac2_->setText("显示底栏");
//        }
//    });

    auto set_Act=menu->addAction(tr("配置设置"));
    connect(set_Act,&QAction::triggered,[this](bool){
        globalSetting->move(width()/2-globalSetting->width()/2,height()/2-globalSetting->height()/2);
        globalSetting->show();
    });
    auto data_InputAct=menu->addAction(tr("数据设定面板"));
    connect(data_InputAct,&QAction::triggered,[this](bool){
        dataInputPane->setCurrentIndex(ui->menuList->currentIndex());
        dataInputPane->show();
    });

    auto menu_sub1=menu->addMenu(tr("其他设置"));
    auto autop_Act=menu_sub1->addAction(tr("自动放映"));
    autop_Act->setCheckable(true);
    autop_Act->setChecked(true);
    mode=Automatic;
    connect(autop_Act,&QAction::triggered,[this](bool checked){
        if(!checked){
            mode=Manual;
            ui->rightContainerWidget->stopAnimation();
            ui->textBrowser->append(tr("关闭自动放映:请使用 [ 和 ] 进行放映"));
        }else{
            mode=Automatic;
            ui->textBrowser->append(tr("自动放映开启:请使用鼠标左键开始/暂停"));
        }
    });

    //    ui->leftContainerWidget->hide();
    auto show_SideAct=menu_sub1->addAction(tr("显示侧栏"));
    show_SideAct->setCheckable(true);
    show_SideAct->setChecked(true);
    connect(show_SideAct,&QAction::triggered,[this](bool checked){
            checked?ui->leftContainerWidget->show():ui->leftContainerWidget->hide();
    });

    auto choseOne=menu->addMenu(tr("选择画布"));
    auto actionG=new QActionGroup(choseOne);
    actionG->setExclusive(true);
    for(const auto & [size_,fd_]:ui->rightContainerWidget->getAllPixSizeFd()){
        auto ac=actionG->addAction(tr("%1 * %2").
                           arg(size_.width()).
                           arg(size_.height()));
        ac->setData(fd_);
        ac->setCheckable(true);
        canvasActionMapping.append({fd_,ac});
    }
    choseOne->addActions(actionG->actions());
    /// 假定返回的列表元素是按照初始添加顺序,且fd从0开始!!
//    canvasActionMapping=(actionG->actions()).toVector();
    if(canvasActionMapping.size()>0)
        std::get<1>(canvasActionMapping[0])->setChecked(true);
    connect(actionG,&QActionGroup::triggered,[this](QAction* action_){
        ui->rightContainerWidget->chosePix(action_->data().toInt());
    });
    connect(ui->rightContainerWidget,&FlutteringWings::choseSomeOnePixFD,[this](int fd_){
        auto iterator=std::find_if(canvasActionMapping.begin(),canvasActionMapping.end(),[fd_](const std::tuple<int,QAction *> & item_){
            auto [fd,ac]=item_;
            if(fd==fd_){
                ac->setChecked(true);
                return true;
            }
            return false;
        });
        if(iterator==canvasActionMapping.end()){
            //log...
        }
    });

    auto fmk=menu->addAction(tr("解除封印!"));
    fmk->setCheckable(true);
    fmk->setChecked(true);
    connect(fmk,&QAction::triggered,[this](bool checked){
        !checked?ui->rightContainerWidgetWrap->setWidgetResizable(true):ui->rightContainerWidgetWrap->setWidgetResizable(false);
        ui->rightContainerWidget->haveNoIdea();
    });
}

void Widget::prepare()
{
    connect(ui->rightContainerWidget,&FlutteringWings::playCompleted,[this](){
        simMappingContainer[currentobjFd].isAnimationComplete=true;
        ui->textBrowser->append(tr("放映完成"));
    });

    connect(ui->rightContainerWidget,&FlutteringWings::elementsSizeChanged,[this](bool ok_){
        if(ok_)
            ui->textBrowser->append(tr("缩放指定大小ok"));
        else
            ui->textBrowser->append(tr("无法进行当前缩放"));
    });

    connect(ui->menuList,QOverload<int>::of(&QComboBox::currentIndexChanged),[this](int x){      
        changeSimulator(getObjFdFromMenuList(x));
    });

    connect(this,&Widget::changeElementsSize,[this](int x_){
//        if(currentSimulator->currentStatus()!=Simulator::Status::HasModelData)
//            ui->textBrowser->append(tr("无法进行当前大小的缩放"));
//        else if(ui->rightContainerWidget->makeElementsBig(x_))
//            ui->textBrowser->append(tr("能够进行当前大小的缩放"));
        auto s=ui->rightContainerWidget->size();
        qDebug()<<s;
        auto [w,h]=std::make_tuple(s.width(),s.height());
        ui->rightContainerWidget->resize(w+50,h+50);
    });

    //原始数据生成完成信号
    connect(alarm,&Alarm::completed,[this](){
        ui->textBrowser->append(tr("[%1]模型数据生成完成").arg(simMappingContainer[currentobjFd].showName));
        currentSimulator->clearModelData();
        currentSimulator->produceModelData();
        ui->rightContainerWidget->preCheck();
//        ui->rightContainerWidget->initMesgOnPix(tr(""));
        simMappingContainer[currentobjFd].isAnimationComplete=false;
    });

    connect(this,&Widget::switchClickd,[this](){
        if(currentSimulator->currentStatus()==Simulator::Status::HasModelData){
            !ui->rightContainerWidget->isRunning()?
                ui->rightContainerWidget->playAnimation():
                ui->rightContainerWidget->stopAnimation();
            ui->textBrowser->append(ui->rightContainerWidget->isRunning()?tr("已开始"):tr("已暂停"));
        }else if((currentSimulator->currentStatus()==Simulator::Status::UnCertain)){
            ui->textBrowser->append(tr("可能正在进行,请稍后"));
        }else
            ui->textBrowser->append(tr("无数据,请先生成"));

    });

    connect(ui->rightContainerWidget,&FlutteringWings::errorResult,[this](int fuck_){
        if(fuck_&FlutteringWings::Ok)
            ui->textBrowser->append(tr("播放器准备就绪"));
        else
            ui->textBrowser->append(tr("需要的画布大小超过最大大小"));
    });

    ui->horizontalSlider->setValue(settings.animationInterval);
}

void Widget::changeSimulator(int objFd)
{
    currentobjFd=objFd;
    currentSimulator=Util::obj.GetObj(objFd);
    ui->rightContainerWidget->setSim(objFd);
    alarm->set(currentSimulator->getFA());
    //相应的控制面板变更
    dataInputPane->setCurrentIndex(simMappingContainer[objFd].dataInputPaneIndex);
    dataInputPane->resize(dataInputPane->currentWidget()->size());

    setWindowTitle(simMappingContainer[objFd].showName);
    showMsg();
}

void Widget::showMsg()
{
    auto cout=ui->textBrowser;
    cout->clear();
    cout->append(tr("当前模拟:[ %1 ]").arg(simMappingContainer[currentobjFd].showName));
    if(mode==Automatic)
        cout->append(QString(tr("自动放映开启")));
}

int Widget::getObjFdFromMenuList(int index_)
{
    for(const auto & i :simMappingContainer)
        if(i.menuListIndex==index_)
            return i.fd;
    return -1;
}

int Widget::getObjFdFromDataInputPane(int index_)
{
    for(const auto & i :simMappingContainer)
        if(i.dataInputPaneIndex==index_)
            return i.fd;
    return -1;
}

void Widget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Control)
        isctl=true;
    //下面可以先统一测试isctl
    else if(event->key()==Qt::Key_Plus){
        if(isctl){
            emit changeElementsSize(factor);
        }
    }else if(event->key()==Qt::Key_Minus){
        if(isctl){
            emit changeElementsSize(-factor);
        }
    }
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Control)
        isctl=false;
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if(mode==Automatic){
        if(event->button()==Qt::LeftButton){
            emit switchClickd();
            event->accept();
        }
    }
}

bool Widget::eventFilter(QObject *object, QEvent *event)
{
    Q_UNUSED(object)
    if(event->type()==QEvent::KeyPress||event->type()==QEvent::KeyRelease){
        QCoreApplication::sendEvent(this,event);
        return true;
    }
    return false;
}

void Widget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event)
    menu->exec(QCursor::pos()+QPoint(5,5));
}

void Widget::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
//    QCoreApplication::quit();
    qApp->closeAllWindows();
}


void Widget::on_startBtn_clicked()
{
    qDebug()<<ui->leftContainerWidget->size();
}

void Widget::on_horizontalSlider_valueChanged(int value)
{
    ui->rightContainerWidget->setInterval(value);
}
