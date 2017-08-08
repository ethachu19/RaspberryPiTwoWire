#include "Wire.hpp"

TwoWire::TwoWire():
	sendFP(-1)
{
}

int TwoWire::getFP(int address)
{
	if (address > 0x7F)
		address = address >> 1;
	if (pointers[address] < 0) 
	{
		int fp = ::open("/dev/i2c-1",O_RDWR);
		if (fp < 0)
			return -4;
		if (ioctl(fp,I2C_SLAVE,address) < 0)
			return -2;
		pointers[address] = fp;
	}
	return pointers[address];
}

void TwoWire::begin() 
{
	for(int i = 0; i < 128; i++)
		pointers[i] = -1;
}

void TwoWire::beginTransmission(int address) 
{
	sendFP = getFP(address);
	sendBuff.clear();
}

void TwoWire::write(char data[], int length)
{
	for (int i = 0; i < length; i++)
		sendBuff.push_back(data[i]);
}

void TwoWire::write(char data)
{
	sendBuff.push_back(data);
}

int TwoWire::endTransmission()
{
	if (sendBuff.size() > 20)
		return 1;
	if (sendFP < 0)
		return sendFP*-1;
	char buff[20];
	for (unsigned int i = 0; i < sendBuff.size(); i++)
		buff[i] = sendBuff[i];
	if(::write(sendFP,buff,sendBuff.size()) < 0)
		return 3;
	sendBuff.clear();
	sendFP = -4;
	return 0;
}

int TwoWire::requestFrom(int address, int length)
{
	int fp = getFP(address);
	if (fp < 0)
		return 0;
	char* buff = new char[length];
	int i = ::read(fp,buff,length);
	for (int j = 0; j < i; j++)
		readBuff.push(buff[j]);
	delete[] buff;
	return i;
}

int TwoWire::available()
{
	return readBuff.size();
}

char TwoWire::read()
{
	if (available() <= 0)
		return 0xFF;
	char ret = readBuff.front();
	readBuff.pop();
	return ret;
}

void delayMicroseconds(int num)
{
	usleep(num);
}

TwoWire Wire = TwoWire();
