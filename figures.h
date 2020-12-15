#pragma once

#include <iostream>
#include <fstream>
#include <utility>
#include <memory>
#include <cmath>
#include <stdexcept>


enum FigureType {
    rec,
    rhomb,
    trap,
};

class Figure {
    public:
    virtual double Area() const = 0;
    virtual std::pair<double, double> Center() const = 0;
    virtual std::ostream& Print(std::ostream& out) const = 0;
    virtual void Serialize(std::ofstream& os) const = 0;
    virtual void Deserialize(std::ifstream& is) = 0;
    virtual int getId() const = 0;
    virtual ~Figure() = default;
};

std::pair<double, double> getCenter(
    const std::pair<double, double> *vertices,
    uint16_t& n
    ) {
    double x = 0, y = 0;
    for (uint16_t i = 0; i < n; ++i) {
        x += vertices[i].first;
        y += vertices[i].second;
    }
    return {x / n, y / n};
}

std::pair<double, double> operator- (
    const std::pair<double, double> &p1,
    const std::pair<double, double> &p2
    ) {
    return {p1.first - p2.first, p1.second - p2.second};
}

bool collinear(
    const std::pair<double, double> &a,
    const std::pair<double, double> &b,
    const std::pair<double, double> &c,
    const std::pair<double, double> &d
    ){
    return (b.second-a.second)*(d.first-c.first) - (d.second-c.second)*(b.first-a.first) <= 1e-9;
}


bool perpendicular(
    const std::pair<double, double> &a,
    const std::pair<double, double> &b,
    const std::pair<double, double> &c,
    const std::pair<double, double> &d
    ){
    std::pair<double, double> AC = c - a;
    std::pair<double, double> BD = d - b;

    double normaAC = sqrt(pow(AC.second, 2) + pow(AC.first, 2));
    double normaDB = sqrt(pow(BD.second, 2) + pow(BD.first, 2));

    double hightAC = AC.second / normaAC;
    double widthAC = AC.first / normaAC;
    double hightBD = BD.second / normaDB;
    double widthBD = BD.first / normaDB;

    double cos_theta = hightAC * hightBD + widthAC * widthBD;

    return abs(cos_theta) < 1e-9;
}

double dist(
    const std::pair<double, double> &a,
    const std::pair<double, double> &b
    ){
    return sqrt(((b.first - a.first) * (b.first - a.first)) + ((b.second - a.second) * (b.second - a.second)));
}

bool operator==(
    const std::pair<double, double> &a,
    const std::pair<double, double> &b
    ){
    return (a.first == b.first) && (a.second == b.second);
}

std::ostream& operator<<(std::ostream &o, const std::pair<double, double> &p){
    o << "<" << p.first << ", " << p.second << ">";
    return o;
}

std::istream& operator>>(std::istream &is, std::pair<double, double> &p){
    std::string checker;
    p.first = static_cast<double>(std::stod(checker));
    p.second = static_cast<double>(std::stod(checker));
    return is;
}