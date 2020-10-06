CXXFLAGS +=  -DASIO_STANDALONE -Wall -O0 -g --std=c++11
CPPFLAGS+=-I./include -I./asio-1.12.2/include
LDLIBS+= -lpthread
GTKFLAGS = `/usr/bin/pkg-config gtkmm-3.0 --cflags --libs x11`

all: asio-1.12.2 mainwindow server poker_test

asio-1.12.2:
	tar xzf asio-1.12.2.tar.gz


server: dealer.o server.o poker_game.o poker_player.o poker_table.o
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS) dealer.o server.o poker_game.o poker_table.o poker_player.o $(GTKFLAGS) -o dealer




mainwindow:  mainwin.o chat_client.o main.o
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS) main.o mainwin.o chat_client.o $(GTKFLAGS) -o pokerplus

main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS) -c main.cpp $(GTKFLAGS) -o main.o
mainwin.o: mainwin.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS) -c mainwin.cpp $(GTKFLAGS) -o mainwin.o
chat_client.o: chat_client.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS) -c chat_client.cpp $(GTKFLAGS) -o chat_client.o




dealer.o: dealer.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS) -c dealer.cpp $(GTKFLAGS) -o dealer.o

poker_table.o: poker_table.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS) -c poker_table.cpp $(GTKFLAGS) -o poker_table.o

poker_game.o: poker_game.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS) -c poker_game.cpp $(GTKFLAGS) -o poker_game.o

poker_player.o: poker_player.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS) -c poker_player.cpp $(GTKFLAGS) -o poker_player.o




server.o: server.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDLIBS) -c server.cpp $(GTKFLAGS) -o server.o



poker_test: poker_test.cpp dealer.cpp
	${CXX} -o $@ $^ -Wall -std=c++11
test: 
	./poker_test
	
clean:
	rm -f *.o *.gch mainwindow dealer pokerplus poker_test
	rm -rf asio-1.12.2