CC = gcc
OBJS = sp_kdtree_unit_test.o SPKDTree.o KNearestSearch.o SPLogger.o SPPoint.o SPBPriorityQueue.o SPKDArray.o
EXEC = sp_kdtree_unit_test
TESTS_DIR = ./unit_tests
COMP_FLAG = -Wall -Wextra -std=c99 \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
sp_kdtree_unit_test.o: ./unit_tests/sp_kdtree_unit_test.c \
SPKDTree.h SPKDArray.h SPPoint.h SPConfig.h \
SPLogger.h SPBPriorityQueue.h ./unit_tests/unit_test_util.h KNearestSearch.h \
SPKDTree.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
KNearestSearch.o: KNearestSearch.c KNearestSearch.h SPPoint.h SPKDTree.h \
SPKDArray.h SPConfig.h SPLogger.h SPBPriorityQueue.h
	$(CC) $(COMP_FLAG) -c $*.c
SPKDTree.o: SPKDTree.c SPKDTree.h SPKDArray.h SPPoint.h SPConfig.h \
SPLogger.h SPBPriorityQueue.h
	$(CC) $(COMP_FLAG) -c $*.c
SPLogger.o:  SPLogger.c SPLogger.h
	$(CC) $(COMP_FLAG) -c $*.c
SPKDArray.o: SPKDArray.c SPKDArray.h SPPoint.h SPBPriorityQueue.h 
	$(CC) $(COMP_FLAG) -c $*.c
SPBPriorityQueue.o: SPBPriorityQueue.c SPBPriorityQueue.h
	$(CC) $(COMP_FLAG) -c $*.c
SPPoint.o: SPPoint.c SPPoint.h 
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
