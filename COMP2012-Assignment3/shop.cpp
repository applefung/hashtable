//submit this file

#include "shop.h"
Shop::Shop(string name, int m, int (*h)(string), int (*h2)(string), Mode mode, double loadLimit)
{
	inventory=new HashTable<string, Product>(m,h,h2,mode,loadLimit);
	this->name=name;
	this->profit=0;
}

Shop::Shop(const Shop& another)
{
	this->name=another.name;
	this->profit=another.profit;
	inventory=new HashTable<string, Product>(*another.inventory);
	//*this=another;

}

Shop::Shop(Shop&& another): name(another.name), profit(another.profit), inventory(another.inventory)
{
    another.name = "moved";
    another.inventory = nullptr;
}
Shop::~Shop()
{
	delete inventory;
}

Shop& Shop::operator=(const Shop& another)
{
	this->name=another.name;
	this->profit=another.profit;
	//delete this->inventory;
	//inventory=another.inventory;
	//inventory=new HashTable<string, Product>(*another.inventory);
	(*inventory)=(*another.inventory);
	return *this;
}

bool Shop::addProduct(string name, int quantity, int price)
{
	if(this->inventory->get(name) != nullptr)
	{
		Product* x=this->inventory->get(name);
		if(name==x->getName())
		{
			x->add(quantity);
			return true;
		}
	}
	else
	{
		Product* temp=new Product(name,quantity,price);
		int a=this->inventory->add(name,temp);
		quantity+=1;
		if(a!=-1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

bool Shop::sellProduct(string name, int quantity)
{
	Product* x=this->inventory->get(name);
	if(x != nullptr)
	{
		if(name != x->getName())
		{
			return false;
		}
		if(quantity>x->getQuantity())
		{
			return false;
		}
		else if(quantity<x->getQuantity())
		{
			x->take(quantity);
			int y=x->getPrice();
			this->profit+=y*quantity;
			return true;
		}
		else if(quantity==x->getQuantity())
		{
			x->take(quantity);
			int y=x->getPrice();
			this->profit+=y*quantity;
			this->inventory->remove(name);
			if(this->inventory->remove(name))
			{
				return true;
			}
		}
	}
	return false;
}
