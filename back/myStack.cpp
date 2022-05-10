//////////////////////////////请先不要管本文件

//#include "backend/myStack.h"

//template<typename T>
//StackNode<T>::StackNode(T Element)
//{
//    element=Element;lastPtr=nullptr;nextPtr=nullptr;
//}

//template<typename T>
//myStack<T>::myStack()
//{
//    top=0;size=0;
//    headPtr=nullptr;tailPtr=nullptr;nowPos=nullptr;
//}

//template<typename T>
//void myStack<T>::push(T Element)
//{
//    if(hasNextElement())return;//只有下一个元素不存在时，才可以push
//    if(isEmpty()) //原先为空链表，新到来的元素将是链表中的第一个元素
//    {
//        StackNode<T>* p=new StackNode<T>(Element);
//        headPtr=tailPtr=nowPos=p;
//        top++;
//        size++;
//        return;
//    }
//    StackNode<T>* p=new StackNode<T>(Element);
//    //将新节点加入链表尾部
//    tailPtr->nextPtr=p;
//    p->lastPtr=tailPtr;
//    tailPtr=nowPos=p;
//    //此时top==size
//    top++;
//    size++;
//}

//template<typename T>
//bool myStack<T>::isEmpty()
//{
//    return size==0;
//}

//template<typename T>
//bool myStack<T>::hasNextElement()
//{
//    return top<size;
//}

//template<typename T>
//bool myStack<T>::hasLastElement()
//{
//    return top>1;
//}

//template<typename T>
//T myStack<T>::findLastElement()
//{

//}
