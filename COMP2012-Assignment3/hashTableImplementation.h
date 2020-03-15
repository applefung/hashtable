//submit this file

//you do NOT need to include any header in this file
//just write your HashTable implementation here right away
#include <iostream>
using namespace std;

template<typename K, typename T>
HashTable<K,T>::HashTable(int m, int (*h)(K), int (*h2)(K), Mode mode, double loadLimit)
{
	this->m=m;
	this->mode=mode;
	this->loadLimit=loadLimit;
	this->count=1;
	this->h=h;
	this->h2=h2;
	this->table=new Cell[m];
	for(int i=0; i<m; i++)
	{
		table[i].status=CellStatus::EMPTY;
		//table[i].data=nullptr;
	}
}

template<typename K,typename T>
HashTable<K,T>::HashTable(const HashTable& another)//:table(nullptr)
{
	//*this=another;
	this->m=another.m;
	this->mode=another.mode;
	this->h=another.h;
	this->h2=another.h2;
	this->loadLimit=another.loadLimit;
	this->count=another.count;

	this->table=new Cell[this->m];

	for(int i=0; i<this->m; i++)
	{
		this->table[i].status=CellStatus::EMPTY;

	}
	//deep copy

	for(int i=0; i<this->m; i++)
	{
		if(another.table[i].status==CellStatus::ACTIVE || another.table[i].status==CellStatus::DELETED)
		{
			this->table[i].key=another.table[i].key;
			this->table[i].status=another.table[i].status;
			this->table[i].data=new T(*another.table[i].data);
		}
	}

}

template<typename K,typename T>
HashTable<K,T>::~HashTable()
{
	for(int i=0; i<this->m; i++)
	{
		if(this->table[i].status==CellStatus::ACTIVE || this->table[i].status==CellStatus::DELETED)
		{
			delete this->table[i].data;
		}
	}
	delete []this->table;
}

template<typename K,typename T>
HashTable<K,T>& HashTable<K,T>::operator=(const HashTable& another)
{
	for(int i=0; i<this->m; i++)
	{
		if(this->table[i].status==CellStatus::ACTIVE || this->table[i].status==CellStatus::DELETED)
		{
			delete this->table[i].data;
		}
	}
	delete []this->table;

	this->m=another.m;
	this->mode=another.mode;
	this->h=another.h;
	this->h2=another.h2;
	this->loadLimit=another.loadLimit;
	this->count=another.count;

	//initialize a new table
	this->table=new Cell[this->m];

	for(int i=0; i<this->m; i++)
	{
		this->table[i].status=CellStatus::EMPTY;

	}
	//deep copy
	for(int i=0; i<this->m; i++)
	{
		if(another.table[i].status==CellStatus::ACTIVE || another.table[i].status==CellStatus::DELETED)
		{
			this->table[i].key=another.table[i].key;
			this->table[i].status=another.table[i].status;
			this->table[i].data=new T(*another.table[i].data);
			//(*this->table[i].data)=(*another.table[i].data);
		}
	}

	return *this;
}

template<typename K,typename T>
int HashTable<K,T>::add(K key, T* data)
{
	int collision=0;
	if(this->mode==Mode::LINEAR)
	{

		if(count>=this->m*loadLimit)
		{

			//reset the value of count
			count=0;

			//temporary cell save the content of table
			this->m *=2;
			Cell *temp=new Cell[this->m/2];
			for(int i=0; i<this->m/2; i++)
			{
				temp[i].status=CellStatus::EMPTY;
			//	temp[i].data=nullptr;

			}

			for(int i=0; i<this->m/2; i++)
			{
				if(table[i].status == CellStatus::ACTIVE )
				{
					temp[i].key=table[i].key;
					temp[i].data=new T(*table[i].data);
					temp[i].status=CellStatus::ACTIVE;
				}

			}

			for(int i=0; i<this->m/2; i++)
			{
				if(this->table[i].status==CellStatus::ACTIVE || this->table[i].status==CellStatus::DELETED)
				{

					delete this->table[i].data;

				}
			}
			delete []this->table;

			table=new Cell[this->m];
			for(int i=0; i<this->m; i++)
			{
				table[i].status=CellStatus::EMPTY;
			}
			//assign value to table cell

			for(int i=0; i<this->m/2; i++)
			{
				int hash_index=0;
				if(temp[i].status==CellStatus::ACTIVE)
				{
					int hashing=(h(temp[i].key)+hash_index)%this->m;
					if(table[hashing].status==CellStatus::EMPTY)
					{
						count+=1;
						table[hashing].key=temp[i].key;
						table[hashing].data=new T(*temp[i].data);
						table[hashing].status=CellStatus::ACTIVE;

					}
					else if(table[hashing].status==CellStatus::ACTIVE)
					{
						while(table[hashing].status!=CellStatus::EMPTY)
						{
							hash_index+=1;
							hashing=(h(temp[i].key)+hash_index)%this->m;
							while(hashing>this->m)
							{
								hashing-=this->m;
							}
						}
						count+=1;
						table[hashing].key=temp[i].key;
						table[hashing].data=new T(*temp[i].data);
						table[hashing].status=CellStatus::ACTIVE;

					}
				}
			}

			//add new value
			collision=0;
			int hash_index=0;
			int hashing=(h(key)+hash_index)%this->m;
			if(table[hashing].status==CellStatus::EMPTY)
			{
				count+=1;
				table[hashing].key=key;
				table[hashing].data=data;
				table[hashing].status=CellStatus::ACTIVE;
			}
			else if(table[hashing].status==CellStatus::ACTIVE)
			{
				while(table[hashing].status!=CellStatus::EMPTY)
				{
					collision+=1;
					hash_index+=1;
					hashing=(h(key)+hash_index)%this->m;
					while(hashing>this->m)
					{
						hashing-=this->m;
					}
				}
				count+=1;
				table[hashing].key=key;
				table[hashing].data=data;
				table[hashing].status=CellStatus::ACTIVE;

			}
			for(int i=0; i<this->m/2; i++)
			{
				if(temp[i].status==CellStatus::ACTIVE  || this->table[i].status==CellStatus::DELETED)
				{
					delete temp[i].data;
				}
			}
			delete []temp;
			return collision;
		}
		else
		{

			collision=0;
			int hash_index=0;
			int hashing=(h(key)+hash_index)%this->m;
			if(table[hashing].status==CellStatus::EMPTY)
			{

				count+=1;
				table[hashing].key=key;
				table[hashing].data=data;
				table[hashing].status=CellStatus::ACTIVE;

			}
			else if(table[hashing].status!=CellStatus::EMPTY)
			{
				while(table[hashing].status!=CellStatus::EMPTY)
				{
					collision+=1;
					hash_index+=1;
					hashing=(h(key)+hash_index)%this->m;
					while(hashing>this->m)
					{
						hashing-=this->m;
					}
				}
				count+=1;
				table[hashing].key=key;
				table[hashing].data=data;
				table[hashing].status=CellStatus::ACTIVE;
			}

			return collision;
		}
	}
	else if(this->mode==Mode::QUADRATIC)
	{
		if(count>=this->m*loadLimit)
		{
			//reset the value of count
			count=0;

			//temporary cell save the content of table
			this->m = this->m*2;
			Cell *temp=new Cell[this->m/2];
			for(int i=0; i<this->m/2; i++)
			{
				temp[i].data=nullptr;
				temp[i].status=CellStatus::EMPTY;
			}
			for(int i=0; i<this->m/2; i++)
			{
				if(table[i].status == CellStatus::ACTIVE)
				{
					temp[i].key=table[i].key;
					temp[i].data=new T(*table[i].data);
					temp[i].status=CellStatus::ACTIVE;
				}
			}

			for(int i=0; i<this->m/2; i++)
			{
				if(this->table[i].status==CellStatus::ACTIVE || this->table[i].status==CellStatus::DELETED)
				{
					delete this->table[i].data;
				}
			}
			delete []this->table;

			table=new Cell[this->m];
			for(int i=0; i<this->m; i++)
			{
				table[i].status=CellStatus::EMPTY;
			}

			//assign value to table cell
			for(int i=0; i<this->m/2; i++)
			{
				int hash_index=0;
				if(temp[i].status==CellStatus::ACTIVE)
				{
					int hashing=(h(temp[i].key)+hash_index*hash_index)%this->m;

					if(table[hashing].status==CellStatus::EMPTY && temp[i].status==CellStatus::ACTIVE)
					{
						count+=1;
						table[hashing].key=temp[i].key;
						table[hashing].data=new T(*temp[i].data);
						table[hashing].status=CellStatus::ACTIVE;
					}
					else if(table[hashing].status==CellStatus::ACTIVE && temp[i].status==CellStatus::ACTIVE)
					{
						while(table[hashing].status!=CellStatus::EMPTY)
						{
							hash_index+=1;
							hashing=(h(temp[i].key)+hash_index*hash_index)%this->m;
							while(hashing>this->m)
							{
								hashing-=this->m;
							}
						}
						count+=1;
						table[hashing].key=temp[i].key;
						table[hashing].data=new T(*temp[i].data);
						table[hashing].status=CellStatus::ACTIVE;
					}
				}
			}
			//add new value
			collision=0;
			int hash_index=0;
			int hashing=(h(key)+hash_index*hash_index)%this->m;
			if(table[hashing].status==CellStatus::EMPTY)
			{
				count+=1;
				table[hashing].key=key;
				table[hashing].data=data;
				table[hashing].status=CellStatus::ACTIVE;
			}
			else if(table[hashing].status==CellStatus::ACTIVE)
			{
				while(table[hashing].status!=CellStatus::EMPTY)
				{
					collision+=1;
					hash_index+=1;
					hashing=(h(key)+hash_index*hash_index)%this->m;
					while(hashing>this->m)
					{
						hashing-=this->m;
					}
				}
				count+=1;
				table[hashing].key=key;
				table[hashing].data=data;
				table[hashing].status=CellStatus::ACTIVE;
			}
			for(int i=0; i<this->m/2; i++)
			{
				if(temp[i].status==CellStatus::ACTIVE || this->table[i].status==CellStatus::DELETED)
				{
					delete temp[i].data;
				}
			}
			delete []temp;
			return collision;
		}
		else
		{
			collision=0;
			int hash_index=0;
			int hashing=(h(key)+hash_index*hash_index)%this->m;
			if(table[hashing].status==CellStatus::EMPTY)
			{
				count+=1;
				table[hashing].key=key;
				table[hashing].data=data;
				table[hashing].status=CellStatus::ACTIVE;
			}
			else if(table[hashing].status!=CellStatus::EMPTY)
			{
				while(table[hashing].status!=CellStatus::EMPTY)
				{
					collision+=1;
					hash_index+=1;
					hashing=(h(key)+hash_index*hash_index)%this->m;
					while(hashing>this->m)
					{
						hashing-=this->m;
					}
				}
				count+=1;
				table[hashing].key=key;
				table[hashing].data=data;
				table[hashing].status=CellStatus::ACTIVE;
			}
			return collision;
		}
	}
	else if(this->mode==Mode::DOUBLE)
	{
		if(count>=this->m*loadLimit)
		{
			//reset the value of count
			count=0;

			//temporary cell save the content of table
			this->m = this->m*2;
			Cell *temp=new Cell[this->m/2];
			for(int i=0; i<this->m/2; i++)
			{
				//temp[i].data=nullptr;
				temp[i].status=CellStatus::EMPTY;
			}
			for(int i=0; i<this->m/2; i++)
			{
				if(table[i].status == CellStatus::ACTIVE)
				{
					temp[i].key=table[i].key;
					temp[i].data=new T(*table[i].data);
					temp[i].status=CellStatus::ACTIVE;
				}
			}
			for(int i=0; i<this->m/2; i++)
			{
				if(this->table[i].status==CellStatus::ACTIVE || this->table[i].status==CellStatus::DELETED)
				{
					delete this->table[i].data;
				}
			}
			delete []this->table;

			table=new Cell[this->m];
			for(int i=0; i<this->m; i++)
			{
				table[i].status=CellStatus::EMPTY;
			}

			//assign value to table cell
			for(int i=0; i<this->m/2; i++)
			{
				int hash_index=0;
				if(temp[i].status==CellStatus::ACTIVE)
				{
					int hashing=(h(temp[i].key)+hash_index*h2(temp[i].key))%this->m;
					if(table[hashing].status==CellStatus::EMPTY && temp[i].status==CellStatus::ACTIVE)
					{
						count+=1;
						table[hashing].key=temp[i].key;
						table[hashing].data=new T(*temp[i].data);
						table[hashing].status=CellStatus::ACTIVE;
					}
					else if(table[hashing].status==CellStatus::ACTIVE && temp[i].status==CellStatus::ACTIVE)
					{
						while(table[hashing].status!=CellStatus::EMPTY)
						{
							hash_index+=1;
							int hashing=(h(temp[i].key)+hash_index*h2(temp[i].key))%this->m;
							while(hashing>this->m)
							{
								hashing-=this->m;
							}
						}
						count+=1;
						table[hashing].key=temp[i].key;
						table[hashing].data=new T(*temp[i].data);
						table[hashing].status=CellStatus::ACTIVE;
					}
				}
			}
			//add new value
			collision=0;
			int hash_index=0;
			int hashing=(h(key)+hash_index*h2(key))%this->m;;
			if(table[hashing].status==CellStatus::EMPTY)
			{
				count+=1;
				table[hashing].key=key;
				table[hashing].data=data;
				table[hashing].status=CellStatus::ACTIVE;
			}
			else if(table[hashing].status==CellStatus::ACTIVE)
			{
				while(table[hashing].status!=CellStatus::EMPTY)
				{
					collision+=1;
					hash_index+=1;
					hashing=(h(key)+hash_index*h2(key))%this->m;
					while(hashing>this->m)
					{
						hashing-=this->m;
					}
				}
				count+=1;
				table[hashing].key=key;
				table[hashing].data=data;
				table[hashing].status=CellStatus::ACTIVE;
			}
			for(int i=0; i<this->m/2; i++)
			{
				if(temp[i].status==CellStatus::ACTIVE || this->table[i].status==CellStatus::DELETED)
				{
					delete temp[i].data;
				}
			}
			delete []temp;
			return collision;
		}
		else
		{
			collision=0;
			int hash_index=0;
			int hashing=(h(key)+hash_index*h2(key))%this->m;
			if(table[hashing].status==CellStatus::EMPTY)
			{
				count+=1;
				table[hashing].key=key;
				table[hashing].data=data;
				table[hashing].status=CellStatus::ACTIVE;
			}
			else if(table[hashing].status!=CellStatus::EMPTY)
			{
				while(table[hashing].status!=CellStatus::EMPTY)
				{
					collision+=1;
					hash_index+=1;
					hashing=(h(key)+hash_index*h2(key))%this->m;
					while(hashing>this->m)
					{
						hashing-=this->m;
					}
				}
				count+=1;
				table[hashing].key=key;
				table[hashing].data=data;
				table[hashing].status=CellStatus::ACTIVE;
			}
			return collision;
		}
	}
	if(collision>=m)
	{
		return -1;
	}
}

template<typename K,typename T>
bool HashTable<K,T>::remove(K key)
{
	for(int i=0; i<this->m; i++)
	{
		if(this->table[i].status==CellStatus::ACTIVE)
		{
			if(key==this->table[i].key)
			{
				this->table[i].status=CellStatus::DELETED;
				return true;
			}
		}
	}
	return false;
}

template<typename K,typename T>
T* HashTable<K,T>::get(K key) const
{
	int i=0;
	for(; i<this->m; i++)
	{
		if(this->table[i].status==CellStatus::ACTIVE)
		{
			if(key==this->table[i].key)
			{
				return this->table[i].data;
			}
		}
	}
	if(i==this->m)
	{
		return nullptr;
	}
	return nullptr;
}

