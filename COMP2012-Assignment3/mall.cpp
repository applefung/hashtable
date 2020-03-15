//submit this file

#include "mall.h"
Mall::Mall(string name)
{
	this->name=name;
}

//add a shop given the name
//if an existing shop has the same name as the given name, do nothing and return false
//otherwise, add the shop and return true
//all shops use LINEAR mode for collision resolving and have initial hash table sizes of 7
//and they should use the hash function shown in "How to Deal with String Keys: Method 3"
//(page 14 of the Hashing lecture slides)
//you can assume the given name has English letters only,
//and that we would treat lowercase letters and uppercase letters the same
//in the calculation, we use natural numbers (instead of ASCII codes) to represent the letters
//that means we will use 1, 2, 3, ..., 25, 26 to denote, respectively,
//'a' or 'A', 'b' or 'B', 'c' or 'C', ..., 'y' or 'Y', 'z' or 'Z'
//you may find a calculation example in a comment in main.cpp for you to verify your understanding
bool Mall::buildShop(string name)
{
	int (*fun3)(string) = [](string name)
	{
		int length=name.length();
		int temp[length];
		const char *x=name.c_str();
		for(int i=0; i<length; i++)
		{
			char y=tolower(x[i]);
			temp[i]=y-'a'+1;
		}
		int index[length-1];
		int product=0;
		for(int i=0; i<length; i++)
		{
			index[i]=37;
		}
		for(int i=length-2; i>0; i--)
		{
			for(int j=0; j<i; j++)
			{
				index[j]*=37;
			}
		}
		for(int i=0; i<length-1; i++)
		{
			product+=temp[i]*index[i];
		}

		product+=temp[length-1];
		int modulus=product%7;
		return modulus;
	};
	Shop a(name, 7, fun3, nullptr, Mode::LINEAR, 0.5);
	bool check=this->shops.add(name,a);
	if(check==false)
	{
		return false;
	}
	else
	{
		return true;
	}

}

bool Mall::demolishShop(string name)
{
	return this->shops.remove(name);
}

Shop* Mall::getShop(string name)
{
	return this->shops.get(name);
}

void Mall::mergeMall(Mall& another, string newName)
{
	this->name=newName;
	this->shops.merge(another.shops);
}
