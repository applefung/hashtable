//submit this file

//you do NOT need to include any header in this file
//just write your BST implementation here right away
#ifndef BSTIMPLEMENTATION_H_
#define BSTIMPLEMENTATION_H_

#include "bst.h"
#include <iostream>
#include <typeinfo>
using namespace std;
struct BSTNode;

template <typename T>
BST<T>::BST(const BST &another)
{
	this->root->key=another.root.key;
	this->root->left=another.root.left;
	this->root->right=another.root.right;
	this->root->value=another.root.value;
	delete this->root;
	this->root=new BSTNode(this->root->key,this->root->value);
	*(this->root)=*(another.root);
}

template <typename T>
bool BST<T>::isEmpty() const
{
	if(this->root==nullptr)
	{
		return true;
	}
	return false;
}

template <typename T>
bool BST<T>::add(string key, T value)
{
	bool temp=false;
	if(isEmpty())
	{
		this->root=new BSTNode(key,value);
		return true;
	}
	/*if ((this->root != nullptr && key.compare(this->root->key)==0) || (this->root->left.root != nullptr && key.compare(this->root->left.root->key)==0) || (this->root->right.root != nullptr && key.compare(this->root->right.root->key)==0))
	{
		return false;
	}*/
	else if(key.compare(this->root->key)<0)
	{
		temp=this->root->left.add(key, value);
	}
	else if(key.compare(this->root->key)>0 )
	{
		temp=this->root->right.add(key, value);
	}
	else if(key.compare(this->root->key)==0 )
	{
		temp=false;
	}
	return temp;
	// ||(this->root != nullptr && value==this->root->value)
	// && value<this->root->value
	//&& value>this->root->value
}

template <typename T>
bool BST<T>::remove(string key)
{
	//find max and compare with key, if key is out of boundary, then return false
	const BSTNode* node=this->root;
	while(!node->right.isEmpty())
	{
		node=node->right.root;
	}
	if(key.compare(node->key)>0)
	{
		return false;
	}

	if(this->isEmpty())
	{
		return false;
	}
	else if(key.compare(this->root->key)<0 && (this->root->left.root != nullptr || this->root->right.root != nullptr))
	{
		this->root->left.remove(key);
	}
	else if(key.compare(this->root->key)>0 && (this->root->left.root != nullptr || this->root->right.root != nullptr))
	{
		this->root->right.remove(key);
	}
	else if(this->root->left.root != nullptr && this->root->right.root !=nullptr)
	{
		const BST<T> *temp=this->findMin();
		this->root->key=temp->root->key;
		this->root->value=temp->root->value;
		this->root->left.remove(this->root->key);
	}
	else
	{
		BSTNode* deleting_node=this->root;
		this->root=(this->root->left.isEmpty()) ? this->root->right.root : this->root->left.root;
		deleting_node->left.root=nullptr;
		deleting_node->right.root=nullptr;
		delete deleting_node;
	}

}

template <typename T>
const BST<T>* BST<T>::findMin() const
{
	const BSTNode *node=this->root;
	while(!node->left.root->left.isEmpty())
	{
		node=node->left.root;
	}
	return &node->left;
}

template <typename T>
void BST<T>::merge(BST &another)
{
	BST<T> temp;
	if(another.isEmpty())
	{
		return;
	}

	else if( !another.isEmpty())
	{
		if(this->root->left.root == nullptr && this->root->right.root != nullptr)
		{
			temp.add(this->root->key,this->root->value);
			while(!this->root->right.isEmpty())
			{
				temp.add(this->root->right.root->key,this->root->right.root->value);
				this->remove(this->root->right.root->key);
			}
			this->remove(this->root->key);
		}
		else
		{
			while(!this->isEmpty())
			{
				if(this->root==nullptr)
				{
					return;
				}
				else if(this->root!=nullptr)
				{
					if(this->root->left.root != nullptr)
					{
						temp.add(this->root->left.root->key,this->root->left.root->value);
						this->remove(this->root->left.root->key);
					}
					else if(this->root->right.root != nullptr)
					{
						temp.add(this->root->right.root->key,this->root->right.root->value);
						this->remove(this->root->right.root->key);
					}
					temp.add(this->root->key,this->root->value);
					this->remove(this->root->key);

				}
			}
		}

		if(another.root->left.root == nullptr && another.root->right.root != nullptr)
		{

			temp.add(another.root->key,another.root->value);
			while(!another.root->right.isEmpty())
			{
				temp.add(another.root->right.root->key,another.root->right.root->value);
				another.remove(another.root->right.root->key);
			}
			another.remove(another.root->key);
		}
		else
		{
			while(!another.isEmpty())
			{
				if(another.root==nullptr)
				{
					return;
				}
				else if(another.root!=nullptr)
				{
					if(another.root->left.root != nullptr)
					{
						temp.add(another.root->left.root->key,another.root->left.root->value);
						another.remove(another.root->left.root->key);
					}
					else if(another.root->right.root != nullptr)
					{
						temp.add(another.root->right.root->key,another.root->right.root->value);
						another.remove(another.root->right.root->key);
					}
					temp.add(another.root->key,another.root->value);
					another.remove(another.root->key);

				}
			}
		}

		while(!temp.isEmpty())
		{
			if(temp.root==nullptr)
			{
				return;
			}
			if(temp.root!=nullptr)
			{
				this->add(temp.root->key,temp.root->value);
				temp.remove(temp.root->key);
			}
		}


	}

}

template <typename T>
T* BST<T>::get(string key) const
{

	if(isEmpty())
	{
		return nullptr;
	}
	if(key==this->root->key)
	{
		return &this->root->value;
	}
	else if(key<this->root->key)
	{
		return this->root->left.get(key);
	}
	else if(key>this->root->key)
	{
		return this->root->right.get(key);
	}
}
#endif
