disassembler: disassembler.o
	g++ disassembler.o -o disassembler 
disassembler.o: disassembler.cpp
	g++ -O3 disassembler.cpp -std=c++11  

#tests: test0.out test1.out test2.out test3.out test4.out test5.out test6.out test7.out test8.out test9.out   

tests:
	./disassembler *.mips > *.out

   
test0.out:
	./disassembler test0.mips > test0.out
test1.out:
	./disassembler test1.mips > test1.out
test2.out:
	./disassembler test2.mips > test2.out
test3.out:
	./disassembler test3.mips > test3.out
test4.out:
	./disassembler test4.mips > test4.out
test5.out:
	./disassembler test5.mips > test5.out
test6.out:
	./disassembler test6.mips > test6.out
test7.out:
	./disassembler test7.mips > test7.out
test8.out:
	./disassembler test8.mips > test8.out
test9.out:
	./disassembler test9.mips > test9.out 
clean:
	@rm -rf disassembler.o
cleanall:
	@rm -rf disassembler.o disassembler