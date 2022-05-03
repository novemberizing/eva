#ifndef   __NOVEMBERIZING_X_COMMAND_EVENT__GENERATOR__H__
#define   __NOVEMBERIZING_X_COMMAND_EVENT__GENERATOR__H__

#include <x/std.h>

struct xcommandeventgenerator;
struct xcommandeventgeneratorset;

typedef struct xcommandeventgenerator xcommandeventgenerator;
typedef struct xcommandeventgeneratorset xcommandeventgeneratorset;

struct xcommandeventgenerator
{
    const xcommandeventgeneratorset * set;
};

struct xcommandeventgeneratorset
{
    
};

#define xcommandeventgeneratorOn(o)

#endif // __NOVEMBERIZING_X_COMMAND_EVENT__GENERATOR__H__
