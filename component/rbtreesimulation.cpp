
#include "rbtreesimulation.h"
#include<initializer_list>
#include<QPainter>
#include<QWidget>

RBtreeSimulation::RBtreeSimulation()
{
    initialTree();
}

RBtreeSimulation::~RBtreeSimulation()
{
    qDeleteAll(fakeNodeContainer);
    emptyTree(root);
    delete NIL;
    delete SentinelNode;
}

void RBtreeSimulation::produceActionData()
{
    _qvectorForData.clear();
    for(int i=0;i<100;i++){
        auto x=i;/*rand() % 200;*/
        if(insert(x))
            _qvectorForData<<x;
    }
    currentActionNumber=instructions.size();
}

void RBtreeSimulation::clearActionData()
{
    instructions.clear();
    emptyTree(root);
    root=NIL;
    SentinelNode->_prev=SentinelNode->_next=SentinelNode;
    Root=NIL;
    NIL->_parent=NIL->_left=NIL->_right=NIL;
    fakeNodeContainer.remove(-9998);
    qDeleteAll(fakeNodeContainer);
    fakeNodeContainer.clear();
    fakeNodeContainer.insert(NIL->_value,NIL);

    current=0;
    currentAnmIndex=0;
    currentAnmNumber=0;
}

QWidget *RBtreeSimulation::getUi()
{
    return new QWidget;
}

QString RBtreeSimulation::getName() const
{
    return QStringLiteral("红黑树");
}

void RBtreeSimulation::setPixmap(std::shared_ptr<QPixmap> p)
{
    pix=p;
    pix->fill();
}

void RBtreeSimulation::currentSnapshot() const
{
    QPainter pp(pix.get());
    pix->fill();
    pp.translate(0,_diameter/2);
    QFont font = pp.font();
    font.setPixelSize(_fontSize);
    pp.setFont(font);
    drawAllElement(pp,Root);
    pp.end();
    auto x=current-1;
    if(instructions[x]._ope==Operate::Search)
        drawCurrentNodeItem(_searchNodeItem);
}

int RBtreeSimulation::actionNumber() const
{
    return instructions.size();
}

void RBtreeSimulation::nextAction()
{
    dispatchActionAndDraw();
//    currentAction++;
}

QSize RBtreeSimulation::calculationMinPixSize()
{
    auto w_=_qvectorForData.size()*_radius+_radius;
    return {w_,0};
}

void RBtreeSimulation::makeElementsBig(int factor)
{

    _diameter +=factor;
    _radius = _diameter / 2;
    _nodeLineHeight = 3 * _diameter / 2;
    _fontSize=_radius;
}

void RBtreeSimulation::prepareReplay()
{
    Root=NIL;

    SentinelNode->_prev=SentinelNode->_next=SentinelNode;

    current=0;
    currentAnmIndex=0;
}

void RBtreeSimulation::nextFrame()
{
    if(!hasAnimation){
        nextAction();
//        hasAnimation=true;
    }else{
        auto & action=instructions[current];
        switch (action._ope) {
        case Operate::Search:{
            if(currentAnmIndex>=currentAnmNumber){
                drawCurrentNodeItem(_searchNodeItem);
                hasAnimation=false;
                current++;
            }
            QPainter pp(pix.get());
            pp.translate(0,_diameter/2);
            QFont font = pp.font();
            font.setPixelSize(_fontSize);
            pp.setFont(font);
            auto & _nodeItem=_searchNodeItem->_parent;
            pp.setPen(QPen(Qt::blue,30));
            pp.drawLine(_nodeItem->x*_diameter/2,_nodeItem->y*_nodeLineHeight,(_nodeItem->x+xLine)*_diameter/2,(_nodeItem->y+yLine)*_nodeLineHeight);
            pp.setPen(Qt::black);
            pp.setBrush(_nodeItem->color==Red?Qt::red:Qt::black);
            pp.drawEllipse(QPoint(_nodeItem->x*_diameter/2,_nodeItem->y*_nodeLineHeight),_radius+5,_radius+5);
            pp.setBrush(_searchNodeItem->color==Red?Qt::red:Qt::black);
            pp.drawEllipse(QPoint(_searchNodeItem->x*_diameter/2,_searchNodeItem->y*_nodeLineHeight),_radius,_radius);
            pp.setPen(Qt::white);
            pp.drawText(QRect(_nodeItem->x*_diameter/2-_radius,_nodeItem->y*_nodeLineHeight-_radius,_diameter,_diameter),Qt::AlignCenter,QString::number(_nodeItem->_value));
            pp.drawText(QRect(_searchNodeItem->x*_diameter/2-_radius,_searchNodeItem->y*_nodeLineHeight-_radius,_diameter,_diameter),Qt::AlignCenter,QString::number(_searchNodeItem->_value));
            currentAnmIndex++;
        }
            break;
        default:
            break;
        }
    }
}

void RBtreeSimulation::animationStart()
{

}

bool RBtreeSimulation::isOver() const
{
    return current>=currentActionNumber;
}

void RBtreeSimulation::searchANM()
{
    _searchNodeItem->_parent;
}

void RBtreeSimulation::initialTree()
{
    root=NIL=new Node<int>(-9998);
    NIL->color=Black;
    NIL->parent=NIL->left=NIL->right=NIL;

    SentinelNode=new ModelNode(-9999);
    SentinelNode->_prev=SentinelNode->_next=SentinelNode;

    Root=NIL=new ModelNode(-9998);
    NIL->color=Black;
    NIL->_parent=NIL->_left=NIL->_right=NIL;

    fakeNodeContainer.insert(NIL->_value,NIL);
}

void RBtreeSimulation::emptyTree(Node<int> *&root)
{
    if(root!=NIL)
    {
        emptyTree(root->left);
        emptyTree(root->right);
        delete root;
        root=nullptr;
    }
}

void RBtreeSimulation::rotationWithLeftChild(Node<int> *&root)
{
    Node<int> *left_child = root->left;

    root->left = left_child->right;
    left_child->right->parent = root;

    left_child->right = root;
    left_child->parent = root->parent;

    root->parent = left_child;
    root = left_child;
}

void RBtreeSimulation::rotationWithRightChild(Node<int> *&root)
{
    Node<int> *right_child = root->right;

    root->right = right_child->left;
    right_child->left->parent = root;

    right_child->left = root;
    right_child->parent = root->parent;

    root->parent = right_child;
    root = right_child;
}

bool RBtreeSimulation::insert(int x)
{
    instructions.append({Operate::NextValue,{0}});
    auto temp=root;
    //红黑树在调整时,其哨兵节点NIL的父节点在旋转过程中会改变,因此,在树进行一轮清空后,
    //此时root=NIL,即root.parent不再是NIL.所以这里不可以写成root->parent
    auto tempParent=NIL;
    while (temp!=NIL) {
        tempParent=temp;
        instructions.append({Operate::Search,{temp->item}});  //Search
        if(temp->item>x){
            temp=temp->left;
        }
        else if(temp->item<x){
            temp=temp->right;
        }
        else
            break;
    }
    if(temp!=NIL){
        instructions.append({Operate::Done,{0}});
        return false;
    }
    auto newNode=new Node<int>(x,tempParent,NIL,NIL);
    _nodeSize++;
    if(tempParent==NIL){
        root=newNode;
        instructions.append({Operate::Add,{x,tempParent->item}});
    }
    else {
        if(x<tempParent->item){
            tempParent->left=newNode;
        }
        else{
            tempParent->right=newNode;
        }
        instructions.append({Operate::Add,{x,tempParent->item}});
    }
    insertionFixUpOfDoubleRed(newNode);
    instructions.append({Operate::Done,{1}});
    return true;
}

void RBtreeSimulation::insertionFixUpOfDoubleRed(Node<int> *root)
{
    while (root->parent->color == Red) {
        if (root->parent == root->parent->parent->left) {
            if (root->parent->parent->right->color == Red) {  //case 1
                root->parent->color = Black;
                root->parent->parent->right->color = Black;
                root->parent->parent->color = Red;
                instructions.append({Operate::ChangeColor,{root->parent->item,1,
                                                              root->parent->parent->right->item,1,
                                                              root->parent->parent->item,0}});  //record
                root = root->parent->parent;
                instructions.append({Operate::Search,{root->item}}); //record
            } else {
                Node<int> *&gp = getParentReference(root->parent->parent);
                if (root == root->parent->right) {   //case 2
                    root = root->parent;
                    instructions.append({Operate::Rotate,{getParentReference(root)->item,1}});
                    rotationWithRightChild(getParentReference(root));
                }
                instructions.append({Operate::Rotate,{gp->item,0}});
                rotationWithLeftChild(gp);    //case 3
                gp->color = Black;
                gp->right->color = Red;
                instructions.append({Operate::ChangeColor,{gp->item,1,gp->right->item,0,-1,2}});
                break;
            }
        } else {
            if (root->parent->parent->left->color == Red) {
                root->parent->color = Black;
                root->parent->parent->left->color = Black;
                root->parent->parent->color = Red;
                instructions.append({Operate::ChangeColor,{root->parent->item,1,
                                                              root->parent->parent->left->item,1,
                                                              root->parent->parent->item,0}});
                root = root->parent->parent;
                instructions.append({Search,{root->item}});
            } else {
                Node<int> *&gp = getParentReference(root->parent->parent);
                if (root == root->parent->left) {
                    root = root->parent;
                    instructions.append({Rotate,{getParentReference(root)->item,0}});
                    rotationWithLeftChild(getParentReference(root));
                }
                instructions.append({Rotate,{gp->item,1}});
                rotationWithRightChild(gp);
                gp->color = Black;
                gp->left->color = Red;
                instructions.append({ChangeColor,{gp->item,1,gp->left->item,0,-1,2}});
                break;
            }
        }
    }
    this->root->color = Black;
    instructions.append({Operate::ChangeColor,{this->root->item,1,-1,2,-1,2}});
}

void RBtreeSimulation::replace(Node<int> *y, Node<int> *x)
{
    if(y==root)
        root=x;
    else if(y==y->parent->left)
        y->parent->left=x;
    else
        y->parent->right=x;
    x->parent=y->parent;
}

RBtreeSimulation::Node<int> *RBtreeSimulation::findMinValueNode(const Node<int> *root)
{
    auto temp=root;
    while (temp->left!=NIL) {
        instructions.append({Operate::Search,{temp->item}});
        temp=temp->left;
    }
    return const_cast<Node<int> *>(temp);
}

void RBtreeSimulation::remove(int v)
{
    auto temp=root;
    while (temp!=NIL) {
        instructions.append({Operate::Search,{temp->item}});  //Search
        if(temp->item>v){
            temp=temp->left;
        }
        else if(temp->item<v){
            temp=temp->right;
        }
        else
            break;
    }
    if(temp==NIL){
        instructions.append({Operate::Done,{0}});
        return ;
    }
    auto y=temp;
    auto temp1=v;
    auto yOriginalColor=y->color;
    Node<int> * x=nullptr;
    if(temp->left==NIL){
        x=temp->right;
        replace(y,x);

    }else if(temp->right==NIL){
        x=temp->left;
        replace(y,y->left);
    }else{
        auto minNode=findMinValueNode(temp->right);
        //这里使用一种简单的方式,保留源节点,仅仅替换值的方式间接删除找到的节点
        y->item=minNode->item;
        y=minNode;
        x=y->right;
        yOriginalColor=y->color;
        if(y->parent==temp)
            x->parent=y;//算法导论中这几行没看明白,暂时把形式保存在这里好了.
        //喔,调试源码偶然发现了,这一步当x为哨兵节点是很重要,因为哨兵节点也参与之后调整过程
        //向上回溯时,需要赋值root=root.parent.哨兵节点父节点每次都会变化,所以这里必须强制更新
        //这里虽然采用删除方式不同,但还是保留了算法导论中的结构
        else {
            ;
        }
        replace(y,y->right);
    }
    instructions.append({Operate::Substitute,{y->item,x->item,temp1}});
    delete y;
    y=nullptr;
    if(yOriginalColor==Black)
        removeFixUpOfLostOfBlack(x);
    instructions.append({Operate::Done,{1}});
}

void RBtreeSimulation::removeFixUpOfLostOfBlack(Node<int> *root)
{
    while (root->color == Black && root != this->root) {
        Node<int> *root_brother = nullptr;
        if (root == root->parent->left) {
            root_brother = root->parent->right;
            if (root_brother->color == Red) {	//case 1
                Node<int> *&gp = getParentReference(root->parent);
                instructions.append({Operate::Rotate,{gp->item,1}});
                rotationWithRightChild(gp);
                gp->color = Black;
                gp->left->color = Red;
                root_brother = root->parent->right;
                instructions.append({Operate::ChangeColor,{gp->item,1,gp->left->item,0,-1,2}});
            }
            if (root_brother->left->color == Black
                    && root_brother->right->color == Black) {	//case 2
                root_brother->color = Red;
                instructions.append({Operate::ChangeColor,{root_brother->item,0,-1,2,-1,2}});
                root = root->parent;
                instructions.append({Operate::Search,{root->item}});
            } else {
                Node<int> * &root_parent = getParentReference(root->parent);
                if (root_brother->right->color == Black) {	//case 3
                    instructions.append({Operate::Rotate,{root_brother->item,0}});
                    rotationWithLeftChild(root_parent->right);
                    root_parent->right->color = Black;
                    root_parent->right->right->color = Red;
                    root_brother=root_parent->right;
                    instructions.append({Operate::ChangeColor,{root_parent->right->item,1,root_parent->right->right->item,0,-1,2}});
                }
                instructions.append({Operate::Rotate,{root_parent->item,1}});
                rotationWithRightChild(root_parent);	//case 4
                root_parent->color = root_parent->left->color;
                root_parent->left->color = Black;
                root_parent->right->color = Black;
                //利用枚举默认值也是和我们的规定值相等,否则下面要判断
                instructions.append({Operate::ChangeColor,{root_parent->item,root_parent->color,root_parent->left->item,1,root_parent->right->item,1}});
                root = this->root;
                instructions.append({Operate::Search,{root->item}});
            }
        } else {
            root_brother = root->parent->left;
            if (root_brother->color == Red) {	//case 1
                Node<int> *&gp = getParentReference(root->parent);
                instructions.append({Operate::Rotate,{gp->item,0}});
                rotationWithLeftChild(gp);
                gp->color = Black;
                gp->right->color = Red;
                root_brother = root->parent->left;
                instructions.append({Operate::ChangeColor,{gp->item,1,gp->right->item,0,-1,2}});
            }
            if (root_brother->left->color == Black
                    && root_brother->right->color == Black) {	//case 2
                root_brother->color = Red;
                instructions.append({Operate::ChangeColor,{root_brother->item,0,-1,2,-1,2}});
                root = root->parent;
                instructions.append({Operate::Search,{root->item}});
            } else {
                Node<int> * &root_parent = getParentReference(root->parent);
                if (root_brother->left->color == Black) {	//case 3
                    instructions.append({Operate::Rotate,{getParentReference(root_brother)->item,1}});
                    rotationWithRightChild(root_parent->left);
                    root_parent->left->color = Black;
                    root_parent->left->left->color = Red;
                    instructions.append({Operate::ChangeColor,{root_parent->left->item,1,root_parent->left->left->item,0,-1,2}});
                }
                instructions.append({Operate::Rotate,{root_parent->item,0}});
                rotationWithLeftChild(root_parent);	//case 4
                root_parent->color = root_parent->right->color;
                root_parent->left->color = Black;
                root_parent->right->color = Black;
                instructions.append({Operate::ChangeColor,{root_parent->item,root_parent->color,root_parent->left->item,1,root_parent->right->item,1}});
                root = this->root;
                instructions.append({Operate::Search,{root->item}});
            }
        }
    }
    root->color = Black;
    instructions.append({Operate::ChangeColor,{root->item,1,-1,2,-1,2}});
}

void RBtreeSimulation::dispatchActionAndDraw()
{
    auto & action=instructions[current];
    switch (action._ope) {
        case Operate::Search:{
                auto x=search(action);
//                drawCurrentNodeItem(x);
                currentAnmNumber=10;
                xLine=(_searchNodeItem->x-_searchNodeItem->xParent)/currentAnmNumber;
                yLine=(_searchNodeItem->y-_searchNodeItem->yParent)/currentAnmNumber;
                currentAnmIndex=0;
                hasAnimation=true;
            }
            break;
        case Operate::Add:
            add(action);
            drawAllElement();
            current++;
            break;
        case Operate::Rotate:
            rotate(action);
            drawAllElement();
            current++;
            break;
        case Operate::Substitute:
            substitute(action);
            drawAllElement();
            current++;
            break;
        case Operate::ChangeColor:{
                recolorNodeItem(changeColor(action));
            }
        current++;
            break;
        case Operate::NextValue:
            showNextValue();
            current++;
            break;
        case Operate::Done:
            done(action);
            drawAllElement();
            current++;
            break;
    }
}

//深度优先
void RBtreeSimulation::drawAllElement(QPainter &_painter, RBtreeSimulation::ModelNode *_nodeItem) const
{
    if(_nodeItem!=NIL){
        drawAllElement(_painter,_nodeItem->_left);
        drawAllElement(_painter,_nodeItem->_right);
        //draw line
        if(_nodeItem->xParent>0)
            _painter.drawLine(_nodeItem->x*_diameter/2,_nodeItem->y*_nodeLineHeight,_nodeItem->xParent*_diameter/2,_nodeItem->yParent*_nodeLineHeight);
        //draw cycle
        QPainterPath myPath;
        myPath.addEllipse(QPoint(_nodeItem->x*_diameter/2,_nodeItem->y*_nodeLineHeight),_radius,_radius);
        _painter.drawPath(myPath);
        _painter.fillPath(myPath,_nodeItem->color==Red?Qt::red:Qt::black);
        //draw text
        _painter.setPen(Qt::white);
        _painter.drawText(QRect(_nodeItem->x*_diameter/2-_radius,_nodeItem->y*_nodeLineHeight-_radius,_diameter,_diameter),Qt::AlignCenter,QString::number(_nodeItem->_value));
        _painter.setPen(Qt::black);
    }
}

void RBtreeSimulation::drawAllElement() const
{
    pix->fill();
    QPainter pp(pix.get());
    pp.translate(0,_diameter/2);
    QFont font = pp.font();
    font.setPixelSize(_fontSize);
    pp.setFont(font);
    drawAllElement(pp,Root);
}

void RBtreeSimulation::drawCurrentNodeItem(RBtreeSimulation::ModelNode *_nodeItem) const
{
    pix->fill();
    QPainter pp(pix.get());
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

void RBtreeSimulation::paintColor(RBtreeSimulation::ModelNode *root, QPainter &pp, int dx) const
{
    if(root==nullptr)
        return;
    QPainterPath myPath;
    if(_searchNodeItem==root)
        dx=5;
    myPath.addEllipse(QPoint(root->x*_diameter/2,root->y*_nodeLineHeight),_radius+dx,_radius+dx);
    pp.drawPath(myPath);
    pp.fillPath(myPath,root->color==Red?Qt::red:Qt::black);
    pp.setPen(Qt::white);
    pp.drawText(QRect(root->x*_diameter/2-_radius,root->y*_nodeLineHeight-_radius,_diameter,_diameter),Qt::AlignCenter,QString::number(root->_value));
    pp.setPen(Qt::black);
}

void RBtreeSimulation::animation()
{

}

RBtreeSimulation::ModelNode *RBtreeSimulation::search(RBtreeSimulation::Instruction &action)
{
    //保存当前正在查找的节点,在染色时会考虑相应变大一点
    _searchNodeItem=fakeNodeContainer.value(action.array[0]);
    return _searchNodeItem;
}

void RBtreeSimulation::add(RBtreeSimulation::Instruction &action)
{
    auto _parentNode=fakeNodeContainer.value(action.array[1]);
    ModelNode * _newNode=nullptr;
    if(fakeNodeContainer.contains(action.array[0])){
        _newNode=fakeNodeContainer.value((action.array[0]));
        _newNode->_left=NIL;
        _newNode->_right=NIL;
        _newNode->_parent=_parentNode;
        _newNode->init();
    }else{
        _newNode=new ModelNode(action.array[0],NIL,NIL,_parentNode);
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

void RBtreeSimulation::rotate(RBtreeSimulation::Instruction &action)
{
    auto _currentNodeItem=fakeNodeContainer.value(action.array[0]);
    if(action.array[1]==0)
        rotationWithLeftChildForNodeItem(getParentReferenceForNodeItem(_currentNodeItem));
    else
        rotationWithRightChildForNodeItem(getParentReferenceForNodeItem(_currentNodeItem));
    setY();
}

Util::TupleWrapArray<RBtreeSimulation::ModelNode*,3> RBtreeSimulation::changeColor(RBtreeSimulation::Instruction &action)
{
    int i=0;
    ModelNode * a[3]={};
    while(i<3){
        auto colorValue=action.array[2*i+1];
        if(colorValue>1)
            break;
        a[i]=fakeNodeContainer.value(action.array[2*i]);
        a[i]->color=colorValue==0?Red:Black;
        i++;
    }
    // 等价于return std::make_tuple(a[0],a[1],a[2]);
    return Util::make_TupleForArray(a);
}

void RBtreeSimulation::showNextValue()
{

}

void RBtreeSimulation::substitute(RBtreeSimulation::Instruction &action)
{
    auto y=fakeNodeContainer.value(action.array[0]);
    deleteNodeItemFromLinkedList(y);
    fakeNodeContainer.remove(y->_value);
    auto x=fakeNodeContainer.value(action.array[1]);
    if(action.array[0]!=action.array[2]){
        auto yOriginal=fakeNodeContainer.value(action.array[2]);
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

void RBtreeSimulation::done(RBtreeSimulation::Instruction &action)
{
    Q_UNUSED(action)
}

void RBtreeSimulation::fillPropertyInInsert(RBtreeSimulation::ModelNode *_nodeItem)
{
    //insert value into linkedlist-ordered and update x coordinate
    insertNodeItemIntoLinkedList(_nodeItem);
    //set right infomation left without x by level-order
    setY();
}

void RBtreeSimulation::insertNodeItemIntoLinkedList(RBtreeSimulation::ModelNode *_nodeItem)
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

void RBtreeSimulation::deleteNodeItemFromLinkedList(RBtreeSimulation::ModelNode *_nodeItem)
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

void RBtreeSimulation::setY()
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

void RBtreeSimulation::rotationWithLeftChildForNodeItem(RBtreeSimulation::ModelNode *&root)
{
    auto left_child = root->_left;

    root->_left = left_child->_right;
    left_child->_right->_parent = root;

    left_child->_right = root;
    left_child->_parent = root->_parent;

    root->_parent = left_child;
    root = left_child;
}

void RBtreeSimulation::rotationWithRightChildForNodeItem(RBtreeSimulation::ModelNode *&root)
{
    auto right_child = root->_right;

    root->_right = right_child->_left;
    right_child->_left->_parent = root;

    right_child->_left = root;
    right_child->_parent = root->_parent;

    root->_parent = right_child;
    root = right_child;
}

void RBtreeSimulation::recolorNodeItem(Util::TupleWrapArray<ModelNode *, 3> tuple_) const
{
    QPainter pp(pix.get());
    pp.translate(0,_diameter/2);
    QFont font = pp.font();
    font.setPixelSize(_fontSize);
    pp.setFont(font);
    auto [a,b,c]=tuple_;
    paintColor(a,pp);
    paintColor(b,pp);
    paintColor(c,pp);
}

