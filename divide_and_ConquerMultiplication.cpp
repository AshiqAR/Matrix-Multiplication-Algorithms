#include <iostream>
#include <vector>
#include <chrono>

using namespace std;
#define MAXn 80
#define MAXvalue 10

vector<vector<int>> generateMatrix(int n){
    vector<vector<int>> v(n,vector<int>(n,0));
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            v[i][j]=rand()%MAXvalue;
        }
    }
    return v;
}

vector<vector<int>> MSum(vector<vector<int>> a,vector<vector<int>> b){
    vector<vector<int>> c(a.size());
    for(int i=0;i<a.size();i++){
        for(int j=0;j<a[0].size();j++){
            c[i].push_back(a[i][j]+b[i][j]);
        }
    }
    return c;
}

void displayMatrix(vector<vector<int>> a){
    for(int i=0;i<a.size();i++){
        for(int j=0;j<a[i].size();j++){
            cout<<a[i][j]<<"\t";
        }
        cout <<endl;
    }
    cout <<endl;
}

vector<vector<int>> MMult(vector<vector<int>> &a,vector<vector<int>> &b){

    int a_row_size = a.size();
    int a_col_size = a[0].size();
    int b_row_size = b.size();
    int b_col_size = b[0].size();
    int i,j,k;
    if(a_col_size != b_row_size){
        cout << "Not compatible for Matrix Multiplication"<<endl;
        return a;
    }
    vector<vector<int>> c(a_row_size,vector<int>(b_col_size,0));
    if(a_row_size <= 2 || b_row_size <= 2){
        for(i=0;i<a_row_size;i++){
            for(j=0;j<b_col_size;j++){
                for(k=0;k<a_col_size;k++)
                    c[i][j] += a[i][k]*b[k][j];
            }
        }
        return c;
    }

    int mar = (a_row_size/2); 
    int mac = (a_col_size/2);
    int mbr = (b_row_size/2);
    int mbc = (b_col_size/2);

    vector<vector<int>> a1(mar);
    vector<vector<int>> a2(mar);
    vector<vector<int>> a3(a_row_size-mar);
    vector<vector<int>> a4(a_row_size-mar);
    vector<vector<int>> b1(mbr);
    vector<vector<int>> b2(mbr);
    vector<vector<int>> b3(b_row_size-mbr);
    vector<vector<int>> b4(b_row_size-mbr);


    for( i=0,k=0;i<mar;i++,k++){
        for(j=0;j<mac;j++)
            a1[k].push_back(a[i][j]);
        for(j=mac;j<a_col_size;j++)
            a2[k].push_back(a[i][j]);
    }
    for(i=mar,k=0;i<a_row_size;i++,k++){
        for(j=0;j<mac;j++)
            a3[k].push_back(a[i][j]);
        for(j=mac;j<a_col_size;j++)
            a4[k].push_back(a[i][j]);
    }

    for( i=0,k=0;i<mbr;i++,k++){
        for(j=0;j<mbc;j++)
            b1[k].push_back(b[i][j]);
        for(j=mbc;j<b_col_size;j++)
            b2[k].push_back(b[i][j]);
    }
    for(i=mbr,k=0;i<b_row_size;i++,k++){
        for(j=0;j<mbc;j++)
            b3[k].push_back(b[i][j]);
        for(j=mbc;j<b_col_size;j++)
            b4[k].push_back(b[i][j]);
    }
    
    vector<vector<int>> x1 = MSum(MMult(a1,b1),MMult(a2,b3));
    vector<vector<int>> x2 = MSum(MMult(a1,b2),MMult(a2,b4));
    vector<vector<int>> x3 = MSum(MMult(a3,b1),MMult(a4,b3));
    vector<vector<int>> x4 = MSum(MMult(a3,b2),MMult(a4,b4));

    for(i=0;i<mar;i++){
        for(j=0;j<mac;j++){
            c[i][j] = x1[i][j];
        }
        for(j=mac;j<b_col_size;j++){
            c[i][j] = x2[i][j-mac];
        }
    }
    for(i=mar;i<a_row_size;i++){
        for(j=0;j<mac;j++){
            c[i][j] = x3[i-mar][j];
        }
        for(j=mac;j<b_col_size;j++){
            c[i][j] = x4[i-mar][j-mac];
        }
    }
    return c;
}


int main(){
    vector<vector<int>> a;
    vector<vector<int>> b;
    vector<vector<int>> c;

    cout<<"Enter the number of test cases: ";
    int t; cin>>t;
    int i,n;

    for(i=1;i<=t ;i++){
        cout<<"\n-------------------------------------- TEST CASE "<<i<<" --------------------------------------"<<endl;
        // n = (rand()%MAXn)+1;
        n = 100;
        cout << "n = "<<n<<endl;
        a = generateMatrix(n);
        b = generateMatrix(n);
        cout<<"Matrix A"<<endl;
        displayMatrix(a);
        cout<<"Matrix B"<<endl;
        displayMatrix(b);

        auto start = chrono::steady_clock::now();
        cout<<"Matrix C = A*B using Divide and Conquer Multiplication"<<endl;
        c = MMult(a,b);
        auto end = chrono::steady_clock::now();
        auto elapsed_time = end-start;
        // displayMatrix(c);
        cout<<"Time Elapsed = " << chrono::duration<double, milli>(elapsed_time).count() << " ms" <<endl;

    }
    return 0;
}
