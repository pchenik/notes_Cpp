#include <iostream>
#include <vector>
#include <valarray>
#include <set>
#include <thread>
#include <mutex>

using namespace std;

typedef long double ld;
typedef long long ll;
const long double eps = 1e-9;

struct Vertex {

    Vertex() {}
    Vertex(const valarray<ld> &arr, ld val) : ans(arr), maxi(val) {}
    Vertex(const vector<pair<int, int> > &v, size_t len) : add_constr(v), ans(len) {}
    void print_constr() {
        cout << "print added constraints" << endl;
        for(auto const &x: add_constr)
            cout << x.first << " " << x.second << " // ";
        cout << endl;
    }
    void print_ans() {
        cout << "print ans" << endl;
        for (auto const &x: ans)
            cout << x << " ";
        cout << endl;
    }
    explicit Vertex(ld maxi) : maxi(maxi) {}//educational
    valarray<ld> ans;
    vector<pair<int, int> > add_constr; //?How should it be implemented
    ld maxi;
};

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

struct UserData;

struct Table {

    Table () : num_m(0), num_n(0), constraints(nullptr) {}

    void print_matrix() {
        cout << "print simplex table:" << endl;
        for(auto const &arr: A) {
            for (auto x : arr)
                cout << x << " ";
            cout << endl;
        }
        cout << endl;
    }

    ~Table() {
        delete constraints;
        //for(auto &arr: A)
        //    arr.~valarray<ld>();
        A.clear();
    }

    void SimplexMethod(valarray<ld> &ans) {

        //type code here
        vector<valarray<ld> > B = A; //operator "=" must be overloaded to keep matrix A in previous form???
        int len_row = num_n + num_m;
        //valarray<int> accord(num_n + num_m);
        vector<int> accord(num_m);
        for(int j = num_n; j < len_row - 1; ++j)
            accord[j - num_n] = j;
        //check
        int k = 0;
        //cout << "m = " << num_m << endl;
        //end
        do
        {
            //k++;
            //if (k > 15)
            //    break;
            //Choosing a pivoting element
            int col = 0;

            for(int i = 0; i < len_row - 1; ++i)
                if (A[0][i] < A[0][col] && A[0][i] < eps)
                    col = i;

            //temp
            //cout << "temp" << endl;
            //if (k > 10)
            //    for(int i = 0; i < len_row - 1; ++i)
            //        cout << A[0][i] << " ";
            ////cout << endl;
            if (A[0][col] + eps >= 0) break;
            int row = -1;
            for(int i = 1; i < num_m; ++i)
                if (row != -1 && (A[i][len_row - 1] / abs(A[i][col]) < A[row][len_row - 1] / abs(A[row][col]) + eps) && A[i][col] > eps)
                    row = i;
                else
                if (row == -1 && A[i][col] + eps > 0)
                    row = i;

            accord[row - 1] = col;


            //Straight-step of Gauss algorithm
            for(int i = 0; i < num_m; ++i)
                if (i != row)
                    A[i] -= A[row] * (A[i][col] / A[row][col]);
            A[row] *= 1 / A[row][col];

            //print_matrix();

        } while(true);

        /*cout << "accord before" << endl;
        for(auto const &x: accord)
            cout << x << " ";
        cout << endl;*/
        //Initialize ans vector
        for(int i = 0; i < num_m - 1; ++i)
            if (accord[i] < num_n)
                ans[accord[i]] = A[i + 1][len_row - 1];
        /*cout << "ans" << endl;
        for(auto const &x: ans)
            cout << double(x) << " ";
        cout << endl;*/
        //recovery
        //accord.clear();
        A = B;
        //print_matrix();

    }

    Table& operator=(const Table &tab);
    Table(const vector<valarray<ld> > &table, const UserData *giving_constr, bool is_proper_b = true);
    void merge(const Vertex &v);
    bool is_proper_b = true;
    int num_n; // number of unknown variables
    int num_m; // number of constraints
    //static int accord[]; // in perspective
    vector<valarray<ld> > A; //matrix are ready to be used by simplex_method
    //should be implemented as vvld which is inherited from vector<vallarray<ld> > probably??//have to be a separate class
    UserData *constraints; //it's supposed to be here, but too mush efforts//ok it doesn't work properly otherwise, i should try

};

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

        //is_proper_b check
        bool ok = true;
        for(auto const &x: b)
            if (x <= 0) {
                ok = false;
                break;
            }

        return Table(mesh, this, ok);
    }

    vector<vector<int> > A; // <= constraints
    vector<int> b;// right part of equation
    vector<int> c;//objective function
};

//Constructor
//add UserData *constraints
Table::Table(const vector<valarray<ld> > &table, const UserData *giving_constr, bool is_proper_b) :
        num_n(table.back().size() - table.size()), num_m(table.size()), is_proper_b(is_proper_b) {
    for(auto const &arr: table)
        A.emplace_back(arr);
    constraints = new UserData(*giving_constr);
}

//
Table& Table::operator=(const Table &tab) {//overload the operator new??
    if (constraints != tab.constraints) {
        num_n = tab.num_n;
        num_m = tab.num_m;
        if (constraints != nullptr)//we shouldn't call destructor of Table directly, because of how valarray's arranged its memory allocation
            delete constraints;
        //print_matrix();
        A = tab.A;//memory leak because of absence of a vector<valarray<ld> > class and its destructor respectively//nope, i was wrong//or not
        //int x = reinterpret_cast<int>(&(this->constraints));
        //int y = reinterpret_cast<int>(&(tab.constraints));
        this->constraints = new UserData(*tab.constraints);//check this out
    }
    return *this;
}

void Table::merge(const Vertex &v) {//merge the supplementary constraints
    vector<int> cur(num_n);

    //constraints->print_constr();
    vector<vector<int> > fork_A(constraints->A);//here!!!
    vector<int> fork_b(constraints->b);
    vector<int> fork_c(constraints->c);


    //add "x <= " constraints in merging into table
    for(auto const &pair: v.add_constr)
        if (pair.second >= 0) {
            cur[pair.first] = 1;
            fork_A.emplace_back(cur);
            fork_b.emplace_back(pair.second);
            cur[pair.first] = 0;
            num_m++;
        }

    //create another one "constraints" to distinguish different Table//worked???
    UserData *new_constraints = new UserData(fork_A, fork_b, fork_c);//??
    delete constraints;
    constraints = new_constraints;

    //change basis in ">=" cases and update constraints to convert to the table
    for(auto const &x: v.add_constr) {
        int temp_second = x.second;
        //cout << x.first << " " << x.second <<
        if (x.second < 0) {
            for(int i = 0; i < constraints->b.size(); ++i) {
                constraints->b[i] -= constraints->A[i][x.first] * (-x.second);
                int xx = constraints->b[i];
                if (constraints->b[i] <= 0) {
                    is_proper_b = false;
                    return;
                }
            }
        }
    }

    A.clear();

    /*cout << "constraints->b" << endl;
    for(int i = 0; i < constraints->b.size(); ++i)
        cout << constraints->b[i] << "/ ";
    cout << endl;
    cout << "new table" << endl;*/

    Table new_table = Table(*constraints);//operator "=" must be overloaded!


    A = new_table.A;
    //print_matrix();
}

//comparator(functor) for set
bool operator<(const Vertex &V1, const Vertex &V2) {
    return V1.maxi > V2.maxi;
}

class LinealProblem {

    Table table; //it's been obtained from UserData in main.cpp
    //UserData constraints; //has to be settled down in Table, but who cares
    ld inf = 0;// miminum of max
    set<Vertex> pool; // the leafs of the current bypass tree
    vector<int> func; //objective function
    bool find; // Non-const static data must be initialized out of line
    mutex mtx;//mutex

public:

    LinealProblem() {}

    LinealProblem(const Table &T, const vector<int> &f) : table(T.A, T.constraints, T.is_proper_b), func(f) {}

    ld getter() const {
        return inf;
    }

    inline bool IsInt(ld x)  {return abs(x - static_cast<ll>(x)) < eps; }

    ld getter(const valarray<ld> &v) {
        ld res = 0;
        for (int i = 0; i < v.size(); ++i)
            res += ld(func[i]) * v[i];
        return res;
    }

    void solve(const Vertex &v, valarray<ld> &ans) { // v.ans must be declared

        pool.insert(v);

        //
        Table T1;
        Table T2;

        T1 = table;
        T2 = table;

        while (!pool.empty()) {

            Vertex cur = *pool.begin();
            pool.erase(pool.begin());
            if (cur.maxi - eps <= inf + 1)
                continue;

            //Check out on integer values
            //cout << "ans for cur is " << endl;
            //for (auto x: cur.ans) cout << x << " ";
            //cout << endl;

            bool ok = true;
            for (auto x: cur.ans)
                if (!IsInt(x)) {
                    ok = false;
                    break;
                }

            if (ok) {
                inf = cur.maxi;
                ans = cur.ans;
                if (cur.maxi > pool.begin()->maxi) {
                    find = true;
                    return;
                }
                else
                    continue;
            }

            //Initialize constraints for branching ans
            pair<int, int> val1;
            pair<int, int> val2;
            for(int i = 0; i < cur.ans.size(); ++i)
                if (!IsInt(cur.ans[i])) {
                    val1 = {i, static_cast<ll>(cur.ans[i])}; //here fixed?!?!
                    //cout << "between 0 and 1 " << cur.ans[i] << "/" << endl << endl;
                    val2 = {i, -static_cast<ll>(cur.ans[i]) - 1};
                    break;
                }

            //Initialize two adjacent Vertex
            vector<pair<int, int> > cur_constr(cur.add_constr);

            cur_constr.emplace_back(val1);
            Vertex V1(cur_constr, ans.size());
            //V1.print_constr();

            cur_constr.pop_back();
            /*cout << "cur_constr" << endl;
            for(auto x: cur_constr)
                cout << x.first << " " << x.second << endl;
            cout << endl;*/

            cur_constr.emplace_back(val2);
            Vertex V2(cur_constr, ans.size());
            //V2.print_constr();
            cur_constr.clear();

            //Launch of two simplex method and proceed pushing in pool
            //T1.merge(V1);//merging table constraints and vertex constraints


            auto Push = [this](Table &T, Vertex &V) mutable -> void {
                T.merge(V);
                if (!T.is_proper_b)
                    return;
                T.SimplexMethod(V.ans);

                //recovery
                for(auto const &pair: V.add_constr)
                    if (pair.second < 0)
                        V.ans[pair.first] += (-pair.second);

                //get a new value

                ld z = this->getter(V.ans);
                if (z + eps >= this->inf + 1) {
                    V.maxi = z;
                    std::lock_guard<std::mutex> lck(this->mtx);
                    this->pool.insert(V);
                }
            };

            thread t1(Push, std::ref(T1), std::ref(V1));
            thread t2(Push, std::ref(T2), std::ref(V2));
            t1.join();
            t2.join();

            T1 = table;
            T2 = table;

        }

    }

};


int main()
{

    UserData samp;
    valarray<ld> b(2);

    /*samp.A = {{1, 1}, {3, 5}, {2, 1}};
    samp.b = {22, 100, 32};
    samp.c = {5, 4};
    for (auto arr: samp.A) {
        for(auto x: arr)
            cout << x << " ";
        cout << endl;
    }
    cout << endl;

    Table tab = Table(samp);
    tab.print_matrix();
    tab.SimplexMethod(b);

    cout << endl;
    for(auto x: b)
        cout << x << " ";
    cout << endl;
    set<Vertex> q;
    q.insert(Vertex(5));
    q.insert(Vertex(6));
    q.insert(Vertex(7));
    for(auto it: q)
        cout << it.maxi << " ";
    cout << endl;
    cout << q.begin()->maxi << endl;
    cout << endl;
    //tab.SimplexMethod();*/

    UserData kek;
    kek.A = {{20, 10}, {12, 7}};
    kek.b = {75, 55};
    kek.c = {1, 1};


    Table table = Table(kek);
    LinealProblem Task(table, kek.c);
    valarray<ld> ans(2);
    table.print_matrix();
    table.SimplexMethod(ans);
    cout << "optimal solution in RxR is " << endl;
    for(auto x: ans)
        cout << x << " ";
    cout << endl;
    ld max_value = Task.getter(ans);
    cout << "max is " << max_value << endl;
    cout << endl;

    UserData *kek2 = new UserData(kek);
    for (auto x: kek2->b)
        cout << x << " ";
    cout << endl;

    //main part
    Vertex V(ans, max_value);
    for(auto x: V.ans)
        cout << x << " ";
    cout << endl;
    table.print_matrix();
    Task.solve(V, ans);
    cout << "optimal integer solution in RxR is " << endl;
    for(auto x: ans)
        cout << x << " ";
    cout << endl;
    max_value = Task.getter(ans);
    cout << "max is " << max_value << endl;
    return 0;
}

/*kek.A = {{20, 10}, {12, 7}};
    kek.b = {75, 55};
    kek.c = {1, 1};*/



/*kek.A = {{2, 3, 3}, {4, 1, 2}, {5, 6, 1}};
    kek.c = {1, 1, 1};
    kek.b = {78, 89, 66};*/

