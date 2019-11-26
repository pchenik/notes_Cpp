#include <bits/stdc++.h>

using namespace std;

typedef long double ld;
const long double eps = 1e-9;

valarray<ld> operator * (valarray<ld> v, ld num) {
    v *= num;
    return v;
}

/*struct vvld{
    vvld () {}
    vvld& operator=(vvld u) {
        for(auto arr: v)
            v.push_back(arr);
        return
    }

    vector<valarray<ld> > v;
};*/

struct Table {


    Table () {}
    Table(const vector<valarray<ld> > &table) : num_n(table.back().size()), num_m(table.size()) {
        for(auto arr: table)
            A.push_back(arr);
    }

    void print() {
        for(auto arr: A) {
            for (auto x : arr)
                cout << x << " ";
            cout << endl;
        }
        cout << endl;
    }

    void SimplexMethod(valarray<ld> &ans) {

        //type code here
        vector<valarray<ld> > B = A; //operator "=" must be overloaded to keeps matrix A in previous form???
        int len_row = num_n + num_m;
        int accord[num_n] = {0};
        do
        {
            //Choosing a pivoting element
            int col = 0;

            for(int i = 0; i < len_row - 1; ++i)
                if (A[0][i] < A[0][col])
                    col = i;

            if (A[0][col] > 0) break;
            int row = 1;
            for(int i = 1; i < num_m; ++i)
                if (A[i][len_row - 1] / A[i][col] + eps < A[row][len_row - 1] / A[row][col])
                    row = i;

            accord[col] = row;


            //Straight-step of Gauss algorithm
            for(int i = 0; i < num_m; ++i)
                if (i == row) {
                    A[i] /= A[row][col];
                }
                else
                    A[i] -= A[row] * (A[i][col] / A[row][col]);

        } while(1);

        //Initialize ans vector
        for(int i = 0; i < num_n; ++i)
            ans[i] = A[accord[i]][len_row - 1];

        //recovery
        A = B;

    }

    void merge(const vector<valarray<ld> > &constraints, valarray<ld> &ans) {} //merge the supplementary constraints



    int num_n; // number of unknown variables
    int num_m; // number of constraints

    vector<valarray<ld> > A; //matrix are ready to be used by simplex_method
    //should be implemented as vvld which is inherited from vector<vallarray<ld> > probably??

};

vector<valarray<ld> > a;



struct user_data {

    user_data() {}

    //Conversion all the constraints to appropriate simplex table
    operator Table() const {
        vector<valarray<ld> > mesh;
        int szc = c.size();
        int sza = A.size();


        valarray<ld> cur(1 + szc + sza);

        for(int i = 0; i < szc; ++i)
            cur[i] = -c[i];
        mesh.push_back(cur);

        //cover all with zeros
        for(auto &x : cur) x = 0;

        for(int i = 0; i < sza; ++i)
        {
            for(int j = 0; j < szc; ++j)
                cur[j] = A[i][j];
            cur[szc + i] = 1;
            mesh.push_back(cur);
            for(auto &x : cur) x = 0;
        }

        for(int i = 0; i < b.size(); ++i)
            mesh[i + 1][szc + sza] = b[i];

        return Table(mesh);
    }

    vector<vector<int> > A; // <= constraints
    vector<int> b;// right part of equation
    vector<int> c;//objective function
};




int main()
{
    user_data samp;
    valarray<ld> b(2);
    //vector<ld> c = {2, 4};

    samp.A = {{1, 1}, {3, 5}, {2, 1}};
    samp.b = {22, 100, 32};
    samp.c = {5, 4};
    for (auto arr: samp.A) {
        for(auto x: arr)
            cout << x << " ";
        cout << endl;
    }
    cout << endl << endl;

    Table tab = Table(samp);
    tab.SimplexMethod(b);
    for(auto x: b)
        cout << x << " ";
    //tab.SimplexMethod();



    return 0;
}
/*valarray<ld> b(10);
    valarray<ld> c(10);
    vector<ld> e;
    for(auto &x: b)
        x += 1;
    for(auto &x: c)
        x += 2;
    for(auto x: c)
        cout << x << " ";
    cout << endl;
    //ld x = 1 / ld(3);
    c -= b * (1 / ld(3));
    a.push_back(b);
    a.push_back(c);
    for(auto arr: a)
    {
        for(auto x: arr)
            cout << x << " ";
        cout << endl;
    }*/
