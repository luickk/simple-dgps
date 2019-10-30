CC       = g++
CFLAGS   = -std=c++11 -DRASPBERRY_PI -DBCM2835_NO_DELAY_COMPATIBILITY -D__BASEFILE__=\"$*\"
LIBS     = -lwiringPi -lbcm2835 -pthread -lboost_system
INCLUDE  = -Ilibs/
BUILD_DIR  = build/

all: basestation

#$(BUILD_DIR)test.o: test.cpp
#				$(CC) -o $(BUILD_DIR)/test.o $(CFLAGS) -c test.cpp $(INCLUDE)

basestation: basestation.cpp
				$(CC) $< $(LIBS) $(include) $(CFLAGS) -o basestation

clean:
				rm -rf build/*.o basestation
				rm -rf *.o basestation
