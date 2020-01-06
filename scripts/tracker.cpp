#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <math.h>
using namespace std;
struct vec {
    double p[3];
    vec operator-(vec v) {
        return {p[0] - v.p[0], p[1] - v.p[1], p[2] - v.p[2]};
    }
    vec operator+(vec v) {
        return {p[0] + v.p[0], p[1] + v.p[1], p[2] + v.p[2]};
    }
    vec operator*(double c) {
        return {p[0] * c, p[1] * c, p[2] * c};
    }
    vec operator/(double c) {
        return {p[0] / c, p[1] / c, p[2] / c};
    }
    void operator+=(vec v) {
        *this = *this + v;
    }
    void operator-=(vec v) {
        *this = *this - v;
    }
    void operator*=(double c) {
        *this = *this * c;
    }
    void operator/=(double c) {
        *this = *this / c;
    }
    vec _round() {
        return {round(p[0]), round(p[1]), round(p[2])};
    }
    vec image(double thr) {
        vec p = *this;
        return p - (p / thr)._round() * thr;
    }
    double len() {
        return sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
    }
    double len2() {
        return p[0] * p[0] + p[1] * p[1] + p[2] * p[2];
    }
};
struct atom {
    vec pos;
    bool type;
    int deviation[2];
    int tnum;
    atom(ifstream& in, vector<atom>& pool) {
        tnum = -1;
        string t; in >> t;
        type = t == "O" || t == "2";
        in >> pos.p[0] >> pos.p[1] >> pos.p[2];
        in >> deviation[0] >> deviation[1];
        deviation[type]--;
        while(deviation[!type] > 0) {
            atom res = *this;
            res.deviation[!type] = 1;
            res.deviation[type] = 0;
            pool.push_back(res);
            deviation[!type]--;
        }
        while(deviation[type] > 0) {
            atom res = *this;
            res.deviation[type] = 1;
            res.deviation[!type] = 0;
            pool.push_back(res);
            deviation[type]--;
        }
        if(deviation[type] == -1)
            pool.push_back(*this);
    }
};

struct series {
    int nvac;
    double side;
    vector<vector<atom> > data;
    vector<vector<atom> > travels;
    bool error;
    series(string filename, int _nvac) {
        error = false;
        nvac = _nvac;
        ifstream in(filename);
        if(!in.is_open()) {
            cerr << "ERROR: file not found" << endl;
            error = true;
            return;
        }
        for(int i = 0;; i++) {
            int n;
            in >> n;
            in.ignore(0x1000, '"');
            in >> side;
            in.ignore(0x1000, '\n');
            if(in.eof())
                break;
            data.push_back(vector<atom>());
            for(int j = 0; j < n; j++)
                atom(in, data[i]);
            if(data[0].size() != nvac) {
                data.clear();
                i--;
            }
        }
        in.close();
        cout << "Imported from " << filename << endl;
        if(data.size() == 0) {
            cerr << "ERROR: unstable lattice" << endl;
            error = true;
            return;
        }
        travels.push_back(data[0]);
        for(int i = 1; i < data.size(); i++) {
            travels.push_back(travels[i - 1]);
            for(int j = 0; j < travels[i].size(); j++) {
                double min_len = side;
                int min_i;
                vec min_d;
                for(int k = 0; k < data[i].size(); k++) {
                    if(data[i][k].tnum != -1)
                        continue;
                    vec d = (data[i][k].pos - travels[i][j].pos).image(side);
                    if(d.len() < min_len) {
                        min_len = d.len();
                        min_i = k;
                        min_d = d;
                    }
                }
                travels[i][j].pos += min_d;
                data[i][min_i].tnum = j;
            }
        }
    }

    void export_xyz(string filename) {
        if(error)
            return;
        ofstream out(filename);
        for(int i = 0; i < travels.size(); i++) {
            out << travels[i].size() << "\n\n";
            for(int j = 0; j < travels[i].size(); j++)
                out << (int)travels[i][j].type + 1 << ' ' << travels[i][j].pos.p[0] << ' ' << travels[i][j].pos.p[1] << ' ' << travels[i][j].pos.p[2] << endl;
        }
        out.close();
        cout << "Exported to " << filename << endl;
    }

    void diff_data(string filename, bool type) {
        if(error)
            return;
        vector<double> res(travels.size(), 0);
        for(int i = 0; i < travels.size(); i+=100) {
            cout << i + 1 << " out of " << travels.size() << endl;
            vector<double> r2;
            for(int j = 0; j < travels.size(); j++)
                if(j + i < travels.size())
                    for(int k = 0; k < travels[j].size(); k++)
                        if(travels[j][k].type == type)
                            r2.push_back((travels[j + i][k].pos - travels[j][k].pos).len2());
            for(int i = 0; i < r2.size(); i++)
                for(int j = 0; j < r2.size(); j++)
                    res[i] += r2[j] / r2.size();
        }


        ofstream out(filename);
        for(int i = 0; i < res.size(); i++)
            out<<res[i]<<endl;
        out.close();
        cout << "Exported to " << filename << endl;
    }
};
int main() {
    string path = "output/results/";
    string input_prefix = path + "raw/raw";
    string output_prefix = path + "diff/diff";
    string dump_prefix = path + "track/track";
    string temps[] = {"1500"};
    for(int i = 0; i < sizeof(temps) / sizeof(string); i++) {
        series s(input_prefix + temps[i] + "big.xyz", 1);
        s.export_xyz(dump_prefix + temps[i] + "big.xyz");
        s.diff_data(output_prefix + temps[i] + "big.csv", true);
    }
}