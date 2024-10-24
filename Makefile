
CPPFLAGS=-std=c++20 -Wall -Wextra -pedantic -g

test: test.cpp vector.hpp matrix.hpp matrix_operations.hpp vector_operations.hpp
	g++ $(CPPFLAGS) -o test test.cpp -lblas -llapack

clean:	
	rm -f test *.o *.mod