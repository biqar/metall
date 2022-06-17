#BOOST_ROOT:=/home/aislam6/metall/tutorial/boost_1_75_0
#export BOOST_ROOT

all:
	g++ -std=c++17 -o vector vector_metall.cpp -I../../include -I${BOOST_ROOT} -I${PRIVATEER_ROOT}/build/include -L${PRIVATEER_ROOT}/build/lib -lprivateer -fopenmp -lstdc++ -lstdc++fs -lcrypto -lrt -pthread -DMETALL_USE_PRIVATEER
	#g++ -std=c++17 -o vector vector_metall.cpp -I../../include -I${BOOST_ROOT} -I${PRIVATEER_ROOT}/build/include -L${PRIVATEER_ROOT}/build/lib -lprivateer -fopenmp -lstdc++ -lstdc++fs -lcrypto -lrt -pthread
	g++ -std=c++17 -o list list_metall.cpp -I../../include -I${BOOST_ROOT} -I${PRIVATEER_ROOT}/build/include -L${PRIVATEER_ROOT}/build/lib -lprivateer -fopenmp -lstdc++ -lstdc++fs -lcrypto -lrt -pthread -DMETALL_USE_PRIVATEER
	g++ -std=c++17 -o deque deque_metall.cpp -I../../include -I${BOOST_ROOT} -I${PRIVATEER_ROOT}/build/include -L${PRIVATEER_ROOT}/build/lib -lprivateer -fopenmp -lstdc++ -lstdc++fs -lcrypto -lrt -pthread -DMETALL_USE_PRIVATEER
	g++ -std=c++17 -o set set_metall.cpp -I../../include -I${BOOST_ROOT} -I${PRIVATEER_ROOT}/build/include -L${PRIVATEER_ROOT}/build/lib -lprivateer -fopenmp -lstdc++ -lstdc++fs -lcrypto -lrt -pthread -DMETALL_USE_PRIVATEER
	g++ -std=c++17 -o unordered_set unordered_set_metall.cpp -I../../include -I${BOOST_ROOT} -I${PRIVATEER_ROOT}/build/include -L${PRIVATEER_ROOT}/build/lib -lprivateer -fopenmp -lstdc++ -lstdc++fs -lcrypto -lrt -pthread -DMETALL_USE_PRIVATEER
	g++ -std=c++17 -o map map_metall.cpp -I../../include -I${BOOST_ROOT} -I${PRIVATEER_ROOT}/build/include -L${PRIVATEER_ROOT}/build/lib -lprivateer -fopenmp -lstdc++ -lstdc++fs -lcrypto -lrt -pthread -DMETALL_USE_PRIVATEER
	g++ -std=c++17 -o unordered_map unordered_map_metall.cpp -I../../include -I${BOOST_ROOT} -I${PRIVATEER_ROOT}/build/include -L${PRIVATEER_ROOT}/build/lib -lprivateer -fopenmp -lstdc++ -lstdc++fs -lcrypto -lrt -pthread -DMETALL_USE_PRIVATEER

.PHONY: clean
clean:
	rm -f vector list deque set unordered_set map unordered_map
