#pragma once

#include <vector>

struct rect_t {
    double x;
    double y;
    double w;
    double h;

    double rx() const;
    double ty() const;
};

bool has_overlap(const rect_t& r1, const rect_t& r2);
bool has_overlap(const rect_t& rectangle, const std::vector<rect_t>& list);

bool are_adjacent(const rect_t& r1, const rect_t& r2);

std::vector<rect_t> generate_rects_random(size_t num_rectangles);
std::vector<rect_t> generate_rects_vline(size_t num_rectangles); // The first rectangle is adjacent to every other rectangle
std::vector<rect_t> generate_rects_hline(size_t num_rectangles); // Every rectangle has 1 adjacency to its right

struct adj_t {
    rect_t* main; // The rectangle that all rects in list are adjacent to
    std::vector<rect_t*> list;
};

std::vector<adj_t> construct_adjs_bf(std::vector<rect_t>& rects);
std::vector<adj_t> construct_adjs_opt(std::vector<rect_t>& rects);