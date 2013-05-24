INC_DIR = ..
SRC_DIR = .
OBJ_DIR = .

CFLAGS = -Wall -Wextra -Os -g -fomit-frame-pointer -pipe
DEFINES = 
INCLUDE = -I$(INC_DIR)

all : $(OBJ_DIR)/myhello

$(OBJ_DIR)/myhello.o : $(SRC_DIR)/myhello.c
	@echo  "Compiling $@ "; gcc $(CFLAGS) $(DEFINES) $(INCLUDE) -MD -MP -c -o "$@" "$<"

$(OBJ_DIR)/myhello : $(SRC_DIR)/myhello.o
	@echo  "Building $@ "; gcc -o "$@" "$<"

clean :
	@echo  "Removing objs "; 
	@rm -rf $(OBJ_DIR)/myhello
	@rm -rf $(OBJ_DIR)/*.o
	@rm -rf $(OBJ_DIR)/*.d
	@rm -rf $(OBJ_DIR)/*~