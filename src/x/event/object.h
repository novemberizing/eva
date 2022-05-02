#ifndef   __NOVEMBERIZING_X_EVENT__OBJECT__H__
#define   __NOVEMBERIZING_X_EVENT__OBJECT__H__

#include <x/event.h>

struct xeventengine;
struct xeventobject;
struct xeventobjectset;

typedef struct xeventobject xeventobject;
typedef struct xeventobjectset xeventobjectset;



struct xeventobject
{
    const xeventobjectset * set;

    struct {
        xeventengine * engine;
        
        xeventsubscription * prev;
        xeventsubscription * next;
    } subscription;
};

struct xeventobjectset
{
    xeventobject * (*del)(xeventobject *);
};

#endif // __NOVEMBERIZING_X_EVENT__OBJECT__H__
