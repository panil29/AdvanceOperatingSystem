
cc=gcc
all:pcrt tcrt mm_bench
pcrt:crt.o et.o ep.o etime.o
	$(cc) crt.o et.o ep.o etime.o -lpthread -lm -o pcrt
tcrt:crt.o et.o ep.o etime.o
	$(cc) crt.o et.o ep.o etime.o -lpthread -lm -o tcrt
mm_bench:mm_thr.o etime.o
	$(cc) mm_thr.o etime.o -lpthread -o mm_bench