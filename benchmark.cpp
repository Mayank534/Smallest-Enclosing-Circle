#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <map>
#include <fstream> // Added for file output

// CGAL Headers
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Min_circle_2_traits_2<K> Traits;
typedef CGAL::Min_circle_2<Traits> Min_circle;
typedef K::Point_2 Point;

int main() {
    // Configuration based on your requirements
    std::vector<int> n_values = {100, 1000, 10000, 100000, 1000000};
    std::vector<int> thresholds = {10, 20, 30, 50, 75, 100};
    int repetitions = 10000;

    // Matrix to store the results: threshold -> list of counts
    std::map<int, std::vector<int>> results;
    for (int t : thresholds) {
        results[t] = std::vector<int>(n_values.size(), 0);
    }

    // Setup fast random number generation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1000.0);

    std::cout << "Starting benchmark (10,000 repetitions each).\n";
    std::cout << "Note: n=1,000,000 will take a few minutes...\n\n";

    for (size_t col = 0; col < n_values.size(); ++col) {
        int n = n_values[col];
        std::vector<double> times(repetitions);
        double total_time = 0.0;

        std::cout << "Processing n = " << n << "... " << std::flush;

        std::vector<Point> points;
        points.reserve(n);
        for (int i = 0; i < n; ++i) {
            points.emplace_back(dist(gen), dist(gen));
        }
        for (int r = 0; r < repetitions; ++r) {
            // 1. Generate fresh random points for this run

            // 2. Start the clock - measure ONLY the algorithm
            auto start = std::chrono::high_resolution_clock::now();
            
            Min_circle mc(points.begin(), points.end(), true); // 'true' applies randomization
            
            auto end = std::chrono::high_resolution_clock::now();
            // ------------------------------------------------

            // Calculate duration in microseconds for high precision
            std::chrono::duration<double, std::micro> duration = end - start;
            times[r] = duration.count();
            total_time += times[r];
        }

        // 3. Calculate empirical average for O(n) expected baseline
        double avg_time = total_time / repetitions;

        // 4. Count exceedances
        for (int r = 0; r < repetitions; ++r) {
            for (int t : thresholds) {
                double limit = avg_time * (1.0 + (t / 100.0));
                if (times[r] > limit) {
                    results[t][col]++;
                }
            }
        }
        std::cout << "Done. (Avg Time: " << avg_time << " microseconds)\n";
    }

    // --- CONSOLE OUTPUT (Markdown Table) ---
    std::cout << "\n\n### Variance Results (Repetitions = " << repetitions << ")\n\n";
    std::cout << "| Exceeds average by |";
    for (int n : n_values) std::cout << " " << n << " |";
    std::cout << "\n|---|";
    for (size_t i = 0; i < n_values.size(); ++i) std::cout << "---|";
    std::cout << "\n";

    for (int t : thresholds) {
        std::cout << "| **" << t << "%** |";
        for (size_t col = 0; col < n_values.size(); ++col) {
            std::cout << " " << results[t][col] << " |";
        }
        std::cout << "\n";
    }

    // --- FILE OUTPUT (CSV Generation) ---
    std::string filename = "benchmark_results.csv";
    std::ofstream csv_file(filename);

    if (csv_file.is_open()) {
        // Write CSV Header
        csv_file << "Exceeds_average_by(%)";
        for (int n : n_values) {
            csv_file << "," << n;
        }
        csv_file << "\n";

        // Write CSV Data Rows
        for (int t : thresholds) {
            csv_file << t;
            for (size_t col = 0; col < n_values.size(); ++col) {
                csv_file << "," << results[t][col];
            }
            csv_file << "\n";
        }
        
        csv_file.close();
        std::cout << "\nSuccess: Data successfully saved to " << filename << "\n";
    } else {
        std::cerr << "\nError: Unable to open file " << filename << " for writing.\n";
    }

    return 0;
}
