build: lab2_list
lab2_list:
	gcc lab2_list.c SortedList.c -o lab2_list -lpthread -g -Wall -Wextra -lprofiler 
clean:
	-rm -f lab2_list 

dist: clean graphs
cleandata:
	-rm lab2b_list.csv lab2_list-1.png
graphs: clean tests
	chmod +x lab2_list.gp
	./lab2_list.gp
profile:clean build
	-rm -f raw.gperf profile.out 
	LD_PRELOAD=/usr/lib64/libprofiler.so CPUPROFILE=./raw.gperf ./lab2_list --threads=12 --iterations=1000 --sync=s
	pprof --text lab2_list raw.gperf > profile.out
	pprof --list=wrapper lab2_list raw.gperf >> profile.out
	-rm -f raw.gperf
tests: build
	./lab2_list --threads=1  --iterations=1000 --sync=m > lab2b_list.csv
	./lab2_list --threads=2  --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=4  --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=8  --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --threads=1  --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=2  --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=4  --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=8  --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=16 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --threads=24 --iterations=1000 --sync=s >> lab2b_list.csv

#Running tests for --list option
	-./lab2_list --list=4 --threads=1 --iterations=1 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=1 --iterations=2 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=1 --iterations=4 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=1 --iterations=8 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=1 --iterations=16 --yield=id >> lab2b_list.csv

	-./lab2_list --list=4 --threads=4 --iterations=1 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=4 --iterations=2 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=4 --iterations=4 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=4 --iterations=8 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=4 --iterations=16 --yield=id >> lab2b_list.csv

	-./lab2_list --list=4 --threads=8 --iterations=1 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=8 --iterations=2 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=8 --iterations=4 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=8 --iterations=8 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=8 --iterations=16 --yield=id >> lab2b_list.csv

	-./lab2_list --list=4 --threads=12 --iterations=1 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=12 --iterations=2 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=12 --iterations=4 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=12 --iterations=8 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=12 --iterations=16 --yield=id >> lab2b_list.csv

	-./lab2_list --list=4 --threads=16 --iterations=1 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=16 --iterations=2 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=16 --iterations=4 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=16 --iterations=8 --yield=id >> lab2b_list.csv
	-./lab2_list --list=4 --threads=16 --iterations=16 --yield=id >> lab2b_list.csv

#running with --list option and sync options
	./lab2_list --list=4 --threads=1 --iterations=10 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=1 --iterations=20 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=1 --iterations=40 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=1 --iterations=80 --yield=id --sync=s >> lab2b_list.csv

	./lab2_list --list=4 --threads=4 --iterations=10 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=4 --iterations=20 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=4 --iterations=40 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=4 --iterations=80 --yield=id --sync=s >> lab2b_list.csv

	./lab2_list --list=4 --threads=8 --iterations=10 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=8 --iterations=20 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=8 --iterations=40 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=8 --iterations=80 --yield=id --sync=s >> lab2b_list.csv

	./lab2_list --list=4 --threads=12 --iterations=10 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=12 --iterations=20 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=12 --iterations=40 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=12 --iterations=80 --yield=id --sync=s >> lab2b_list.csv

	./lab2_list --list=4 --threads=16 --iterations=10 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=16 --iterations=20 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=16 --iterations=40 --yield=id --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=16 --iterations=80 --yield=id --sync=s >> lab2b_list.csv

	./lab2_list --list=4 --threads=1 --iterations=10 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=1 --iterations=20 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=1 --iterations=40 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=1 --iterations=80 --yield=id --sync=m >> lab2b_list.csv

	./lab2_list --list=4 --threads=4 --iterations=10 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=4 --iterations=20 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=4 --iterations=40 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=4 --iterations=80 --yield=id --sync=m >> lab2b_list.csv

	./lab2_list --list=4 --threads=8 --iterations=10 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=8 --iterations=20 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=8 --iterations=40 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=8 --iterations=80 --yield=id --sync=m >> lab2b_list.csv

	./lab2_list --list=4 --threads=12 --iterations=10 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=12 --iterations=20 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=12 --iterations=40 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=12 --iterations=80 --yield=id --sync=m >> lab2b_list.csv

	./lab2_list --list=4 --threads=16 --iterations=10 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=16 --iterations=20 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=16 --iterations=40 --yield=id --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=16 --iterations=80 --yield=id --sync=m >> lab2b_list.csv

#rerun without yields
	./lab2_list --list=1 --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --list=1 --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --list=1 --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --list=1 --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --list=1 --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv

	./lab2_list --list=4 --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --list=4 --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv

	./lab2_list --list=8 --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --list=8 --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --list=8 --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --list=8 --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --list=8 --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv

	./lab2_list --list=16 --threads=1 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --list=16 --threads=2 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --list=16 --threads=4 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --list=16 --threads=8 --iterations=1000 --sync=s >> lab2b_list.csv
	./lab2_list --list=16 --threads=12 --iterations=1000 --sync=s >> lab2b_list.csv

	./lab2_list --list=1 --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --list=1 --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --list=1 --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --list=1 --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --list=1 --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv

	./lab2_list --list=4 --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --list=4 --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv

	./lab2_list --list=8 --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --list=8 --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --list=8 --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --list=8 --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --list=8 --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv

	./lab2_list --list=16 --threads=1 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --list=16 --threads=2 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --list=16 --threads=4 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --list=16 --threads=8 --iterations=1000 --sync=m >> lab2b_list.csv
	./lab2_list --list=16 --threads=12 --iterations=1000 --sync=m >> lab2b_list.csv
