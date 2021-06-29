#include "Helper.h"
#include "Dynamic.h"
#include <iostream>

int main()
{
	std::cout << nc::squ(5) << std::endl; 

	nc::point p1{ 10, 10 }; 
	nc::point p2{ 10, 10 }; 
	nc::point p3 = p1 + p2; 
	std::cout << p3.x << " " << p3.y << std::endl; 

	system("pause"); 
}
