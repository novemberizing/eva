#ifndef   __NOVEMBERIZING_X__COMMAND__H__
#define   __NOVEMBERIZING_X__COMMAND__H__

#include <x/std.h>

struct xeventengine;
struct xeventsubscription;
typedef struct xeventengine xeventengine;
typedef struct xeventsubscription xeventsubscription;

struct xcommand;
struct xcommandset;

typedef struct xcommand xcommand;
typedef struct xcommandset xcommandset;

typedef void (*xcommandexec)(xcommand *);

struct xcommand
{
    const xcommandset * set;

    

    struct {
        xeventengine * engine;
        
        xeventsubscription * prev;
        xeventsubscription * next;
    } subscription;

    xcommandexec exec;
};

struct xcommandset
{
    xcommand * (*del)(xcommand *);
};

extern xcommand * xcommandNew(xcommandexec exec, const xcommandset * set, xuint64 size);

#define xcommandDel(o)      (o->set->del(o))

#define xcommandExec(o)     (o->set->exec(o))

#endif // __NOVEMBERIZING_X__COMMAND__H__
