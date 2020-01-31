#ifndef debug_hh
#define debug_hh debug_hh

#include <iostream>
#include <string.h>

using namespace std;

template<typename first_t, typename second_t>
inline ostream& operator<<(ostream& lhs, const pair<first_t, second_t>& rhs)
{
  return lhs << "pair{" << rhs.first << "," << rhs.second << "}";
};
#ifdef HAVE_COMMANDLINE
inline ostream& operator<<(ostream& lhs, const CommandLine& cmdline)
{
  lhs << "CommandLine{\n";
  lhs << "  FileList{\n";
  {
    auto p = cmdline.FileList;
    if (*p)
      lhs << "    " << *p++;
    while (*p)
      lhs << ",\n    " << *p++;
  };
  lhs << "\n";
  lhs << "  }\n";
  lhs << "}\n";
  return lhs;
};
#endif
inline ostream& operator<<(ostream& lhs, const char** list)
{
  if (*list) {
    lhs << *list++;
    while (*list)
      lhs << "\n" << *list++;
    lhs << endl;
  };
  return lhs;
};

template<typename val_t>
inline void do_show_val(const char* file,
                        unsigned line,
                        const char* name,
                        const val_t& val)
{
  cerr << file << ":" << line << ":" << name << "=>(" << val << ")" << endl;
};
inline void do_show_func(const char* file, unsigned line, const char* func)
{
  cerr << file << ":" << line << ":" << func << endl;
};
#define show_val(x) do_show_val(__FILE__, __LINE__, #x, x)
struct show_func_t {
  const char *file;
  size_t line;
  const char *func;
  show_func_t(const char *file, size_t line, const char *func);
  ~show_func_t();

};
#define show_func() show_func_t __show_me(__FILE__,__LINE__,__PRETTY_FUNCTION__)

inline void perror(const char *msg1, const char *msg2)
{
  size_t len1=strlen(msg1);
  size_t len2=strlen(msg2);
  char msg[len1+len2+2];
  char *dst=msg;
  while(char ch=*msg1++)
    *dst++=ch;
  *dst++=':';
  while(char ch=*msg2++)
    *dst++=ch;
  *dst=0;
  perror(msg);
};
inline void pexit(const char *msg1, const char *msg2)
{
  perror(msg1,msg2);
  exit(1);
};
inline void pexit(const char *msg)
{
  perror(msg);
  exit(1);
};

#endif
