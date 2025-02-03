#include <iostream>
#include <cmath>

using namespace std;

struct point {
    float xcor, ycor;
};

struct line {
    point p;
    float slope;

    string equation() {
        float c = p.ycor - (slope * p.xcor);
        return "(" + to_string(slope) + ")x - y + (" + to_string(c) + ") = 0";
    }

    bool is_point(point k) {
        return (((slope * k.xcor)- k.ycor + p.ycor - (slope * p.xcor)) == 0);
    }

    line normal(point q) {
        float normal_slope = -1 / slope;
        line normal_line;
        normal_line.p = q;
        normal_line.slope = normal_slope;
        return normal_line;
    }
};

struct circle {
    point centre;
    float radius;

    string equation() {
        float h = 2 * centre.xcor, k = 2 * centre.ycor, c  = (h * h) + (k * k) - (radius * radius);
        return "x^2 + y^2 - (" + to_string(h) + ")x - (" + to_string(k) + ")y - (" + to_string(-c) + ") = 0";
    }

    bool is_point(point k) {
        float distance_squared = pow(k.xcor - centre.xcor, 2) + pow(k.ycor - centre.ycor, 2);
        return ((distance_squared - (radius * radius)) == 0);
    }

    float eccentricity() {
        return 0.0;
    }

    line normal(point q) {
        float normal_slope = (q.ycor - centre.ycor) / (q.xcor - centre.xcor);
        line normal_line;
        normal_line.p = centre;
        normal_line.slope = normal_slope;
        return normal_line;
    }

    line tangent(point q) {
        float slope = (q.ycor - centre.ycor) / (q.xcor - centre.xcor);
        float tangent_slope = -1 / slope;
        line tangent_line;
        tangent_line.p = q;
        tangent_line.slope = tangent_slope;
        return tangent_line;
    }
};

struct parabola {
    point vertex;
    point focus;
    string equation() {
        if (focus.xcor == vertex.xcor) {
            return "(x - " + to_string(vertex.xcor) + ")^2 = " + to_string(4 * (focus.ycor - vertex.ycor)) + "(y - " + to_string(vertex.ycor) + ")";
        }
        else if (focus.ycor == vertex.ycor) {
            return "(y - " + to_string(vertex.ycor) + ")^2 = " + to_string(4 * (focus.xcor - vertex.xcor)) + "(x - " + to_string(vertex.xcor) + ")";
        }
        return "";
    }

    bool is_point(point k) {
        if (focus.xcor == vertex.xcor) {
            return pow((k.xcor -  vertex.xcor), 2) == (4 * (focus.ycor - vertex.ycor) * (k.ycor - vertex.ycor));
        }
        else if (focus.ycor == vertex.ycor) {
            return pow((k.ycor -  vertex.ycor), 2) == (4 * (focus.xcor - vertex.xcor) * (k.xcor - vertex.xcor));
        }
        return false;
    }

    float eccentricity() {
        return 1.0;
    }

    line normal(point k) {
        float normal_slope;
        if (focus.xcor == vertex.xcor) {
            normal_slope = (2 * (focus.ycor - vertex.ycor)) / (vertex.xcor - k.xcor);
        }
        else if (focus.ycor == vertex.ycor) {
            normal_slope = (vertex.ycor - k.ycor) / (2 * (focus.xcor - vertex.xcor));
        }
        line normal_line;
        normal_line.p = k;
        normal_line.slope = normal_slope;
        return normal_line;
    }

    line tangent(point k) {
        float tangent_slope;
        if (focus.xcor == vertex.xcor) {
            tangent_slope = (k.xcor -  vertex.xcor) / (2 * (focus.ycor - vertex.ycor));
        }
        else if (focus.ycor == vertex.ycor) {
            tangent_slope = (2 * (focus.xcor - vertex.xcor)) / (k.ycor - vertex.ycor);
        }
        line tangent_line;
        tangent_line.p = k;
        tangent_line.slope = tangent_slope;
        return tangent_line;
    }
};

struct ellipse {
    point centre;
    float a, b;
    string equation() {
        return "(" + to_string(1 / (a * a)) + ")[x - " + to_string(centre.xcor) + "]^2 + (" + to_string(1 / (b * b)) + ")[y - " + to_string(centre.ycor) + "]^2 = 1";
    }

    bool is_point(point k) {
        return ((pow(((k.xcor - centre.xcor) / a), 2) + pow(((k.ycor - centre.ycor) / b), 2)) == 1);
    }

    float eccentricity() {
        if (a > b) {
            return 1 - pow((b / a), 2);
        }
        return 1 - pow((a / b), 2);
    }

    line normal(point k) {
        float normal_slope = pow((b / a), 2) * ((k.ycor - centre.ycor) / (k.xcor - centre.xcor));
        line normal_line;
        normal_line.p = k;
        normal_line.slope = normal_slope;
        return normal_line;
    }

    line tangent(point k) {
        float tangent_slope = pow((a / b), 2) * ((centre.xcor - k.xcor) / (k.ycor - centre.ycor));
        line tangent_line;
        tangent_line.p = k;
        tangent_line.slope = tangent_slope;
        return tangent_line;
    }
};

struct hyperbola {
    point centre;
    float a, b;
    string equation() {
        return "(" + to_string(1 / (a * a)) + ")[x - " + to_string(centre.xcor) + "]^2 - (" + to_string(1 / (b * b)) + ")[y - " + to_string(centre.ycor) + "]^2 = 1";
    }

    bool is_point(point k) {
        return ((pow(((k.xcor - centre.xcor) / a), 2) - pow(((k.ycor - centre.ycor) / b), 2)) == 1);
    }

    float eccentricity() {
        return 1 + pow((b / a), 2);
    }

    line normal(point k) {
        float normal_slope = pow((b / a), 2) * ((k.ycor - centre.ycor) / (centre.xcor - k.xcor));
        line normal_line;
        normal_line.p = k;
        normal_line.slope = normal_slope;
        return normal_line;
    }

    line tangent(point k) {
        float tangent_slope = pow((a / b), 2) * ((k.xcor - centre.xcor) / (k.ycor - centre.ycor));
        line tangent_line;
        tangent_line.p = k;
        tangent_line.slope = tangent_slope;
        return tangent_line;
    }
};

int main() {
    point mypoint;
    mypoint.xcor = 0;
    mypoint.ycor = 0;

    line myLine;
    myLine.p = {1.0, 2.0};
    myLine.slope = 3.0;

    cout << "Equation of the line: " << myLine.equation() << endl;

    point testPoint = {2.0, 5.0};
    cout << "Is the point on the line? " << (myLine.is_point(testPoint) ? "Yes" : "No") << endl;

    line normalLine = myLine.normal({2.0, 5.0});
    cout << "Equation of the normal line: " << normalLine.equation() << endl;

{// Example usage
    circle myCircle;
    myCircle.centre = {1.0, 2.0};
    myCircle.radius = 3.0;

    cout << "Equation of the circle: " << myCircle.equation() << endl;

    point testPoint = {4.0, 2.0};
    cout << "Is the point on the circle? " << (myCircle.is_point(testPoint) ? "Yes" : "No") << endl;

    cout << "Eccentricity of the circle: " << myCircle.eccentricity() << endl;

    line normalLine = myCircle.normal({4.0, 2.0});
    cout << "Equation of the normal line: " << normalLine.equation() << endl;

    line tangentLine = myCircle.tangent({4.0, 2.0});
    cout << "Equation of the tangent line: " << tangentLine.equation() << endl;}

// Example usage for parabola
    parabola myParabola;
    myParabola.vertex = {0.0, 0.0};
    myParabola.focus = {0.0, 1.0};

    cout << "Equation of the parabola: " << myParabola.equation() << endl;

    point testPointPara = {1.0, 1.0};
    cout << "Is the point on the parabola? " << (myParabola.is_point(testPointPara) ? "Yes" : "No") << endl;

    cout << "Eccentricity of the parabola: " << myParabola.eccentricity() << endl;

    line normalLinePara = myParabola.normal({1.0, 1.0});
    cout << "Equation of the normal line for parabola: " << normalLinePara.equation() << endl;

    line tangentLinePara = myParabola.tangent({1.0, 1.0});
    cout << "Equation of the tangent line for parabola: " << tangentLinePara.equation() << endl;

    // Example usage for ellipse
    ellipse myEllipse;
    myEllipse.centre = {0.0, 0.0};
    myEllipse.a = 2.0;
    myEllipse.b = 1.0;

    cout << "Equation of the ellipse: " << myEllipse.equation() << endl;

    point testPointEllipse = {1.0, 0.0};
    cout << "Is the point on the ellipse? " << (myEllipse.is_point(testPointEllipse) ? "Yes" : "No") << endl;

    cout << "Eccentricity of the ellipse: " << myEllipse.eccentricity() << endl;

    line normalLineEllipse = myEllipse.normal({1.0, 0.0});
    cout << "Equation of the normal line for ellipse: " << normalLineEllipse.equation() << endl;

    line tangentLineEllipse = myEllipse.tangent({1.0, 0.0});
    cout << "Equation of the tangent line for ellipse: " << tangentLineEllipse.equation() << endl;

    // Example usage for hyperbola
    hyperbola myHyperbola;
    myHyperbola.centre = {0.0, 0.0};
    myHyperbola.a = 2.0;
    myHyperbola.b = 1.0;

    cout << "Equation of the hyperbola: " << myHyperbola.equation() << endl;

    point testPointHyperbola = {1.0, 0.0};
    cout << "Is the point on the hyperbola? " << (myHyperbola.is_point(testPointHyperbola) ? "Yes" : "No") << endl;

    cout << "Eccentricity of the hyperbola: " << myHyperbola.eccentricity() << endl;

    line normalLineHyperbola = myHyperbola.normal({1.0, 0.0});
    cout << "Equation of the normal line for hyperbola: " << normalLineHyperbola.equation() << endl;

    line tangentLineHyperbola = myHyperbola.tangent({1.0, 0.0});
    cout << "Equation of the tangent line for hyperbola: " << tangentLineHyperbola.equation() << endl;

    return 0;
}