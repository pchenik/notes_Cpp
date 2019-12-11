#include <iostream>

typedef long long ll;

using  namespace std;

const int N = 10, _W = 100, _V = 100, _K = 100;

int c[N], w[N], v[N], k[N], F[N][_W][_V][_K];
int par[N][_W][_V][_K];

void rec(int n, int W, int V, int K) {

    if (n < 0) {
        return;
    }
    else {
        int cur_y = par[n][W][V][K];
        rec(n - 1, W - w[n] * cur_y, V - v[n] * cur_y, K - k[n] * cur_y);
        cout << "y[" << n << "] = " << par[n][W][V][K] << "  ";
    }
}

int main() {
    int n, W, V, K;
    cin >> n >> W >> V >> K;
    for(int i = 0; i < n; ++i)
        cin >> c[i] >> w[i] >> v[i] >> k[i];

    for(int _w = 0; _w <= W; ++_w)
        for(int _v = 0; _v <= V; ++_v)
            for(int _k = 0; _k <= K; ++_k) {
                F[0][_w][_v][_k] = c[0] * min(min(_w / w[0], _v / v[0]), _k / k[0]);
                par[0][_w][_v][_k] = min(min(_w / w[0], _v / v[0]), _k / k[0]);
            }

    for(int i = 1; i < n; ++i)
        for(int _w = 0; _w <= W; ++_w)
            for(int _v = 0; _v <= V; ++_v)
                for(int _k = 0; _k <= K; ++_k)
                    for (int y = 0; y <= min(min(_w / w[i], _v / v[i]), _k / k[i]); ++y)
                        if (F[i][_w][_v][_k] < F[i - 1][_w - w[i] * y][_v - v[i] * y][_k - k[i] * y] + c[i] * y) {
                            F[i][_w][_v][_k] = F[i - 1][_w - w[i] * y][_v - v[i] * y][_k - k[i] * y] + c[i] * y;
                            par[i][_w][_v][_k] = y;
                        }
    rec(n - 1, W, V, K);
    cout << endl;
    cout << F[n - 1][W][V][K] << endl;
    return 0;
}