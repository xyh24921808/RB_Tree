#pragma once
#include<iostream>
#include<vector>
using namespace std;

//红黑树 是一颗二叉搜索树 搜索时间复杂度是 O(log2n)
//1.一个结点要么是红节点要么是黑节点
//2.从根最长路径不超过最短路径的2倍
//比AVL树优势在旋转次数变少

enum Color
{
	RED,
	BLACK,
};
//Tree_val结构
template<class T>
struct Tree_val
{
	Tree_val<T>* _left;
	Tree_val<T>* _right;
	Tree_val<T>* _parent;

	T _date;
	Color _color=RED;
	Tree_val(const T&date,Color color)
	{
		_left = nullptr;
		_right = nullptr;
		_parent = nullptr;
		 _date= date;
		_color = color;
	}
};

//红黑树迭代器
template<class T,class Ref,class Ptr>
struct RB_TreeIterator
{
	typedef Tree_val<T> Node;
	typedef RB_TreeIterator<T, Ref, Ptr> Self;
	Node* _node;
	RB_TreeIterator(Node* root)
	{
		_node = root;
	}
	
	Ref operator*()
	{
		return _node->_date;
	}
	Ptr operator->()
	{
		return &_node->_date;
	}
	Self operator++()
	{
		if (_node->_right)
		{
			Node* min = _node->_right;
			while (min->_left)
			{
				min = min->_left;
			}
			_node = min;
		}
		else
		{
			Node* cur = _node;
			Node* parent = cur->_parent;
			while (parent && cur == parent->_right)
			{
				cur = cur->_parent;
				parent = parent->_parent;
			}
			_node = parent;
		}
		return *this;
	}
	Self operator--()
	{
		if (_node->_left)
		{
			Node* max = _node->_left;
			while (max->_right)
			{
				max = max->_right;
			}
			_node=max;
		}
		else
		{
			Node* cur = _node;
			Node* parent = cur->_parent;
			while (parent && cur==parent->_left)
			{
				cur = cur->_parent;
				parent = parent->_parent;
			}
			_node = parent;
		}
		return *this;
	}
	bool operator!=(const Self&i)
	{
		return _node != i._node;
	}
	bool operator==(const Self& b)
	{
		return _node == b._node;
	}
};


template<class K,class T,class KeyOft>
struct R_B_Tree
{
	KeyOft kot;
	typedef Tree_val<T> Node;
	typedef R_B_Tree<K, T, KeyOft> RBTree;
public:
	typedef RB_TreeIterator<T, T&, T*> iterator;
	R_B_Tree()
	{
		_root = nullptr;
	}
	R_B_Tree(const RBTree& b)
	{
		_root = Copy(b._root);
	}
	~R_B_Tree()
	{
		_root = nullptr;
		Destroy(_root);
	}
	iterator Find(const K&key)
	{
		Node* cur = _root;
		while (cur)
		{
			if (kot(cur->_date) > key)
			{
				cur = cur->_left;
			}
			else if (kot(cur->_date) < key)
			{
				cur = cur->_right;
			}
			else
			{
				return iterator(cur);
			}
		}
		return iterator(nullptr);
	}
	iterator begin()
	{
		Node* cur = _root;
		while (cur&&cur->_left)
			cur = cur->_left;
		return iterator(cur);
	}
	iterator end()
	{
		return iterator(nullptr);
	}
	//这里跟AVL树一样的右旋
	void Rx(Node* parent)
	{
		Node* cur = parent->_left;
		Node* Rcur = cur->_right;

		parent->_left = Rcur;
		if (Rcur != nullptr)
		{
			Rcur->_parent = parent;
		}

		Node* pp = parent->_parent;

		cur->_right = parent;
		parent->_parent = cur;

		if (parent == _root)
		{
			_root = cur;
			cur->_parent = nullptr;
		}
		else
		{
			if (pp->_left == parent)
			{
				pp->_left = cur;
			}
			else
			{
				pp->_right = cur;
			}
			cur->_parent = pp;
		}
	}

	//这里跟AVL树一样左旋
	void Lx(Node* parent)
	{
		Node* cur = parent->_right;
		Node* Lcur = cur->_left;


		parent->_right = Lcur;
		if (Lcur != nullptr)
		{
			Lcur->_parent = parent;
		}

		Node* pp = parent->_parent;
		parent->_parent = cur;
		cur->_left = parent;

		if (parent == _root)
		{
			_root = cur;
			cur->_parent = nullptr;
		}
		else
		{
			if (pp->_left == parent)
			{
				pp->_left = cur;
			}
			else
			{
				pp->_right = cur;
			}
			cur->_parent = pp;
		}
	}

	
	pair<iterator,bool> insert(const T&val)
	{
		if (_root == nullptr)
		{
			_root = new Node(val,BLACK);
			return make_pair(iterator(_root),true);
		}

		Node* parent = nullptr;
		Node* cur = _root;
		while (cur)
		{
			if (kot(cur->_date) >kot(val))
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (kot(cur->_date) < kot(val))
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return make_pair(iterator(cur), false);
			}
		}
		cur = new Node(val,RED);
		Node* newnode = cur;
		if (kot(parent->_date) >kot(cur->_date))
		{
			parent->_left = cur;
		}
		else
		{
			parent->_right = cur;
		}
		cur->_parent = parent;

		while (parent && parent->_color == RED)
		{
			Node* pp = parent->_parent;
			if (pp->_left == parent)
			{
				Node* unlce = pp->_right;
				if (unlce && unlce->_color == RED)
				{
					parent->_color = BLACK;
					unlce->_color = BLACK;
					pp->_color = RED;
					
					cur = pp;
					parent = cur->_parent;
				}
				else
				{
					if (cur == parent->_left)
					{
						Rx(pp);
						parent->_color = BLACK;
						pp->_color = RED;
					}
					else
					{
						Lx(parent);
						Rx(pp);
						cur->_color = BLACK;
						pp->_color = RED;
					}
					break;
				}
			}
			else
			{
				Node* unlce = pp->_left;
				if (unlce && unlce->_color == RED)
				{
					parent->_color = BLACK;
					unlce->_color = BLACK;
					pp->_color = RED;

					cur = pp;
					parent = cur->_parent;
				}
				else
				{
					if (cur==parent->_right)
					{
						Lx(pp);
						parent->_color = BLACK;
						pp->_color = RED;
					}
					else
					{
						Rx(parent);
						Lx(pp);
						cur->_color = BLACK;
						pp->_color = RED;
					}
					break;
				}
			}
			_root->_color = BLACK;
		}
		return make_pair(iterator(newnode),true);
	}
	int treeH(Node* root)
	{
		if (root == nullptr)
			return  0;
		int L = treeH(root->_left);
		int R = treeH(root->_right);
		return L > R ? L + 1 : R + 1;
	}
	
	void tree_K()
	{
		_tree_b(_root);
	}

	bool isban()
	{
		if (_root && _root->_color == RED)
		{
			return false;
		}
		int Black_d = 0;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_color == BLACK)
				Black_d++;
			cur = cur->_left;
		}
		return _isban(_root,0,Black_d);
	}
	RBTree& operator==(RBTree T)
	{
		swap(_root, T._root);
		return *this;
	}
private:
	void Destroy(Node* root)
	{
		if (!root)
			return;
		Destroy(root->_left);
		Destroy(root->_right);
		delete root;
	}

	bool _isban(Node* root, int count, int& BC)
	{
		if (!root)
		{
			if (count != BC)
				return false;
			else
				return true;
		}
		if (root->_color == RED && root->_parent->_color == RED)
		{
			cout << "出现连续红节点" << endl;
			return false;
		}
		if (root->_color == BLACK)
			count++;
		return _isban(root->_left, count, BC) && _isban(root->_right, count, BC);
	}
	void _tree_b(Node* root)
	{
		if (root == nullptr)
			return;
		_tree_b(root->_left);
		cout << kot(root->_date) << endl;
		_tree_b(root->_right);
	}

	Node* Copy(Node* root)
	{
		if (!root)
			return nullptr;
		
		Node* newnode = new Node(root->_date,root->_color);
		newnode->_left = Copy(root->_left);
		newnode->_right = Copy(root->_right);
		if (newnode->_left)
		{
			newnode->_left->_parent = newnode;
		}
		if (newnode->_right)
		{
			newnode->_right->_parent = newnode;
		}
		return newnode;
	}
	Node* Copy(Node* root,Node*prev)
	{
		if (!root)
			return nullptr;
		Node* newnode = new Node(root->_date, root->_color);
		if (prev != nullptr)
		{
			newnode->_parent = prev;
		}
		newnode->_left = Copy(root->_left,newnode);
		newnode->_right = Copy(root->_right,newnode);
		return newnode;
	}
	Node* _root;
};
