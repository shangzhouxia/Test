#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_BINDING_NUMBER 10

//观察者：当接收到被观察者的动作后，自己做出相应的反应
typedef struct _Observer Observer;
struct _Observer {
    int type;
    void ( *update )( struct _Observer *pObserver , const char* str);
};


//被观察者：可以增加观察者、删除观察者、通知观察者
typedef struct _Object Object;
struct _Object {
    Observer *pObserverList[MAX_BINDING_NUMBER];

    //观察者数量计数
    int number;
    void (*notify)(Object* pObject , const char* str);
    //增加一个观察者
    void (*addObserver) (Object* pObject, Observer* pObserver);
    //删除一个观察者
    void (*delObserver) (Object* pObject, Observer* pObserver);
};

#define LOWEND_TYPE  0x01
#define HIGHEND_TYPE 0x02
void notify(Object* pObject , const char* str)
{
    Observer *pObserver;
    int index;

    assert( NULL != pObject );
    for ( index = 0; index < pObject->number; index++ ) {
        pObserver = pObject->pObserverList[index];
        pObserver->update( pObserver ,str);
        //pObject->pObserverList[index]->update(pObject->pObserverList[index]);
    }
}

void AddObserver(Object* pObject, Observer* pObserver)
{
    int i=0;
    if (NULL == pObject || NULL == pObserver) {
        return;
    }
    //for (i=0;i<=pObject->number;i++) {
        if (NULL == pObject->pObserverList[pObject->number]) {
            pObject->pObserverList[pObject->number] = pObserver;
            pObject->number ++;
        }
    //}
}

void DelObserver(Object* pObject, Observer* pObserver)
{
    int i=0;

    if (NULL == pObject || NULL == pObserver) {
        return;
    }
    for (i=0;i<MAX_BINDING_NUMBER;i++) {
        if (pObserver == pObject->pObserverList[i]) {
            pObject->pObserverList[i] = NULL;
            pObject->number --;
        }
    }
}


void lowend_type_update( Observer* observer , const char* str)
{
    observer->type = LOWEND_TYPE;
    printf( "%s %d type:%d\r\n%s\n", __func__, __LINE__, observer->type ,str);
}
void highend_type_update( Observer* observer , const char* str)
{
    observer->type = HIGHEND_TYPE;
    printf( "%s %d type:%d\r\n%s\n", __func__, __LINE__, observer->type, str);
}

int main( void )
{
    //实例化一个观察者，并实现update之后的动作
    Observer lowend_observer;
    lowend_observer.update = lowend_type_update;

    Observer highend_observer;
    highend_observer.update = highend_type_update;

    //定义一个被观察者
    Object object;
/*     object.number = 2;
    object.pObserverList[0] = &lowend_observer;
    object.pObserverList[1] = &highend_observer; */
    object.addObserver = AddObserver;
    object.delObserver = DelObserver;

    //实例化具体的被观察者
    object.notify = notify;

    object.pObserverList[0] = NULL;
    object.pObserverList[1] = NULL;

    //增加观察者
    object.addObserver(&object, &lowend_observer);
    object.addObserver(&object, &highend_observer);

    //通知观察者
    object.notify( &object , "add 2 observer");

    //删除一个观察者
    object.delObserver(&object, &highend_observer);
    object.notify( &object ,"del highend_observer");

    object.addObserver(&object, &highend_observer);
    object.notify( &object , "add highend_observer");
}