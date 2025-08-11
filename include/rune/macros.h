#ifndef C_MACROS_H_
#define C_MACROS_H_

#ifndef inline_
    #define inline_ inline
#endif

#ifndef force_inline_
    #define force_inline_ __attribute__((always_inline, hot))
#endif

#ifndef aligned_
    #define aligned_(n) __attribute__((aligned(n)))
#endif

#ifndef pure_
    #define pure_ __attribute__((pure))
#endif

#ifndef const_
    #define const_ __attribute__((const))
#endif

#ifndef constructor_
    #define constructor_ __attribute__((constructor))
#endif

#ifndef destructor_
    #define destructor_ __attribute__((destructor))
#endif

#ifndef unused_
    #define unused_ __attribute__((unused))
#endif

#ifndef defer_
    #define defer_(func) __attribute__((cleanup(func)))
#endif

#endif
