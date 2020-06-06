#include "flutteringwings.h"
#include<QPainter>
#include<QPixmap>
#include"freeze_painter.h"
#include"simulator.h"
#include"register_type.h"
#include"something.h"
#include"alarm.h"

FlutteringWings::FlutteringWings(QWidget *parent) : QWidget(parent),
    animationTimer(new QTimer)
{
    for(int i=0;i<Util::numberOfobjFd;i++)
        mappingVec.push_back({{i},0});

    animationTimer->setInterval(200);
    connect(animationTimer,&QTimer::timeout,[this](){
        if(currentFp->isBlow()){
            animationTimer->stop();
            emit playCompleted();
            return ;
        }
        currentFp->nextNFrame();
        update();
    });

    throttleTimer=new QTimer(this);
    throttleTimer->setSingleShot(true);
    connect(throttleTimer,&QTimer::timeout,[this](){
        resize(fpWidth,fpHeight);
        if(!animationTimer->isActive()){
            if(currentFp->isRunning()){
                pix->fill();
                currentFp->currentSnapshot();
            }
            update();
        }
    });
}

FlutteringWings::~FlutteringWings()
{

}

void FlutteringWings::addCanvas(std::initializer_list<QSize> s)
{
    for(const auto & i:s){
        pixContainer.append(std::make_shared<QPixmap>(i));
        pixContainer.last()->fill();
    }
    if(pixContainer.size()<1)
        return ;
    std::sort(pixContainer.begin()+1,pixContainer.end(),[](const auto & x1,const auto & x2){
        return x1->width()<x2->width()||x1->height()<x2->height();
    });
}

QVector<std::tuple<QSize, int> > FlutteringWings::getAllPixSizeFd() const
{
    QVector<std::tuple<QSize, int>> v{};
    int i=0;
    for(const auto & x: pixContainer){
        v.append({x->size(),i++});
    }
    return v;
}

//----<---current--->----
//    left        right
//舍不得删除,暂时不用了
void FlutteringWings::autoChangeCanvasSize(Util::__width_int w_,Util::__height_int h_, bool isForce_)
{
    auto maxW_=w_;
    auto maxH_=h_;
    auto cur_=currentPixIndex;

    //测试是否变小方向
    while (cur_>=0) {
        if(maxW_<=pixContainer[cur_]->width()&&maxH_<=pixContainer[cur_]->height()){
            cur_--;
        }else{
            break;
        }
    }
    if(cur_<currentPixIndex){
        currentPixIndex=cur_+1;
        pix=pixContainer[cur_+1].get();
        currentFp->setPix(pix);
        return ;
    }
    //否则为变大方向
    while (cur_<pixContainer.size()) {
        if(maxW_<=pixContainer[cur_]->width()&&maxH_<=pixContainer[cur_]->height())
            break ;
        else
            cur_++ ;
    }
    try {
        if(cur_>=pixContainer.size())
            throw std::range_error("Exceeding the maximum number");
    }  catch (std::range_error & e) {
        Util::logExcept(e.what());
        cur_=pixContainer.size()-1;
    }
    currentPixIndex=cur_;
    pix=pixContainer[cur_].get();
    currentFp->setPix(pix);
}

bool FlutteringWings::makeElementsBig(int factor)
{
    fuck=SizeError;
//    auto [w,h]={width()*(10+factor)/10,width()*(10+factor)/10};
    int w,h;
//    if(!accept)
        return false;
    if(w>Util::MAX_WIDTH||h>Util::MAX_HEIGHT)
        return false;
    fuck=Ok;
    this->factor=factor;
    std::tie(vWidth,vHeight)={w,h};
    throttleTimer->start(200);      //清除重置之前未完成的计时器，开始新计时
    return true;
}

void FlutteringWings::initMesgOnPix(const QString & s)
{
    pix->fill(Qt::white);
    QPainter p(pix);
    auto font_=p.font();
    font_.setPixelSize(36);
    p.setFont(font_);
    p.drawText(50,50,s);
    p.drawText(50,100,"多使用鼠标右键上下文菜单");
}

void FlutteringWings::setDefaultCanva(QSize size_)
{
    if(pixContainer.size()==0)
        pixContainer.append(std::make_shared<QPixmap>(size_));
    else
        pixContainer[0]=std::make_shared<QPixmap>(size_);
    pix=pixContainer[0].get();
    initMesgOnPix();
}

void FlutteringWings::haveNoIdea()
{
    if(currentFp->canRunning()){
        chosePix(currentPixIndex);
    }
    else
        resize(pix->size());
//    chosePix(currentPixIndex);
}

bool FlutteringWings::chosePix(int pfd_)
{
    if(pfd_<0||pfd_>=pixContainer.size())
        return false;
    pix=pixContainer[pfd_].get();
    currentFp->setPix(pix);
    currentPixIndex=pfd_;
    if(currentFp->canRunning())
        std::tie(fpWidth,fpHeight)=currentFp->getSize();/*std::make_tuple(pix->width(),pix->height());*/
    else
        std::tie(fpWidth,fpHeight)=std::make_tuple(pix->width(),pix->height());
    throttleTimer->start();
    return true;
}

void FlutteringWings::playAnimation()
{
    if(!animationTimer->isActive())
        animationTimer->start();
}

void FlutteringWings::stopAnimation()
{
    if(animationTimer->isActive())
        animationTimer->stop();
}

void FlutteringWings::prepareReplayAnimation()
{
    currentFp->initModelData();
    initMesgOnPix();
}

void FlutteringWings::setInterval(int millisecond_)
{
    animationTimer->setInterval(millisecond_);
}

void FlutteringWings::setSim(int which_)
{
    stopAnimation();
    if(which_>Util::numberOfobjFd||which_<0)
        throw std::range_error("No such Simulator!");
    else if(currentSimIndex==which_)
        ;
    else{
        if(currentSimIndex>=0)
            saveStatus(currentSimIndex);
        applyStatus(which_);
        update();
    }
}

void FlutteringWings::preCheck()
{
    fuck=UnCheck;
    std::tie(fpWidth,fpHeight)=currentFp->getSize();
    autoChangeCanvasSize(fpWidth,fpHeight);
    chosePix(currentPixIndex);
    if(fuck==UnCheck){
        fuck=Ok;
        initMesgOnPix(tr("[%1]检查完成，可进行放映").arg(Util::name[currentSimIndex]));
        emit choseSomeOnePixFD(currentPixIndex);
    }else
        emit errorResult(fuck);
}

void FlutteringWings::saveStatus(int which_)
{
    mappingVec[which_].pixFd=currentPixIndex;
    currentFp=nullptr;
}

void FlutteringWings::applyStatus(int which_)
{
    //模拟器下标
    currentSimIndex=which_;
    //获取模拟器
    sim=Util::obj.GetObj(mappingVec[which_].fd);
    //模拟器pixmap的下标
    currentPixIndex=mappingVec[which_].pixFd;
    //模拟器对应pixmap
    pix=pixContainer[mappingVec[which_].pixFd].get();
    //模拟器的Fp
    currentFp=sim->getFP().get();
    //设置画布
    currentFp->setPix(pix);
    std::tie(fpWidth,fpHeight)=std::make_tuple(pix->width(),pix->height());
    pix->fill();
    resize(pix->size());
    if(currentFp->isRunning())
        currentFp->currentSnapshot();
    else
        initMesgOnPix(tr("切换到 %1").arg(Util::name[which_]));
}

void FlutteringWings::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.setWindow(0,0,fpWidth,fpHeight);
    p.setViewport(0,0,vWidth,vHeight);
    p.drawPixmap(0,0,*pix,0,0,fpWidth,fpHeight);
}

void FlutteringWings::resizeEvent(QResizeEvent *event)
{
    auto f=double(fpWidth)/fpHeight;
    if(width()>fpWidth&&height()>fpHeight)
        ;
    else{
        double pw=height()*f;
        double ph=width()/f;
        if(pw<width())
            ph=height();
        else
            pw=width();
        vWidth=pw;
        vHeight=ph;
    }
}

