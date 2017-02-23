#ifndef MURO_H
#define MURO_H
#include<iostream>
using namespace std;

class Muro
{
	public:
		Muro(int x, int y): muro_x(x), muro_y(y) {}
		Muro(const Muro& m): muro_x(m.muro_x), muro_y(m.muro_y) {}
		Muro& operator=(const Muro& m) {
			if (this != &m)
			{
				muro_x = m.muro_x;
				muro_y = m.muro_y;
			}

			return *this;
		}
		~Muro() { }
		int getX() const {return muro_x;}
		int getY() const {return muro_y;}		

	private:
		int muro_x;
		int muro_y;
};

#endif
