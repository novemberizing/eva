#ifndef   __NOVEMBERIZING_X_SERVER__SOCKET__H__
#define   __NOVEMBERIZING_X_SERVER__SOCKET__H__

#include <x/socket.h>
#include <x/object.h>
#include <x/session/socket/pool.h>

struct xsessionsocket;
struct xsessionsocketpool;

typedef struct xsessionsocket xsessionsocket;
typedef struct xsessionsocketpool xsessionsocketpool;

#define xserversocket_invalid_value         xsocket_invalud_value

#define xserversocketmode_nonblock          xsocketmode_nonblock
#define xserversocketmode_reuseaddr         xsocketmode_reuseaddr

#define xserversocketshutdown_in            xsocketshutdown_in
#define xserversocketshutdown_out           xsocketshutdown_out
#define xserversocketshutdown_all           xsocketshutdown_all

#define xserversocketevent_none             xsocketevent_none
#define xserversocketevent_open             xsocketevent_open
#define xserversocketevent_in               xsocketevent_in
#define xserversocketevent_out              xsocketevent_out
#define xserversocketevent_close            xsocketevent_close
#define xserversocketevent_connect          xsocketevent_open
#define xserversocketevent_error            xsocketevent_error
#define xserversocketevent_release          xsocketevent_release

#define xserversocketstatus_none            xsocketstatus_none
#define xserversocketstatus_open            xsocketstatus_open
#define xserversocketstatus_in              xsocketstatus_in
#define xserversocketstatus_out             xsocketstatus_out
#define xserversocketstatus_close           xsocketstatus_close
#define xserversocketstatus_connect         xsocketstatus_open
#define xserversocketstatus_error           xsocketstatus_error
#define xserversocketstatus_release         xsocketstatus_release

#define xserversocketevent_type_none        xsocketevent_type_none
#define xserversocketevent_type_open        xsocketevent_type_open
#define xserversocketevent_type_in          xsocketevent_type_in
#define xserversocketevent_type_out         xsocketevent_type_out
#define xserversocketevent_type_close       xsocketevent_type_close
#define xserversocketevent_type_error       xsocketevent_type_error
#define xserversocketevent_type_release     xsocketevent_type_release

struct xserversocket;
struct xserversocketset;

typedef struct xserversocket xserversocket;
typedef struct xserversocketset xserversocketset;

struct xserversocket
{
    const xserversocketset * set;

    xsync * sync;
    xint32 value;
    xuint32 status;
    xuint32 mode;

    xint32 domain;
    xint32 type;
    xint32 protocol;

    xobject address;

    xsessionsocketpool * sessionsocketpool;

    xsessionsocket * head;
    xsessionsocket * tail;
    xuint64 size;
};

struct xserversocketset
{
    xserversocket * (*del)(xserversocket *);
    xint32 (*val)(xserversocket *);
    xint32 (*open)(xserversocket *);
    xint64 (*read)(xserversocket *);
    xint64 (*write)(xserversocket *);
    xint32 (*close)(xserversocket *);
    xuint32 (*interest)(xserversocket *);
    xint32 (*shutdown)(xserversocket *, xint32);

    xsessionsocket * (*accept)(xserversocket *);
    void (*release)(xserversocket *, xsessionsocket *);
};

extern xserversocket * xserversocketNew(xint32 value, xint32 domain, xint32 type, xint32 protocol, const void * address, xuint64 addresslen);

#define xserversocketDel(o)                     (o->set->del(o))
#define xserversocketVal(o)                     (o->set->val(o))
#define xserversocketOpen(o)                    (o->set->open(o))
#define xserversocketRead(o)                    (o->set->read(o))
#define xserversocketWrite(o)                   (o->set->write(o))
#define xserversocketClose(o)                   (o->set->close(o))
#define xserversocketInterest(o)                (o->set->interest(o))
#define xserversocketShutdown(o, how)           (o->set->shutdown(o, how))
#define xserversocketAccept(o)                  (o->set->accept(o))
#define xserversocketRel(o, sessionsocket)      (o->set->release(o, sessionsocket))

#define xserversocketSetMode(o, value)          (o->mode = o->mode | value)

#endif // __NOVEMBERIZING_X_SERVER__SOCKET__H__
