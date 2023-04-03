#ifndef UTIL_H
#define UTIL_H 1

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct string {
	char* data;
	size_t len;
	size_t cap;
} string;

#ifdef __cplusplus
}
#endif

#endif
