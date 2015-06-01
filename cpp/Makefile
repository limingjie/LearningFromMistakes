# Check OS
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

ifneq (,$(findstring MINGW,$(uname_S)))
	X=.exe
endif

ifeq ($(OS),Windows_NT)
	X=.exe
endif

# Check CXX flag.
ifeq ($(CXX),clang++)
	CXX=clang++
else
	CXX=g++
endif

# Some earlier version of compilers know c++0x but not c++11
CXXFLAGS = -Wall -g -O2 -std=c++0x

all: UnexpectedString$(X) \
	 ReverseIterationUnsigned$(X)

UnexpectedString$(X): UnexpectedString.cpp
	@echo "==>Compiling/Linking UnexpectedString.cpp..."
	$(CXX) $(CXXFLAGS) -o UnexpectedString$(X) UnexpectedString.cpp

ReverseIterationUnsigned$(X): ReverseIterationUnsigned.cpp
	@echo "==>Compiling/Linking ReverseIterationUnsigned.cpp..."
	$(CXX) $(CXXFLAGS) -o ReverseIterationUnsigned$(X) ReverseIterationUnsigned.cpp

clean:
	@echo "==>Cleaning..."
	rm -rf UnexpectedString$(X)
	rm -rf ReverseIterationUnsigned$(X)
