#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>

struct Point {
    double x, y;
    Point(double x_, double y_) : x(x_), y(y_) {}
};

struct Circle {
    Point center;
    double radius;
};

// --- Geometry Helpers ---
double get_distance(Point a, Point b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

bool is_inside(const Circle& c, const Point& p) {
    return get_distance(c.center, p) <= c.radius + 1e-9;
}

Circle circle_from_2_points(Point a, Point b) {
    Point center = { (a.x + b.x) / 2.0, (a.y + b.y) / 2.0 };
    return { center, get_distance(a, b) / 2.0 };
}

Circle circle_from_3_points(Point a, Point b, Point c) {
    double bx = b.x - a.x, by = b.y - a.y;
    double cx = c.x - a.x, cy = c.y - a.y;
    double B = bx * bx + by * by;
    double C = cx * cx + cy * cy;
    double D = bx * cy - by * cx;
    
    if (std::abs(D) < 1e-9) return {{0, 0}, 0}; // Handling collinearity

    Point center = { (cy * B - by * C) / (2 * D) + a.x, 
                     (bx * C - cx * B) / (2 * D) + a.y };
    return { center, get_distance(center, a) };
}

// --- The O(n^3) Deterministic Algorithm ---
Circle min_enclosing_circle_deterministic(const std::vector<Point>& P) {
    int n = P.size();
    if (n == 0) return {{0, 0}, 0};
    if (n == 1) return {P[0], 0};

    Circle mec = circle_from_2_points(P[0], P[1]);

    for (int i = 2; i < n; i++) {
        if (!is_inside(mec, P[i])) {
            
            mec = circle_from_2_points(P[0], P[i]);
            
            for (int j = 1; j < i; j++) {
                if (!is_inside(mec, P[j])) {
                    
                    mec = circle_from_2_points(P[i], P[j]);
                    
                    for (int k = 0; k < j; k++) {
                        if (!is_inside(mec, P[k])) {
                            
                            mec = circle_from_3_points(P[i], P[j], P[k]);
                        }
                    }
                }
            }
        }
    }
    return mec;
}

int main() {
    int n;
    const double X_MAX = 1000.0;
    const double Y_MAX = 1000.0;

    std::cout << "Enter the number of points (n): ";
    if (!(std::cin >> n) || n <= 0) {
        std::cerr << "Invalid input. Please enter a positive integer." << std::endl;
        return 1;
    }

    std::vector<Point> points;
    points.reserve(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist_x(0.0, X_MAX);
    std::uniform_real_distribution<double> dist_y(0.0, Y_MAX);

    for (int i = 0; i < n; ++i) {
        points.emplace_back(dist_x(gen), dist_y(gen));
    } 
    Circle res = min_enclosing_circle_deterministic(points);

    std::cout << "Center: (" << res.center.x << ", " << res.center.y << ")\n";
    std::cout << "Radius: " << res.radius << "\n";

    return 0;
}
