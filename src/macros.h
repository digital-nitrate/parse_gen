#ifndef MACROS_H
#define MACROS_H 1

#define DYNARR_CHK(cnt, cap, arr, res) do {\
	if ((cnt) == (cap)) {\
		size_t nc = 2 * (cap);\
		void* np = realloc((arr), (sizeof *(arr)) * nc);\
		if (np == NULL) {\
			(res) = 1;\
		} else {\
			(cnt) = nc;\
			(arr) = np;\
		}\
	}\
} while (0)

#endif
