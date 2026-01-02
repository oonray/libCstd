#
# LibCStd
#
name=libCstd

#SOURCES
SRC_D=./src
DATA_D=$(SRC_D)/data
STATIC_D=$(BUILD_D)/lib

#BUILD
BUILD_D=./build
OBJ_BD=$(BUILD_D)/obj
TEST_BD=$(BUILD_D)/tests

LIB_BD=$(BUILD_D)/lib
DATA_OBJ_D=$(OBJ_BD)/data

CMD_C=./compile_commands.json
CC=bear --append --output $(CMD_C) -- clang

VPATH = $(wildcard src/*):$(wildcard build/*):$(wildcard lib/*):tests
FLAGS = $(CFLAGS) $(patsubst %,-I%,$(subst :, ,$(VPATH))) -Wall --std=c23

.PHONY: all
all: dirs libs data

clean:
	rm -rf $(BUILD_D)

dirs:
	mkdir -p $(BUILD_D)
	mkdir -p $(LIB_BD)
	mkdir -p $(DATA_OBJ_D)
	mkdir -p $(TEST_BD)

dev:
	bear -- append --output $(CMD_C) -- make

#RULES
%.o: %.c %.h
	$(CC) $(FLAGS) -fPIC -c -o $(OBJ_BD)/$@ $<

lib%.a: %.o
	ar rcs $(LIB_BD)/$@ $(OBJ_BD)/$<

%_tests: libca_%.a libmunit.a
	$(CC) $(FLAGS) -o $(TEST_BD)/$@ tests/$@.c -L $(LIB_BD) -l munit -l ca_$(subst _tests,,$@)

#LIBS
.PHONY: bstrlib
bstrlib: libbstrlib.a

.PHONY: munit
munit: libmunit.a

.PHONY: mongoose
mongoose: libmongoose.a

.PHONY: libs
libs: bstrlib mongoose munit

.PHONY: vector
vector: libca_vector.a libmunit.a vector_tests

.PHONY: data
data: vector $(LIB_BD)/libca_data.a
$(LIB_BD)/libca_data.a: $(patsubst %c,%o,$(notdir $(wildcard src/data/*.c)))
