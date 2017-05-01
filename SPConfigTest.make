CC = gcc
OBJS = sp_config_unit_test.o SPConfig.o SPLogger.o
EXEC = sp_config_unit_test.o
TESTS_DIR = ./unit_tests
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
sp_config_unit_test.o: ./unit_tests/sp_config_unit_test.c \
 SPConfig.h SPLogger.h \
 ./unit_tests/unit_test_util.h
	$(CC) $(COMP_FLAG) -c $(TESTS_DIR)/$*.c
SPConfig.o: SPConfig.c SPConfig.h SPLogger.h
	$(CC) $(COMP_FLAG) -c $*.c
SPLogger.o: SPLogger.c SPLogger.h 
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
