#ifndef c_str_hh
#define c_str_hh c_str_hh

#include <cassert>
#include <iostream>
#include <string.h>
#include <sys/types.h>

struct c_str
{
  char* beg;
  char* end;
  static const char null_str[ 1 ];
  c_str()
      : beg( (char*)null_str )
      , end( (char*)null_str )
  {
  };
  c_str( const char* b, const char* e = 0 )
      : beg( (char*)b )
      , end( (char*)e )
  {
    if ( end )
      return;
    end = beg;
    while ( *end )
      ++end;
  };
  c_str( char* b, char* e = 0 )
      : beg( b )
      , end( e )
  {
    if ( end )
      return;
    end = beg;
    while ( *end )
      ++end;
  };
  c_str( const char* b, size_t l )
      : beg( (char*)b )
      , end( (char*)b + l )
  {
  };
  c_str( char* b, size_t l )
      : beg( b )
      , end( b + l )
  {
  };
  c_str( const c_str& lhs )
      : beg( lhs.beg )
      , end( lhs.end )
  {
  };
  template < size_t n >
  c_str( const char ( &str )[ n ] )
      : beg( str )
      , end( str + n - 1 ){};

  size_t size() const
  {
    return end - beg;
  };
  size_t len() const
  {
    return end - beg;
  };
  char operator[]( size_t pos ) const
  {
    return beg[ pos ];
  };
  explicit operator bool() const
  {
    return beg < end;
  }
  bool operator!() const { return !bool( *this ); }
  c_str operator++( int )
  {
    c_str res( *this );
    if ( beg < end )
      ++beg;
    return res;
  };
  c_str& operator++()
  {
    if ( beg < end )
      ++beg;
    return *this;
  };
  char operator*() const
  {
    return *beg;
  };
  static int cmp( size_t lhs, size_t rhs );
  static int cmp( const c_str& lhs, const c_str& rhs );
};
#define cmp_op( x )                                                       \
  inline bool operator x( const c_str& lhs, const c_str& rhs )            \
  {                                                                       \
    return c_str::cmp( lhs, rhs ) x 0;                                    \
  };
cmp_op( < );
cmp_op( > );
cmp_op( <= );
cmp_op( >= );
cmp_op( == );
cmp_op( != );
typedef int fd_t;
ssize_t write( fd_t fd, const c_str& str );
std::ostream& operator<<( std::ostream& lhs, const c_str& rhs );
inline int c_str::cmp( size_t lhs, size_t rhs )
{
  return lhs - rhs;
};
inline int c_str::cmp( const c_str& lhs, const c_str& rhs )
{
  int res = 0; // cmp(lhs.len(),rhs.len());
  if ( !res )
    res = lhs.len()-rhs.len();
  if ( !res )
    res = strncmp( lhs.beg, rhs.beg, lhs.len() );
  return res;
};
namespace std
{
  template <>
  struct hash< c_str > : public __hash_base< size_t, c_str >
  {
    size_t operator()( const c_str&s ) const noexcept
    {
      return std::_Hash_impl::hash(s.beg, s.len() );
    }
  };
}; // namespace std
inline const char* end( const c_str& cont )
{
  return cont.end;
};
inline const char* begin( const c_str& cont )
{
  return cont.beg;
};
inline char* end( c_str& cont )
{
  return cont.end;
};
inline char* begin( c_str& cont )
{
  return cont.beg;
};
#endif
