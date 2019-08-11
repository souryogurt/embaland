#ifndef EMBALAND_COMPILER_H
#define EMBALAND_COMPILER_H 1

#ifdef __GNUC__
#define member_type(type, member) __typeof__(((type *)0)->member)
#else
#define member_type(type, member) const void
#endif

#define container_of(ptr, type, member)                                  \
	((type *)((char *)(void *)(member_type(type, member) *){ ptr } - \
		  offsetof(type, member)))

#endif
