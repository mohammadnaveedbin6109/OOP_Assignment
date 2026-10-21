                  // Q2
#include<iostream>
using namespace std;
int* findEquilibriumIndices(int* arr, int size, int& count) {
	int* temprindice = new int[size];
	for (int i = 0;i < size;i++)
		*(temprindice + i) = -1;
	count = 0;
	for (int i = 0;i < size;i++) {
		// left sum
		int sumlt = 0;
		for (int lt = 0;lt < i;lt++)
			sumlt += *(arr + lt);
		// right sum
		int sumrt = 0;
		for (int rt = size - 1;rt > i;rt--)
			sumrt += *(arr + rt);

		if (sumrt == sumlt) {
			count++;
			*(temprindice + i) = i;
		}
	}
    if (count == 0) {
    delete[] temprindice;
    return nullptr;
    }
	int* eqindices = new int[count];
	int a = 0;
	for (int i = 0, a = 0;i < size;i++) {
		if (*(temprindice + i) != -1) {
			*(eqindices + a) = *(temprindice + i);
			a++;
		}
	}
	delete[] temprindice;
	return eqindices;
}
int balanceValue(int* arr, int size) {
	int tsum = 0, rt = 0;
	bool first = false;
	for (int i = 0;i < size;i++) {
		// left sum
		int sumlt = 0;
		for (int lt = 0;lt < i;lt++)
			sumlt += *(arr + lt);
		// right sum
		int sumrt = 0;
		for (int rt = size - 1;rt > i;rt--)
			sumrt += *(arr + rt);

		if (sumrt == sumlt) {
			if (first == false) {
				first = true;
				tsum += sumlt;
			}
			if (first) tsum -= rt; 
            else { first = true; tsum += sumlt; }
            rt = sumrt;
            tsum += rt;

		}
	}
    if (!first) 
    return -1;
    return tsum;

}
int* createBalancedArray(int* arr, int size, int& newSize) {
    int *fequi=findEquilibriumIndices(arr,size,newSize);
    int count=size-newSize;
    int*barray=new int[count];
    // left of the first equilibrium indice
    int i=0;
    for(i=0;i<*(fequi);i++){
        *(barray+i)=*(arr+i);
    }
    // right of the last equilibrium
    for(int j=*(fequi+newSize)+1;j<size;j++,i++){
        *(barray+i)=*(arr+i);
    }
    delete [] fequi;
    return barray;
}

int main() {
	int arr[] = { -7, 1, 5, 2, -4, 3, 0 };
	int size = 7;
	int count = 0;
	int* equi = createBalancedArray(arr, size, count);
    int* fequi= findEquilibriumIndices(arr,7,count);
	for (int i = 0;i < count;i++)
		cout << *(equi + i) << " ";
	cout << endl;
	cout << balanceValue(arr, size);
    // deleting 
    delete[] equi;
    delete[] fequi;
}