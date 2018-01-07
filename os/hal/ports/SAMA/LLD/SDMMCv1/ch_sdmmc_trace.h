#ifndef CH_SDMMC_TRACE_H_
#define CH_SDMMC_TRACE_H_


#if SAMA_SDMMC_TRACE == 1

#include "chprintf.h"
extern BaseSequentialStream * ts;

#define TRACE(s)		chprintf(ts,s)
#define TRACE_1(s,v1)	chprintf(ts,s,v1)
#define TRACE_2(s,v1,v2)	chprintf(ts,s,v1,v2)
#define TRACE_3(s,v1,v2,v3)	chprintf(ts,s,v1,v3)
#define TRACE_4(s,v1,v2,v3,v4)	chprintf(ts,s,v1,v2,v3,v4)
#define TRACE_5(s,v1,v2,v3,v4,v5)	chprintf(ts,s,v1,v2,v3,v4,v5)
#define TRACE_6(s,v1,v2,v3,v4,v5,v6)	chprintf(ts,s,v1,v2,v3,v4,v5,v6)
#define TRACE_LEV_1(s,v1) TRACE_1(s,v1);
#else
#define TRACE(s)
#define TRACE_1(s,v1)
#define TRACE_2(s,v1,v2)
#define TRACE_3(s,v1,v2,v3)
#define TRACE_4(s,v1,v2,v3,v4)
#define TRACE_5(s,v1,v2,v3,v4,v5)
#define TRACE_6(s,v1,v2,v3,v4,v5,v6)
#define TRACE_LEV_1(s,v1)
#endif



#endif /* CH_SDMMC_TRACE_H_ */
