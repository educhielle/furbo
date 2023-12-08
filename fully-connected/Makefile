# directories
ROOTDIR=$(abspath ..)
USERDIR=$(abspath .)
LIBDIR=$(ROOTDIR)/lib
TYPEDIR=$(ROOTDIR)/type
WRAPPERDIR=$(ROOTDIR)/seal/wrapper

# config
POOL=1
TEMPLATE=0
DEBUG=0
EVAL=0
SIZE=-1
POLYNOMIAL_DEGREE=8192
CT_ADD=0
CT_MUL=0
CT_SUB=0
PT_ADD=0
PT_MUL=1
PT_SUB=0
LRU=1
CACHE_RESIZE=-1

INCS_SEAL=-I$(ROOTDIR)/3p/seal_unx/include

CPPS_WRAPPER=\
	$(WRAPPERDIR)/seal_bfv_keys.cpp \
	$(WRAPPERDIR)/seal_bfv_plaintext.cpp \
	$(WRAPPERDIR)/seal_bfv_ciphertext.cpp

LIBS_SEAL=$(ROOTDIR)/3p/seal_unx/target/libseal.a

DEFINES=-DDEBUG=$(DEBUG) -DEVAL=$(EVAL) -DSHARED_POOL=$(POOL) \
	-DMAX_CACHE_SIZE=$(SIZE) -DPOLYNOMIAL_DEGREE=$(POLYNOMIAL_DEGREE) \
	-DCTADD=$(CT_ADD) -DCTMUL=$(CT_MUL) -DCTSUB=$(CT_SUB) \
	-DPTADD=$(PT_ADD) -DPTMUL=$(PT_MUL) -DPTSUB=$(PT_SUB) \
	-DLRU=$(LRU) -DCACHE_RESIZE=$(CACHE_RESIZE)

DEFINES+=-DSEAL


# compiler, flags, incs, and libs
CC=g++
FLAGS=-O2 -std=c++17
INCS=$(INCS_SEAL) -I$(LIBDIR) -I$(TYPEDIR) -I$(WRAPPERDIR)
CPPS=$(LIBDIR)/crypto.cpp $(LIBDIR)/math.cpp $(LIBDIR)/matrix.cpp \
	$(TYPEDIR)/common.cpp $(CPPS_WRAPPER)

ifeq ($(TEMPLATE),8)
CPPS+=\
	$(TYPEDIR)/cache_notempl_cache.cpp \
	$(TYPEDIR)/cache_notempl_wrapper.cpp
	# $(TYPEDIR)/cache_notempl_manager.cpp
endif

LIBS=$(LIBS_SEAL)
DEFINES+=-DTEMPLATE=$(TEMPLATE) -DMAX_CACHE_SIZE=$(SIZE)

# parameters
MAIN=main
N=8192
T=65537
ROW=$(N)
MID=1
COL=1
DEPTH=

all: compile

%: %.cpp
	@echo -n "Compiling .. " && \
	$(CC) $(FLAGS) $(INCS) $(CPPS) $(LIBS) -o $@.exe $< $(DEFINES) && \
	echo "ok"

clean:
	rm -f *.o *.exe *.tmp

compile: main

run:
	./$(MAIN).exe $(N) $(T) $(ROW) $(MID) $(COL) $(DEPTH)
