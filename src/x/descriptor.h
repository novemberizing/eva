#ifndef   __NOVEMBERIZING_X__DESCRIPTOR__H__
#define   __NOVEMBERIZING_X__DESCRIPTOR__H__

#include <x/sync.h>

#include <x/event.h>

#define xdescriptor_invalid_value           (-1)

#define xdescriptormode_nonblock            (0x00000001U << 0U)

#define xdescriptorevent_none               (0x00000000U << 0U)
#define xdescriptorevent_open               (0x00000001U << 0U)
#define xdescriptorevent_in                 (0x00000001U << 1U)
#define xdescriptorevent_out                (0x00000001U << 2U)
#define xdescriptorevent_close              (0x00000001U << 3U)
#define xdescriptorevent_error              (0x00000001U << 4U)
#define xdescriptorevent_release            (0x00000001U << 5U)

#define xdescriptorstatus_none              (0x00000000U << 0U)
#define xdescriptorstatus_open              (0x00000001U << 0U)
#define xdescriptorstatus_in                (0x00000001U << 1U)
#define xdescriptorstatus_out               (0x00000001U << 2U)
#define xdescriptorstatus_close             (0x00000001U << 3U)
#define xdescriptorstatus_error             (0x00000001U << 4U)
#define xdescriptorstatus_release           (0x00000001U << 5U)

#define xdescriptorevent_type_none          (0x00000000U << 0U)
#define xdescriptorevent_type_open          (0x00000001U << 0U)
#define xdescriptorevent_type_in            (0x00000001U << 1U)
#define xdescriptorevent_type_out           (0x00000001U << 2U)
#define xdescriptorevent_type_close         (0x00000001U << 3U)
#define xdescriptorevent_type_error         (0x00000001U << 4U)
#define xdescriptorevent_type_release       (0x00000001U << 5U)

struct xdescriptoreventsubscription;
struct xdescriptoreventgenerator;

typedef struct xdescriptoreventsubscription xdescriptoreventsubscription;
typedef struct xdescriptoreventgenerator xdescriptoreventgenerator;

struct xdescriptor;
struct xdescriptorset;

typedef struct xdescriptor xdescriptor;
typedef struct xdescriptorset xdescriptorset;

typedef void (*xdescriptoreventhandler)(xdescriptor *, xdescriptoreventsubscription *, xuint32);

struct xdescriptorset
{
    xdescriptor * (*del)(xdescriptor *);
    xint32 (*open)(xdescriptor *);
    xint64 (*read)(xdescriptor *);
    xint64 (*write)(xdescriptor *);
    xint32 (*close)(xdescriptor *);
    xuint32 (*interest)(xdescriptor *);
};

struct xdescriptor
{
    const xdescriptorset * set;

    xsync * sync;
    xint32  value;
    xuint32 status;
    xuint32 mode;
   
};

extern xdescriptor * xdescriptorNew(xint32 value, const xdescriptorset * set, xuint64 size);

#define xdescriptorDel(o)               (o->set->del(o))
#define xdescriptorOpen(o)              (o->set->open(o))
#define xdescriptorRead(o)              (o->set->read(o))
#define xdescriptorWrite(o)             (o->set->write(o))
#define xdescriptorClose(o)             (o->set->close(o))
#define xdescriptorInterestEvent(o)     (o->set->interest(o))

#endif // __NOVEMBERIZING_X__DESCRIPTOR__H__
