#pragma once
#include<iostream>
#include<vector>
using namespace std;

//红黑树 是一颗二叉搜索树 搜索时间复杂度是 O(log2n)
//1.一个结点要么是红节点要么是黑节点
//2.从根最长路径不超过最短路径的2倍
enum Color
{
	RED,
	BLACK,
};
//Tree_val结构
template<class K,class V>
struct Tree_val
{
	Tree_val<K, V>* _left;
	Tree_val<K, V>* _right;
	Tree_val<K, V>* _parent;

	pair<K, V>_kv;
	Color _color=RED;
	Tree_val(const pair<K,V>&kv,Color color)
	{
		_left = nullptr;
		_right = nullptr;
		_parent = nullptr;
		_kv = kv;
		_color = color;
	}
};

template<class K,class V>
class R_B_Tree
{
public:
	typedef Tree_val<K, V> Node;
	R_B_Tree()
	{
		_root = nullptr;
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

	
	bool insert(const pair<K, V>& val)
	{
		if (_root == nullptr)
		{
			_root = new Node(val,BLACK);
			return true;
		}

		Node* parent = nullptr;
		Node* cur = _root;
		while (cur)
		{
			if (cur->_kv.first > val.first)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (cur->_kv.first < val.first)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return false;
			}
		}
		cur = new Node(val,RED);
		if (parent->_kv.first > cur->_kv.first)
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
		return true;
	}
	int treeH(Node* root)
	{
		if (root == nullptr)
			return  0;
		int L = treeH(root->_left);
		int R = treeH(root->_right);
		return L > R ? L + 1 : R + 1;
	}
	void _tree_b(Node* root)
	{
		if (root == nullptr)
			return;

		_tree_b(root->_left);
		cout << root->_kv.first << endl;
		_tree_b(root->_right);
	}
	void tree_K()
	{
		_tree_b(_root);
	}

	bool _isban(Node* root,int count,int&BC)
	{
		if (!root)
		{
			if (count != BC)
				return false;
			else
				return true;
		}
		if (root->_color == RED&&root->_parent->_color==RED)
		{
			cout << "出现连续红节点" << endl;
			return false;
		}
		if (root->_color == BLACK)
			count++;
		return _isban(root->_left,count,BC) && _isban(root->_right,count,BC);
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
private:
	Node* _root;
};
