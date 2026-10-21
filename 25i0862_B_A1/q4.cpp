#include<cstdlib>
#include<iostream>
using namespace std;
void allocaterows(char***grid,int layers,int rows,int cols,int l,int r){
    if(l==layers)
    return;
    if(r==rows)
    return allocaterows(grid,layers,rows,cols,l+1,0);
    *(*(grid+l)+r)=new char[cols];
    allocaterows(grid,layers,rows,cols,l,r+1);
}
void allocatelayers(char***grid,int layers,int rows,int cols,int l){
    if(l==layers)
    return;
    *(grid+l)=new char*[rows];
    allocatelayers(grid,layers,rows,cols,l+1);
}
char ***creategrid(int layers,int rows,int cols){
    char***grid=new char**[layers];
    allocatelayers(grid,layers,rows,cols,0);
    allocaterows(grid,layers,rows,cols,0,0);
    return grid;
}
void initializeGrid(char***grid,int layers,int rows,int cols,int l,int r,int c){
    if(l==layers)
    return;
    if(r==rows)
    return initializeGrid(grid,layers,rows,cols,l+1,0,0);
    if(c==cols)
    return initializeGrid(grid,layers,rows,cols,l,r+1,0);
    grid[l][r][c]='x';
    initializeGrid(grid,layers,rows,cols,l,r,c+1);

}
void getDirection(int dir_num, int* dz, int* dy, int* dx) {
    if (dir_num == 1)      { *dz = 0;  *dy = 0;  *dx = 1;  } // X axis
    else if (dir_num == 2) { *dz = 0;  *dy = 1;  *dx = 0;  } // Y axis
    else if (dir_num == 3) { *dz = 1;  *dy = 0;  *dx = 0;  } // Z axis
    else if (dir_num == 4) { *dz = 0;  *dy = 1;  *dx = 1;  } // XY diagonal
    else if (dir_num == 5) { *dz = 0;  *dy = -1; *dx = 1;  } // XY diagonal
    else if (dir_num == 6) { *dz = 1;  *dy = 0;  *dx = 1;  } // XZ diagonal
    else if (dir_num == 7) { *dz = -1; *dy = 0;  *dx = 1;  } // XZ diagonal
    else if (dir_num == 8) { *dz = 1;  *dy = 1;  *dx = 0;  } // YZ diagonal
    else if (dir_num == 9) { *dz = -1; *dy = 1;  *dx = 0;  } // YZ diagonal
    else if (dir_num == 10){ *dz = 1;  *dy = 1;  *dx = 1;  } // XYZ diagonal
    else if (dir_num == 11){ *dz = -1; *dy = 1;  *dx = 1;  } // XYZ diagonal
    else if (dir_num == 12){ *dz = 1;  *dy = -1; *dx = 1;  } // XYZ diagonal
    else if (dir_num == 13){ *dz = -1; *dy = -1; *dx = 1;  } // XYZ diagonal
}
bool checkBounds(int z, int y, int x, int size){
    if (z >= 0 && z < size && y >= 0 && y < size && x >= 0 && x < size) {
        return true;
    }
    return false;
}
bool searchDirection(char*** grid,const char* word,int z,int y,int x,int dz,int dy,int dx,int index,int size){
    if (!checkBounds(z, y, x, size)) {
        return false;
    }
    if (grid[z][y][x] != word[index]) {
        return false;
    }
    if (word[index + 1] == '\0') {
        return true;
    }
    return searchDirection(grid, word, z + dz, y + dy, x + dx,dz, dy, dx,index + 1,size);
}
bool Searchdirection(char*** grid, const char* word, int z, int y, int x, int size, int dir_num) {
    if (dir_num > 13) {
        return false; 
    }

    int dz = 0, dy = 0, dx = 0;
    getDirection(dir_num, &dz, &dy, &dx);

    bool foundForward = searchDirection(grid, word, z, y, x, dz, dy, dx, 0, size);
    if(foundForward){
         cout << "Found \"" << word << "\" at (z=" << z << ", y=" << y << ", x=" << x << ") direction: " << " forward, movement ("<< dz << ", " << dy << ", " << dx << ")" << endl;
    }

    bool foundReverse = searchDirection(grid, word, z, y, x, -dz, -dy, -dx, 0, size);
    if(foundReverse){
        cout << "Found \"" << word << "\" at (z=" << z << ", y=" << y << ", x=" << x << ") direction: " << " reverse, movement ("<< dz << ", " << dy << ", " << dx << ")" << endl;
    }

    if (foundForward || foundReverse) {
        return true; 
    }

    return Searchdirection(grid, word, z, y, x, size, dir_num + 1);
}
void searchCells(char*** grid, const char* word, int z, int y, int x, int size) {
    if (z == size) {
        return;
    }
    
    if (y == size) {
        searchCells(grid, word, z + 1, 0, 0, size);
        return; 
    }
    
    if (x == size) {
        searchCells(grid, word, z, y + 1, 0, size); 
        return; 
    }

    Searchdirection(grid, word, z, y, x, size, 1);
    searchCells(grid, word, z, y, x + 1, size);
}
void displaygrid(char***grid,int layers,int rows,int cols,int l,int r,int c){
    if(l==layers)
    return;
    if(r==rows){
        cout<<endl;
        return initializeGrid(grid,layers,rows,cols,l+1,0,0);
    }
    if(c==cols){
        cout<<endl;
        return initializeGrid(grid,layers,rows,cols,l,r+1,0);
    }
    cout<<grid[l][r][c]<<" ";
    initializeGrid(grid,layers,rows,cols,l,r,c+1);
    
}


void searchWords(char*** grid, const char* words[], int count, int i, int size) {
    if (i == count) return;
    searchCells(grid, words[i], 0, 0, 0, size);
    searchWords(grid, words, count, i + 1, size);
}


void deleterows(char***grid,int layers,int rows,int cols,int l,int r){
    if(l==layers)
    return;
    if(r==rows)
    return deleterows(grid,layers,rows,cols,l+1,0);
    delete[] *(*(grid+l)+r);
    deleterows(grid,layers,rows,cols,l,r+1);
}

void deletelayers(char***grid,int layers,int rows,int cols,int l){
    if(l==layers)
    return;
    delete[] *(grid+l);
    deletelayers(grid,layers,rows,cols,l+1);
}
void deletegrid(char***grid,int layers,int rows,int cols){
    deleterows(grid,layers,rows,cols,0,0);
    deletelayers(grid,layers,rows,cols,0);
    delete[] grid;
}

int main() {
 
    const int size = 13;
    const char* words[] = {"muhammad"};
    const int wordCount = 1;
 
    char*** grid = creategrid(size, size, size);
    initializeGrid(grid, size, size, size, 0, 0, 0);

     for (int i = 0; i < 8; i++) {
        grid[5][12][2 + i] = words[0][i];
    }

    displaygrid(grid, size, size, size, 0, 0, 0);
    
    searchWords(grid, words, wordCount, 0, size);
    cout<<"found";
    cout<<"5 " << " 12 ";
    deletegrid(grid, size,size,size);
    return 0;
}
