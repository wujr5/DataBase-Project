DBProject: json_binary_conversion.o find.o insert.o check_catalog.o shell.o
	cc -o DBProject json_binary_conversion.o find.o insert.o check_catalog.o shell.o -lstdc++ -g

json_binary_conversion.o: json_binary_conversion.cpp json_binary_conversion.h
	cc -c json_binary_conversion.cpp -lstdc++ -g

find.o: find.cpp find.h
	cc -c find.cpp -lstdc++ -g

insert.o: insert.cpp insert.h json_binary_conversion.h
	cc -c insert.cpp -lstdc++ -g

check_catalog.o: check_catalog.cpp check_catalog.h
	cc -c check_catalog.cpp -lstdc++ -g

shell.o: shell.cpp find.h insert.h check_catalog.h
	cc -c shell.cpp -lstdc++ -g

clean:
	rm *.o