CC = gcc
OBJS = sp_kd_array_unit_test.o SPKDArray.o SPPoint.o SPLogger.o
EXEC = sp_kd_array_unit_test
TESTS_DIR = ./unit_tests
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
sp_kd_array_unit_test.o: ./unit_tests/sp_kd_array_unit_test.c \
 SPKDArray.h SPPoint.h \
 SPPoint.c SPLogger.h ./unit_tests/unit_test_util.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
SPKDArray.o: SPKDArray.c SPKDArray.h SPPoint.h SPBPriorityQueue.h 
	$(CC) $(COMP_FLAG) -c $*.c
SPPoint.o: SPPoint.c SPPoint.h 
	$(CC) $(COMP_FLAG) -c $*.c
SPLogger.o: SPLogger.c SPLogger.h 
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
