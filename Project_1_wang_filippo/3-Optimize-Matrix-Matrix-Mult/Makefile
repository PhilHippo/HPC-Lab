CC = gcc                         # compiler (& linker)
OPT = -O3 -march=native \
      -funroll-loops    \
      -ftree-vectorize           # optimization flags (you may add more)
CFLAGS = -Wall -std=gnu99 $(OPT) # standard compiler flags

CFLAGS += -I"${MKLROOT}/include"  # more header files
LDLIBS = -L${MKLROOT}/lib/intel64 \
      -lmkl_intel_lp64         \
      -lmkl_sequential         \
      -lmkl_core               \
      -lpthread -lm -ldl       # linker options

################################################################################

targets = benchmark-naive benchmark-blocked-naive benchmark-blocked-LR benchmark-blocked-LR-DP benchmark-blocked-LR-LU benchmark-blas
objects = benchmark.o dgemm-naive.o dgemm-blocked-naive.o dgemm-blocked-LR.o dgemm-blocked-LR-DP.o dgemm-blocked-LR-LU.o dgemm-blas.o
results = timing_basic_dgemm.data   \
          timing_blas_dgemm.data    \
          timing_blocked_dgemm_LR.data \
          timing_blocked_dgemm_LR_DP.data \
          timing_blocked_dgemm_LR_LU.data \
          timing.pdf

.PHONY: default
default: all

.PHONY: all
all: $(targets)

benchmark-naive: benchmark.o dgemm-naive.o 
	$(CC) -o $@ $^ $(LDLIBS)
benchmark-blocked-naive: benchmark.o dgemm-blocked-naive.o
	$(CC) -o $@ $^ $(LDLIBS)
benchmark-blocked-LR: benchmark.o dgemm-blocked-LR.o
	$(CC) -o $@ $^ $(LDLIBS)
benchmark-blocked-LR-DP: benchmark.o dgemm-blocked-LR-DP.o
	$(CC) -o $@ $^ $(LDLIBS)
benchmark-blocked-LR-LU: benchmark.o dgemm-blocked-LR-LU.o
	$(CC) -o $@ $^ $(LDLIBS)
benchmark-blas: benchmark.o dgemm-blas.o
	$(CC) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) -c $(CFLAGS) $<

.PHONY: clean
clean:
	$(RM) $(targets) $(objects)

.PHONY: cleanall
cleanall: clean
	$(RM) $(results)
