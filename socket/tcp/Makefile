
.PHONY: all clean

all: tcp_server tcp_client

tcp_server:tcp_server.cpp
	g++ -o $@ $^

tcp_client:tcp_client.cpp
	g++ -o $@ $^

clean:
	rm tcp_client tcp_server

rebuild:clean
