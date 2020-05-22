#ifndef RBDATA_H
#define RBDATA_H
#include"wrap_far_away.h"

class RbData : public WrapFarAway
{
public:
    RbData();
    ~RbData();    
    // FarAway interface
public:
    virtual void prepareWorks() override;

    int treeMaxNumberY();

    int treeMaxNumberX();

    //只有无作用域枚举可以自动类型转换为底层类型
    enum MethodA:int{Insert=0,Remove};
private:
    //***********************************红黑树开始********************
    //红黑树颜色
    enum Color :int { Red, Black };
    //红黑树节点
    template <typename T>
    class Node {
    public:
        // for rbtree
        T item;
        Node<T> *parent;
        Node<T> *left;
        Node<T> *right;
        Color color;

        Node(const T &_item, Node<T> *_parent = nullptr, Node<T> *_left = nullptr,
             Node<T> *_right = nullptr, Color _Color = Red)
            : item(_item),
              parent(_parent),
              left(_left),
              right(_right),
              color(_Color) {}
    };

    //红黑树节点类
    Node<int> *root;        //红黑树根节点
    Node<int> *NIL;     //红黑树的哨兵节点

    //红黑树
    void initialTree();
    void emptyTree(Node<int> * &root);
    void rotationWithLeftChild(Node<int> *&root);
    void rotationWithRightChild(Node<int> *&root);
    bool insert(int );
    void insertionFixUpOfDoubleRed(Node<int> *root);
    void remove(int );
    void removeFixUpOfLostOfBlack(Node<int> *root);
    void replace(Node<int> *y, Node<int> *x);
    Node<int> * findMinValueNode(const Node<int> * root);
    Node<int> *&getParentReference(Node<int> *child) {
        if (child->parent == NIL) return this->root;
        return child == child->parent->left ? child->parent->left
                                            : child->parent->right;
    }
    //****************************红黑树结束*****************

    enum Operate : int{Search,Add,Rotate,ChangeColor,NextValue,Substitute,Done};
    /* 下面是Instruction规则的说明
     * 1.增加节点( Add)
     *      索引0:新节点值
     *      索引1:父节点值
     * 2.旋转(Rotate)
     *      索引0:父节点值
     *      索引1:值为0与左孩子旋转,值为1与右孩子旋转
     * 3.变色(ChangeColor)
     *      0是红色,值1是黑色.索引5的值大于1表示不存在当前节点
     *      索引0:当前节点值,索引1:当前节点值颜色
     *      索引2:当前节点值,索引3:当前节点值颜色
     *      索引4:当前节点值,索引5:当前节点值颜色
     *      三个键值对组成
     * 4.查找过程(Search)
     *      索引0:当前节点值
     * 5.下一个值(NextValue)
     *      索引0值固定为0
     * 6.插入或删除完成(Done)
     *      索引0值:当成功时为1,失败时0
     * 7.替换某个节点(Substitute),并且删除那个节点
     *      索引0:被替换的节点,y
     *      索引1:将要用来替换的新节点,x
     *      索引2:被替换节点的原始节点,开始赋值的y
     *  注意:这种方式的作用不好说清,是不是换一种麻烦的代码但可读性较高的算法呢?
     */
private:
    void insert()
    {
        auto & v=input[index];       
        for(int i=0;i<v.dataLength;i++)
            insert((v.data)[i]);
    }

    void remove()
    {
        auto & v=input[index];
        for(int i=0;i<v.dataLength;i++)
            remove((v.data)[i]);
    }

    int xNodeNumber{0};
};

#endif // RBDATA_H
