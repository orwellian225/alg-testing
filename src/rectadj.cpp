#include <ctime>
#include <vector>
#include <random>

#include "rectadj.h"

double rect_t::rx() const { return x + w; }
double rect_t::ty() const { return y + h; }

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
        0, 0,
        10.0, num_rectangles * 15.0
    });

    for (size_t i = 1; i < num_rectangles; ++i) {
        results.push_back( rect_t {
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
        0, 0,
        10.0, 10.0 
    });

    for (size_t i = 1; i < num_rectangles; ++i) {
        results.push_back( rect_t {
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

std::vector<adj_t> construct_adjs_opt(std::vector<rect_t>& rects) {}