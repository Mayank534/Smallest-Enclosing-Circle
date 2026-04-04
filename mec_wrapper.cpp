#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>

namespace py = pybind11;

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Min_circle_2_traits_2<K>  Traits;
typedef CGAL::Min_circle_2<Traits>      Min_circle;
typedef K::Point_2                      Point;

// Structure to return to Python
struct CircleResult {
    double x, y, r;
};

CircleResult compute_mec(const std::vector<std::pair<double, double>>& input_points) {
    if (input_points.empty()) return {0, 0, 0};

    std::vector<Point> points;
    for (const auto& p : input_points) {
        points.emplace_back(p.first, p.second);
    }

    // Welzl's algorithm (randomized O(n))
    Min_circle mc(points.begin(), points.end(), true);
    
    auto circle = mc.circle();
    Point center = circle.center();
    double radius = std::sqrt(CGAL::to_double(circle.squared_radius()));

    return { CGAL::to_double(center.x()), CGAL::to_double(center.y()), radius };
}

PYBIND11_MODULE(mec_lib, m) {
    py::class_<CircleResult>(m, "CircleResult")
        .def_readwrite("x", &CircleResult::x)
        .def_readwrite("y", &CircleResult::y)
        .def_readwrite("r", &CircleResult::r);

    m.def("compute_mec", &compute_mec, "Compute MEC using CGAL");
}