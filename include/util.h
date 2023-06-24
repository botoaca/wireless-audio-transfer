#ifdef _WIN32
#include <stdlib.h>
#define realpath(N,R) _fullpath((R), (N), _MAX_PATH)
#endif