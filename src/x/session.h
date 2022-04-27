#ifndef   __NOVEMBERIZING_X__SESSION__H__
#define   __NOVEMBERIZING_X__SESSION__H__

#include <x/server.h>
#include <x/session/socket.h>

struct xsession;
struct xsessionset;

typedef struct xsession xsession;
typedef struct xsessionset xsessionset;

struct xsession
{
    const xsessionset * set;

    xsessionsocket * socket;

};

struct xsessionset
{
    xsession * (*del)(xsession *);
};

#endif // __NOVEMBERIZING_X__SESSION__H__
