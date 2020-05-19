#include "rbtree_model.h"
#include<tuple>
#include<iostream>
#include<QPainterPath>
#include<QPainter>
#include"far_away.h"

RbtreeModel::RbtreeModel()
{

    SentinelNode=new ModelNode(-9999);
    SentinelNode->_prev=SentinelNode->_next=SentinelNode;

    Root=NIL=new ModelNode(-9998);
    NIL->color=Black;
    NIL->_parent=NIL->_left=NIL->_right=NIL;

    fakeNodeContainer.insert(NIL->_value,NIL);
}

RbtreeModel::~RbtreeModel()
{
    //所有节点包括Root和NIL都被删除
    qDeleteAll(fakeNodeContainer);
    delete SentinelNode;
}

void RbtreeModel::nextNFrame(int )
{
    dispatchActionAndDraw();
    current++;
}

void RbtreeModel::previousFrame(int )
{

}

void RbtreeModel::currentSnapshot() const
{
    QPainter pp(pix);
    pix->fill();
    pp.translate(0,ele.diameter/2);
    QFont font = pp.font();
    font.setPixelSize(ele.fontSize);
    pp.setFont(font);
    drawAllElement(pp,Root);
    pp.end();
    auto x=current-1;
    if(instructions[x].action==Operate::Search)
        drawCurrentNodeItem(_searchNodeItem);
}

std::tuple<Util::__width_int, Util::__height_int> RbtreeModel::getSize()
{
    return {(xNodeNumber+1)*ele.radius,(yNodeNumber-1)*ele.nodeLineHeight+ele.diameter};
}

bool RbtreeModel::isBlow()
{
    return current>=all;
}

void RbtreeModel::clearAllModelDatas()
{
    instructions.clear();
    SentinelNode->_prev=SentinelNode->_next=SentinelNode;
    Root=NIL;
    NIL->_parent=NIL->_left=NIL->_right=NIL;
    fakeNodeContainer.remove(-9998);
    qDeleteAll(fakeNodeContainer);
    fakeNodeContainer.clear();
    fakeNodeContainer.insert(NIL->_value,NIL);

    current=0;
}

void RbtreeModel::initModelData()
{
    Root=NIL;

    SentinelNode->_prev=SentinelNode->_next=SentinelNode;

    current=0;
    currentAnmIndex=0;
}

void RbtreeModel::dispatchActionAndDraw()
{
    auto & action=instructions[current];
    switch (action.action) {
        case Operate::Search:
            drawCurrentNodeItem(search(action));
            break;
        case Operate::Add:
            add(action);
            drawAllElement();
            break;
        case Operate::Rotate:
            rotate(action);
            drawAllElement();
            break;
        case Operate::Substitute:
            substitute(action);
            drawAllElement();
            break;
        case Operate::ChangeColor:
            recolorNodeItem(changeColor(action));
            break;
        case Operate::NextValue:
            showNextValue();
            break;
        case Operate::Done:
            done(action);
            drawAllElement();
            break;
    }
}

void RbtreeModel::drawAllElement(QPainter &_painter, RbtreeModel::ModelNode *_nodeItem) const
{
    if(_nodeItem!=NIL){
        drawAllElement(_painter,_nodeItem->_left);
        drawAllElement(_painter,_nodeItem->_right);
        //draw line
        if(_nodeItem->xParent>0)
            _painter.drawLine(_nodeItem->x*ele.diameter/2,_nodeItem->y*ele.nodeLineHeight,_nodeItem->xParent*ele.diameter/2,_nodeItem->yParent*ele.nodeLineHeight);
        //draw cycle
        QPainterPath myPath;
        myPath.addEllipse(QPoint(_nodeItem->x*ele.diameter/2,_nodeItem->y*ele.nodeLineHeight),ele.radius,ele.radius);
        _painter.drawPath(myPath);
        _painter.fillPath(myPath,_nodeItem->color==Red?Qt::red:Qt::black);
        //draw text
        _painter.setPen(Qt::white);
        _painter.drawText(QRect(_nodeItem->x*ele.diameter/2-ele.radius,_nodeItem->y*ele.nodeLineHeight-ele.radius,ele.diameter,ele.diameter),Qt::AlignCenter,QString::number(_nodeItem->_value));
        _painter.setPen(Qt::black);
    }
}

void RbtreeModel::drawAllElement() const
{
    pix->fill();
    QPainter pp(pix);
    pp.translate(0,ele.diameter/2);
    QFont font = pp.font();
    font.setPixelSize(ele.fontSize);
    pp.setFont(font);
    drawAllElement(pp,Root);
}

void RbtreeModel::drawCurrentNodeItem(RbtreeModel::ModelNode *_nodeItem) const
{
    auto & [_diameter,_radius,_nodeLineHeight,_fontSize]=ele;
    pix->fill();
    QPainter pp(pix);
    pp.translate(0,_diameter/2);
    QFont font = pp.font();
    font.setPixelSize(_fontSize);
    pp.setFont(font);
    drawAllElement(pp,Root);

    QPainterPath myPath;
    myPath.addEllipse(QPoint(_nodeItem->x*_diameter/2,_nodeItem->y*_nodeLineHeight),_radius+5,_radius+5);
    pp.drawPath(myPath);
    pp.fillPath(myPath,_nodeItem->color==Red?Qt::red:Qt::black);
    pp.setPen(Qt::white);
    pp.drawText(QRect(_nodeItem->x*_diameter/2-_radius,_nodeItem->y*_nodeLineHeight-_radius,_diameter,_diameter),Qt::AlignCenter,QString::number(_nodeItem->_value));
    pp.setPen(Qt::black);
}

void RbtreeModel::paintColor(RbtreeModel::ModelNode *root, QPainter &pp, int dx) const
{
    if(root==nullptr)
        return;
    QPainterPath myPath;
    if(_searchNodeItem==root)
        dx=5;
    auto & [_diameter,_radius,_nodeLineHeight,_fontSize]=ele;
    myPath.addEllipse(QPoint(root->x*_diameter/2,root->y*_nodeLineHeight),_radius+dx,_radius+dx);
    pp.drawPath(myPath);
    pp.fillPath(myPath,root->color==Red?Qt::red:Qt::black);
    pp.setPen(Qt::white);
    pp.drawText(QRect(root->x*_diameter/2-_radius,root->y*_nodeLineHeight-_radius,_diameter,_diameter),Qt::AlignCenter,QString::number(root->_value));
    pp.setPen(Qt::black);
}

void RbtreeModel::recolorNodeItem(Util::TupleWrapArray<RbtreeModel::ModelNode *,3> tuple_) const
{
    auto & [_diameter,_radius,_nodeLineHeight,_fontSize]=ele;
    QPainter pp(pix);
    pp.translate(0,_diameter/2);
    QFont font = pp.font();
    font.setPixelSize(_fontSize);
    pp.setFont(font);
    auto [a,b,c]=tuple_;
    paintColor(a,pp);
    paintColor(b,pp);
    paintColor(c,pp);
}

RbtreeModel::ModelNode *RbtreeModel::search(Instruction &action)
{
    //保存当前正在查找的节点,在染色时会考虑相应变大一点
    _searchNodeItem=fakeNodeContainer.value(action.data[0]);
    return _searchNodeItem;
}

void RbtreeModel::add(Instruction &action)
{
    auto _parentNode=fakeNodeContainer.value(action.data[1]);
    ModelNode * _newNode=nullptr;
    if(fakeNodeContainer.contains(action.data[0])){
        _newNode=fakeNodeContainer.value((action.data[0]));
        _newNode->_left=NIL;
        _newNode->_right=NIL;
        _newNode->_parent=_parentNode;
        _newNode->init();
    }else{
        _newNode=new ModelNode(action.data[0],NIL,NIL,_parentNode);
        fakeNodeContainer.insert(_newNode->_value,_newNode);
    }
    if(_parentNode==NIL)
        Root=_newNode;
    else if(_parentNode->_value>_newNode->_value)
        _parentNode->_left=_newNode;
    else
        _parentNode->_right=_newNode;

    fillPropertyInInsert(_newNode);
}

void RbtreeModel::rotate(Instruction &action)
{
    auto _currentNodeItem=fakeNodeContainer.value(action.data[0]);
    if(action.data[1]==0)
        rotationWithLeftChildForNodeItem(getParentReferenceForNodeItem(_currentNodeItem));
    else
        rotationWithRightChildForNodeItem(getParentReferenceForNodeItem(_currentNodeItem));
    setY();
}

Util::TupleWrapArray<RbtreeModel::ModelNode *, 3> RbtreeModel::changeColor(Instruction &action)
{
    int i=0;
    ModelNode * a[3]={};
    while(i<3){
        auto colorValue=action.data[2*i+1];
        if(colorValue>1)
            break;
        a[i]=fakeNodeContainer.value(action.data[2*i]);
        a[i]->color=colorValue==0?Red:Black;
        i++;
    }
    // 等价于return std::make_tuple(a[0],a[1],a[2]);
    return Util::make_TupleForArray(a);
}

void RbtreeModel::showNextValue()
{

}

void RbtreeModel::substitute(Instruction &action)
{
    auto y=fakeNodeContainer.value(action.data[0]);
    deleteNodeItemFromLinkedList(y);
    fakeNodeContainer.remove(y->_value);
    auto x=fakeNodeContainer.value(action.data[1]);
    if(action.data[0]!=action.data[2]){
        auto yOriginal=fakeNodeContainer.value(action.data[2]);
        fakeNodeContainer.remove(yOriginal->_value);
        yOriginal->_value=y->_value;
        fakeNodeContainer.insert(y->_value,yOriginal);
    }
    if(y==Root)
        Root=x;
    else if(y==y->_parent->_left)
        y->_parent->_left=x;
    else
        y->_parent->_right=x;
    x->_parent=y->_parent;
    setY();
    delete y;
}

void RbtreeModel::done(Instruction &action)
{
    Q_UNUSED(action)
}

void RbtreeModel::fillPropertyInInsert(RbtreeModel::ModelNode *_nodeItem)
{
    //insert value into linkedlist-ordered and update x coordinate
    insertNodeItemIntoLinkedList(_nodeItem);
    //set correct infomation left without x by level-order
    setY();
}

void RbtreeModel::insertNodeItemIntoLinkedList(RbtreeModel::ModelNode *_nodeItem)
{
    ModelNode * _pCurrent=SentinelNode->_prev;
    while (_pCurrent!=SentinelNode&&_nodeItem->_value<_pCurrent->_value) {
        _pCurrent->x++;
        _pCurrent=_pCurrent->_prev;
    }
    _nodeItem->x=_pCurrent->x+1;
    _nodeItem->_next=_pCurrent->_next;
    _pCurrent->_next->_prev=_nodeItem;
    _nodeItem->_prev=_pCurrent;
    _pCurrent->_next=_nodeItem;
}

void RbtreeModel::deleteNodeItemFromLinkedList(RbtreeModel::ModelNode *_nodeItem)
{
    if(_nodeItem==NIL)
        return ;
    auto brokenPoint=_nodeItem->_prev;
    brokenPoint->_next=_nodeItem->_next;
    _nodeItem->_next->_prev=brokenPoint;
    brokenPoint=brokenPoint->_next;
    while(brokenPoint!=SentinelNode){
        brokenPoint->x--;
        brokenPoint=brokenPoint->_next;
    }
}

void RbtreeModel::setY()
{
    ModelNode * temp=nullptr;
    Root->color=Black;
    SentinelNode->y=-1;
    NIL->y=-1;
    NodeQueue.enqueue(Root);
    int level=0;
    int column=NodeQueue.size();
    while (NodeQueue.size()>0) {
        temp=NodeQueue.dequeue();
        column--;
        temp->y=level;
        temp->xParent=temp->_parent->x;
        temp->yParent=temp->_parent->y;
        if(temp->_left!=NIL)
            NodeQueue.enqueue(temp->_left);
        if(temp->_right!=NIL)
            NodeQueue.enqueue(temp->_right);
        if(column==0){
            level++;
            column=NodeQueue.size();
        }
    }
}

void RbtreeModel::rotationWithLeftChildForNodeItem(RbtreeModel::ModelNode *&root)
{
    auto left_child = root->_left;

    root->_left = left_child->_right;
    left_child->_right->_parent = root;

    left_child->_right = root;
    left_child->_parent = root->_parent;

    root->_parent = left_child;
    root = left_child;
}

void RbtreeModel::rotationWithRightChildForNodeItem(RbtreeModel::ModelNode *&root)
{
    auto right_child = root->_right;

    root->_right = right_child->_left;
    right_child->_left->_parent = root;

    right_child->_left = root;
    right_child->_parent = root->_parent;

    root->_parent = right_child;
    root = right_child;
}

void RbtreeModel::replaceForNodeItem(RbtreeModel::ModelNode *y, RbtreeModel::ModelNode *x)
{

}

void RbtreeModel::setXYNodeNumber(int x_, int y_)
{
    xNodeNumber=x_;
    yNodeNumber=y_;
}
