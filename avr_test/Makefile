#DEPS - avr-gcc avr-libc avrdude

CXX := avr-gcc
CXXPROCESSOR := -mmcu=atmega328p
CXXFLAGS := 

USB := /dev/ttyUSB0
BAUD_RATE := 115200

COPY := avr-objcopy

SOURCE := source
BIN := bin

INCLUDE_PATHS := -I/usr/avr/include/

SRC := $(wildcard $(SOURCE)/*.cpp)
OBJ := $(patsubst $(SOURCE)%.cpp, $(BIN)%.o, $(SRC))

NAME := main

all:
	@mkdir -p bin
	make main

$(NAME):$(OBJ)
	$(CXX) $(CXXPROCESSOR) $(OBJ) -o $(BIN)/$@ $(INCLUDE_PATHS)
	$(COPY) -j .text -j .data -O ihex ./$(BIN)/$(NAME) ./$(BIN)/$(NAME).hex

run:
	./bin/main

usb:
	ls -l /dev/ttyUSB* && ls -l /dev/ttyS*

flash:
	sudo avrdude -v -p atmega328p -c arduino -P $(USB) -b $(BAUD_RATE) -D -U flash:w:$(BIN)/$(NAME).hex:i

clean:
	rm -fr ./bin

$(BIN)/%.o: $(SOURCE)/%.cpp
	$(CXX) $(CXXPROCESSOR) $(INCLUDE_PATHS) -c $< -o $@