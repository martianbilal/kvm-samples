#include <stdint.h>
#ifndef mpers_ptr_t_is_uint32_t
typedef uint32_t mpers_ptr_t;
#define mpers_ptr_t_is_uint32_t
#endif
typedef
struct {
struct {
mpers_ptr_t p;
char sc;
unsigned char mpers_filler_1[1];
int16_t ss;
unsigned char uc;
unsigned char mpers_filler_2[3];
int32_t si;
uint32_t ui;
int32_t sl;
uint16_t us;
unsigned char mpers_filler_3[6];
int64_t sll;
uint64_t ull;
uint32_t ul;
int32_t asl[3][5][7];
char f;
unsigned char mpers_end_filler_4[7];
} ATTRIBUTE_PACKED s;
union {
int64_t sll;
uint64_t ull;
mpers_ptr_t p;
int32_t sl;
uint32_t ul;
int32_t si;
uint32_t ui;
int16_t ss[7][9];
uint16_t us[4];
char sc;
unsigned char uc;
signed char i8;
int16_t i16;
int32_t i32;
int64_t i64;
unsigned char u8;
uint16_t u16;
uint32_t u32;
uint64_t u64;
} u[3][2];
int16_t f[0];
} ATTRIBUTE_PACKED mx32_sample_struct;
#define MPERS_mx32_sample_struct mx32_sample_struct
