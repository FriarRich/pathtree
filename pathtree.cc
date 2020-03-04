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

#include <boost/range.hpp>

#include "debug.hh"
#include "mmap.hh"
#include "path_tree.hh"
#include "c_str.hh"

#undef show_func
#define show_func()

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
    str_t str { p, b };
    cout << str << endl;
    paths[ str_t( p,b ) ]++;
    npaths++;
    if(stats)
      show_stats();
  };
  show_stats();
  paths.dump();
};
namespace nobody {
  template<typename str_t>
  void path_tree<str_t>::dump()
  {
    if ( getenv( "NO_DUMP" ) )
      return;
    vector<char> text;
    data.dump( text, 0 );
    size_t pos = 0;
    while ( pos < text.size() )
    {
      size_t res = xwrite( 1, text.data() + pos, text.size() - pos );
      cerr << "res=" << res << endl;
      pos += res;
    };
  };
  template<typename str_t>
  void path_tree<str_t>::data_t::dump( vector<char> &text , size_t depth )
  {
    show_func();
    for(int i=0;i<depth;i++)
      text.push_back(' ');
    show_val(text.size());
    cout << "key: " << key << endl;
    if ( key.size() ) {
      show_val(text.size());
      auto b(begin(key));
      auto e(end(key));
      show_val(e-b);
      for(size_t i=0;i<key.size();i++)
        text.push_back(key[i]);
      show_val(text.size());
    } else
      text.push_back('/');
    text.push_back('\n');
    cout << "text{" << text.size() << ",'";
    for( auto ch : text )
      cout << ch;
    cout << "'}" << endl;
    for ( auto sub : subs )
    {
      sub.second.dump( text, 1+depth );
    };
  };
};
int main( int, char**, const char** envp )
{
  signal( SIGALRM, sigalrm );
  cout.sync_with_stdio( false );
  xdup2(1,2);
  if(getenv("USE_STRING")){
    doit<string>();
  } else {
    doit<c_str>();
  };
  return 0;
};
