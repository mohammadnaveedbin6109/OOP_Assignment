#include<iostream>
using namespace std;
int** createMatrix(int rows, int cols){
    int**matrix=new int *[rows];
    for(int i=0;i<rows;i++)
    *(matrix+i)=new int[cols];
    return matrix;
}
void inputMatrix(int** matrix, int rows, int cols){
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            cin>>*(*(matrix+i)+j);
        }
    }
}
int diagonalDifference(int** matrix, int n){
    // primary diagonal
    int psum=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)
        if(i==j)
        psum+=*(*(matrix+i)+j);
    }

    // secondry sum
    int ssum=0;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)
        if(i+j==n-1)
        ssum+=*(*(matrix+i)+j);
    }

    //difference
    return psum-ssum;
}
int dominantRow(int** matrix, int n){
    int sum=0;
    int ind=0;
    for(int i=0;i<n;i++){
        int isum=0;
        for(int j=0;j<n;j++){
            isum+=*(*(matrix+i)+j);
        }
        if(isum>sum){
            sum=isum;
            ind=i;
        }
    }
    return ind;

}
int** rotateMatrix(int** matrix, int n){
    int**rmatrix=createMatrix(n,n);
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            *(*(rmatrix+j)+i)=*(*(matrix+i)+j);
        }
    }
    // now rotating it
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            rmatrix[n-1-i][j]=matrix[i][j];
        }
    }
    return rmatrix;
}
int matrixBalance(int** matrix, int n){
    // above the primary diagonal
    int asum=0;
    for(int i=0;i<n-1;i++){
        for(int j=i+1;j<n;j++){
            asum+=*(*(matrix+i)+j);
        }
    }
    // below the primary diagonal
    int bsum=0;
    for(int i=1;i<n;i++){
        for(int j=0;j<i;j++){
            bsum+=*(*(matrix+i)+j);
        }
    }
    return asum-bsum;
}
bool isBalancedMatrix(int** matrix, int n){
    int dsum=diagonalDifference(matrix,n);
    int matrixb=matrixBalance(matrix,n);
    int maxsum=dominantRow(matrix,n);
    cout<<"diagonal difference  "<<dsum<<endl;
    cout<<"matrix balance "<<matrixb<<endl;
    cout<<"dominant row  "<<maxsum<<endl;
    // calculating dominant row sum
    int sum=0;
    for(int j=0;j<n;j++){
        sum+=*(*(matrix+maxsum)+j);
    }
    cout<<"dominant row sum "<<sum<<endl;
    // checking if more than 1 has dominant rows
    bool check=0,check1,check2;
     for(int i=0;i<n;i++){
        int isum=0;
        for(int j=0;j<n;j++){
            isum+=*(*(matrix+i)+j);
        }
        if(isum==sum){
            if(check){
                check=0;
                break;
            }
            check=1;
        }
    }
    check1=(dsum==0);
    check2=(matrixb==0);
    if(check && check1 && check2)
    return true;
    else
    return false;

}
int** transformMatrix(int** matrix, int n){
    int**tmatrix=createMatrix(n,n);
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            if(i+j==n-1){
                if(*(*(tmatrix+i)+j)<0)
                *(*(tmatrix+i)+j)*=-1;
            }
            if(i==j){
                *(*(tmatrix+i)+j)*=*(*(tmatrix+i)+j);
            }

        }
    }
    return tmatrix;
}

int main(){
    int rows=3 ,cols=3;
    int **matrix=createMatrix(rows,cols);
    inputMatrix(matrix,rows,cols);
    isBalancedMatrix(matrix,rows);
    int**rmatrix=rotateMatrix(matrix,rows);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            cout<<*(*(rmatrix+i)+j)<<" ";
        }
        cout<<endl;
    }
    for(int i=0;i<rows;i++){
        delete[] *(matrix+i);
        delete[] *(rmatrix+i);
    }
    delete[] matrix;
    delete[] rmatrix;

}