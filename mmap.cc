#include "global.hh"
#include "mmap.hh"
#include "debug.hh"
#include "xunistd.hh"

#include <fcntl.h>
#include <sys/mman.h>

mmap_t::body_t::~body_t()
{
  xmunmap(data,size); 
};
mmap_t::body_t::body_t( const string& name )
    : name( name )
    , data( 0 )
    , size( 0 )
    , fd( -1 )
{
  fd = xopen( name.c_str(), O_RDONLY );
  // we use a temporary signed value because we have to
  // check for a negative
  size = xlseek( fd, 0, SEEK_END );
  data = (mmap_t::arr_t*)xmmap( 0, size, PROT_READ, MAP_PRIVATE, fd, 0 );
};
