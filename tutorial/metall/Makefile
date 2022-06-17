#BOOST_ROOT:=/home/aislam6/metall/tutorial/boost_1_75_0
#export BOOST_ROOT

all:
	g++ -std=c++17 -o vector vector_metall.cpp -lstdc++fs -pthread -I../../include -I${BOOST_ROOT}
	g++ -std=c++17 -o list list_metall.cpp -lstdc++fs -pthread -I../../include -I${BOOST_ROOT}
	g++ -std=c++17 -o deque deque_metall.cpp -lstdc++fs -pthread -I../../include -I${BOOST_ROOT}
	g++ -std=c++17 -o set set_metall.cpp -lstdc++fs -pthread -I../../include -I${BOOST_ROOT}
	g++ -std=c++17 -o unordered_set unordered_set_metall.cpp -lstdc++fs -pthread -I../../include -I${BOOST_ROOT}
	g++ -std=c++17 -o map map_metall.cpp -lstdc++fs -pthread -I../../include -I${BOOST_ROOT}
	g++ -std=c++17 -o unordered_map unordered_map_metall.cpp -lstdc++fs -pthread -I../../include -I${BOOST_ROOT}

.PHONY: clean
clean:
	rm -f vector list deque set unordered_set map unordered_map
