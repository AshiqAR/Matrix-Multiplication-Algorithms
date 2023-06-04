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

void displayMatrix(vector<vector<int>> a){
    for(int i=0;i<a.size();i++){
        for(int j=0;j<a[i].size();j++){
            cout<<a[i][j]<<"\t";
        }
        cout <<endl;
    }
    cout <<endl;
}

vector<vector<int>> iterativeMultiplication(vector<vector<int>> a,vector<vector<int>> b){
    int r = a.size();
    int c = b[0].size();
    vector<vector<int>> v(r,vector<int>(c,0));
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            v[i][j] = 0;
            for(int k=0;k<a[0].size();k++){
                v[i][j] += a[i][k]*b[k][j];
            }
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

int main(){
    vector<vector<int>> a;
    vector<vector<int>> b;
    vector<vector<int>> c;

    cout<<"Enter the number of test cases: ";
    int t; cin>>t;
    int i,n;

    for(i=1;i<=t ;i++){
        cout<<"\n-------------------------------------- TEST CASE "<<i<<" --------------------------------------"<<endl;
        // n = (rand()%MAXn )+1;
        n = 400;
        cout << "n = "<<n<<endl;
        a = generateMatrix(n);
        b = generateMatrix(n);
        cout<<"Matrix A"<<endl;
        // displayMatrix(a);
        cout<<"Matrix B"<<endl;
        // displayMatrix(b);

        cout<<"Matrix C = A*B using itereative Multiplication"<<endl;
        auto start = chrono::steady_clock::now();
        c = iterativeMultiplication(a,b);
        auto end = chrono::steady_clock::now();
        auto elapsed_time = end-start;
        // displayMatrix(c);
        cout<<"Time Elapsed = " << chrono::duration<double, milli>(elapsed_time).count() << " ms" <<endl;
    }
    return 0;
}
