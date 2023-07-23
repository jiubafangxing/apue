#ifndef  __PROTO_H__
#define  __PROTO_H__
#include <stdint.h>
#define MAXSIZE (512 -8 -8 )
#define IPSTRSIZE 40
struct msg_st{
  uint32_t math;
  uint32_t chinese;
  uint8_t name[1];
}__attribute__((packed));
#endif
