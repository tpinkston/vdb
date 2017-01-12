#ifndef VDIS_SYSTEM_H
#define VDIS_SYSTEM_H

// C++ headers
#include <algorithm>
#include <cerrno>
#include <cmath>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <limits>
#include <list>
#include <iomanip>
#include <map>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>

// C headers
#include <arpa/inet.h>
#include <endian.h>
#include <netdb.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/udp.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef __CYGWIN__
#include <cygwin/socket.h>
#else
#include <sys/socket.h>
#endif

typedef float               float32_t;
typedef double              float64_t;
typedef std::string         string_t;

#endif
