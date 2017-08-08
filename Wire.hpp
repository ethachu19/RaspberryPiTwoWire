#ifndef __WIRE_H__
#define __WIRE_H__

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#include <queue>
#include <vector>

using namespace std;

typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef bool boolean;

class TwoWire  {
	public:
		void begin();
		void beginTransmission(int);
		int endTransmission();
		void write(char[], int);
		void write(char);
		int requestFrom(int, int);
		int available();
		char read();
		TwoWire();
	private:
		int pointers[128];
		int getFP(int);
		vector<char> sendBuff;
		int sendFP;
		queue<char> readBuff;
};

extern TwoWire Wire;

#endif
