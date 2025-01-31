.SUFFIXES: .c .o
CC      = gcc
CFLAGS  = -g -fPIC -Wall
INC     = -I${GALYK_HOME}/include \
          -I$(GOLDILOCKS_HOME)/include
LFLAGS  = -L$(GOLDILOCKS_HOME)/lib
LIB     = -lgoldilocks -lpthread -ldl -lm -lrt

SRC_DIR = ${GALYK_HOME}/src
BUILD_DIR = ${GALYK_HOME}/build
BIN_DIR = ${GALYK_HOME}/bin

TARGET  = $(BIN_DIR)/galyk
C_SRC   = $(wildcard $(SRC_DIR)/*.c)
OBJECT  = $(addprefix $(BUILD_DIR)/, $(notdir $(C_SRC:.c=.o)))

all : $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $(INC) -o $@ $<

$(TARGET) : $(OBJECT)
	$(CC) -o $@ $^ $(LFLAGS) $(LIB)

clean :
	rm -rf $(BUILD_DIR)/*.o $(TARGET)
