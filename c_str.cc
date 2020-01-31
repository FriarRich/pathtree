#include "global.hh"
#include "c_str.hh"
#include <unistd.h>
#include <string.h>

std::ostream &operator<<(std::ostream &lhs, const c_str &rhs)
{
  return lhs.write(rhs.beg,rhs.len());
};
ssize_t write(fd_t fd, const c_str& str)
{
  return write(fd, str.beg, str.end - str.beg);
};
const char c_str::null_str[1]="";
