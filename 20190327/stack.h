#pragma once
struct node
{
	int row;
	int col;
	unsigned char color;
	node* next;
};
class stack
{
public:
	stack()
	{
		count = 0;
		top = nullptr;
	}
	~stack() {}
	bool push(int r, int c, unsigned char cl)
	{
		if (count == 0)
		{
			node* p = new node;
			p->row = r; p->col = c; p->color = cl;
			p->next = nullptr;
			top = p;
			count++;
			return true;
		}
		else
		{
			node* p = top;
			/*if (p->row == r && p->col == c)
				return false;
			while (p->next != nullptr)
			{
				p = p->next;
				if (p->row == r && p->col == c)
					return false;
			}*/
			p = new node;
			p->row = r; p->col = c; p->color = cl;
			p->next = top;
			top = p;
			count++;
			return true;
		}
	}
	bool pop(int& r, int& c, unsigned char& cl)
	{
		if (count == 0)
			return false;
		node* p = top->next;
		r = top->row; c = top->col; cl = top->color;
		delete top;
		top = p;
		count--;
		return true;
	}
	void showlist()
	{
		if (count == 0)
			return;
		cout << top->row << ' ' << top->col << ' ' << endl;
		while (top->next != nullptr)
		{
			top = top->next;
			cout << top->row << ' ' << top->col << ' ' << endl;
		}
	}
	bool isEmpty()
	{
		return count == 0;
	}
	node* top;
	int count;
	
};