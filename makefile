run: family
	./family i_ug1
family: family.o
	gcc -Wall -o family family.o
family.o: family.c
	gcc -Wall -c family.c
