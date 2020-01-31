MAKEFLAGS= -rR
STATIC=
DEPFLAGS= -MF $<.d -MT $@ -MD -MP
all:

test: all locate.dat
	./pathtree

cxx_exe:= pathtree
cxx_ign:=
cxx_src:= $(wildcard *.cc)
cxx_lib:= $(filter-out $(patsubst %,%.cc,$(cxx_ign) $(cxx_exe)), $(cxx_src))
cxx_obj:= $(patsubst %,%.o,$(cxx_lib))

.PHONY:

libutil.a: $(cxx_obj)
	ar rU $@ $(cxx_obj)

all: $(cxx_exe)

$(cxx_obj): %.cc.o: %.cc cxxflags
	g++ @cxxflags $(STATIC)  -g -E $<    -o $<.i -fPIC $(DEPFLAGS)
	g++ @cxxflags $(STATIC)  -g -S $<.i  -o $<.s -fPIC
	g++ @cxxflags $(STATIC)  -g -c $<.s  -o $@   -fPIC

$(cxx_exe): %: %.cc cxxflags libutil.a
	g++ @cxxflags $(STATIC)  -g -E $<    -o $<.i -fPIC $(DEPFLAGS)
	g++ @cxxflags $(STATIC)  -g -S $<.i  -o $<.s -fPIC
	g++ @cxxflags $(STATIC)  -g    $<.s  -o $@   -fPIC -L. -lutil

include /dev/null $(wildcard *.d)

clean:
	rm -f $(cxx_exe) libutil.a tags gmon.out
	rm -f *.cc.[isod]

tags: $(patsubst %.cc,%.cc.d,$(cxx_src))
	cat $(patsubst %.cc,%.cc.d,$(cxx_src)) | \
		tr ' \\:' '\n' | sort -u | \
		xargs ctags

locate.dat:
	locate / > $@.new
	mv $@.new $@
