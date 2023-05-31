#include <ctime>
#include <vector>
#include <random>
#include <algorithm>
#include <set>

#include <fmt/core.h>


#include "rectadj.h"

double rect_t::rx() const { return x + w; }
double rect_t::ty() const { return y + h; }

std::string rect_t::to_string() const {
    return fmt::format(
        "{},{},{},{},{}",
        id, x, y, rx(), ty()
    );
}

std::string adj_t::to_string() const {
    std::string rect_str = fmt::format("{},{}", main->id, list.size());
    std::string list_str = "";
    for (size_t i = 0; i < list.size(); ++i) {
        list_str += fmt::format(",{},{},{},{}", 
            list[i]->id, list[i]->rx(), list[i]->y, list[i]->ty()
        );
    }

    return fmt::format("{}{}", rect_str, list_str);
}

bool has_overlap(const rect_t& r1, const rect_t& r2) {
    return r1.rx() > r2.x && r2.rx() > r1.x && r1.ty() > r2.y && r2.ty() > r1.y;
}

bool has_overlap(const rect_t& rectangle, const std::vector<rect_t>& list) {
    bool result = false;
    for (auto other_rect: list) {
        result = result || has_overlap(rectangle, other_rect);
    }

    return result;
}

bool are_adjacent(const rect_t& r1, const rect_t& r2) {
    return r1.rx() == r2.x && (
        (r1.y <= r2.ty() && r2.ty() <= r1.ty()) 
        || (r1.ty() >= r2.y && r2.y >= r1.y)
    );
}

std::vector<rect_t> generate_rects_random(size_t num_rectangles) {
    std::mt19937 rng_engine;
    rng_engine.seed(static_cast<unsigned>(time(NULL)));

    double min_dimension = 10.0;
    double max_dimension = 100.0;
    std::uniform_real_distribution<double> dimensions(min_dimension, max_dimension);

    std::vector<rect_t> results;
    results.push_back( rect_t {
        0,
        0, 0,
        dimensions(rng_engine), dimensions(rng_engine)
    });

    // Start at 1 as first rectangle already made
    size_t i = 1;
    while (i < num_rectangles) {
        std::uniform_int_distribution<size_t> spawn_rect_dist(0, results.size() - 1);
        rect_t* spawn_rect = &results[spawn_rect_dist(rng_engine)];

        std::uniform_real_distribution<double> possible_y (spawn_rect->y, spawn_rect->ty() - 0.1);
        rect_t new_rectangle = rect_t {
            i,
            spawn_rect->rx(), possible_y(rng_engine),
            dimensions(rng_engine), dimensions(rng_engine)
        };

        if (!has_overlap(new_rectangle, results)) {
            ++i;
            results.push_back(new_rectangle);
        }
    }

    return results;
}

std::vector<rect_t> generate_rects_vline(size_t num_rectangles) {
    std::mt19937 rng_engine;
    rng_engine.seed(static_cast<unsigned>(time(NULL)));

    std::vector<rect_t> results;
    results.push_back( rect_t {
        0,
        0, 0,
        10.0, num_rectangles * 15.0
    });

    for (size_t i = 1; i < num_rectangles; ++i) {
        results.push_back( rect_t {
            i,
            10.0, i * 15.0,
            10.0, 10.0
        });
    }

    return results;
}

std::vector<rect_t> generate_rects_hline(size_t num_rectangles) {
    std::mt19937 rng_engine;
    rng_engine.seed(static_cast<unsigned>(time(NULL)));

    std::vector<rect_t> results;
    results.push_back( rect_t {
        0,
        0, 0,
        10.0, 10.0 
    });

    for (size_t i = 1; i < num_rectangles; ++i) {
        results.push_back( rect_t {
            i,
            i * 10.0, 0.0,
            10.0, 10.0
        });
    }

    return results;
}

std::vector<adj_t> construct_adjs_bf(std::vector<rect_t>& rects) {
    std::vector<adj_t> result;
    for (size_t i = 0; i < rects.size(); ++i) {
        std::vector<rect_t*> adj_list;
        for (size_t j = 0; j < rects.size(); ++j) {
            if (are_adjacent(rects[i], rects[j])) {
                adj_list.push_back(&rects[j]);
            }
        }
        result.push_back(adj_t { &rects[i], adj_list });
    }
    return result;
}

std::vector<adj_t> construct_adjs_opt_lin_search(std::vector<rect_t>& rects) {
    std::vector<adj_t> result;

    auto compare_rects = [](const rect_t& a, const rect_t& b) {
        return a.x < b.x;
    };
    std::sort(rects.begin(), rects.end(), compare_rects); // nlogn

    for (size_t i = 0; i < rects.size(); ++i) { // n
        std::vector<rect_t*> adj_list;
        size_t j = i + 1;
        while (j < rects.size() && rects[j].x <= rects[i].rx()) { // ?
            if (are_adjacent(rects[i], rects[j])) {
                adj_list.push_back(&rects[j]);
            }
            ++j;
        }
        result.push_back(adj_t { &rects[i], adj_list });
    }

    return result;
}

// Event Point
struct ep_t {
    uint8_t type; // Left = 0, Right = 1
    double x;
    rect_t* rectangle;

    bool operator < (const ep_t& rhs) const { 
        if (x == rhs.x) {
            return type < rhs.type;
        }

        return x < rhs.x;
     };
};

// Candidate List Element
struct cl_t {
    double y;
    rect_t* rectangle;

    bool operator < (const cl_t& rhs) const { return y < rhs.y || rectangle < rhs.rectangle; };
};

std::vector<adj_t> construct_adjs_opt_line_sweep(std::vector<rect_t>& rects) {
    std::vector<adj_t> result;

    std::vector<ep_t> event_points;
    for (size_t i = 0; i < rects.size(); ++i) {
        event_points.push_back(ep_t { 0, rects[i].x, &rects[i] });
        event_points.push_back(ep_t { 1, rects[i].rx(), &rects[i] });
    }

    std::set<cl_t> candidates;

    // Sort by left most x and type
    std::sort(event_points.begin(), event_points.end());
    for (size_t i = 0; i < event_points.size(); ++i) {
        cl_t ep_cl = cl_t { event_points[i].rectangle->ty(), event_points[i].rectangle };
        if (event_points[i].type == 0) {
            candidates.insert(ep_cl);
        } else if (event_points[i].type == 1) {
            candidates.erase(ep_cl);
            cl_t search_start = cl_t { event_points[i].rectangle->y, event_points[i].rectangle };
            cl_t search_end = cl_t {event_points[i].rectangle->ty(), event_points[i].rectangle };
    
            auto start_it = candidates.lower_bound(search_start);
            auto it = start_it;
            std::vector<rect_t*> adj_list;
            while (it != candidates.end()) {
                if (are_adjacent(*event_points[i].rectangle, *it->rectangle)) {
                    adj_list.push_back(it->rectangle);
                }
    
                if (event_points[i].rectangle->ty() < it->rectangle->y) {
                    break;
                }

                ++it;
            }

            result.push_back(adj_t{ event_points[i].rectangle, adj_list });
        }
    }

    return result;
}