#include <stdint.h>
#ifndef mpers_ptr_t_is_uint32_t
typedef uint32_t mpers_ptr_t;
#define mpers_ptr_t_is_uint32_t
#endif
typedef
struct {
int64_t tms_utime;
int64_t tms_stime;
int64_t tms_cutime;
int64_t tms_cstime;
} ATTRIBUTE_PACKED mx32_tms_t;
#define MPERS_mx32_tms_t mx32_tms_t
