#include "rbdata.h"
#include<queue>
#include<stdexcept>

RbData::RbData()
{
//    registerMethod(MethodA::AddMuch,[](){
//        for(int i=0;i<3;i++){
//            std::cout<<"添加许多 "<<i+1<<std::endl;
//            QThread::sleep(1);
//        }
//        std::cout<<"添加许多结束"<<std::endl;
//    });
    registerMethod(MethodA::Insert,[this](){insert();});
    registerMethod(MethodA::Remove,[this](){remove();});
    initialTree();
}

RbData::~RbData()
{
    emptyTree(root);
    delete NIL;
}

void RbData::prepareWorks()
{
    output.clear();
    root=NIL;
    NIL->color=Black;
    NIL->parent=NIL->left=NIL->right=NIL;
    xNodeNumber=0;
}

int RbData::treeMaxNumberY()
{
    Node<int> * temp=nullptr;
    std::queue<Node<int> *> NodeQueue;
    NodeQueue.push(root);
    int level=0;
    int column=NodeQueue.size();
    while (NodeQueue.size()>0) {
        temp=NodeQueue.front();
        NodeQueue.pop();
        column--;
        if(temp->left!=NIL)
            NodeQueue.push(temp->left);
        if(temp->right!=NIL)
            NodeQueue.push(temp->right);
        if(column==0){
            level++;
            column=NodeQueue.size();
        }
    }
    return level;
}

int RbData::treeMaxNumberX()
{
    if(status()==FAStatus::GodJob)
        return xNodeNumber<0?0:xNodeNumber;
    throw  std::runtime_error("cannot get value yNodeNumber");
}

void RbData::initialTree()
{
    root=NIL=new Node<int>(-9998);
    NIL->color=Black;
    NIL->parent=NIL->left=NIL->right=NIL;
}

void RbData::emptyTree(Node<int> *&root)
{
    if(root!=NIL)
    {
        emptyTree(root->left);
        emptyTree(root->right);
        delete root;
        root=nullptr;
    }
}

void RbData::rotationWithLeftChild(Node<int> *&root)
{
    Node<int> *left_child = root->left;

    root->left = left_child->right;
    left_child->right->parent = root;

    left_child->right = root;
    left_child->parent = root->parent;

    root->parent = left_child;
    root = left_child;
}

void RbData::rotationWithRightChild(Node<int> *&root)
{
    Node<int> *right_child = root->right;

    root->right = right_child->left;
    right_child->left->parent = root;

    right_child->left = root;
    right_child->parent = root->parent;

    root->parent = right_child;
    root = right_child;
}

bool RbData::insert(int x)
{
    output.push_back({Operate::NextValue,{0}});
    auto temp=root;
    //红黑树在调整时,其哨兵节点NIL的父节点在旋转过程中会改变,因此,在树进行一轮清空后,
    //此时root=NIL,即root.parent不再是NIL.所以这里不可以写成root->parent
    auto tempParent=NIL;
    while (temp!=NIL) {
        tempParent=temp;
        output.push_back({Operate::Search,{temp->item}});  //Search
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
        output.push_back({Operate::Done,{0}});
        return false;
    }
    auto newNode=new Node<int>(x,tempParent,NIL,NIL);
    xNodeNumber++;
    if(tempParent==NIL){
        root=newNode;
        output.push_back({Operate::Add,{x,tempParent->item}});
    }
    else {
        if(x<tempParent->item){
            tempParent->left=newNode;
        }
        else{
            tempParent->right=newNode;
        }
        output.push_back({Operate::Add,{x,tempParent->item}});
    }
    insertionFixUpOfDoubleRed(newNode);
    output.push_back({Operate::Done,{1}});
    return true;
}

void RbData::insertionFixUpOfDoubleRed(Node<int> *root)
{
    while (root->parent->color == Red) {
        if (root->parent == root->parent->parent->left) {
            if (root->parent->parent->right->color == Red) {  //case 1
                root->parent->color = Black;
                root->parent->parent->right->color = Black;
                root->parent->parent->color = Red;
                output.push_back({Operate::ChangeColor,{root->parent->item,1,
                                                              root->parent->parent->right->item,1,
                                                              root->parent->parent->item,0}});  //record
                root = root->parent->parent;
                output.push_back({Operate::Search,{root->item}}); //record
            } else {
                Node<int> *&gp = getParentReference(root->parent->parent);
                if (root == root->parent->right) {   //case 2
                    root = root->parent;
                    output.push_back({Operate::Rotate,{getParentReference(root)->item,1}});
                    rotationWithRightChild(getParentReference(root));
                }
               output.push_back({Operate::Rotate,{gp->item,0}});
                rotationWithLeftChild(gp);    //case 3
                gp->color = Black;
                gp->right->color = Red;
               output.push_back({Operate::ChangeColor,{gp->item,1,gp->right->item,0,-1,2}});
                break;
            }
        } else {
            if (root->parent->parent->left->color == Red) {
                root->parent->color = Black;
                root->parent->parent->left->color = Black;
                root->parent->parent->color = Red;
                output.push_back({Operate::ChangeColor,{root->parent->item,1,
                                                              root->parent->parent->left->item,1,
                                                              root->parent->parent->item,0}});
                root = root->parent->parent;
                output.push_back({Search,{root->item}});
            } else {
                Node<int> *&gp = getParentReference(root->parent->parent);
                if (root == root->parent->left) {
                    root = root->parent;
                    output.push_back({Rotate,{getParentReference(root)->item,0}});
                    rotationWithLeftChild(getParentReference(root));
                }
                output.push_back({Rotate,{gp->item,1}});
                rotationWithRightChild(gp);
                gp->color = Black;
                gp->left->color = Red;
                output.push_back({ChangeColor,{gp->item,1,gp->left->item,0,-1,2}});
                break;
            }
        }
    }
    this->root->color = Black;
    output.push_back({Operate::ChangeColor,{this->root->item,1,-1,2,-1,2}});
}

void RbData::remove(int v)
{
    auto temp=root;
    while (temp!=NIL) {
        output.push_back({Operate::Search,{temp->item}});  //Search
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
        output.push_back({Operate::Done,{0}});
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
    output.push_back({Operate::Substitute,{y->item,x->item,temp1}});
    delete y;
    y=nullptr;
    xNodeNumber--;
    if(yOriginalColor==Black)
        removeFixUpOfLostOfBlack(x);
    output.push_back({Operate::Done,{1}});
}

void RbData::removeFixUpOfLostOfBlack(Node<int> *root)
{
    while (root->color == Black && root != this->root) {
        Node<int> *root_brother = nullptr;
        if (root == root->parent->left) {
            root_brother = root->parent->right;
            if (root_brother->color == Red) {	//case 1
                Node<int> *&gp = getParentReference(root->parent);
                output.push_back({Operate::Rotate,{gp->item,1}});
                rotationWithRightChild(gp);
                gp->color = Black;
                gp->left->color = Red;
                root_brother = root->parent->right;
                output.push_back({Operate::ChangeColor,{gp->item,1,gp->left->item,0,-1,2}});
            }
            if (root_brother->left->color == Black
                    && root_brother->right->color == Black) {	//case 2
                root_brother->color = Red;
                output.push_back({Operate::ChangeColor,{root_brother->item,0,-1,2,-1,2}});
                root = root->parent;
                output.push_back({Operate::Search,{root->item}});
            } else {
                Node<int> * &root_parent = getParentReference(root->parent);
                if (root_brother->right->color == Black) {	//case 3
                    output.push_back({Operate::Rotate,{root_brother->item,0}});
                    rotationWithLeftChild(root_parent->right);
                    root_parent->right->color = Black;
                    root_parent->right->right->color = Red;
                    root_brother=root_parent->right;
                    output.push_back({Operate::ChangeColor,{root_parent->right->item,1,root_parent->right->right->item,0,-1,2}});
                }
                output.push_back({Operate::Rotate,{root_parent->item,1}});
                rotationWithRightChild(root_parent);	//case 4
                root_parent->color = root_parent->left->color;
                root_parent->left->color = Black;
                root_parent->right->color = Black;
                //利用枚举默认值也是和我们的规定值相等,否则下面要判断
                output.push_back({Operate::ChangeColor,{root_parent->item,root_parent->color,root_parent->left->item,1,root_parent->right->item,1}});
                root = this->root;
                output.push_back({Operate::Search,{root->item}});
            }
        } else {
            root_brother = root->parent->left;
            if (root_brother->color == Red) {	//case 1
                Node<int> *&gp = getParentReference(root->parent);
                output.push_back({Operate::Rotate,{gp->item,0}});
                rotationWithLeftChild(gp);
                gp->color = Black;
                gp->right->color = Red;
                root_brother = root->parent->left;
                output.push_back({Operate::ChangeColor,{gp->item,1,gp->right->item,0,-1,2}});
            }
            if (root_brother->left->color == Black
                    && root_brother->right->color == Black) {	//case 2
                root_brother->color = Red;
                output.push_back({Operate::ChangeColor,{root_brother->item,0,-1,2,-1,2}});
                root = root->parent;
                output.push_back({Operate::Search,{root->item}});
            } else {
                Node<int> * &root_parent = getParentReference(root->parent);
                if (root_brother->left->color == Black) {	//case 3
                    output.push_back({Operate::Rotate,{getParentReference(root_brother)->item,1}});
                    rotationWithRightChild(root_parent->left);
                    root_parent->left->color = Black;
                    root_parent->left->left->color = Red;
                    output.push_back({Operate::ChangeColor,{root_parent->left->item,1,root_parent->left->left->item,0,-1,2}});
                }
                output.push_back({Operate::Rotate,{root_parent->item,0}});
                rotationWithLeftChild(root_parent);	//case 4
                root_parent->color = root_parent->right->color;
                root_parent->left->color = Black;
                root_parent->right->color = Black;
                output.push_back({Operate::ChangeColor,{root_parent->item,root_parent->color,root_parent->left->item,1,root_parent->right->item,1}});
                root = this->root;
                output.push_back({Operate::Search,{root->item}});
            }
        }
    }
    root->color = Black;
    output.push_back({Operate::ChangeColor,{root->item,1,-1,2,-1,2}});
}

void RbData::replace(Node<int> *y, Node<int> *x)
{
    if(y==root)
        root=x;
    else if(y==y->parent->left)
        y->parent->left=x;
    else
        y->parent->right=x;
    x->parent=y->parent;
}

RbData::Node<int> *RbData::findMinValueNode(const Node<int> *root)
{
    auto temp=root;
    while (temp->left!=NIL) {
        output.push_back({Operate::Search,{temp->item}});
        temp=temp->left;
    }
    return const_cast<Node<int> *>(temp);
}
