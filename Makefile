CC      = gcc               
CFLAGS  = -g -Wall -Wextra  

SRC_DIR     = src
LEX_DIR     = $(SRC_DIR)/lex
SYN_DIR     = $(SRC_DIR)/syn
SEM_DIR     = $(SRC_DIR)/sem
COMPIL_DIR  = $(SRC_DIR)/compil
RUNTIME_DIR = $(SRC_DIR)/runtime
BUILD_DIR   = build

TESTFILES = $(SRC_DIR)/testfiles

# -----------------------------------------------------------------------------
# Lexical Analyzer
# -----------------------------------------------------------------------------
LEX_SRCS    = $(LEX_DIR)/analex.c
LEX_OBJS    = $(BUILD_DIR)/analex.o

TEST_ALEX_SRCS = $(LEX_DIR)/test_analex.c
TEST_ALEX_OBJS = $(BUILD_DIR)/test_analex.o
LEX_OUTPUT     = $(BUILD_DIR)/test_analex

# -----------------------------------------------------------------------------
# Syntax Analyzer
# -----------------------------------------------------------------------------
SYN_SRCS    = $(SYN_DIR)/anasynt.c
SYN_OBJS    = $(BUILD_DIR)/anasynt.o

TEST_ASYN_SRCS = $(SYN_DIR)/test_anasynt.c
TEST_ASYN_OBJS = $(BUILD_DIR)/test_anasynt.o
SYN_OUTPUT     = $(BUILD_DIR)/test_anasynt

# -----------------------------------------------------------------------------
# Semantic Analyzer
# -----------------------------------------------------------------------------
SEM_SRCS    = $(SEM_DIR)/anasem.c
SEM_OBJS    = $(BUILD_DIR)/anasem.o

TEST_SEM_SRCS = $(SEM_DIR)/test_anasem.c
TEST_SEM_OBJS = $(BUILD_DIR)/test_anasem.o
SEM_OUTPUT    = $(BUILD_DIR)/test_anasem

# -----------------------------------------------------------------------------
# Compiler Module
# -----------------------------------------------------------------------------
COMPIL_SRCS    = $(COMPIL_DIR)/compilateur.c
COMPIL_OBJS    = $(BUILD_DIR)/compilateur.o

TEST_COMPIL_SRCS = $(COMPIL_DIR)/test_compilateur.c
TEST_COMPIL_OBJS = $(BUILD_DIR)/test_compilateur.o
COMPIL_OUTPUT    = $(BUILD_DIR)/test_compilateur

# -----------------------------------------------------------------------------
# Runtime Module
# -----------------------------------------------------------------------------
RUNTIME_SRCS    = $(RUNTIME_DIR)/runtime.c
RUNTIME_OBJS    = $(BUILD_DIR)/runtime.o

TEST_RUN_SRCS = $(RUNTIME_DIR)/test_runtime.c
TEST_RUN_OBJS = $(BUILD_DIR)/test_runtime.o
RUNTIME_OUTPUT = $(BUILD_DIR)/test_runtime

.PHONY: all clean


all: $(LEX_OUTPUT) $(SYN_OUTPUT) $(SEM_OUTPUT) \
     $(COMPIL_OUTPUT) $(RUNTIME_OUTPUT)

# Linking Targets
# 1) Link test_analex
$(LEX_OUTPUT): $(LEX_OBJS) $(TEST_ALEX_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 2) Link test_anasynt
$(SYN_OUTPUT): $(SYN_OBJS) $(TEST_ASYN_OBJS) $(LEX_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 3) Link test_anasem
$(SEM_OUTPUT): $(SEM_OBJS) $(TEST_SEM_OBJS) $(SYN_OBJS) $(LEX_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 4) Link test_compilateur
$(COMPIL_OUTPUT): $(COMPIL_OBJS) $(TEST_COMPIL_OBJS) \
                  $(LEX_OBJS) $(SYN_OBJS) $(SEM_OBJS) $(RUNTIME_OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# 5) Link test_runtime
$(RUNTIME_OUTPUT): $(RUNTIME_OBJS) $(TEST_RUN_OBJS) \
                   $(COMPIL_OBJS) $(LEX_OBJS) $(SYN_OBJS) $(SEM_OBJS)
	$(CC) $(CFLAGS) -o $@ $^


$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(LEX_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test_analex.o: $(LEX_DIR)/test_analex.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


$(BUILD_DIR)/%.o: $(SYN_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test_anasynt.o: $(SYN_DIR)/test_anasynt.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


$(BUILD_DIR)/%.o: $(SEM_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test_anasem.o: $(SEM_DIR)/test_anasem.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


$(BUILD_DIR)/%.o: $(COMPIL_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test_compilateur.o: $(COMPIL_DIR)/test_compilateur.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


$(BUILD_DIR)/%.o: $(RUNTIME_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test_runtime.o: $(RUNTIME_DIR)/test_runtime.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -rf $(BUILD_DIR)
