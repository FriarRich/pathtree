#include <signal.h>
#include <unistd.h>

#include <algorithm>
#include <cassert>
#include <cstdarg>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <chrono>
#include <cmath>

#include "debug.hh"
#include "mmap.hh"
#include "path_tree.hh"
#include "c_str.hh"

using namespace std;
bool stats = false;
void sigalrm( int sig )
{
  stats = true;
};

class IosFlagSaver {
public:
    explicit IosFlagSaver(std::ostream& _ios):
        ios(_ios),
        f(_ios.flags()) {
    }
    ~IosFlagSaver() {
        ios.flags(f);
    }

    IosFlagSaver(const IosFlagSaver &rhs) = delete;
    IosFlagSaver& operator= (const IosFlagSaver& rhs) = delete;

private:
    std::ostream& ios;
    std::ios::fmtflags f;
};
static size_t npaths=0;
void show_stats()
{
  IosFlagSaver save(cerr);
  static auto start = chrono::high_resolution_clock::now(); 
  auto curr=chrono::system_clock::now();
  chrono::duration<double> elapsed_seconds = curr-start;
  double elap=round(elapsed_seconds.count()*10000)/10000;
  if(elap) {
    cerr << "elapsed: " << setw(10) << fixed << setprecision(5) << elap;
    cerr << setw( 10 ) << npaths << " paths ";
    if(elap) {
      long persec=round(10000*npaths/elap)/10000;
      cerr << setw( 10 ) << setprecision(5)
        << persec << " paths/second" << endl;
    };
  };
  stats = false;
  alarm( 1 );
};

template<typename str_t>
void doit()
{
  show_func();
  path_tree<str_t> paths;
  void *vp = new char[1024];
  show_stats();
  mmap_t file( "locate.dat" );
  auto b(begin(file)), e(end(file));
  while ( b != e )
  {
    while(b!=e && *b=='\n')
      ++b;
    auto p(b);
    while(b!=e && *b!='\n')
      ++b;
    paths[ str_t( p,b ) ]++;
    npaths++;
    if(stats)
      show_stats();
  };
  show_stats();
  paths.dump();
};
int main( int, char**, const char** envp )
{
  signal( SIGALRM, sigalrm );
  cout.sync_with_stdio( false );

  float pi=3.1;
  cerr << pi << endl;
  if(getenv("USE_STRING")){
    doit<string>();
  } else {
    doit<c_str>();
  };
  cerr << pi << endl;
  return 0;
};
