#pragma once

#include"figures.h"



class Rhombus: public Figure {
public:
    Rhombus() : id{0}, vertices{new std::pair<double, double>[4]} {
        for (uint16_t i = 0; i < 4; ++i){
            this->vertices[i] = {0, 0};
        }
    }
    Rhombus(std::pair<double, double> &a, std::pair<double, double> &b, std::pair<double, double> &c, std::pair<double, double> &d, uint16_t id) :
    id{id}, vertices{new std::pair<double, double>[4]} {
        auto AB = dist(a, b);
        auto AD = dist(a, d);
        auto BC = dist(b,c);
        auto CD = dist(c,d);
        if (a == b || a == c || b == c || a == d || b == d || c == d ||
            !(AB == AD) || !(CD == BC) || !(AB == CD) ||
            !perpendicular(a,b,c,d) ||
            !collinear(a, b, c, d) ||
            !collinear(a, d, c, b)
        ) {
            throw std::logic_error("The entered coordinates of the vertices do not belong to the trapezoid.");
        } else {
            this->vertices[0] = a;
            this->vertices[1] = b;
            this->vertices[2] = c;
            this->vertices[3] = d;
        }
    }

    ~Rhombus() override {
        delete [] this->vertices;
        this->vertices = nullptr;
    }

    std::pair<double, double> Center() const override {
        uint16_t num = 4;
        return getCenter(this->vertices, num);
    }

    double Area() const override {
        double AC = dist(vertices[0],vertices[2]);
        double DB = dist(vertices[3],vertices[1]);
        return (AC*DB) /2;
    }

    std::ostream &Print(std::ostream &out) const override{
        out << "id: " << id << "\n";
        out << "Figure: Trapezoid\n";
        out << "Coords:\n";
        for (uint16_t i = 0; i < 4; ++i) {
            out << vertices[i] << "\n";
        }
        return out;
    }

    void Serialize(std::ofstream &os) const override{
        FigureType type = rhomb;
        os.write((char *) &type, sizeof(type));
        os.write((char *) &id, sizeof(id));
        for (uint16_t i = 0; i < 4; ++i) {
            os.write((char *) &(vertices[i].first),sizeof(vertices[i].first));
            os.write((char *) &(vertices[i].second),sizeof(vertices[i].second));
        }
    }

    void Deserialize(std::ifstream &is) override {
        is.read((char *) &id, sizeof(id));
        for (uint16_t i = 0; i < 4; ++i) {
            is.read((char *) &(vertices[i].first),
                    sizeof(vertices[i].first));
            is.read((char *) &(vertices[i].second),
                    sizeof(vertices[i].second));
        }
    }

    int getId() const override {
        return id;
    }

private:
    uint16_t id;
    std::pair<double, double> *vertices;
};