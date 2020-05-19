#ifndef COMMON_H
#define COMMON_H

enum Mode{Automatic,Manual};

struct Configuration
{
    int animationInterval;  //动画间隔
    int canvasHeight;          //pixmap高度
    int canvasWidth;           //pixmap宽度
};

#endif // COMMON_H
