#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>

// CGAL Headers
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

// Define the Kernel and the Min_circle type
typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Min_circle_2_traits_2<K>              Traits;
typedef CGAL::Min_circle_2<Traits>                  Min_circle;
typedef K::Point_2                                  Point;

int main() {
    int n;
    const double X_MAX = 1000.0;
    const double Y_MAX = 1000.0;

    std::cout << "Enter the number of points (n): ";
    if (!(std::cin >> n) || n <= 0) {
        std::cerr << "Invalid input. Please enter a positive integer." << std::endl;
        return 1;
    }

    // 1. Initialize n points in range [0,0] to [X_MAX, Y_MAX]
    std::vector<Point> points;
    points.reserve(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist_x(0.0, X_MAX);
    std::uniform_real_distribution<double> dist_y(0.0, Y_MAX);

    for (int i = 0; i < n; ++i) {
        points.emplace_back(dist_x(gen), dist_y(gen));
    }

    // 2. Compute MEC and measure time
    // CGAL's Min_circle_2 uses Welzl's algorithm (randomized O(n))
    auto start = std::chrono::high_resolution_clock::now();
    
    Min_circle mc(points.begin(), points.end(), true); // 'true' triggers randomization
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;

    // 3. Output results
    // We convert the exact representation back to double for readable output
    auto circle = mc.circle();
    Point center = circle.center();
    double radius_squared = CGAL::to_double(circle.squared_radius());

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "\n--- Results ---" << std::endl;
    std::cout << "Center: (" << CGAL::to_double(center.x()) << ", " << CGAL::to_double(center.y()) << ")" << std::endl;
    std::cout << "Radius: " << std::sqrt(radius_squared) << std::endl;
    std::cout << "Time taken: " << duration.count() << " ms" << std::endl;

    return 0;
}
