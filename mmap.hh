#ifndef mmap_hh
#define mmap_hh mmap_hh

#include <array>
#include <string>

using std::array;
using std::string;
struct mmap_t
{
  typedef array< char, 1 > arr_t;
  typedef typename arr_t::iterator iterator;

  struct body_t
  {
    string name;
    arr_t* data;
    size_t size;
    int fd;
    body_t( const string& name );
    ~body_t();
  } body;

  mmap_t( const string& filename )
      : body( filename )
  {
  }
  mmap_t( const char* filename )
      : body( filename )
  {
  }
  int size() const {
    return body.size;
  };
  iterator begin() { return body.data->begin(); };
  iterator end() { return begin() + body.size; };

private:
  mmap_t( const mmap_t& );
  mmap_t& operator=( const mmap_t& );
};

#endif
