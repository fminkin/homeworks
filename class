#include <vector>
#include<iostream>
#include<ctime>
#include<stack>
#include<algorithm>
#include <fstream>
#define N 100000
using namespace std;

// public, protected, private
// public - всё, что прописано с паблик, доступно из всех мест программы
// private - есть доступ только изнутри класса.

// инкапсуляция, наследование, полиморфизм
/*
class Point{
 // если убрать private: если поменять класс на структуру, то x, y, Num... будет public
	int x, y;
    mutable	int NumberUseLen2; // константные методы могут ее изменять
public:
	Point(int x, int y) : x(x), y(y) {};
	Point() {};
	int len2() const{
		//NumberUseLen2++; Ломается const, без mutable 
		NumberUseLen2++;
		return x*x + y*y;
	}
	//Point operator+ (Point p1)
	//{
	//	return Point(x + p1.x, y + p1.y);
	//}
	int operator^(Point p1)
	{
		return p1.x *x + p1.y*y;
	}
	int getx(){
		return x;
	}
	int gety(){
		return y;
	}

};

Point operator+ (Point p1, Point p2)
{
	return Point(p1.getx() + p2.getx(), p1.gety() + p2.gety());
}*/

struct Point{
	int x, y;
public: Point(int x, int y) :
	x(x), y(y)
	{}	

};
struct Point3D:public Point{
	int z;
	Point3D(int x, int y, int z) :
		Point(x, y), z(z)
	{}
};
class Meat{
	static int NumUseMeat; // сколько экземпляров класса Meat создано
public:
	Meat(){
		++NumUseMeat;
	}
	int getmeat(){
		return NumUseMeat;
	}
};
int Meat::NumUseMeat = 0;
//Point operator^(Point)



/*
class Animal{
public:
	void say() const{ // const для гарантии, что внутренние переменные класса изменены не будут
		cout << "I`m an animal!!!\n";
	}
};

class Dog:public Animal{ // обладает теми же свойствами и переменными, что и animal 
public:
	int legsnumber(){
		return 4;
	}
};
void getsay(Animal a){
a.say();
}
*/

struct Animal{
	virtual void say()const {
		cout << "I`m an Animal!!!\n";
	}
};
struct Cat: public Animal{
	void say() const{
		cout << "Meow\n";
	}
};

struct NyanCat : public Cat{
	void say() const{ //перегрузка virtual наследуется
		cout << "Nya-Nya-Nya\n";
	}
};
//void getsay(Animal *a){
	//a->say();
//}
//void getSay(Animal *a)
//{
	//Cat *cat = dynamic_cast<Cat*> (a);  // 0, если там не кошка. 
	// надо написать виртуальный деструктор

	//if (cat)
	//	cat->say();
	//else
	//	a->say();
//}
void getsay(vector<Animal*> c){
	for (int i = 0; i < c.size(); i++)
		c[i]->say();
}
int main()
{

	Animal a;
	Cat cat;
	vector<Animal*> v;
	NyanCat nCat;
	v.push_back(&cat);
	v.push_back(&cat);
	v.push_back(&nCat);
	getsay(v);
	return 0;
}
// наследуемый тип



// Тип наследования			->			    private						protected					public
//  | переменная в базовом классе
// \/
// private                           Не передается совсем         Не передается совсем       Не передается совсем
//																																																
//protected						    		PRIVATE					   PROTECTED                    PROTECTED																
//																																																		
//public									PRIVATE					   PROTECTED					PUBLIC	
