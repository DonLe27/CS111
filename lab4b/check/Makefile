default:
	gcc -lm -lmraa -Wall -Wextra -g -o lab4b lab4b.c
clean:
	-rm -f lab4b lab4b-804971410.tar.gz
dist:
	tar -czvf lab4b-804971410.tar.gz README lab4b.c Makefile tests
check:
#referencing some tests from provided test script
	-chmod +x tests
	-./tests
