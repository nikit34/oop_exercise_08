#pragma once

#include"figures.h"



class Trapezoid : public Figure {
public:
    Trapezoid() : id{0}, vertices{new std::pair<double, double>[4]} {
        for (uint16_t i = 0; i < 4; ++i){
            this->vertices[i] = std::make_pair(0,0);
        }
    }

    Trapezoid(std::pair<double, double> &a, std::pair<double, double> &b, std::pair<double, double> &c, std::pair<double, double> &d, uint16_t id) :
    id{id}, vertices{new std::pair<double, double>[4]} {
        if (a == b || a == c || b == c || a == d || b == d || c == d ||
            collinear(a, b, c, a) || collinear(a, b, d, a) || collinear(a, c, d, a)
            || collinear(b, c, d, b)) {
            throw std::logic_error("The entered coordinates of the vertices do not belong to the trapezoid.");
        } else {
            this->vertices[0] = a;
            this->vertices[1] = b;
            this->vertices[2] = c;
            this->vertices[3] = d;
        }
    }

    ~Trapezoid() override {
        delete [] this->vertices;
        this->vertices = nullptr;
    }

    std::pair<double, double> Center() const override {
        uint16_t num = 4;
        return getCenter(this->vertices, num);
    }

    double Area() const override {
        auto area = ((vertices[0].first * vertices[1].second - vertices[1].first * vertices[0].second) +
        (vertices[1].first * vertices[2].second - vertices[2].first * vertices[1].second) +
        (vertices[2].first * vertices[3].second - vertices[3].first * vertices[2].second)) / 2;
        return std::abs(area);
    }

    std::ostream &Print(std::ostream &out) const override{
        out << "Trapezoid id: " << this->id << "\n";
        out << "Coords: ";
        for (uint16_t i = 0; i < 4; ++i) {
            out << this->vertices[i];
        }
        out << "\n";
        return out;
    }

    void Serialize(std::ofstream &os) const override{
        FigureType type = trap;
        os.write((char *) &type, sizeof(type));
        os.write((char *) &id, sizeof(id));
        for (uint16_t i = 0; i < 4; ++i) {
            os.write((char *) &(this->vertices[i].first), sizeof(this->vertices[i].first));
            os.write((char *) &(this->vertices[i].second), sizeof(this->vertices[i].second));
        }
    }

    void Deserialize(std::ifstream &is) override {
        is.read((char *) &id, sizeof(id));
        for (uint16_t i = 0; i < 4; ++i) {
            is.read((char *) &(this->vertices[i].first), sizeof(this->vertices[i].first));
            is.read((char *) &(this->vertices[i].second),sizeof(this->vertices[i].second));
        }
    }

    int getId() const override {
        return this->id;
    }

private:
    uint16_t id;
    std::pair<double, double> *vertices;
};
