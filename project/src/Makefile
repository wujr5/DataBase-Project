DBProject: find.o insert.o check_catalog.o shell.o
	cc -o DBProject find.o insert.o check_catalog.o shell.o -lstdc++ -g

find.o: find.cpp find.h
	cc -c find.cpp -lstdc++ -g -Wno-deprecated

insert.o: insert.cpp insert.h
	cc -c insert.cpp -lstdc++ -g -Wno-deprecated

check_catalog.o: check_catalog.cpp check_catalog.h
	cc -c check_catalog.cpp -lstdc++ -g

shell.o: shell.cpp find.h insert.h check_catalog.h
	cc -c shell.cpp -lstdc++ -g -Wno-deprecated

clean:
	rm *.o
