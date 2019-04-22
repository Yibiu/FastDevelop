#include "sort.h"


template <typename T>
CSort<T>::CSort()
{
}

template <typename T>
CSort<T>::~CSort()
{
}

/************************************************
* 名称：冒泡排序
* 描述：
* 1. 将第一个元素与第二个元素比较大小，如果第一个元素大于第二个元素则调换他们两的位置；
* 2. 比较第二个元素和第三个元素的大小，如果第二个元素大于第三个元素则调换他们两的位置；
* 3. 依次类推，进行两两元素的比较和交换，最终最大的元素排在了最后面；
* 4. 重复1~3过程，直到所有元素都排序。
*************************************************/
template <typename T>
void CSort<T>::bubble(T *arrays, int n)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n - 1 - i; j++) {
			if (arrays[j] > arrays[j + 1])
				_swap(arrays[j], arrays[j + 1]);
		}
	}
}

/**********************************************
* 名称：插入排序
* 描述：
* 1. 对于第K个元素，将该元素的值存储在零时变量中，比较第前一个元素与该元素的大小，如果大于该元素就将前一个元素往后移动一步；
* 2. 比较前面第二个元素与该元素的大小，如果大于该元素就将前第二个元素往后移动一步；
* 3. 重复上述过程直到找到小于等于原来第K个元素（保存在零时变量中）的位置，并将第K个元素插入到这个元素的后面。
*	或者找不到小于等于第K个元素的位置，就将原来第K个元素插入到数组的首地址。
***********************************************/
template <typename T>
void CSort<T>::insert(T *arrays, int n)
{
	for (int i = 1; i < n; i++) {
		T tmp = arrays[i];
		int j = 0;
		for (j = i - 1; j >= 0; j--) {
			if (tmp < arrays[j]) {
				arrays[j + 1] = arrays[j];
			}
			else
				break;
		}
		arrays[j + 1] = tmp;
	}
}

/**********************************************
* 名称：选择排序
* 描述：
*	每一次从后面选择出一个最小的值，替换到前面来。
***********************************************/
template <typename T>
void CSort<T>::select(T *arrays, int n)
{
	for (int i = 0; i < n; i++) {
		int index = i;
		T min = arrays[i];
		for (int j = i; j < n; j++) {
			if (arrays[j] < min) {
				index = j;
				min = arrays[j];
			}
		}
		_swap(arrays[i], arrays[index]);
	}
}

/*********************************************
* 名称：希尔排序
* 描述：
* 教科书式表述：
* 1. 先取一个小于n的整数d1作为第一个增量，把文件的全部记录分组；所有距离为d1的倍数的记录放在同一个组中；
* 2. 先在各组内进行直接插入排序；
* 3. 合并各组；
* 4. 取第二个增量d2<d1重复上述的分组和排序；
* 5. 重复4，直至所取的增量dt=1(dt<dt-l<…<d2<d1)，即所有记录放在同一组中进行直接插入排序为止。
* 大白话表述：
* 1. 一个书架放着一排书，现在从第一本书起每数X本书，就在那本书上贴红色贴纸，贴完红色贴纸后，
*	再次从第二本书起每数X本书就贴上蓝色贴纸（跟之前颜色不同即可），重复贴纸过程，直到所有书都贴满贴纸；
* 2. 接着对有相同颜色贴纸的书做插入排序；
* 3. 然后撕掉所有贴纸；
* 4. 重新对书进行贴纸，这次则每数Y本书就贴纸（Y>X）,所有书贴满后再进行插入排序；
* 5. 重复4，直到最后每数1本书就贴纸（也就是每本书都贴同样颜色贴纸），再插入排序为止。
**********************************************/
template <typename T>
void CSort<T>::shell(T *arrays, int n)
{
	for (int gap = n / 2;gap > 0; gap /= 2) {
		for (int i = 0; i < gap; i++) {
			for (int j = i; j < n; j += gap) {
				T tmp = arrays[j];
				int k = 0;
				for (k = j - gap; k >= 0; k -= gap) {
					if (tmp < arrays[k]) {
						arrays[k + gap] = arrays[k];
					}
					else
						break;
				}
				arrays[k + gap] = tmp;
			}
		}
	}
}

template <typename T>
void CSort<T>::show(T *arrays, int n)
{
	cout << "{ ";
	for (int i = 0; i < n; i++) {
		cout << arrays[i] << " ";
	}
	cout << "}" << endl;
}


template <typename T>
void CSort<T>::_swap(T &a, T &b)
{
	T tmp = a;
	a = b;
	b = tmp;
}
