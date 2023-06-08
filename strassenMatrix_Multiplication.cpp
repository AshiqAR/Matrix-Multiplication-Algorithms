#include <iostream>
#include <vector>
#include <cmath>
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

void displayMatrix(vector<vector<int>> a){
    for(int i=0;i<a.size();i++){
        for(int j=0;j<a[i].size();j++){
            cout<<a[i][j]<<"\t";
        }
        cout <<endl;
    }
    cout <<endl;
}


void addZeroPadding(vector<vector<int>> &a){
    int n = pow(2, ceil(log2(a.size())));
    int i;
    for(i=0;i<a.size();i++){
        for(int j=a[i].size();j<n;j++)
            a[i].push_back(0);
    }
    for(i=a.size();i<n;i++){
        a.push_back(vector <int> (n,0));
    }
}

vector<vector<int>> MSum(vector<vector<int>> a,vector<vector<int>> b){
    for(int i=0;i<a.size();i++){
        for(int j=0;j<a[0].size();j++){
            a[i][j] = a[i][j] + b[i][j];
        }
    }
    return a;
}

vector<vector<int>> subMatrix(vector<vector<int>> a,vector<vector<int>> b){
    for(int i=0;i<a.size();i++){
        for(int j=0;j<a[i].size();j++)
            a[i][j] = a[i][j]-b[i][j];
    }
    return a;
}

vector<vector<int>> strassen(vector<vector<int>> a,vector<vector<int>> b){
    int original_size = a.size();
    addZeroPadding(a);
    addZeroPadding(b);
    int n = a.size(),i,j,k;
    vector<vector<int>> c(original_size,vector<int>(original_size,0));
    if(n <= 2){
        for(i=0;i<original_size;i++){
            for(j=0;j<original_size;j++){
                for(k=0;k<original_size;k++)
                    c[i][j] += a[i][k]*b[k][j];
            }
        }
        return c;
    }

    vector<vector<int>> a1(n/2);
    vector<vector<int>> a2(n/2);
    vector<vector<int>> a3(n/2);
    vector<vector<int>> a4(n/2);
    vector<vector<int>> b1(n/2);
    vector<vector<int>> b2(n/2);
    vector<vector<int>> b3(n/2);
    vector<vector<int>> b4(n/2);

    for(i=0,k=0;i<n/2;i++,k++){
        for(j=0;j<n/2;j++){
            a1[k].push_back(a[i][j]);
            b1[k].push_back(b[i][j]);
        }
        for(;j<n;j++){
            a2[k].push_back(a[i][j]);
            b2[k].push_back(b[i][j]);
        }
    }
    for(k=0;i<n;i++,k++){
        for(j=0;j<n/2;j++){
            a3[k].push_back(a[i][j]);
            b3[k].push_back(b[i][j]);
        }
        for(;j<n;j++){
            a4[k].push_back(a[i][j]);
            b4[k].push_back(b[i][j]);
        }
    }

    vector<vector<int>> p1 = strassen(a1,subMatrix(b2,b4));
    vector<vector<int>> p2 = strassen(MSum(a1,a2),b4);
    vector<vector<int>> p3 = strassen(MSum(a3,a4),b1);
    vector<vector<int>> p4 = strassen(a4,subMatrix(b3,b1));
    vector<vector<int>> p5 = strassen(MSum(a1,a4),MSum(b1,b4));
    vector<vector<int>> p6 = strassen(subMatrix(a2,a4),MSum(b3,b4));
    vector<vector<int>> p7 = strassen(subMatrix(a1,a3),MSum(b1,b2));

    vector<vector<int>> x1 = MSum(MSum(p5,p4),subMatrix(p6,p2));
    vector<vector<int>> x2 = MSum(p1,p2);
    vector<vector<int>> x3 = MSum(p3,p4);
    vector<vector<int>> x4 = MSum(subMatrix(p1,p3),subMatrix(p5,p7));

    for(i=0;i<n/2;i++){
        for(j=0;j<n/2;j++)
            c[i][j] = x1[i][j];
        for(j=n/2;j<original_size;j++)
            c[i][j] = x2[i][j-n/2];
    }
    for(i=n/2;i<original_size;i++){
        for(j=0;j<n/2;j++)
            c[i][j] = x3[i-(n/2)][j];
        for(j=n/2;j<original_size;j++)
            c[i][j] = x4[i-(n/2)][j-(n/2)];
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
        n = 19;
        cout << "n = "<<n<<endl;
        a = generateMatrix(n);
        b = generateMatrix(n);
        cout<<"Matrix A"<<endl;
        displayMatrix(a);
        cout<<"Matrix B"<<endl;
        displayMatrix(b);

        auto start = chrono::steady_clock::now();
        cout<<"Matrix C = A*B using Strassens Matrix Multiplication"<<endl;
        c = strassen(a,b);
        auto end = chrono::steady_clock::now();
        auto elapsed_time = end-start;
        displayMatrix(c);
        cout<<"Time Elapsed = " << chrono::duration<double, milli>(elapsed_time).count() << " ms" <<endl;

    }
    return 0;
}
