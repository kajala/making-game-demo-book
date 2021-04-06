#ifndef _LANG_PP
#define _LANG_PP


#ifndef PLATFORM_PS2
#if (defined(__ee__) && defined(__GCC__)) || defined(SN_TARGET_PS2)
#define PLATFORM_PS2
#endif
#endif

#ifndef PLATFORM_WIN32
#if defined(WIN32) || defined(_WIN32)
#define PLATFORM_WIN32
#endif
#endif

#ifndef PLATFORM_SYMBIAN
#ifdef __SYMBIAN32__
#define PLATFORM_SYMBIAN
#endif
#endif


#endif // _LANG_PP
