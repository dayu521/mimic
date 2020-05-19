#include "setting.h"
#include "ui_form.h"
#include<QValidator>
#include"widget.h"

SettingPane::SettingPane(Configuration &cfg, Widget *parent) :
    settings(cfg),
    parent(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    //动画间隔设置
    connect(ui->animationSpeed,&QSlider::valueChanged,this,&SettingPane::changeAnimationSpeed);
    //长宽设置
    ui->widthAndHeightLedit->setPlaceholderText("长:宽");
    ui->widthAndHeightLedit->setValidator(new QRegExpValidator(QRegExp("\\d+:\\d+"),this));
    connect(ui->widthAndHeightLedit,&QLineEdit::editingFinished,[=](){
        auto values=ui->widthAndHeightLedit->text().split(":");
        settings.canvasWidth=values[0].toInt();
        settings.canvasHeight=values[1].toInt();
    });
//    connect(parent,&Widget::changeElementsSize,[=](QSize size_){
//        settings.canvasWidth=size_.width();
//        settings.canvasHeight=size_.height();
//        ui->widthAndHeightLedit->setText(QString("%1:%2").arg(settings.canvasWidth).arg(settings.canvasHeight));
//    });
    setWindowTitle("dasfs发生的");
}

SettingPane::~SettingPane()
{
    delete ui;
}

void SettingPane::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)
    ui->animationSpeed->setValue(settings.animationInterval);
    ui->widthAndHeightLedit->setText(QString("%1:%2").arg(settings.canvasWidth).arg(settings.canvasHeight));
}


