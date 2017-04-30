CC = gcc
OBJS = sp_logger_unit_test.o SPLogger.o
EXEC = sp_logger_unit_test
TESTS_DIR = ./unit_tests
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
sp_logger_unit_test.o: unit_tests/sp_logger_unit_test.c \
 unit_tests/unit_test_util.h unit_tests/../SPLogger.h

	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
SPLogger.o: SPLogger.c SPLogger.h 
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
