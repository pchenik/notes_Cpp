#include <bits/stdc++.h>

using namespace std;

typedef long double ld;
typedef long long ll;
const long double eps = 1e-9;

struct UserData {

    UserData() {}

    UserData(const vector<vector<int> > &A, const vector<int> &b, const vector<int> &c) :
        A(A), b(b), c(c) {}

    ~UserData() {}

    void print_constr() {
        cout << "print constraints" << endl;
        for (auto const &arr: A) {
            for(auto const &x: arr)
                cout << x << " ";
            cout << endl;
        }
    }

    int object_function(const vector<int> &a) {
        int res = 0;
        for(int i = 0; i < c.size(); ++i)
            res += c[i] * a[i];
        return res;
    }

    //get VC
    int get_VCi(int i, const vector<int> &a) {
        int res = 0;
        for (int j = 0; j < c.size(); ++j)
            res += A[i][j] * a[j];
        return (res > b[i]) ? res - b[i] : 0;
    }

    //fitness
    int fitness_value(const vector<int> &a) {
        //let MC = 20;
        int MC = 20;
        int res = object_function(a);
        int sum = 0;
        for (int i = 0; i < A.size(); ++i)
            sum += get_VCi(i, a);
        sum *= MC;
        return (res > sum) ? res - sum : 0;
    }

    vector<vector<int> > A; // <= constraints
    vector<int> b;// right part of equation
    vector<int> c;//objective function
};

const int NX = 100, INF = 1e9 + 7;
int max_xi[NX];
UserData kek;

bool cmp(const vector<int> &a, const vector<int> &b) {
    return kek.fitness_value(a) > kek.fitness_value(b);
}

int main()
{

    kek.A = {{20, 10}, {12, 7}};
    kek.b = {75, 55};
    kek.c = {1, 1};

    //Create an initial population P1 = {xi: xi º Rn, i=1... I1}.
    int N = 600;
    int K = 10000;
    vector<vector<int> > Population;
    //initialize number of the unknown variables
    int n = kek.c.size();
    for (int i = 0; i < NX; ++i) max_xi[i] = INF;

    //print max_xi before
    cout << "max_xi" << endl;
    for(int i = 0; i < n; ++i)
        cout << max_xi[i] << " ";
    cout << endl << endl;

    for(int i = 0; i < kek.A.size(); ++i)
        for(int j = 0; j < n; ++j)
            if (kek.A[i][j] > 0)
                max_xi[j] = min(max_xi[j], kek.b[i] / kek.A[i][j]);

    //change max_xi where max_xi == 0
    for(int i = 0; i < n; ++i)
        if (max_xi[i] == INF)
            max_xi[i] = 0;
        else
            max_xi[i] <<= 1;

    //fill with (max_x0, 0, 0, 0...), (0, 0, 0, max_x3)

    vector<int> cur(n);
    for(int i = 0; i < n; ++i) {
        cur[i] = max_xi[i];
        Population.emplace_back(cur);
        cur[i] = 0;
    }


    //print max_xi after
    for(int i = 0; i < n; ++i)
        cout << max_xi[i] << " ";
    cout << endl << endl;

    srand(time(NULL));
    for (int i = 0; i < N - n; ++i) {
        for(int j = 0; j < n; ++j)
            cur[j] = rand() % (max_xi[j] + 1);
        Population.emplace_back(cur);
        for(auto &x : cur) x = 0;
    }

    //print populations
    /*for (auto &arr: Population) {
        for (auto x: arr)
            cout << x << " ";
        cout << endl;
    }*/

    //prev
    vector<vector<int> > Prev;
    vector<int> ans(n);
    //for crossover
    vector<int> cur1(n);
    vector<int> cur2(n);

    for (int i = 0; i < N; ++i) {
        //crossover
        for (int j = 0; j < N / 2; ++j) {
            int i1 = rand() % N;
            int i2 = rand() % N;
            for(int k = 0; k < n / 2; ++k) {
                cur1[k] = Population[i1][k];
                cur2[k] = Population[i1][k] ^ Population[i2][k];
            }
            for(int k = n / 2; k < n; ++k) {
                cur2[k] = Population[i1][k];
                cur1[k] = Population[i1][k] ^ Population[i2][k];
            }
            Population.emplace_back(cur1);
            Population.emplace_back(cur2);
        }
        //mutation
        for(int j = 0; j < N / 5; ++j) {
            int ind = rand() % N;
            for (auto &x: Population[ind]) {
                //calc bit_num
                int bit, lg = 0;
                int y = x;
                while (y > 0) {lg++; y >>= 1;}
                if (!lg)
                    x = rand() % 2;
                else {
                    bit = rand() % (lg);
                    x = x ^ (1 << bit);
                }
            }

        }

        //get the best X
        //vector<int> ans_per_population(n);
        for(int j = 0; j < N; ++j)
            if (kek.fitness_value(ans) < kek.fitness_value(Population[j]) || i == 0)
                ans = Population[j];


        Prev = Population;
        Population.clear();
        sort(Prev.begin(), Prev.end(), cmp);
        for(int j = 0; j < N; ++j)
            Population.emplace_back(Prev[i]);
        Prev.clear();
        cout << "i = " << i << endl;

    }

    //Comparison two fitness value
    cout << "fit1 " << kek.fitness_value({0, 4, 12, 0}) << endl;
    cout << "fit2 " << kek.fitness_value(ans) << endl;


    cout << "X is ";
    for (auto &x: ans)
        cout << x << " ";
    cout << endl;

    cout << "integer solution is ";
    cout << kek.object_function(ans);

    return 0;
}




/*kek.A = {{2, 3, 3}, {4, 1, 2}, {5, 6, 1}};
    kek.c = {1, 1, 1};
    kek.b = {78, 89, 66};*/


/*kek.A = {{1, 7, 1, 7}, {3, -1, 1, 2}, {2, 3, -1, 1}};
    kek.b = {46, 8, 10};
    kek.c = {2, 6, 3, -2};*/
