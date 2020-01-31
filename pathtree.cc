#include "global.hh"
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
path_tree<c_str> paths;
template<typename str_t>
void show_stats( const path_tree<str_t>& paths )
{
  cerr << setw( 10 ) << paths.size() << " paths" << endl;
  stats = false;
  if(0)
    alarm( 1 );
};

template<typename str_t>
void doit()
{
  show_func();
  path_tree<str_t> paths;
#if 1
  show_stats(paths);
  mmap_t file( "locate.out" );
  auto b(begin(file)), e(end(file));
  while ( b != e )
  {
    while(b!=e && *b=='\n')
      ++b;
    auto p(b);
    while(b!=e && *b!='\n')
      ++b;
    paths[ str_t( p,b ) ]++;
    if(stats)
      show_stats(paths);
  };
  show_stats(paths);
#elif 0
  string line;
  array< char, 64 * 1024 > buf;
  size_t pos = 0;
  while ( size_t nread = xread( 0, buf.begin() + pos, buf.size() - pos ) )
  {
    auto b( buf.begin() );
    auto e( b + nread + pos );
    auto x( e );
    while ( b != e && e[ -1 ] != '\n' )
      --e;
    while ( b != e )
    {
      auto p( b );
      while ( b != e && *b != '\n' )
        ++b;
      string path( p, b );
      paths[ path ]++;
      if ( stats )
      {
        show_stats( paths );
        stats = false;
        alarm( 1 );
      }
      ++b;
    };
    auto d( buf.begin() );
    while ( e != x )
      *d++ = *e++;
    pos = d - buf.begin();
  };
#else
  while ( getline( cin, line ) )
  {
    paths[ line ]++;
    if ( stats )
    {
      if ( paths.size() )
      {
        time_t etime = time( 0 ) - stime;
        cerr << setw( 5 ) << etime;
        cerr << setw( 10 ) << paths.size();
        cerr << setw( 10 ) << paths.size() / etime;
        cerr << endl;
      };
      stats = false;
      alarm( 1 );
    }
    paths[ line ]++;
  };
#endif
  paths.dump();
};
ptrdiff_t diff(void *vp1, void *vp2)
{
  char *cp1=(char*)vp1;
  char *cp2=(char*)vp2;
  return cp2-cp1;
};
int main( int, char**, const char** envp )
{
  cout.sync_with_stdio( false );
  signal( SIGALRM, sigalrm );

  show_val(sizeof(string));
  show_val(sizeof(c_str));
// Using time point and system_clock 
  chrono::duration<double> elapsed_seconds;
  auto start = chrono::system_clock::now(); 
  auto end = start;
  int runs=10;
  char *str_runs=getenv("NUM_RUNS");
  if(str_runs)
    runs=atoi(str_runs);
  cerr << "doing " << runs << " runs" << endl;
  for(int i=0;i<runs;i++) {
    start=end;
    void *brk1=(char*)sbrk(0);
    if(getenv("USE_STRING")){
      doit<string>();
    } else {
      doit<c_str>();
    };
    void *brk2=(char*)sbrk(0);
    cerr << "brk: from " << brk1 << " to " << brk2 << " delta "
      << diff(brk2,brk1) << endl;
    end = chrono::system_clock::now(); 
    elapsed_seconds = end - start;
    cerr << "elapsed time: " << elapsed_seconds.count() << "s\n"; 
  };

#if 1
#else
  cerr << "running locate '/'" << endl;
  int fds[ 2 ];
  xpipe( fds );
  if ( pid_t pid = fork() )
  {
    cerr << "p:" << pid << endl;
    xdup2( fds[ 0 ], 0 );
    xclose( fds[ 1 ] );
    doit();
    sleep( 1 );
    xclose( 0 );
    xclose( 1 );
    exit( 0 );
  }
  else
  {
    cerr << "c:" << pid << endl;
    xclose( fds[ 0 ] );
    xdup2( fds[ 1 ], 1 );
    xexeclp( "locate", "locate", "/", nullptr );
    abort();
  };
#endif
  return 0;
};
