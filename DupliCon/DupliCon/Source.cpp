#include<iostream>
#include<string>
#include<cstdlib>
#include<functional>
#include<algorithm>
#include"DupliCon.h"

using namespace std;

template<typename T>
class Comp{
public:
	bool operator()(T a, T b){
		return a > b;
	}
};

bool test(const string *str){
	return true;
}

int main(){

	const string strs[] = { "alpha", "beta", "gamma", "delta", "epsilon", "zeta" };
	DupliCon<string> owen(strs, strs + 6);

	owen.print();



	system("pause");
	return 0;
}