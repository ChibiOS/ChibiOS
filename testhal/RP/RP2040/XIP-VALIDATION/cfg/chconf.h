#ifndef CHCONF_H
#define CHCONF_H

#define _CHIBIOS_RT_CONF_
#define _CHIBIOS_RT_CONF_VER_8_0_

#define CH_CFG_SMP_MODE                     FALSE

#define CH_CFG_HARDENING_LEVEL              0
#define CH_CFG_ST_RESOLUTION                32
#define CH_CFG_ST_FREQUENCY                 1000000
#define CH_CFG_INTERVALS_SIZE               32
#define CH_CFG_TIME_TYPES_SIZE              32
#define CH_CFG_ST_TIMEDELTA                 20

#define CH_CFG_TIME_QUANTUM                 0
#define CH_CFG_NO_IDLE_THREAD               FALSE
#define CH_CFG_OPTIMIZE_SPEED               TRUE

#define CH_CFG_USE_TM                       FALSE
#define CH_CFG_USE_TIMESTAMP                TRUE
#define CH_CFG_USE_REGISTRY                 TRUE
#define CH_CFG_USE_WAITEXIT                 TRUE
#define CH_CFG_USE_SEMAPHORES               TRUE
#define CH_CFG_USE_SEMAPHORES_PRIORITY      FALSE
#define CH_CFG_USE_MUTEXES                  TRUE
#define CH_CFG_USE_MUTEXES_RECURSIVE        FALSE
#define CH_CFG_USE_CONDVARS                 FALSE
#define CH_CFG_USE_CONDVARS_TIMEOUT         FALSE
#define CH_CFG_USE_EVENTS                   TRUE
#define CH_CFG_USE_EVENTS_TIMEOUT           TRUE
#define CH_CFG_USE_MESSAGES                 TRUE
#define CH_CFG_USE_MESSAGES_PRIORITY        FALSE
#define CH_CFG_USE_DYNAMIC                  FALSE

#define CH_CFG_USE_MAILBOXES                FALSE
#define CH_CFG_USE_MEMCHECKS                FALSE
#define CH_CFG_USE_MEMCORE                  TRUE
#define CH_CFG_MEMCORE_SIZE                 0
#define CH_CFG_USE_HEAP                     TRUE
#define CH_CFG_USE_MEMPOOLS                 FALSE
#define CH_CFG_USE_OBJ_FIFOS                FALSE
#define CH_CFG_USE_PIPES                    FALSE
#define CH_CFG_USE_OBJ_CACHES               FALSE
#define CH_CFG_USE_DELEGATES                FALSE
#define CH_CFG_USE_JOBS                     FALSE
#define CH_CFG_USE_FACTORY                  FALSE
#define CH_CFG_FACTORY_MAX_NAMES_LENGTH     8
#define CH_CFG_FACTORY_OBJECTS_REGISTRY     FALSE
#define CH_CFG_FACTORY_GENERIC_BUFFERS      FALSE
#define CH_CFG_FACTORY_SEMAPHORES           FALSE
#define CH_CFG_FACTORY_MAILBOXES            FALSE
#define CH_CFG_FACTORY_OBJ_FIFOS            FALSE
#define CH_CFG_FACTORY_PIPES                FALSE

#define CH_DBG_STATISTICS                   FALSE
#define CH_DBG_SYSTEM_STATE_CHECK           TRUE
#define CH_DBG_ENABLE_CHECKS                TRUE
#define CH_DBG_ENABLE_ASSERTS               TRUE
#define CH_DBG_TRACE_MASK                   CH_DBG_TRACE_MASK_DISABLED
#define CH_DBG_TRACE_BUFFER_SIZE            128
#define CH_DBG_ENABLE_STACK_CHECK           TRUE
#define CH_DBG_FILL_THREADS                 TRUE
#define CH_DBG_THREADS_PROFILING            FALSE

#define CH_CFG_SYSTEM_EXTRA_FIELDS
#define CH_CFG_SYSTEM_INIT_HOOK() do { } while (false)
#define CH_CFG_OS_INSTANCE_EXTRA_FIELDS
#define CH_CFG_OS_INSTANCE_INIT_HOOK(oip) do { (void)(oip); } while (false)
#define CH_CFG_THREAD_EXTRA_FIELDS
#define CH_CFG_THREAD_INIT_HOOK(tp) do { (void)(tp); } while (false)
#define CH_CFG_THREAD_EXIT_HOOK(tp) do { (void)(tp); } while (false)
#define CH_CFG_CONTEXT_SWITCH_HOOK(ntp, otp) do { (void)(ntp); (void)(otp); } while (false)
#define CH_CFG_IRQ_PROLOGUE_HOOK() do { } while (false)
#define CH_CFG_IRQ_EPILOGUE_HOOK() do { } while (false)
#define CH_CFG_IDLE_ENTER_HOOK() do { } while (false)
#define CH_CFG_IDLE_LEAVE_HOOK() do { } while (false)
#define CH_CFG_IDLE_LOOP_HOOK() do { } while (false)
#define CH_CFG_SYSTEM_TICK_HOOK() do { } while (false)
#define CH_CFG_SYSTEM_HALT_HOOK(reason) do { (void)(reason); } while (false)
#define CH_CFG_TRACE_HOOK(tep) do { (void)(tep); } while (false)
#define CH_CFG_RUNTIME_FAULTS_HOOK(mask) do { (void)(mask); } while (false)
#define CH_CFG_SAFETY_CHECK_HOOK(l, f) do { chSysHalt(f); } while (false)

#endif /* CHCONF_H */
