#pragma once

#include"figures.h"



class Rectangle : public Figure {
public:
    Rectangle(): id{0}, vertices{new std::pair<double, double>[4]} {
        for (uint16_t i = 0; i < 4; ++i){
            this->vertices[i] = std::make_pair(0, 0);
        }
    }

    Rectangle(
        std::pair<double, double> &a,
        std::pair<double, double> &b,
        std::pair<double, double> &c,
        std::pair<double, double> &d,
        uint16_t id
        ) : id{id}, vertices{new std::pair<double, double>[4]} {
        if (a == b || a == c || b == c || a == d ||
            !(perpendicular(a, b, a, d)) ||
            !collinear(a, d, c, b) ||
            !collinear(a, b, d, c)
        ) {
            throw std::logic_error("Entered coordinates of vertices do not belong to rectangle.");
        } else {
            this->vertices[0] = a;
            this->vertices[1] = b;
            this->vertices[2] = c;
            this->vertices[3] = d;
        }
    }

    ~Rectangle() override {
        delete [] this->vertices;
        this->vertices = nullptr;
    }

    std::pair<double, double> Center() const override {
        uint16_t number = 4;
        return getCenter(this->vertices, number);
    }

    double Area() const override {
        auto AB = dist(this->vertices[0], vertices[1]);
        auto AD = dist(this->vertices[0],vertices[3]);
        return AD * AB;
    }

    std::ostream &Print(std::ostream &out) const override {
        out << "Rectangle - id: " << id << "\n";
        for (uint16_t i = 0; i < 4; ++i) {
            out << vertices[i] << " ";
        }
        out << "\n";
        return out;
    }

    void Serialize(std::ofstream &os) const override {
        FigureType type = rec;
        os.write((char *) &type, sizeof(type));
        os.write((char *) &id, sizeof(id));
        for (uint16_t i = 0; i < 4; ++i) {
            os.write((char*) &(vertices[i].first), sizeof(vertices[i].first));
            os.write((char*) &(vertices[i].second), sizeof(vertices[i].second));
        }
    }

    void Deserialize(std::ifstream &is) override {
        is.read((char *) &id, sizeof(id));
        for (uint16_t i = 0; i < 4; ++i) {
            is.read((char *) &(vertices[i].first), sizeof(vertices[i].first));
            is.read((char *) &(vertices[i].second), sizeof(vertices[i].second));
        }
    }

    int getId() const override {
        return (int)id;
    }

private:
    uint16_t id;
    std::pair<double, double> *vertices;
};