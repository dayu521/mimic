#ifndef MYSCREEN_H
#define MYSCREEN_H
#include<QWidget>
#include<QTimer>
#include<memory>

namespace Util {
    using __width_int=int;
    using __height_int=int;
    using __factor_int=int;
    struct ObjFD
    {
        int fd{0};     //不得已,不能是const
    };
}

class Simulator;
class FreezePainter;
class Alarm;

class FlutteringWings : public QWidget
{
    Q_OBJECT
public:
    explicit FlutteringWings(QWidget *parent = nullptr);
    ~FlutteringWings() override;

    struct SimMapping :Util::ObjFD
    {
        int pixFd{0};
    };

    void addCanvas(std::initializer_list<QSize> s);

    QVector<std::tuple<QSize,int>> getAllPixSizeFd() const;

    void autoChangeCanvasSize(Util::__width_int w_,Util::__height_int h_,bool isForce_=true);

    bool makeElementsBig(int factor=1/*10%*/);

    void initMesgOnPix(const QString & s="请先进行数据模拟,生成绘图数据");

    void setDefaultCanva(QSize size_={800,500});

    void haveNoIdea();

    bool chosePix(int pfd_=0);
    //播放动画
    virtual void playAnimation();

    //停止动画
    virtual void stopAnimation();

    bool isRunning() const
    {
        return animationTimer->isActive();
    }

    //重放动画
    void prepareReplayAnimation();

    //设置动画间隔
    void setInterval(int millisecond_);

    //切换模拟器
    void setSim( int which_);

    void preCheck();

    enum CheckResult{UnCheck=0x0,Ok=0x1,SizeError=0x2,OtherError=0x4};

private:
    //保存状态
    void saveStatus(int which_);

    //恢复
    void applyStatus(int which_);

signals:
    void hasNoModelData();
    void canNotPlay();
    void errorResult(int r);
    void elementsSizeChanged(bool);
    void playCompleted();
    void choseSomeOnePixFD(int fd_);
//    void
public slots :
    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
private:

    int currentPixIndex{0};
    QPixmap * pix{};
    QVector<std::shared_ptr<QPixmap>> pixContainer{};

    int currentSimIndex{-1};
    std::shared_ptr<Simulator> sim{};

    FreezePainter * currentFp{nullptr};

    QVector<SimMapping> mappingVec{};

    QTimer * animationTimer{nullptr};
    QTimer* throttleTimer{nullptr};     //节流计时器

    Util::__factor_int factor{};
    Util::__width_int vWidth{};
    Util::__height_int vHeight{};
    Util::__width_int fpWidth{};
    Util::__height_int fpHeight{};

    int fuck{};

    // QWidget interface
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
};


#endif // MYSCREEN_H
