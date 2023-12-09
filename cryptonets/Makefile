# directories
ROOTDIR=$(abspath ..)
USERDIR=$(abspath .)
PROJECTDIR=$(abspath .)
SRCDIR=$(ROOTDIR)/src
LIBDIR=$(PROJECTDIR)/util
DATADIR=$(PROJECTDIR)/data
SMARTLIB=$(ROOTDIR)/lib
TYPEDIR=$(ROOTDIR)/type
WRAPPERDIR=$(ROOTDIR)/seal/wrapper

# input data
IN=$(DATADIR)/x_test.tmp
OUT=$(DATADIR)/y_test.tmp
W1=$(DATADIR)/W1.tmp
W2=$(DATADIR)/W4.tmp
W3=$(DATADIR)/W8.tmp
B1=$(DATADIR)/b1.tmp
B2=$(DATADIR)/b4.tmp
B3=$(DATADIR)/b8.tmp

TEMPLATE=0
DEPTH=3
POOL=1 # { 0: standard, 1: shared }
CLEAR=0
N=8192
LRU=1
SIZE=-1
CACHE_RESIZE=-1
CT_ADD=0
CT_MUL=0
CT_SUB=0
PT_ADD=0
PT_MUL=1
PT_SUB=0

# compiler, flags, incs, and libs
CC=g++
FLAGS=-O2 -std=c++17
INCS=-I$(ROOTDIR)/3p/seal_unx/include \
	-I$(LIBDIR) -I$(SMARTLIB) -I$(TYPEDIR) -I$(WRAPPERDIR)
CPPS=\
	$(SMARTLIB)/math.cpp \
	$(WRAPPERDIR)/seal_bfv_keys.cpp \
	$(WRAPPERDIR)/seal_bfv_plaintext.cpp \
	$(WRAPPERDIR)/seal_bfv_ciphertext.cpp \
	$(TYPEDIR)/common.cpp
ifeq ($(TEMPLATE),8)
CPPS+=\
	$(TYPEDIR)/cache_notempl_cache.cpp \
	$(TYPEDIR)/cache_notempl_wrapper.cpp
endif
LIBS=$(ROOTDIR)/3p/seal_unx/target/libseal.a
DEFINES=-DUSING_CRT \
 	-DTEMPLATE=$(TEMPLATE) -DDEFAULT_DEPTH=$(DEPTH) -DSHARED_POOL=$(POOL) \
	-DMAX_CACHE_SIZE=$(SIZE) -DPOLYNOMIAL_DEGREE=$(N) \
	-DCTADD=$(CT_ADD) -DCTMUL=$(CT_MUL) -DCTSUB=$(CT_SUB) \
	-DPTADD=$(PT_ADD) -DPTMUL=$(PT_MUL) -DPTSUB=$(PT_SUB) \
	-DLRU=$(LRU) -DCACHE_RESIZE=$(CACHE_RESIZE)
ifeq ($(CLEAR),1)
DEFINES+=-DVECTOR_CLEAR
endif

all: compile

%: %.cpp
	$(CC) $(FLAGS) $(INCS) $(CPPS) $(LIBS) $< -o $@.exe $(DEFINES)

clean:
	rm -f *.o *.exe *.tmp

compile: main

run:
	./main.exe $(IN) $(W1) $(W2) $(W3) $(B1) $(B2) $(B3) $(OUT)
