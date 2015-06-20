DBProject: utils.o find.o insert.o check_catalog.o shell.o
	cc -o DBProject utils.o find.o insert.o check_catalog.o shell.o -lstdc++ -g

utils.o: utils.cpp utils.h
	cc -c utils.cpp -lstdc++ -g

find.o: find.cpp find.h utils.h
	cc -c find.cpp -lstdc++ -g

insert.o: insert.cpp insert.h utils.h
	cc -c insert.cpp -lstdc++ -g

check_catalog.o: check_catalog.cpp check_catalog.h utils.h
	cc -c check_catalog.cpp -lstdc++ -g

shell.o: shell.cpp find.h insert.h check_catalog.h utils.h
	cc -c shell.cpp -lstdc++ -g -Wno-deprecated

clean:
	rm *.o
