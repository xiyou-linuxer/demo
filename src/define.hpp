#ifndef __DEFINE_HPP
#define __DEFINE_HPP

#include "easylogging++.h"

namespace Demo {

#define UNUSED __attribute__((unused))

#define UV_OK 0
#define DEFAULT_BACKLOG 512

#define CLIENT_READ_BUFFER_MAX (1024*1024*10)

} // namespace Demo

#endif // __DEFINE_HPP
