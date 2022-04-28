#ifndef   __NOVEMBERIZING_X__STD__H__
#define   __NOVEMBERIZING_X__STD__H__

typedef __INT8_TYPE__       xint8;
typedef __INT16_TYPE__      xint16;
typedef __INT32_TYPE__      xint32;
typedef __INT64_TYPE__      xint64;
typedef __UINT8_TYPE__      xuint8;
typedef __UINT16_TYPE__     xuint16;
typedef __UINT32_TYPE__     xuint32;
typedef __UINT64_TYPE__     xuint64;

#define xnil                ((void *) 0)
#define xsuccess            (0)
#define xfail               (-1)
#define xtrue               (1)
#define xfalse              (0)

#define xaddressof(o)       (&(o))

#define xfunctionAssert(condition, format, ...) do {                                                                                    \
    if(condition) {                                                                                                                     \
        printf("[assertion:%s:%d %s:%ld] " #condition " " format "\n", __FILE__, __LINE__, __func__, pthread_self(), ##__VA_ARGS__);    \
        exit(0);                                                                                                                        \
    }                                                                                                                                   \
} while(0)

#define xfunctionThrow(format,...)   do {                                                                                               \
    printf("[exception:%s:%d %s:%ld] " format "\n", __FILE__, __LINE__, __func__, pthread_self(), ##__VA_ARGS__);                       \
    exit(0);                                                                                                                            \
} while(0)

#define xfunctionStart(format, ...) do {                                                                                                \
    printf("[start:%s:%d %s:%ld] " format "\n", __FILE__, __LINE__, __func__, pthread_self(), ##__VA_ARGS__);                           \
} while(0)

#define xfunctionEnd(format, ...) do {                                                                                                  \
    printf("[end:%s:%d %s:%ld] " format "\n", __FILE__, __LINE__, __func__, pthread_self(), ##__VA_ARGS__);                             \
} while(0)

#define xfunctionError(format, ...) do {                                                                                                \
    printf("[error:%s:%d %s:%ld] " format "\n", __FILE__, __LINE__, __func__, pthread_self(), ##__VA_ARGS__);                           \
} while(0)

#define xfunctionWarning(format, ...) do {                                                                                              \
    printf("[warning:%s:%d %s:%ld] " format "\n", __FILE__, __LINE__, __func__, pthread_self(), ##__VA_ARGS__);                         \
} while(0)

#define xfunctionCaution(format, ...) do {                                                                                              \
    printf("[caution:%s:%d %s:%ld] " format "\n", __FILE__, __LINE__, __func__, pthread_self(), ##__VA_ARGS__);                         \
} while(0)

#define xfunctionNotice(format, ...) do {                                                                                               \
    printf("[notice:%s:%d %s:%ld] " format "\n", __FILE__, __LINE__, __func__, pthread_self(), ##__VA_ARGS__);                          \
} while(0)

#define xfunctionInfo(format, ...) do {                                                                                                 \
    printf("[information:%s:%d %s:%ld] " format "\n", __FILE__, __LINE__, __func__, pthread_self(), ##__VA_ARGS__);                     \
} while(0)

#define xfunctionDebug(format, ...) do {                                                                                                \
    printf("[debug:%s:%d %s:%ld] " format "\n", __FILE__, __LINE__, __func__, pthread_self(), ##__VA_ARGS__);                           \
} while(0)

#define xfunctionVerbose(format, ...) do {                                                                                              \
    printf("[verbose:%s:%d %s:%ld] " format "\n", __FILE__, __LINE__, __func__, pthread_self(), ##__VA_ARGS__);                         \
} while(0)

extern void * xobjectDel(void * o);

#endif // __NOVEMBERIZING_X__STD__H__
