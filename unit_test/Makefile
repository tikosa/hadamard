RM=rm -f

GXX=g++
INCLUDE=-I.. -I${GTEST_DIR} -isystem${GTEST_DIR}/include
CFLAGS=-pthread --std=c++11 
LIBDIRS=-L${GTEST_DIR}
LIBS=-lgtest -lpthread

DEPS=hadamard.h
TARGET=ut
ODIR=obj

SRC=unit_test_hadamard.cpp

OBJ=$(SRC:.cpp=.o)
# OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# ${ODIR}/%.o: %.cpp ${DEPS} 
# 	$(GXX) -c -o $@ $< $(INCLUDE) $(CFLAGS) 

.cpp.o:
	$(GXX) -c -o $@ $(INCLUDE) $(CFLAGS) $^

$(TARGET): $(OBJ)
	${GXX} $(LIBDIRS) -o $@ $^ ${LIBS}

all: $(SRC) $(TARGET)

# .PHONY clean

clean:
	${RM} ${OBJ} ${TARGET}
