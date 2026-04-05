#include <bits/stdc++.h>

// CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

using namespace std;

// --- CGAL TYPES ---
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 Point;

// --- Generate fixed point set ---
vector<Point> generate_points(int n) {
    mt19937 rng(42);
    uniform_real_distribution<double> dist(0.0, 1000.0);

    vector<Point> P;
    P.reserve(n);

    for (int i = 0; i < n; i++)
        P.emplace_back(dist(rng), dist(rng));

    return P;
}

// --- Sample k indices ---
vector<int> sample_k(int n, int k, mt19937 &rng) {
    vector<int> idx(n);
    iota(idx.begin(), idx.end(), 0);
    shuffle(idx.begin(), idx.end(), rng);
    idx.resize(k);
    return idx;
}

// --- Count violations ---
int count_violations(const vector<Point> &P,
                     const vector<int> &sample_idx,
                     const Min_circle &mc) {

    auto circle = mc.circle();
    unordered_set<int> S(sample_idx.begin(), sample_idx.end());

    int violations = 0;

    for (int i = 0; i < (int)P.size(); i++) {
        if (S.count(i)) continue;

        if (circle.has_on_unbounded_side(P[i])) {
            violations++;
        }
    }

    return violations;
}

int main() {
    vector<int> ns = {100, 1000, 10000, 100000, 1000000};
    const int ITER = 10000;

    mt19937 rng(123);

    cout << fixed << setprecision(2);

    cout << "-------------------------------------------------------------\n";
    cout << "n\tk-type\tk\tavg_violations\n";
    cout << "-------------------------------------------------------------\n";

    for (int n : ns) {
        vector<Point> P = generate_points(n);

        vector<pair<string, int>> k_values = {
            {"logn", max(1, (int)log2(n))},
            {"sqrtn", (int)sqrt(n)},
            {"cuberootn", (int)cbrt(n)},
            {"n/2", n / 2}
        };

        for (auto &[label, k] : k_values) {
            double total = 0;

            for (int it = 0; it < ITER; it++) {
                auto idx = sample_k(n, k, rng);

                vector<Point> R;
                R.reserve(k);
                for (int i : idx) R.push_back(P[i]);

                Min_circle mc(R.begin(), R.end(), true);

                int v = count_violations(P, idx, mc);
                total += v;
            }

            double avg = total / ITER;

            cout << n << "\t" << label << "\t" << k << "\t" << avg << "\n";
        }
    }

    cout << "-------------------------------------------------------------\n";

    return 0;
}
