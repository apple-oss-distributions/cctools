# PLATFORM: MACOS IOS

TESTROOT = ../..
include ${TESTROOT}/include/common.makefile

all:
	$(CC) -arch $(ARCH) -Wl,-sectcreate,Memento,Mori,MementoMori1.txt     \
		-o main main.c
	if [ "$(ARCH)" == "x64_64" ]; 					      \
	then								      \
	    ./main | grep "Remember that you will die." | ${FAIL_IF_EMPTY};   \
	fi
	${SEGEDIT} -r Memento Mori MementoMori2.txt -o main2 main
	if [ "$(ARCH)" == "x64_64" ]; 					      \
	then								      \
	    ./main2 | grep "Always remember that you will die." | 	      \
	        ${FAIL_IF_EMPTY};   					      \
	fi
	${PASS_IFF_SUCCESS} true

clean:
	rm -rf main main2
