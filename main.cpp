//============================================================================
// Name        : SPCBIR.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

using namespace std;

int main() {
	char firstArg[] = "34 56";
    int x = atoi(firstArg);
	sprintf(firstArg, "%d", x);
	cout << firstArg << endl;
	return 0;
}
