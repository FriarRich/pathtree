#ifndef pathtree_hh
#define pathtree_hh pathtree_hh
#include "xunistd.hh"
#include <cassert>
#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include <sstream>

namespace nobody
{
  using namespace std;
  template<typename str_t>
  struct path_tree
  {
    struct data_t
    {
      map< str_t, data_t > subs;
      str_t key;
      size_t val;
      data_t( str_t key = str_t() )
          : key( key )
          , val( 0 ){};
      void dump( vector<char>&, size_t );
      auto operator[]( const str_t& key )
      {
        assert( key.find( '/' ) == key.npos );
        return subs[ key ];
      };
    } data;
    size_t bytes;
    size_t _size;
    path_tree()
      : _size(0), bytes(0)
    {
    };
    auto size() const { return _size; }
    size_t& operator[]( const str_t& key )
    {
      data_t* pos = &data;
      auto b( begin(key) ), e( end(key) );
      while ( b != e && *b == '/' )
        ++b;
      while ( b != e )
      {
        auto kb( b );
        while ( b != e && *b != '/' )
          ++b;
        auto ke( b );
        while ( b != e && *b == '/' )
          ++b;
        str_t kk( kb, ke );
        pos = &pos->subs[ kk ];
        bytes++;
        if ( pos->key.size() == 0 )
        {
          pos->key = kk;
          _size++;
          bytes+=kk.size();
        };
      };
      return pos->val;
    };
    void dump();
  };
} // namespace nobody
using namespace nobody;
#endif
