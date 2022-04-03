CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -D _DEFAULT_SOURCE

C_FILE = ./src/c_file/

MAIN_SRC = ./src/main.c
MAIN_OBJS = main.o

SRC_EXE = minimake
SRC = $(C_FILE)minimake.c  $(C_FILE)Variable.c $(C_FILE)Rule.c
SRC += $(C_FILE)parse.c $(C_FILE)check.c $(C_FILE)StrList.c $(C_FILE)tool.c
SRC_OBJS = minimake.o Variable.o Rule.o
SRC_OBJS += parse.o check.o StrList.o tool.o

TEST_EXE = tests.out
TEST = ./tests/minimake_tst.c
TEST_OBJS = minimake_tst.o 

all: minimake


.PHONY: minimake
minimake: $(SRC_OBJS) $(MAIN_OBJS)
	@#echo "minimake"
	@$(CC) $(SRC_OBJS) $(MAIN_OBJS) -o $(SRC_EXE)

$(MAIN_OBJS): $(MAIN_SRC)
	@#echo "main_objs"
	@$(CC) -c $(CFLAGS) $(MAIN_SRC)

$(SRC_OBJS): $(SRC)
	@#echo "src_objs"
	@$(CC) -c $(CFLAGS) $(SRC)

#Create *_test.o
$(TEST_OBJS): $(TEST)
	@#echo "test_objs"
	@$(CC) -c $(CFLAGS) $(TEST)

.PHONY: check
check: $(SRC_OBJS) $(TEST_OBJS)
	@#echo "check"
	@$(CC) $(TEST_OBJS) $(SRC_OBJS) -lcriterion -o $(TEST_EXE)
	./$(TEST_EXE)


.PHONY: clean
clean:
	@#echo "clean"
	${RM} $(TEST_EXE) $(TEST_OBJS) $(MAIN_OBJS)
	${RM} $(SRC_EXE) $(SRC_OBJS)
