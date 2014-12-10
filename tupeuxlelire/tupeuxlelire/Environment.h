/*
 *  Détection de l'OS
**/

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

//WINDOWS
#if !defined(SAG_COM) && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#   define OS_WIN64
#   define OS_WIN32
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#   if defined(WINCE) || defined(_WIN32_WCE)
#       define OS_WINCE
#   elif defined(WINAPI_FAMILY)
#       if WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
#           define OS_WINPHONE
#           define OS_WINRT
#       elif WINAPI_FAMILY==WINAPI_FAMILY_APP
#           define OS_WINRT
#       else
#           define OS_WIN32
#       endif
#   endif
#else
#   define OS_WIN32
#endif

#if defined(OS_WIN32) || defined(OS_WIN64) || defined(OS_WINCE) || defined(OS_WINRT)
#   define OS_WIN
#endif

//LINUX
#if defined(__linux__) || defined(__linux)
#   define OS_LINUX
#endif

//MAC
#if defined(__APPLE__)
#   define OS_MAC
#elif defined(Macintosh) || defined(macintosh)
#   error "Le programme n'est utilisable pour l'instant que sur Mac OS X."
#endif

//Si aucun des OS supporté n'est défini.
#if !defined(OS_WIN) && !defined(OS_MAC) && !defined(OS_LINUX)
#   error "Désolé mais le programme n'est pas compilable sur cet OS. Il est pour l'instant compatible avec Windows, Linux et Mac OS X"
#endif

#endif // ENVIRONMENT_H
