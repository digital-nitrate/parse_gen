#ifndef MACROS_H
#define MACROS_H 1

#define DYNARR_CHK(cnt, cap, arr, res) do {\
	if ((cnt) == (cap)) {\
		size_t nc = 2 * (cap);\
		void* np = realloc((arr), (sizeof *(arr)) * nc);\
		if (np == NULL) {\
			(res) = 1;\
		} else {\
			(cap) = nc;\
			(arr) = np;\
		}\
	}\
} while (0)

#define HASH_INIT(_bin_type, _init, _malloc, _base, _bins, _bcnt, _ecnt, ...) do {\
	_malloc((_bins), ((sizeof *(_bins)) * (_base)), ##__VA_ARGS__);\
	if ((_bins) == NULL) break;\
	_bin_type const* const _end = (_bins) + (_base);\
	for (_bin_type* _curr = (_bins); _curr != _end; ++_curr) {\
		_init((*_curr), ##__VA_ARGS__);\
	}\
	(_bcnt) = (_base);\
	(_ecnt) = 0;\
} while (0)

#define HASH_LD(_ret, _is_match, _err, _is_empty, _hash, _out, _key, _bins, _bcnt, ...) do {\
	unsigned long long int _loc;\
	_hash((_loc), (_key), ##__VA_ARGS__);\
	_loc = _loc % (_bcnt);\
	while (1) {\
		_Bool _flg1;\
		_is_empty((_flg1), ((_bins)[_loc]), ##__VA_ARGS__);\
		if (_flg1) {_err((_out), ##__VA_ARGS__); break;}\
		_Bool _flg2;\
		_is_match((_flg2), (_key), ((_bins)[_loc]), ##__VA_ARGS__);\
		if (_flg2) {_ret((_out), ((_bins)[_loc]), ##__VA_ARGS__); break;}\
		_loc = (_loc + 1) % (_bcnt);\
	}\
} while (0)

#define HASH_ADD(_bin_type, _malloc, _free, _new_size, _init, _fill, _is_empty, _is_match, _pull_hash, _hash, _mem, _exist, _ret, _load, _out, _key, _bins, _bcnt, _ecnt, ...) do {\
	if ((_bcnt) * (_load) <= (_ecnt)) {\
		unsigned long long int _nb;\
		_new_size((_nb), (_bcnt), ##__VA_ARGS__);\
		_bin_type* _nbins;\
		_malloc((_nbins), ((sizeof *_nbins) * _nb), ##__VA_ARGS__);\
		if (_nbins == NULL) {_mem(_out, ##__VA_ARGS__); break;}\
		_bin_type const* const _nend = _nbins + _nb;\
		for (_bin_type* _curr = _nbins; _curr != _nend; ++_curr) {\
			_init((*_curr), ##__VA_ARGS__);\
		}\
		_bin_type const* const _end = (_bins) + (_bcnt);\
		for (_bin_type* _curr = (_bins); _curr != _end; ++_curr) {\
			_Bool _flg;\
			_is_empty((_flg), (*_curr), ##__VA_ARGS__);\
			if (!_flg) {\
				unsigned long long int _loc;\
				_pull_hash((_loc), (*_curr), ##__VA_ARGS__);\
				_loc = _loc % _nb;\
				while (1) {\
					_Bool _flg;\
					_is_empty((_flg), (_nbins[_loc]), ##__VA_ARGS__);\
					if (_flg) break;\
					_loc = (_loc + 1) % _nb;\
				}\
				_nbins[_loc] = *_curr;\
			}\
		}\
		_free((_bins), ##__VA_ARGS__);\
		(_bcnt) = _nb;\
		(_bins) = _nbins;\
	}\
	unsigned long long int _hs;\
	_hash((_hs), (_key), ##__VA_ARGS__);\
	unsigned long long int _loc = _hs % (_bcnt);\
	_Bool _chk = 0;\
	while (1) {\
		_Bool _flg;\
		_is_empty((_flg), ((_bins)[_loc]), ##__VA_ARGS__);\
		if (_flg) break;\
		_is_match((_chk), (_key), ((_bins)[_loc]), ##__VA_ARGS__);\
		if (_chk) break;\
		_loc = (_loc + 1) % (_bcnt);\
	}\
	if (_chk) {_exist((_out), (_hs), (_key), ((_bins)[_loc]), ##__VA_ARGS__); break;}\
	_fill((_chk), (_hs), (_key), ((_bins)[_loc]), ##__VA_ARGS__);\
	if (_chk) {_mem((_out), ##__VA_ARGS__); break;}\
	++(_ecnt);\
	_ret((_out), ((_bins)[_loc]), ##__VA_ARGS__);\
} while (0)

#endif
