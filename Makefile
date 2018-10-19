CC := g++-4.8
FLAGS := -std=c++11 -w

bin/Agenda:: build/Agenda.o build/AgendaUI.o build/Path.o build/AgendaService.o build/User.o build/Date.o build/Meeting.o build/Storage.o
	@mkdir -p bin
	$(CC) $(FLAGS) -I./include build/Agenda.o build/AgendaUI.o build/AgendaService.o build/User.o build/Date.o build/Meeting.o build/Storage.o -o $@

build/Agenda.o: src/Agenda.cpp
	@mkdir -p build
	$(CC) $(FLAGS) -I./include -c -o $@ src/Agenda.cpp

build/AgendaUI.o: src/AgendaUI.cpp
	mkdir -p build
	$(CC) $(FLAGS) -I./include -c -o $@ src/AgendaUI.cpp

build/AgendaService.o: src/AgendaService.cpp
	mkdir -p build
	$(CC) $(FLAGS) -I./include -c -o $@ src/AgendaService.cpp

build/Storage.o: src/Storage.cpp
	mkdir -p build
	$(CC) $(FLAGS) -I./include -c -o $@ src/Storage.cpp

build/Meeting.o: src/Meeting.cpp
	mkdir -p build
	$(CC) $(FLAGS) -I./include -c -o $@ src/Meeting.cpp

build/Date.o: src/Date.cpp
	mkdir -p build
	$(CC) $(FLAGS) -I./include -c -o $@ src/Date.cpp

build/User.o: src/User.cpp
	mkdir -p build
	$(CC) $(FLAGS) -I./include -c -o $@ src/User.cpp

build/Path.o: include/Path.hpp
	mkdir -p build
	$(CC) $(FLAGS) -I./include -c -o $@ include/Path.hpp

clean:
	@rm -rf build
	@rm -rf bin