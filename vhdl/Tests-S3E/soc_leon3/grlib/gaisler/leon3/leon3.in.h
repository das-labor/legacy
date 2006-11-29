
#ifndef CONFIG_LEON3
#define CONFIG_LEON3 0
#endif

#ifndef CONFIG_PROC_NUM
#define CONFIG_PROC_NUM 1
#endif

#ifndef CONFIG_IU_NWINDOWS
#define CONFIG_IU_NWINDOWS 8
#endif

#ifndef CONFIG_IU_RSTADDR
#define CONFIG_IU_RSTADDR 8
#endif

#ifndef CONFIG_IU_LDELAY
#define CONFIG_IU_LDELAY 1
#endif

#ifndef CONFIG_IU_WATCHPOINTS
#define CONFIG_IU_WATCHPOINTS 0
#endif

#ifdef CONFIG_IU_V8MULDIV
#ifdef CONFIG_IU_MUL_LATENCY_4
#define CFG_IU_V8 1
#else
#define CFG_IU_V8 2
#endif
#else
#define CFG_IU_V8 0
#endif
#ifndef CONFIG_PWD
#define CONFIG_PWD 0
#endif

#ifndef CONFIG_IU_MUL_MAC
#define CONFIG_IU_MUL_MAC 0
#endif

#ifndef CONFIG_IU_SVT
#define CONFIG_IU_SVT 0
#endif

#if defined CONFIG_FPU_GRFPU
#define CONFIG_FPU 1
#elif defined CONFIG_FPU_MEIKO
#define CONFIG_FPU 2
#elif defined CONFIG_FPU_GRFPULITE
#define CONFIG_FPU 3
#else
#define CONFIG_FPU 0
#endif

#ifndef CONFIG_ICACHE_ENABLE
#define CONFIG_ICACHE_ENABLE 0
#endif

#if defined CONFIG_ICACHE_ASSO1
#define CFG_IU_ISETS 1
#elif defined CONFIG_ICACHE_ASSO2
#define CFG_IU_ISETS 2
#elif defined CONFIG_ICACHE_ASSO3
#define CFG_IU_ISETS 3
#elif defined CONFIG_ICACHE_ASSO4
#define CFG_IU_ISETS 4
#else
#define CFG_IU_ISETS 1
#endif

#if defined CONFIG_ICACHE_SZ1
#define CFG_ICACHE_SZ 1
#elif defined CONFIG_ICACHE_SZ2
#define CFG_ICACHE_SZ 2
#elif defined CONFIG_ICACHE_SZ4
#define CFG_ICACHE_SZ 4
#elif defined CONFIG_ICACHE_SZ8
#define CFG_ICACHE_SZ 8
#elif defined CONFIG_ICACHE_SZ16
#define CFG_ICACHE_SZ 16
#elif defined CONFIG_ICACHE_SZ32
#define CFG_ICACHE_SZ 32
#elif defined CONFIG_ICACHE_SZ64
#define CFG_ICACHE_SZ 64
#elif defined CONFIG_ICACHE_SZ128
#define CFG_ICACHE_SZ 128
#elif defined CONFIG_ICACHE_SZ256
#define CFG_ICACHE_SZ 256
#else
#define CFG_ICACHE_SZ 1
#endif

#ifdef CONFIG_ICACHE_LZ16
#define CFG_ILINE_SZ 4
#else
#define CFG_ILINE_SZ 8
#endif

#if defined CONFIG_ICACHE_ALGORND
#define CFG_ICACHE_ALGORND 2
#elif defined CONFIG_ICACHE_ALGOLRR
#define CFG_ICACHE_ALGORND 1
#else
#define CFG_ICACHE_ALGORND 0
#endif

#ifndef CONFIG_ICACHE_LOCK
#define CONFIG_ICACHE_LOCK 0
#endif

#ifndef CONFIG_ICACHE_LRAM
#define CONFIG_ICACHE_LRAM 0
#endif

#ifndef CONFIG_ICACHE_LRSTART
#define CONFIG_ICACHE_LRSTART 8E
#endif

#if defined CONFIG_ICACHE_LRAM_SZ2
#define CFG_ILRAM_SIZE 2
#elif defined CONFIG_ICACHE_LRAM_SZ4
#define CFG_ILRAM_SIZE 4
#elif defined CONFIG_ICACHE_LRAM_SZ8
#define CFG_ILRAM_SIZE 8
#elif defined CONFIG_ICACHE_LRAM_SZ16
#define CFG_ILRAM_SIZE 16
#elif defined CONFIG_ICACHE_LRAM_SZ32
#define CFG_ILRAM_SIZE 32
#elif defined CONFIG_ICACHE_LRAM_SZ64
#define CFG_ILRAM_SIZE 64
#elif defined CONFIG_ICACHE_LRAM_SZ128
#define CFG_ILRAM_SIZE 128
#elif defined CONFIG_ICACHE_LRAM_SZ256
#define CFG_ILRAM_SIZE 256
#else
#define CFG_ILRAM_SIZE 1
#endif


#ifndef CONFIG_DCACHE_ENABLE
#define CONFIG_DCACHE_ENABLE 0
#endif

#if defined CONFIG_DCACHE_ASSO1
#define CFG_IU_DSETS 1
#elif defined CONFIG_DCACHE_ASSO2
#define CFG_IU_DSETS 2
#elif defined CONFIG_DCACHE_ASSO3
#define CFG_IU_DSETS 3
#elif defined CONFIG_DCACHE_ASSO4
#define CFG_IU_DSETS 4
#else
#define CFG_IU_DSETS 1
#endif

#if defined CONFIG_DCACHE_SZ1
#define CFG_DCACHE_SZ 1
#elif defined CONFIG_DCACHE_SZ2
#define CFG_DCACHE_SZ 2
#elif defined CONFIG_DCACHE_SZ4
#define CFG_DCACHE_SZ 4
#elif defined CONFIG_DCACHE_SZ8
#define CFG_DCACHE_SZ 8
#elif defined CONFIG_DCACHE_SZ16
#define CFG_DCACHE_SZ 16
#elif defined CONFIG_DCACHE_SZ32
#define CFG_DCACHE_SZ 32
#elif defined CONFIG_DCACHE_SZ64
#define CFG_DCACHE_SZ 64
#elif defined CONFIG_DCACHE_SZ128
#define CFG_DCACHE_SZ 128
#elif defined CONFIG_DCACHE_SZ256
#define CFG_DCACHE_SZ 256
#else
#define CFG_DCACHE_SZ 1
#endif

#ifdef CONFIG_DCACHE_LZ16
#define CFG_DLINE_SZ 4
#else
#define CFG_DLINE_SZ 8
#endif

#if defined CONFIG_DCACHE_ALGORND
#define CFG_DCACHE_ALGORND 2
#elif defined CONFIG_DCACHE_ALGOLRR
#define CFG_DCACHE_ALGORND 1
#else
#define CFG_DCACHE_ALGORND 0
#endif

#ifndef CONFIG_DCACHE_LOCK
#define CONFIG_DCACHE_LOCK 0
#endif

#ifndef CONFIG_DCACHE_SNOOP
#define CONFIG_DCACHE_SNOOP 0
#endif

#ifndef CONFIG_DCACHE_SNOOP_FAST
#define CONFIG_DCACHE_SNOOP_FAST 0
#endif

#ifndef CONFIG_CACHE_FIXED
#define CONFIG_CACHE_FIXED 0
#endif

#ifndef CONFIG_DCACHE_LRAM
#define CONFIG_DCACHE_LRAM 0
#endif

#ifndef CONFIG_DCACHE_LRSTART
#define CONFIG_DCACHE_LRSTART 8F
#endif

#if defined CONFIG_DCACHE_LRAM_SZ2
#define CFG_DLRAM_SIZE 2
#elif defined CONFIG_DCACHE_LRAM_SZ4
#define CFG_DLRAM_SIZE 4
#elif defined CONFIG_DCACHE_LRAM_SZ8
#define CFG_DLRAM_SIZE 8
#elif defined CONFIG_DCACHE_LRAM_SZ16
#define CFG_DLRAM_SIZE 16
#elif defined CONFIG_DCACHE_LRAM_SZ32
#define CFG_DLRAM_SIZE 32
#elif defined CONFIG_DCACHE_LRAM_SZ64
#define CFG_DLRAM_SIZE 64
#elif defined CONFIG_DCACHE_LRAM_SZ128
#define CFG_DLRAM_SIZE 128
#elif defined CONFIG_DCACHE_LRAM_SZ256
#define CFG_DLRAM_SIZE 256
#else
#define CFG_DLRAM_SIZE 1
#endif


#ifdef CONFIG_MMU_ENABLE
#define CONFIG_MMUEN 1

#ifdef CONFIG_MMU_SPLIT
#define CONFIG_TLB_TYPE 0
#endif
#ifdef CONFIG_MMU_COMBINED
#define CONFIG_TLB_TYPE 1
#endif

#ifdef CONFIG_MMU_REPARRAY
#define CONFIG_TLB_REP 0
#endif
#ifdef CONFIG_MMU_REPINCREMENT
#define CONFIG_TLB_REP 1
#endif

#ifdef CONFIG_MMU_I2 
#define CONFIG_ITLBNUM 2
#endif
#ifdef CONFIG_MMU_I4 
#define CONFIG_ITLBNUM 4
#endif
#ifdef CONFIG_MMU_I8 
#define CONFIG_ITLBNUM 8
#endif
#ifdef CONFIG_MMU_I16 
#define CONFIG_ITLBNUM 16
#endif
#ifdef CONFIG_MMU_I32
#define CONFIG_ITLBNUM 32
#endif

#define CONFIG_DTLBNUM 2
#ifdef CONFIG_MMU_D2 
#undef CONFIG_DTLBNUM 
#define CONFIG_DTLBNUM 2
#endif
#ifdef CONFIG_MMU_D4 
#undef CONFIG_DTLBNUM 
#define CONFIG_DTLBNUM 4
#endif
#ifdef CONFIG_MMU_D8 
#undef CONFIG_DTLBNUM 
#define CONFIG_DTLBNUM 8
#endif
#ifdef CONFIG_MMU_D16 
#undef CONFIG_DTLBNUM 
#define CONFIG_DTLBNUM 16
#endif
#ifdef CONFIG_MMU_D32
#undef CONFIG_DTLBNUM 
#define CONFIG_DTLBNUM 32
#endif

#else
#define CONFIG_MMUEN 0
#define CONFIG_ITLBNUM 2
#define CONFIG_DTLBNUM 2
#define CONFIG_TLB_TYPE 1
#define CONFIG_TLB_REP 1
#endif

#ifndef CONFIG_DSU_ENABLE
#define CONFIG_DSU_ENABLE 0
#endif

#if defined CONFIG_DSU_ITRACESZ1
#define CFG_DSU_ITB 1
#elif CONFIG_DSU_ITRACESZ2
#define CFG_DSU_ITB 2
#elif CONFIG_DSU_ITRACESZ4
#define CFG_DSU_ITB 4
#elif CONFIG_DSU_ITRACESZ8
#define CFG_DSU_ITB 8
#elif CONFIG_DSU_ITRACESZ16
#define CFG_DSU_ITB 16
#else
#define CFG_DSU_ITB 0
#endif

#if defined CONFIG_DSU_ATRACESZ1
#define CFG_DSU_ATB 1
#elif CONFIG_DSU_ATRACESZ2
#define CFG_DSU_ATB 2
#elif CONFIG_DSU_ATRACESZ4
#define CFG_DSU_ATB 4
#elif CONFIG_DSU_ATRACESZ8
#define CFG_DSU_ATB 8
#elif CONFIG_DSU_ATRACESZ16
#define CFG_DSU_ATB 16
#else
#define CFG_DSU_ATB 0
#endif

#ifdef CONFIG_DEBUG_PC32
#define CFG_DEBUG_PC32 0 
#else
#define CFG_DEBUG_PC32 2
#endif
#ifndef CONFIG_IU_DISAS
#define CONFIG_IU_DISAS 0
#endif
#ifndef CONFIG_IU_DISAS_NET
#define CONFIG_IU_DISAS_NET 0
#endif

