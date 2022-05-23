#ifndef __BK_COMMON_TYPES__
#define __BK_COMMON_TYPES__

#ifndef NULL
#define NULL                     (0L)
#endif

#ifndef NULLPTR
#define NULLPTR                  ((void *)0)
#endif

#define GLOBAL_INT_DECLARATION() 
#define GLOBAL_INT_DISABLE() 
#define GLOBAL_INT_RESTORE()   

typedef void (*FUNCPTR)(void);
typedef void (*FUNC_1PARAM_PTR)(void *ctxt);
typedef void (*FUNC_2PARAM_PTR)(void *arg, unsigned char vif_idx);
typedef void (*FUNC_2PARAM_CB)(uint32_t larg, uint32_t rarg);

#ifndef __UPPER_CASE_TYPE_DEFINED__
#define __UPPER_CASE_TYPE_DEFINED__

typedef unsigned char          UINT8;
typedef unsigned short         UINT16;
typedef unsigned int           UINT32;
typedef signed char            INT8;
typedef signed short           INT16;
typedef signed int             INT32;
typedef float                  FLOAT;
typedef double                 DOUBLE;
typedef char                   CHAR;
typedef unsigned int           BOOL;
typedef unsigned long long     UINT64;
typedef signed long long       INT64;
typedef unsigned int           UINTPTR;
typedef signed int             INTPTR;
#endif

#endif
