#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include"common.h"

namespace Ui {
class Form;
}
class Widget;

class SettingPane : public QWidget
{
    Q_OBJECT

public:
    explicit SettingPane(Configuration & cfg, Widget *parent = nullptr);
    ~SettingPane();
    virtual void showEvent(QShowEvent *event) override;
signals:
    void changeAnimationSpeed(int x_);
private:
    Configuration &settings;
    Widget * parent;
    Ui::Form *ui;
    // QWidget interface
};

#endif // FORM_H
