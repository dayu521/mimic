#ifndef RBTREEMODEL_H
#define RBTREEMODEL_H
#include"animation/abstract_animation.h"
#include<QHash>
#include<QQueue>
#include"convenience.h"

class QPainter;

class RbtreeModel : public FreezePainter
{
public:
    explicit RbtreeModel();
    ~RbtreeModel();

    // FreezePainter interface
public:
    virtual void nextNFrame(int i) override;
    virtual void previousFrame(int i) override;
    virtual void currentSnapshot() const override;
    virtual std::tuple<Util::__width_int,Util::__height_int> getSize() override;
    //w=(xNodeNumber+1)*ele.radius
    //h=(yNodeNumber-1)*ele.nodeLineHeight+ele.diameter
//    virtual void setElementNewSize(Util::__width_int,Util::__height_int) override;
    virtual bool isBlow() override;
    virtual void clearAllModelDatas() override;
    virtual void initModelData() override;

    //特定于此类的函数
public:
    void setXYNodeNumber(int x_,int y_);

private:
    struct Elements
    {
        int diameter {40};   //节点直径
        int radius {diameter / 2};    //半径
        int nodeLineHeight {3 * diameter / 2};     //垂直距离
        int fontSize {radius};      //字体大小
    };

    enum Color { Red, Black };

    //用于绘图的红黑树节点类
    struct ModelNode {
        int _value;

        ModelNode *_left;
        ModelNode *_right;
        ModelNode *_parent;

        ModelNode *_next = nullptr;
        ModelNode *_prev = nullptr;

        int x = 0, y = -1;               // current coordinate
        int xParent = -1, yParent = -1;  // parent link coordinate
        Color color = Red;

        ModelNode(int value, ModelNode *left = nullptr, ModelNode *right = nullptr,
                 ModelNode *parent = nullptr)
            : _value(value), _left(left), _right(right), _parent(parent) {}
        void init(){
            _next=_prev=nullptr;
            x=0;
            y=xParent=yParent=-1;
            color=Red;
        }
    };
    enum Operate : int{Search,Add,Rotate,ChangeColor,NextValue,Substitute,Done};
    //绘图
    void dispatchActionAndDraw();
    void drawAllElement(QPainter &_painter, ModelNode *_nodeItem)const;
    void drawAllElement()const;
    void drawCurrentNodeItem(ModelNode * _nodeItem)const;
    void paintColor(ModelNode *root,QPainter & pp,int dx=0)const;
    void recolorNodeItem(Util::TupleWrapArray<ModelNode*,3> tuple_)const;

    //动画
    void animation();

    ModelNode *search(Instruction &action);
    void add(Instruction &action);
    void rotate(Instruction &action);

    Util::TupleWrapArray<ModelNode*,3>  changeColor(Instruction &action);
    void showNextValue();
    void substitute(Instruction &action);
    void done(Instruction &action);

    void fillPropertyInInsert(ModelNode *_nodeItem);
    void insertNodeItemIntoLinkedList(ModelNode *_nodeItem);
    void deleteNodeItemFromLinkedList(ModelNode * _nodeItem);
    void setY();

    void rotationWithLeftChildForNodeItem(ModelNode *&root);
    void rotationWithRightChildForNodeItem(ModelNode *&root);
    void replaceForNodeItem(ModelNode *y, ModelNode *x);
    ModelNode *&getParentReferenceForNodeItem(ModelNode *child) {
        if (child->_parent == NIL) return this->Root;
        return child == child->_parent->_left ? child->_parent->_left
                                              : child->_parent->_right;
    }

private:
    Elements ele{};

    ModelNode * Root;     //绘图红黑树的根节点
    ModelNode * NIL;     //绘图红黑树的哨兵节点
    ModelNode * SentinelNode;     //绘图节点形成的序链接链表的哨兵节点
    QHash<int, ModelNode *> fakeNodeContainer;    //持有绘图树节点
    QQueue<ModelNode *> NodeQueue;       //用来进行层序遍历,然后设置每个绘图节点的y轴坐标

    ModelNode * _searchNodeItem{nullptr};

    int currentAnmNumber=0;
    int currentAnmIndex=0;
    bool hasAnimation=false;
    double xLine=0;
    double yLine=0;

    int xNodeNumber{0};
    int yNodeNumber{0};

};

#endif // RBTREEMODEL_H
