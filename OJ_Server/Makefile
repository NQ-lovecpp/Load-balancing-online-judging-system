oj_server:OJ_Server.cc
	g++ -o $@ $^ -I./include -L./lib -std=c++14 -lpthread -lctemplate -ljsoncpp -lmysqlclient
.PHONY:clean
clean:
	rm -f oj_server