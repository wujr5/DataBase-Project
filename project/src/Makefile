DBProject: shell.o
	cc -o DBProject shell.o -lstdc++ -g

shell.o: shell.cpp find.h insert.h check_catalog.h
	cc -c shell.cpp -lstdc++ -g

find.o: find.cpp json_binary_conversion.h
	cc -c find.cpp -lstdc++ -g

insert.o: insert.cpp json_binary_conversion.h
	cc -c insert.cpp -lstdc++ -g

check_catalog.o: check_catalog.cpp
	cc -c check_catalog.cpp -lstdc++ -g

json_binary_conversion.o: json_binary_conversion.cpp
	cc -c json_binary_conversion.cpp -lstdc++ -g

clean:
	rm *.o