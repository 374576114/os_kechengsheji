obj := main.o cpu_name.o version.o start_time.o \
	user.o hostname.o get_process_info.o \
	cpu.o mem.o

1:$(obj)
	cc -o $@ $(obj) `pkg-config --libs --cflags gtk+-2.0`
main.o:main.c task.h
	cc -c $< `pkg-config --libs --cflags gtk+-2.0`
cpu_name.o :cpu_name.c task.h
	cc -c $<
version.o : version.c task.h
	cc -c $<
start_time.o : start_time.c task.h
	cc -c $<
user.o : user.c task.h
	cc -c $<
hostname.o : hostname.c task.h
	cc -c $<
get_process_info.o : get_process_info.c task.h
	cc -c $<
cpu.o : cpu.c task.h
	cc -c $<
mem.o : mem.c task.h
	cc -c $<

.PHONY :clean
	rm *.o 1
