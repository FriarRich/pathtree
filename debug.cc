#include "debug.hh"
#include <iostream>
using namespace std;

show_func_t::show_func_t(const char *file, size_t line, const char *func)
  : file(file), line(line), func(func)
{
  cerr << file << ":" << line << ":" << func << ":" << "enter" << endl;
};
show_func_t::~show_func_t()
{
  cerr << file << ":" << line << ":" << func << ":" << "leave" << endl;
};
