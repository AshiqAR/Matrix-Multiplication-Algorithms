#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

using namespace std;
#define MAXn 128
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
    vector<vector<int>> v(a.size());
    for(int i=0;i<a.size();i++){
        for(int j=0;j<a[0].size();j++){
            v[i].push_back(a[i][j] + b[i][j]);
        }
    }
    return v;
}

vector<vector<int>> subMatrix(vector<vector<int>> a,vector<vector<int>> b){
    vector<vector<int>> v(a.size());
    for(int i=0;i<a.size();i++){
        for(int j=0;j<a[i].size();j++)
            v[i].push_back(a[i][j]-b[i][j]);
    }
    return v;
}

vector<vector<int>> strassen(vector<vector<int>> a,vector<vector<int>> b,int f = 0){
    int original_size = a.size();
    if(f==0){
        addZeroPadding(a);
        addZeroPadding(b);
    }

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

    vector<vector<int>> p1 = strassen(a1,subMatrix(b2,b4),1);
    vector<vector<int>> p2 = strassen(MSum(a1,a2),b4,1);
    vector<vector<int>> p3 = strassen(MSum(a3,a4),b1,1);
    vector<vector<int>> p4 = strassen(a4,subMatrix(b3,b1),1);
    vector<vector<int>> p5 = strassen(MSum(a1,a4),MSum(b1,b4),1);
    vector<vector<int>> p6 = strassen(subMatrix(a2,a4),MSum(b3,b4),1);
    vector<vector<int>> p7 = strassen(subMatrix(a1,a3),MSum(b1,b2),1);

    for(i=0;i<n/2;i++){
        for(j=0;j<n/2;j++){
            c[i][j] = p5[i][j] + p4[i][j] + p6[i][j] - p2[i][j];
        }
        for(j=n/2;j<original_size;j++){
            c[i][j] = p1[i][j-n/2] + p2[i][j-n/2];
        }
    }
    for(i=n/2;i<original_size;i++){
        for(j=0;j<n/2;j++){
            c[i][j] = p3[i-(n/2)][j] + p4[i-(n/2)][j];
        }
        for(j=n/2;j<original_size;j++){
            c[i][j] = p1[i-(n/2)][j-(n/2)] - p3[i-(n/2)][j-(n/2)] + p5[i-(n/2)][j-(n/2)] - p7[i-(n/2)][j-(n/2)];
        }
    }
    return c;
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

vector<vector<int>> MMult(vector<vector<int>> a,vector<vector<int>> b){

    int a_row_size = a.size();
    int a_col_size = a[0].size();
    int b_row_size = b.size();
    int b_col_size = b[0].size();
    int i,j,k;
    if(a_col_size != b_row_size){
        cout << "Not compatible for Matrix Multiplication"<<endl;
        return a;
    }

    if(a_row_size <= 2 || a_col_size <= 2 || b_col_size<=2){
        vector<vector<int>> v(a_row_size);
        int sum;
        for(i=0;i<a_row_size;i++){
            for(j=0;j<b_col_size;j++){
                sum = 0;
                for(k=0;k<a_col_size;k++)
                    sum += a[i][k]*b[k][j];
                v[i].push_back(sum);
            }
        }
        return v;
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
    vector<vector<int>> c(a_row_size,vector<int>(b_col_size,0));

    for(i=0;i<mar;i++){
        for(j=0;j<mbc;j++){
            c[i][j] = x1[i][j];
        }
        for(j=mbc;j<b_col_size;j++){
            c[i][j] = x2[i][j-mbc];
        }
    }
    for(i=mar;i<a_row_size;i++){
        for(j=0;j<mbc;j++){
            c[i][j] = x3[i-mar][j];
        }
        for(j=mbc;j<b_col_size;j++){
            c[i][j] = x4[i-mar][j-mbc];
        }
    }
    return c;
}


int main(){
    vector<vector<int>> a;
    vector<vector<int>> b;
    vector<vector<int>> c1;
    vector<vector<int>> c2;
    vector<vector<int>> c3;
    double avg_algos[1100][3];
    double standard_dev[1100][3];
    double repetition_iterative[15];
    double repetition_divide[15];
    double repetition_strassen[15];
    double iterative_time_avg;
    double divide_time_avg;
    double strassen_time_avg;

    int n1,n2;
    cout<<"Enter the lower limit of n: ";
    cin>>n1;
    while(n1>1000 || n1<=0){
        cout<<"n cannot be greater than 1000"<<endl;
        cout<<"Enter the lower limit of n: ";
        cin>>n1;
    }
    cout<<"Enter the upper limit of n: ";
    cin>>n2;
    while(n2>1000 || n1<=0){
        cout<<"n cannot be greater than 1000"<<endl;
        cout<<"Enter the upper limit of n: ";
        cin>>n2;
    }
    int r;
    cout<<"Enter the number of repetitions in each n: ";
    cin>>r;
    while(r>15 || r<=0){
        cout<<"Repetitions must be less than 15 !"<<endl;
        cout<<"Enter the number of repetitions in each n: ";
        cin>>r;
    }
    int i,j;

    for(i=n1;i<=n2;i++){
        iterative_time_avg = 0;
        divide_time_avg = 0;
        strassen_time_avg = 0;
        for(j=0;j<r;j++){
            cout<<"----------------------------------------------------- TEST CASE "<<j+1<<"-----------------------------------------------------\n"<<endl;
            cout<<"n = "<<i<<endl;
            a = generateMatrix(i);
            b = generateMatrix(i);
            if(j==0){
                cout<<"Matrix A"<<endl;
                displayMatrix(a);
                cout<<"Matrix B"<<endl;
                displayMatrix(b);
            }
            

            auto start = chrono::steady_clock::now();
            c1 = iterativeMultiplication(a,b);
            auto end = chrono::steady_clock::now();
            auto elapsed_time = end-start;
            if(j==0){
                cout<<"Matrix C = A*B using iterative Matrix Multiplication"<<endl; 
                displayMatrix(c1);
            }
            repetition_iterative[j] = chrono::duration<double, milli>(elapsed_time).count();
            iterative_time_avg += repetition_iterative[j];
            cout<<"Time Elapsed in iterative = " << repetition_iterative[j] << " ms" <<endl;


            start = chrono::steady_clock::now();
            c2 = MMult(a,b);
            end = chrono::steady_clock::now();
            elapsed_time = end-start;
            if(j==0){
                cout<<"Matrix C = A*B using divide and conquer Matrix Multiplication"<<endl;
                displayMatrix(c2);
            }
            repetition_divide[j] = chrono::duration<double, milli>(elapsed_time).count();
            divide_time_avg += repetition_divide[j];
            cout<<"Time Elapsed in divide and conquer = " << repetition_divide[j] << " ms" <<endl;
            

            start = chrono::steady_clock::now();
            c3 = strassen(a,b);
            end = chrono::steady_clock::now();
            elapsed_time = end-start;
            if(j==0){
                cout<<"Matrix C = A*B using Strassens Matrix Multiplication"<<endl;
                displayMatrix(c3);
            }
            repetition_strassen[j] = chrono::duration<double, milli>(elapsed_time).count();
            strassen_time_avg += repetition_strassen[j];
            cout<<"Time Elapsed in strassen = " << repetition_strassen[j] << " ms" <<endl;
        }
        iterative_time_avg = iterative_time_avg/(r);
        divide_time_avg = divide_time_avg/(r);
        strassen_time_avg = strassen_time_avg/(r);

        avg_algos[i][0] = iterative_time_avg;
        avg_algos[i][1] = divide_time_avg;
        avg_algos[i][2] = strassen_time_avg;

        standard_dev[i][0] = 0;
        standard_dev[i][1] = 0;
        standard_dev[i][2] = 0;

        for(j=0;j<r;j++){
            standard_dev[i][0] += (repetition_iterative[j]-iterative_time_avg)*(repetition_iterative[j]-iterative_time_avg);
            standard_dev[i][1] += (repetition_divide[j]-divide_time_avg)*(repetition_divide[j]-divide_time_avg);
            standard_dev[i][2] += (repetition_strassen[j]-strassen_time_avg)*(repetition_strassen[j]-strassen_time_avg);
        }
        standard_dev[i][0] = sqrt(standard_dev[i][0]/(r));
        standard_dev[i][0] = sqrt(standard_dev[i][1]/(r));
        standard_dev[i][0] = sqrt(standard_dev[i][1]/(r));
        cout<<"\n"<<endl;
    }
    printf("\t   Iterative Multiplication \t\t\t Divide and Conquer \t\t\tStrassen Algorithm\n");
    printf("n    \taverage_runTime\t   standard_dev \taverage_runTime\t   standard_dev \taverage_runTime\t   standard_dev\n");
    for(i=n1;i<=n2;i++){
        printf("%5d\t%15.7lf\t%15.7lf \t%15.7lf\t%15.7lf \t%15.7lf\t%15.7lf\n",i,avg_algos[i][0],standard_dev[i][0],avg_algos[i][1],standard_dev[i][1],avg_algos[i][2],standard_dev[i][2]);
    }



    return 0;
}
