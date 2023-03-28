/*
 * main.cpp
 *
 *  Created on: 28 Mar 2023
 *      Author: jspin
 */

#include <iostream>

int main()
{
	std::cout << "Hello there" << std::endl;
	system("python --version");
	system("python src/brs_can_flasher.py");
	return 0;
}
