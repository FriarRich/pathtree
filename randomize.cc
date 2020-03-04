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

void doit()
{
  vector<c_str> lines;
  mmap_t file( "locate.dat" );
  for(auto b(begin(file)), e(end(file)); b != e; b++ )
  {
    auto p(b);
    while(b!=e && *b!='\n')
      ++b;
    if(p!=b)
      lines.push_back(c_str(p,b));
  };
  size_t epos=lines.size()-1;
  while(epos>1)
    swap(lines[random()%epos],lines[--epos]);
  if(getenv("UNRANDOMIZE"))
    sort(lines.begin(),lines.end());
  char *buf=new char[file.size()];
  char *pos=buf;
  char *end=buf+file.size();
  for(auto l:lines) {
    assert(pos+l.len()<=end);
    while(l)
      *pos++=*l++;
    *pos++='\n';
  };
  pos=buf;
  while(pos!=end)
    pos+=xwrite(1,pos,end-pos);

};
int main( int, char**, const char** envp )
{
  signal( SIGALRM, sigalrm );
  cout.sync_with_stdio( false );

  doit();
  return 0;
};
