#ifndef xunistd_hh
#define xunistd_hh xunistd_hh

#include <unistd.h>
#include <sys/signal.h>
#include <sys/types.h>

typedef int fd_t;
fd_t xopen(const char *pathname, int flags, mode_t mode=0644);

fd_t xcreat(const char *pathname, mode_t mode);

fd_t xopenat(int dirfd, const char *pathname, int flags);
fd_t xopenat(int dirfd, const char *pathname, int flags, mode_t mode);
void *xmmap(void *addr, size_t length, int prot, int flags,
    fd_t fd, off_t offset);
int xmunmap(void *addr, size_t length);
off_t xlseek(fd_t fd, off_t offset, int whence);
pid_t xfork();
size_t xread( fd_t fd, char *buf, size_t len );
size_t xwrite( fd_t fd, const char *buf, size_t len );
void xpipe( int ( &fds )[ 2 ] );
void xdup2( int ofd, int nfd );
void xclose( fd_t fd );
sighandler_t xsignal( int sig, sighandler_t hand );
void xexeclp( const char* file, const char* arg, ... );


#endif
