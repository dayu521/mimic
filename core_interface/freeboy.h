#ifndef FREEBOY_H
#define FREEBOY_H
#include<QObject>
#include<QThread>

class FarAway;

class FreeBoy : public QObject
{
    Q_OBJECT
public:
    explicit FreeBoy(QObject * parent_=nullptr);
    ~FreeBoy();
public slots :
    void run();
    void switchD(std::shared_ptr<FarAway> p_);
signals:
    void completed();
    void hasError(const char *);
private:
    std::shared_ptr<FarAway> f{};
};

#endif // FREEBOY_H
