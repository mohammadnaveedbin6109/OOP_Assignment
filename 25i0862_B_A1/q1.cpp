#include<iostream>
using namespace std;
int** createGrid(int rows, int cols){
    int**grid=new int*[rows];
    for(int i=0;i<rows;i++){
        *(grid+i)=new int[cols];
    }
    return grid;
}

void inputGrid(int** grid, int rows, int cols){
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            cin>>*(*(grid+i)+j);
            while(*(*(grid+i)+j)<0  ||  *(*(grid+i)+j)>4){
                cin>>*(*(grid+i)+j);
            }
        }
    }
}

void displayGrid(int** grid, int rows, int cols){
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            cout<<*(*(grid+i)+j)<<" ";
        }
        cout<<endl;
    }
}

int countCells(int** grid, int rows, int cols, int value){
    int count=0;
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(*(*(grid+i)+j)==value)
            count++;
        }
    }
    return count;
}

void findFirstCell(int** grid, int rows, int cols, int value, int& row, int& col){
    row=-1;
    col=-1;
    bool found=false;
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(*(*(grid+i)+j)==4){
                row=i;
                col=j;
                found=true;
                break;
            }
        }
        if(found)
        break;
    }
    if(row == -1 || col == -1)
    cout << "Response team could not be located.\n";
}

bool isValidMove(int** grid, int** visited, int rows, int cols, int r, int c){
    if (r < 0 || r >= rows || c < 0 || c >= cols)
    return false;
   else if(*(*(grid+r)+c) == 1 )
    return false;
   else if(*(*(visited+r)+c) == 1 )
    return false;
   else return true;
}

int countReachable(int** grid, int** visited, int rows, int cols, int r, int c){
    int count=1;
    *(*(visited+r)+c)=1;
    if(isValidMove(grid,visited,rows,cols,r-1,c)){
      count+=  countReachable(grid,visited,rows,cols,r-1,c);
    }
    if(isValidMove(grid,visited,rows,cols,r+1,c)){
       count+= countReachable(grid,visited,rows,cols,r+1,c);
    }
    if(isValidMove(grid,visited,rows,cols,r,c+1)){
       count+= countReachable(grid,visited,rows,cols,r,c+1);
    }
    if(isValidMove(grid,visited,rows,cols,r,c-1)){
       count+= countReachable(grid,visited,rows,cols,r,c-1);
    }

    return count;
}

int countAccessibleVictims(int** grid, int** visited, int rows, int cols){
    int count=0;
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(*(*(grid+i)+j)==2  &&  *(*(visited+i)+j)== 1  )
            count++;
        }
    }
    return count;
}

int countIsolatedVictims(int** grid, int** visited, int rows, int cols){
    int count=0;
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(*(*(grid+i)+j) == 2 && *(*(visited+i)+j) == 0){
                bool nbr = false;
                if(i-1 >= 0 && *(*(visited+(i-1))+j) == 1)
                nbr = true;
                if(i+1 < rows && *(*(visited+(i+1))+j) == 1)
                nbr = true;
                if(j-1 >= 0 && *(*(visited+i)+(j-1)) == 1)
                nbr = true;
                if(j+1 < cols && *(*(visited+i)+(j+1)) == 1)
                nbr = true;

                if(!nbr)
                    count++;

            }

        }
    }
    return count;
}

void analyzeSupplyPoints(int** grid, int** visited, int rows, int cols){
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(*(*(grid+i)+j) == 3){
                cout<<"supply point is at "<<i<<" row and "<<j<<" column\n";
                if(*(*(visited+i)+j) == 1)
                    cout << "reachable\n";
                else
                    cout << "not reachable\n";
            }
        }
    }
}

int** createVictimReport(int** grid, int** visited, int rows, int cols, int& count){
    count=countAccessibleVictims(grid,visited,rows,cols);
    int **vreport=new int*[count];
    for(int i=0;i<count;i++)
    *(vreport+i)=new int[2];
    int r=0;
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(*(*(grid+i)+j)==2  &&  *(*(visited+i)+j)== 1  ){
                vreport[r][0]=i;
                vreport[r][1]=j;
                r++;
            }
        }
    }
    return vreport;
}

int** createEvacuationGrid(int** grid, int** visited, int rows, int cols){
    int**evgrade=createGrid(rows,cols);
    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(*(*(visited+i)+j) ==1 && *(*(grid+i)+j)==0 )
            *(*(evgrade+i)+j)=5;
           else if(*(*(visited+i)+j) ==1 && *(*(grid+i)+j)==2 )
            *(*(evgrade+i)+j)=6;
            else
            *(*(evgrade+i)+j)=*(*(grid+i)+j);
        }
    }
    return evgrade;
}

int largestConnectedRegion(int** grid, int rows, int cols){
    int** check = createGrid(rows, cols);
    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            *(*(check+i)+j) = 0;

    int max = 0;

    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            if(*(*(grid+i)+j) != 1 && *(*(check+i)+j) == 0){
                int size = countReachable(grid, check, rows, cols, i, j);
                if(size > max)
                    max = size;
            }
        }
    }

    for(int i=0;i<rows;i++)
        delete[] check[i];
    delete[] check;

    return max;
}

int main(){
    int rows, cols;
    cout << "Enter rows and columns: ";
    cin >> rows >> cols;

    int** grid = createGrid(rows, cols);
    int** visited = createGrid(rows, cols);

    for(int i=0;i<rows;i++){
        for(int j=0;j<cols;j++){
            *(*(grid+i)+j) = 0;
            *(*(visited+i)+j) = 0;   // FIX: visited must be zeroed too
        }
    }

    inputGrid(grid, rows, cols);

    int r=0, c=0;
    findFirstCell(grid, rows, cols, 4, r, c);

    int reachableCount = 0;
    if(r != -1){                     // FIX: guard against missing team
        reachableCount = countReachable(grid, visited, rows, cols, r, c);
    }

    // Gather all report stats
    int totalCells   = rows * cols;
    int emptyCells   = countCells(grid, rows, cols, 0);
    int blockedCells = countCells(grid, rows, cols, 1);
    int victims      = countCells(grid, rows, cols, 2);
    int supplyPts    = countCells(grid, rows, cols, 3);
    int teams        = countCells(grid, rows, cols, 4);
    int accessible   = countAccessibleVictims(grid, visited, rows, cols);
    int inaccessible = victims - accessible;
    int isolated     = countIsolatedVictims(grid, visited, rows, cols);

    int reachableSupply = 0;
    for(int i=0;i<rows;i++)
        for(int j=0;j<cols;j++)
            if(*(*(grid+i)+j)==3 && *(*(visited+i)+j)==1)
                reachableSupply++;

    int largestRegion = largestConnectedRegion(grid, rows, cols);

    int vcount = 0;
    int** vreport = createVictimReport(grid, visited, rows, cols, vcount);
    int** evacGrid = createEvacuationGrid(grid, visited, rows, cols);

    // ---- PART N: FINAL EMERGENCY REPORT ----
    for(int i=0;i<45;i++) cout << "=";
    cout << "\n  EMERGENCY RESPONSE REPORT\n";
    for(int i=0;i<45;i++) cout << "=";
    cout << endl;

    cout << "Grid Size            : " << rows << " x " << cols << endl;
    cout << "Total Cells          : " << totalCells << endl;
    cout << "Empty Areas          : " << emptyCells << endl;
    cout << "Blocked Areas        : " << blockedCells << endl;
    cout << "Victims              : " << victims << endl;
    cout << "Supply Points        : " << supplyPts << endl;
    cout << "Emergency Teams      : " << teams << endl;
    cout << "Reachable Cells      : " << reachableCount << endl;
    cout << "Accessible Victims   : " << accessible << endl;
    cout << "Inaccessible Victims : " << inaccessible << endl;
    cout << "Isolated Victims     : " << isolated << endl;
    cout << "Reachable Supply Pts : " << reachableSupply << endl;
    cout << "Largest Conn. Region : " << largestRegion << endl;

    for(int i=0;i<45;i++) cout << "=";
    cout << endl;

    cout << "\nEvacuation Grid:\n";
    displayGrid(evacGrid, rows, cols);

    //deleting
    for(int i=0;i<rows;i++){
        delete[] *(visited+i);
        delete[] *(grid+i);
    }
    delete[] visited;
    delete[] grid;


}