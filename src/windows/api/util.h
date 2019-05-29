#ifndef API_UTIL_H
#define API_UTIL_H

#include <stdexcept>
#include <string>

#define handleError(func, args, err) {\
    SetLastError(0);\
    if (func args == err) {\
        LPVOID errBuf;\
        FormatMessage(\
            FORMAT_MESSAGE_ALLOCATE_BUFFER |\
            FORMAT_MESSAGE_FROM_SYSTEM |\
            FORMAT_MESSAGE_IGNORE_INSERTS,\
            NULL,\
            GetLastError(),\
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),\
            (LPTSTR) &errBuf,\
            0,\
            NULL\
        );\
        throw std::runtime_error((const char*)errBuf);\
    }\
}

#endif
