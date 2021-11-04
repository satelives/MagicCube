/**
 * @file    gt_define.h
 * @brief
 * @author  chery.zhang (satelives@126.com)
 * @version 1.0
 * @date    2021-08-27
 *
 * (C) COPYRIGHT 2021 FTT
 *
 */

/* Define to prevent recursive inclusion ------------------------------------------------*/
#ifndef __GT_DEFINE_H
#define __GT_DEFINE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes -----------------------------------------------------------------------------*/
//#include "stdio.h"
#include "stdint.h"
//#include "stdlib.h"
#include "stdbool.h"
//#include "stdarg.h"
//#include "string.h"

/* Exported constants -------------------------------------------------------------------*/
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#define ABS ((x) > 0 ? (x) : -(x))

#define enable_all_interrupt()          __enable_irq()
#define disable_all_interrupt()         __disable_irq()

/* Exported types -----------------------------------------------------------------------*/
#if 0
typedef signed   char                   int8_t;      /**<  8bit integer type */
typedef signed   short                  int16_t;     /**< 16bit integer type */
typedef signed   long                   int32_t;     /**< 32bit integer type */
typedef unsigned char                   uint8_t;     /**<  8bit unsigned integer type */
typedef unsigned short                  uint16_t;    /**< 16bit unsigned integer type */
typedef unsigned long                   uint32_t;    /**< 32bit unsigned integer type */
typedef int                             bool_t;      /**< boolean type */
#define true 1
#define false 0
#define NULL 0
#endif

typedef signed   char                   gt_int8_t;      /**<  8bit integer type */
typedef signed   short                  gt_int16_t;     /**< 16bit integer type */
typedef signed   long                   gt_int32_t;     /**< 32bit integer type */
typedef unsigned char                   gt_uint8_t;     /**<  8bit unsigned integer type */
typedef unsigned short                  gt_uint16_t;    /**< 16bit unsigned integer type */
typedef unsigned long                   gt_uint32_t;    /**< 32bit unsigned integer type */
typedef int                             gt_bool_t;      /**< boolean type */

/* 32bit CPU */
typedef long                gt_base_t;     /**< Nbit CPU related date type */
typedef unsigned long       gt_ubase_t;    /**< Nbit unsigned CPU related data type */

typedef gt_ubase_t          gt_size_t;
typedef gt_ubase_t          gt_time_t;
typedef gt_ubase_t          gt_tick_t;
typedef gt_base_t           gt_err_t;

/* boolean type definitions */
#define GT_TRUE                         1               /**< boolean true  */
#define GT_FALSE                        0               /**< boolean fails */

/* maximum value of base type */
#define GT_UINT8_MAX                    (0xff)            /**< Maxium number of UINT8 */
#define GT_UINT16_MAX                   (0xffff)          /**< Maxium number of UINT16 */
#define GT_UINT32_MAX                   (0xffffffff)      /**< Maxium number of UINT32 */
#define GT_TICK_MAX                     (GT_UINT32_MAX)   /**< Maxium number of tick */

typedef union
{
    uint32_t value;
    struct
    {
        uint32_t bit0   : 1;
        uint32_t bit1   : 1;
        uint32_t bit2   : 1;
        uint32_t bit3   : 1;
        uint32_t bit4   : 1;
        uint32_t bit5   : 1;
        uint32_t bit6   : 1;
        uint32_t bit7   : 1;
        uint32_t bit8   : 1;
        uint32_t bit9   : 1;
        uint32_t bit10  : 1;
        uint32_t bit11  : 1;
        uint32_t bit12  : 1;
        uint32_t bit13  : 1;
        uint32_t bit14  : 1;
        uint32_t bit15  : 1;
        uint32_t bit16  : 1;
        uint32_t bit17  : 1;
        uint32_t bit18  : 1;
        uint32_t bit19  : 1;
        uint32_t bit20  : 1;
        uint32_t bit21  : 1;
        uint32_t bit22  : 1;
        uint32_t bit23  : 1;
        uint32_t bit24  : 1;
        uint32_t bit25  : 1;
        uint32_t bit26  : 1;
        uint32_t bit27  : 1;
        uint32_t bit28  : 1;
        uint32_t bit29  : 1;
        uint32_t bit30  : 1;
        uint32_t bit31  : 1;
    }bits;
} gt_bits_32_t;

typedef union
{
    uint16_t value;
    struct
    {
        uint16_t bit0   : 1;
        uint16_t bit1   : 1;
        uint16_t bit2   : 1;
        uint16_t bit3   : 1;
        uint16_t bit4   : 1;
        uint16_t bit5   : 1;
        uint16_t bit6   : 1;
        uint16_t bit7   : 1;
        uint16_t bit8   : 1;
        uint16_t bit9   : 1;
        uint16_t bit10  : 1;
        uint16_t bit11  : 1;
        uint16_t bit12  : 1;
        uint16_t bit13  : 1;
        uint16_t bit14  : 1;
        uint16_t bit15  : 1;
    }bits;
} gt_bits_16_t;

typedef union
{
    uint8_t value;
    struct
    {
        uint8_t bit0   : 1;
        uint8_t bit1   : 1;
        uint8_t bit2   : 1;
        uint8_t bit3   : 1;
        uint8_t bit4   : 1;
        uint8_t bit5   : 1;
        uint8_t bit6   : 1;
        uint8_t bit7   : 1;
    }bits;
} gt_bits_8_t;

/* Compiler Related Definitions */
#if defined(__CC_ARM) || defined(__CLANARM)           /* ARM Compiler */
    #include <stdarg.h>
    #define GT_SECTION(x)               __attribute__((section(x)))
    #define GT_UNUSED                   __attribute__((unused))
    #define GT_USED                     __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))

    #define GT_WEAK                     __attribute__((weak))
    #define gt_inline                   static __inline
    /* module compiling */
    #ifdef GT_USING_MODULE
        #define GT_API                 __declspec(dllimport)
    #else
        #define GT_API                 __declspec(dllexport)
    #endif

#elif defined (__IAR_SYSTEMS_ICC__)     /* for IAR Compiler */
    #include <stdarg.h>
    #define GT_SECTION(x)               @ x
    #define GT_UNUSED
    #define GT_USED                     __root
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)                    PRAGMA(data_alignment=n)
    #define GT_WEAK                     __weak
    #define gt_inline                   static inline
    #define GT_API

#elif defined (__GNUC__)                /* GNU GCC Compiler */
    #define GT_USING_NEWLIB
    #ifdef GT_USING_NEWLIB
        #include <stdarg.h>
    #else
        /* the version of GNU GCC must be greater than 4.x */
        typedef __builtin_va_list       __gnuc_va_list;
        typedef __gnuc_va_list          va_list;
        #define va_start(v,l)           __builtin_va_start(v,l)
        #define va_end(v)               __builtin_va_end(v)
        #define va_arg(v,l)             __builtin_va_arg(v,l)
    #endif

    #define GT_SECTION(x)               __attribute__((section(x)))
    #define GT_UNUSED                   __attribute__((unused))
    #define GT_USED                     __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
    #define GT_WEAK                     __attribute__((weak))
    #define gt_inline                   static __inline
    #define GT_API
#elif defined (__ADSPBLACKFIN__)        /* for VisualDSP++ Compiler */
    #include <stdarg.h>
    #define GT_SECTION(x)               __attribute__((section(x)))
    #define GT_UNUSED                   __attribute__((unused))
    #define GT_USED                     __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
    #define GT_WEAK                     __attribute__((weak))
    #define gt_inline                   static inline
    #define GT_API
#elif defined (_MSC_VER)
    #include <stdarg.h>
    #define GT_SECTION(x)
    #define GT_UNUSED
    #define GT_USED
    #define ALIGN(n)                    __declspec(align(n))
    #define GT_WEAK
    #define gt_inline                   static __inline
    #define GT_API
#elif defined (__TI_COMPILER_VERSION__)
    #include <stdarg.h>
    /* The way that TI compiler set section is different from other(at least
     * GCC and MDK) compilers. See ARM Optimizing C/C++ Compiler 5.9.3 for more
     * details. */
    #define GT_SECTION(x)
    #define GT_UNUSED
    #define GT_USED
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)
    #define GT_WEAK
    #define gt_inline                   static inline
    #define GT_API
#elif defined (__TASKING__)
    #include <stdarg.h>
    #define GT_SECTION(x)               __attribute__((section(x)))
    #define GT_UNUSED                   __attribute__((unused))
    #define GT_USED                     __attribute__((used, protect))
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)                    __attribute__((__align(n)))
    #define GT_WEAK                     __attribute__((weak))
    #define gt_inline                   static inline
    #define GT_API
#else
    #error not supported tool chain
#endif

/* error code definitions */
#define GT_EOK                          (0)               /**< There is no error */
#define GT_ERROR                        (1)               /**< A generic error happens */
#define GT_ETIMEOUT                     (2)               /**< Timed out */
#define GT_EFULL                        (3)               /**< The resource is full */
#define GT_EEMPTY                       (4)               /**< The resource is empty */
#define GT_ENOMEM                       (5)               /**< No memory */
#define GT_ENOSYS                       (6)               /**< No system */
#define GT_EBUSY                        (7)               /**< Busy */
#define GT_EIO                          (8)               /**< IO error */
#define GT_EINTR                        (9)               /**< Interrupted system call */
#define GT_EINVAL                       (10)              /**< Invalid argument */

/**
 * @ingroup BasicDef
 *
 * @def GT_ALIGN(size, align)
 * Return the most contiguous size aligned at specified width. GT_ALIGN(13, 4)
 * would return 16.
 */

#define GT_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))

/**
 * @ingroup BasicDef
 *
 * @def GT_ALIGN_DOWN(size, align)
 * Return the down number of aligned at specified width. GT_ALIGN_DOWN(13, 4)
 * would return 12.
 */
#define GT_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

/**
 * @ingroup BasicDef
 *
 * @def GT_NULL
 * Similar as the \c NULL in C library.
 */
#define GT_NULL                         (0)

/**
 * Double List structure
 */
struct gt_list_node
{
    struct gt_list_node *next;                          /**< point to next node. */
    struct gt_list_node *prev;                          /**< point to prev node. */
};
typedef struct gt_list_node gt_list_t;                  /**< Type for lists. */

/**
 * Single List structure
 */
struct gt_slist_node
{
    struct gt_slist_node *next;                         /**< point to next node. */
};
typedef struct gt_slist_node gt_slist_t;                /**< Type for single list. */

typedef struct gt_mutex
{
    // struct gt_ipc_object parent;                        /**< inherit from ipc_object */

    uint16_t          value;                            /**< value of mutex */

    // uint8_t           original_priority;                /**< priority of last thread hold the mutex */
    // uint8_t           hold;                             /**< numbers of thread hold the mutex */
}gt_mutex_t;

/**
 * device (I/O) class type
 */
enum gt_device_class_type
{
    GT_Device_Class_Char = 0,                           /**< character device */
    GT_Device_Class_Block,                              /**< block device */
    GT_Device_Class_NetIf,                              /**< net interface */
    GT_Device_Class_MTD,                                /**< memory device */
    GT_Device_Class_CAN,                                /**< CAN device */
    GT_Device_Class_RTC,                                /**< RTC device */
    GT_Device_Class_Sound,                              /**< Sound device */
    GT_Device_Class_Graphic,                            /**< Graphic device */
    GT_Device_Class_USART,                              /**< USART bus device */
    GT_Device_Class_SPIBUS,                             /**< SPI bus device */
    GT_Device_Class_I2CBUS,                             /**< I2C bus device */
    GT_Device_Class_USBDevice,                          /**< USB slave device */
    GT_Device_Class_USBHost,                            /**< USB host bus */
    GT_Device_Class_SPIDevice,                          /**< SPI device */
    GT_Device_Class_SDIO,                               /**< SDIO bus device */
    GT_Device_Class_PM,                                 /**< PM pseudo device */
    GT_Device_Class_Pipe,                               /**< Pipe device */
    GT_Device_Class_Portal,                             /**< Portal device */
    GT_Device_Class_Timer,                              /**< Timer device */
    GT_Device_Class_Miscellaneous,                      /**< Miscellaneous device */
    GT_Device_Class_Unknown                             /**< unknown device */
};

typedef struct gt_device *gt_device_t;

struct gt_device
{
    //struct gt_object parent; /**< inherit from gt_object */

    enum gt_device_class_type type; /**< device type */
    gt_uint16_t flag;               /**< device flag */
    gt_uint16_t open_flag;          /**< device open flag */

    gt_uint8_t ref_count; /**< reference count */
    gt_uint8_t device_id; /**< 0 - 255 */

    /* device call back */
    // gt_err_t (*rx_indicate)(gt_device_t dev, gt_size_t size);
    // gt_err_t (*tx_complete)(gt_device_t dev, void *buffer);

    /* common device interface */
    gt_err_t    (*init)(gt_device_t dev);
    gt_err_t    (*open)(gt_device_t dev, gt_uint16_t oflag);
    gt_err_t    (*close)(gt_device_t dev);
    gt_size_t   (*read)(gt_device_t dev, uint32_t pos, void *buffer, gt_size_t size);
    gt_size_t   (*write)(gt_device_t dev, uint32_t pos, const void *buffer, gt_size_t size);
    gt_err_t    (*control)(gt_device_t dev, int cmd, void *args);

#if defined(GT_USING_POSIX)
    const struct dfs_file_ops *fops;
    rt_list_t wait_queue;
#endif

    void *user_data; /**< device private data */
};

#ifdef GT_USING_DEVICE_OPS
/**
 * operations set for device object
 */
struct gt_device_ops
{
    /* common device interface */
    gt_err_t  (*init)   (gt_device_t dev);
    gt_err_t  (*open)   (gt_device_t dev, gt_uint16_t oflag);
    gt_err_t  (*close)  (gt_device_t dev);
    gt_size_t (*read)   (gt_device_t dev, gt_off_t pos, void *buffer, gt_size_t size);
    gt_size_t (*write)  (gt_device_t dev, gt_off_t pos, const void *buffer, gt_size_t size);
    gt_err_t  (*control)(gt_device_t dev, int cmd, void *args);
};
#endif

typedef struct _gt_event_t
{
    volatile uint32_t event;
} gt_event_t;

typedef struct
{
    uint32_t runtime;                   // 系统运行时间: 0.1S
    uint32_t ticks;                     // 系统节拍计数，上电启动后就开始计数
} gt_system_t;

#ifdef __cplusplus
}
#endif

#endif
