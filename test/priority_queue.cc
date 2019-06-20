 ///
 /// @file    priority_queue.cc
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2019-05-30 10:59:27
 ///
 
#include <cmath>
#include <iostream>
#include <vector>
#include <queue>
using std::cout;
using std::endl;
using std::vector;
using std::priority_queue;

void test0()
{
	int array[10] = {5, 7, 1, 2, 3, 4, 6, 9, 8, 0};
	//priority_queue<int> que(array, array + 10);
	//
	//默认情况下，是用小于符号比较, 得到的是一个降序序列(大顶堆)
	//
	//底层实现： 堆排序 ==> 二叉堆 ==> (大顶堆，小顶堆)
	//
	priority_queue<int> que;

	for(int idx = 0; idx != 10; ++idx) {
		que.push(array[idx]);
		cout << "此时优先级最高的元素是: " << que.top() << endl;
	}

	while(!que.empty()) {
		cout << que.top() << " ";
		que.pop();
	}
	cout << endl;
}
 
void test1()
{
	int array[10] = {5, 7, 1, 2, 3, 4, 6, 9, 8, 0};
	//priority_queue<int> que(array, array + 10);
	//
	//用大于符号比较, 得到的是一个升序序列(小顶堆)
	//
	//底层实现： 堆排序 ==> 二叉堆 ==> (大顶堆，小顶堆)
	//
	priority_queue<int, vector<int>, std::greater<int>> que;

	for(int idx = 0; idx != 10; ++idx) {
		que.push(array[idx]);
		cout << "此时优先级最高的元素是: " << que.top() << endl;
	}

	while(!que.empty()) {
		cout << que.top() << " ";
		que.pop();
	}
	cout << endl;
}

class Point
{
public:
	Point(int ix = 0, int iy = 0)
	: _ix(ix)
	, _iy(iy)
	{	cout << "Point(int,int)" << endl;	}

	float getDistance() const
	{
		return sqrt(_ix * _ix + _iy * _iy);
	}

	friend std::ostream & operator<<(std::ostream &os, const Point & rhs);
private:
	int _ix;
	int _iy;
};

std::ostream & operator<<(std::ostream &os, const Point & rhs)
{
	os << "(" << rhs._ix
	   << "," << rhs._iy
	   << ")";
	return os;
}

bool operator<(const Point & lhs, const Point & rhs)//优先队列<>内只写Point时使用，见test2
{
	return lhs.getDistance() < rhs.getDistance();
}

struct PointComparator
{
	bool operator()(const Point & lhs, const Point & rhs) const
	{
		return lhs.getDistance() < rhs.getDistance();//小于符号是降序，大于符号是升序
	}
	
};

void test2()
{
	priority_queue<Point> que;
	//priority_queue<Point, vector<Point>, PointComparator> que;
	que.push(Point(1, 2));
	que.push(Point(-1, 2));
	que.push(Point(3, 2));
	que.push(Point(4, 2));
	que.push(Point(1, 6));

	while(!que.empty()) {
		cout << que.top() << " ";
		que.pop();
	}
	cout << endl;
}

int main(void)
{
	//test0();
	//test1();
	test2();
	return 0;
}
