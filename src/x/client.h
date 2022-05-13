#ifndef   __NOVEMBERIZING_X__CLIENT__H__
#define   __NOVEMBERIZING_X__CLIENT__H__

#include <x/client/socket.h>

struct xclient;
struct xclientset;

typedef struct xclient xclient;
typedef struct xclientset xclientset;

struct xclienteventsubscription;

typedef struct xclienteventsubscription xclienteventsubscription;

struct xclient
{
    const xclientset * set;

    xclienteventsubscription * subscription;
    xsync * sync;
    xint32 value;
    xuint32 status;
    xuint32 mode;

    xint32 domain;
    xint32 type;
    xint32 protocol;

    xobject address;

    struct {
        xstream * in;
        xstream * out;
    } stream;
};

struct xclientset
{
    xclient * (*del)(xclient *);

    xint32 (*open)(xclient *);
    xint64 (*read)(xclient *);
    xint64 (*write)(xclient *);
    xint32 (*close)(xclient *);
    xuint32 (*interest)(xclient *);

    xint32 (*shutdown)(xclient *, xint32);

    xint32 (*connect)(xclient *);
    xint64 (*send)(xclient *, const unsigned char *, xuint64);
    xint64 (*recv)(xclient *, unsigned char *, xuint64);
};

extern xclient * xclientNew(xint32 domain, xint32 type, xint32 protocol, const void * address, xuint64 addressLen);

#define xclientDel(o)                       (o->set->del(o))

#define xclientOpen(o)                      (o->set->open(o))
#define xclientRead(o)                      (o->set->read(o))
#define xclientWrite(o)                     (o->set->write(o))
#define xclientClose(o)                     (o->set->close(o))
#define xclientInterest(o)                  (o->set->interest(o))

#define xclientShutdown(o, how)             (o->set->shutdown(o, how))

#define xclientConnect(o)                   (o->set->connect(o))
#define xclientSend(o, message, length)     (o->set->send(o, message, length))
#define xclientRecv(o, buffer, length)      (o->set->recv(o, buffer, length))

#endif // __NOVEMBERIZING_X__CLIENT__H__
