#include "led.h"
#include <cstdlib>
#include <deque>
#include <vector>
#include <random>
#include <cmath>

extern void SetUrl(char const * url);
extern void SetPort(unsigned short port);
extern void SetLed(int x, int y, int z, int rgb);
extern void Clear();
extern void Show();
extern void Wait(int ms);
extern void EnableSimulator(bool isEnable);
extern void SetChar(int x, int y, int z, char c, int rgb);
extern void ShowMotioningText1(const char * text);
extern void ShowFirework(int x, int y, int z);

int N = 100;
int get_power(int ix)
{
    int fade = N / 5;
    if (ix < fade) {
        return ix * 255 / fade;
    } else if (ix < N - fade) {
        return 255;
    } else {
        return (N - ix) * 255 / fade;
    }
}

template< typename t >
t max3(t const & a, t const & b, t const & c)
{
    auto ab = a < b ? b : a;
    return ab < c ? c : ab;
}

template< typename proc_t >
void concentric(proc_t const & proc)
{
    for (int ix = 0; ix<N; ++ix) {
        int cx = LED_WIDTH / 2;
        int cy = LED_HEIGHT / 2;
        int cz = LED_DEPTH / 2;
        Clear();
        int power = get_power(ix);
        for (int x = 0; x < LED_WIDTH; ++x) {
            for (int y = 0; y < LED_HEIGHT; ++y) {
                for (int z = 0; z < LED_DEPTH; ++z) {
                    double d = proc(x - cx, y - cy, z - cz, ix*1.0/N);
                    int col0 = static_cast<int>(std::round(ix - d)) % 64;
                    int col = (col0 % 6) == 0 ? col0 % 7 + 1 : 0;
                    int r = (col & 1) ? power : 0;
                    int g = (col & 2) ? power : 0;
                    int b = (col & 4) ? power : 0;
                    int rgb = (r << 16) + (g << 8) + b;
                    SetLed(x, y, z, rgb);
                }
            }
        }
        Show();
        Wait(50);
    }
}

struct xyz_t{
    double x, y, z;
    double len() const { return std::sqrt(x*x + y*y + z*z); }
};

xyz_t xyz(double x, double y, double z)
{
    return{ x, y, z };
}

struct xyzc_t{
    xyz_t p;
    int color;
};

xyz_t operator*(xyz_t const & a, double t)
{
    return{ a.x*t, a.y*t, a.z*t };
}

xyz_t operator+(xyz_t const & a, xyz_t const & b)
{
    return{ a.x + b.x, a.y + b.y, a.z + b.z };
}

xyz_t operator-(xyz_t const & a, xyz_t const & b)
{
    return{ a.x - b.x, a.y - b.y, a.z - b.z };
}

xyz_t & operator+=(xyz_t & a, xyz_t const & b)
{
    a = a + b;
    return a;
}

xyz_t & operator-=(xyz_t & a, xyz_t const & b)
{
    a = a - b;
    return a;
}

double ip(xyz_t const & a, xyz_t const & b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

xyz_t op(xyz_t const & a, xyz_t const & b)
{
    return{
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    };
}

xyz_t sphere_face(std::mt19937 & rng)
{
    std::uniform_real_distribution<double> dist(-1, 1);
    for (;;) {
        xyz_t pos = { dist(rng), dist(rng), dist(rng) };
        double d2 = pos.x*pos.x + pos.y*pos.y + pos.z*pos.z;
        if (0.1<d2 && d2 < 1) {
            double d = 0.5;
            return{ pos.x * d, pos.y * d+0.1, pos.z * d };
        }
    }
}

bool can_show(xyz_t const & p)
{
    return 0 <= p.x && p.x < LED_WIDTH
    && 0 <= p.y && p.y < LED_HEIGHT
    && 0 <= p.z && p.z < LED_DEPTH;
}

void color_cube()
{
    using namespace std;
    const int N = 200;
    std::vector<xyzc_t> points;
    auto rgb = [](double x, double y, double z)->int {
        auto c = [](double v)->int {
            return std::round((v + 1) * 127);
        };
        return c(x) * 0x10000 + c(y) * 0x100 + c(z) * 0x1;
    };
    const int P = 90*10;
#if 1
    for (int i = 0; i < P; ++i) {
        double k = 0.8;
        double x = ((rand() % 101) / 50.0 - 1);
        double y = ((rand() % 101) / 50.0 - 1);
        double z = ((rand() % 101) / 50.0 - 1);
        points.push_back({ { k*x, k*y, k*z }, rgb(x,y,z) });
    }
#else
    for (int i = 0; i < P; ++i) {
        double k = 0.5;
        double t = k*(2 * i / (P - 1.0) - 1);
        for (int sig = 0; sig < 4; ++sig) {
            double s0 = sig & 1 ? -k : k;
            double s1 = sig & 2 ? -k : k;
            points.push_back({ { s0, s1, t }, rgb(i)});
            points.push_back({ { s0, t, s1 }, rgb(i)});
            points.push_back({ { t, s0, s1 }, rgb(i)});
        }
    }
#endif
    for (int i = 0; i < N; ++i) {
        Clear();
        for (auto p0 : points) {
            double t = cos(6.28*i / N * 2)*3.14;
            double t1 = sin(6.28*i / N * 2)*3.14;
            xyz_t p1 = {
                p0.p.x * cos(t) + p0.p.z * sin(t),
                p0.p.y,
                - p0.p.x * sin(t) + p0.p.z * cos(t),
            };
            double zo = sin(6.28*i / N * 2)*0.3+1;
            if (N - 30 < i) {
                zo *= (N - i)*1.0/30;
            }
            if (i < 30) {
                zo *= i*1.0 / 30;
            }
            xyz_t p = {
                zo*(p1.x * cos(t1) + p1.y * sin(t1)),
                zo*(-p1.x * sin(t1) + p1.y * cos(t1)),
                zo*p1.z,
            };
            double x = (p.x / 2 + 0.5)* (LED_WIDTH - 1);
            double y = (p.y / 2 + 0.5)* (LED_HEIGHT - 1);
            double z = (p.z / 2 + 0.5)* (LED_DEPTH - 1);
            if (can_show({ x, y, z })) {
                SetLed(x, y, z, p0.color);
            }
        }
        Show();
        Wait(50);
    }
}

void fireworks()
{
    const int N = 200;
    std::vector<xyzc_t> poss;
    std::vector<xyz_t> vs(poss.size());
    std::mt19937 rng;
    const int PS = 1000;
    auto rgb = [](double x)->int {
        auto red = [](int ix)->int {
            int i = ix % 90;
            if (i < 30) {
                return i * 255 / 30;
            } else if (i < 60) {
                return (60 - i) * 255 / 30;
            } else {
                return 0;
            }
        };
        int n = std::floor(x * 1 * 90);
        return red(n) * 0x10000 + red(n+30) * 0x100 + red(n+60) * 0x1;
    };
    auto darken = [](int x)->int
    {
        int r = ((x & 0xff0000) * 49 / 50) & 0xff0000;
        int g = ((x & 0xff00) * 49 / 50) & 0xff00;
        int b = ((x & 0xff) * 49 / 50) & 0xff;
        return r + g + b;
    };
    std::uniform_real_distribution<double> dist(0, 1);
    for (int ix = 0; ix < N; ++ix) {
        Clear();
        if (ix % 20 == 0 && 20<N-ix) {
            double cx = LED_WIDTH * dist(rng);
            double cy = LED_HEIGHT * dist(rng);
            double cz = LED_DEPTH * dist(rng);
            for (int i = 0; i < PS; ++i) {
                vs.push_back(sphere_face(rng));
                poss.push_back({ { cx, cy, cz }, rgb(vs.back().len()) });
            }
        }
        for (size_t i = 0; i<poss.size() ; ++i ){
            auto & p = poss[i];
            auto & v = vs[i];
            if (can_show(p.p)) {
                SetLed(p.p.x, p.p.y, p.p.z, p.color);
            } else {
                
            }
            p.p = p.p + v;
            p.color = darken(p.color);
        }
        Show();
        Wait(50);
    }
}

void repbang()
{
    std::vector<xyzc_t> ps;
    std::mt19937 rng;
    auto newp = [&rng]()->xyzc_t{
        std::uniform_real_distribution<double> dist(-1, 1);
        double x = dist(rng);
        double y = dist(rng);
        double z = dist(rng);
        double r = dist(rng) / sqrt(x*x + y*y + z*z + 1e-5);
        return{
            { x*r, y*r, z*r },
            static_cast<int>(rng() % 0x1000000) };
    };
    for (int i = 0; i < 4000; ++i){
        ps.push_back(newp());
    }
    int N = 216;
    for (int i = 0; i < N; ++i){
        Clear();
        double r = 1-std::cos(i*3.1416 / N * 8);
        double t = i*3.1416 / N * 16;
        for (auto & p : ps){
            xyz_t q = {
                std::cos(t)*p.p.x - std::sin(t)*p.p.z,
                p.p.y,
                std::sin(t)*p.p.x + std::cos(t)*p.p.z,
            };
            xyz_t pos = q * r * LED_HEIGHT + xyz(LED_WIDTH, LED_HEIGHT, LED_DEPTH)*0.5;
            
            if (can_show(pos)){
                SetLed(pos.x, pos.y, pos.z, p.color);
            }
        }
        Show();
        Wait(50);
    }
}

void spiral()
{
    struct rty_t{ double r, t, y; };
    struct rtyc_t { rty_t p; int c; };
    std::vector<rtyc_t> pcs;
    std::mt19937 rng;
    
    for (int i = 0; i < 1000; ++i){
        double r = ((rng() % 1000) / 1000.0) * 0.8 + 0.2;
        double y = (rng() % 1000) / 1000.0;
        double t = y*3.1416 * 16;// rng() % 1000;
        double t0 = 3.1416 * 2 / 3;
        auto c = [t0](double t)->int {
            double v = std::sin(t * 2 /3);
            return v < 0 ? 0 : v * 255;
        };
        int col = c(t)+c(t+t0)*256+c(t+t0*2)*65536;
        pcs.push_back({ { r, t, y }, col });
    }
    const int N = 200;
    const double T = 0.125;
    const double U = 0.25;
    const int D = 20;
    auto darken = [D](int x, int d)->int
    {
        int r = ((x & 0xff0000) * d / D ) & 0xff0000;
        int g = ((x & 0xff00) * d/ D) & 0xff00;
        int b = ((x & 0xff) * d/ D) & 0xff;
        return r + g + b;
    };
    for (int i = 0; i < N; ++i){
        Clear();
        int d = i < D ? i : N < i + D ? N - i : D;
        for (auto & pc : pcs){
            double y = int((pc.p.y) * LED_HEIGHT+N-i*U) % LED_HEIGHT;
            double x = (pc.p.r * std::cos(pc.p.t + i*T)) * LED_WIDTH / 2 + LED_WIDTH/2;
            double z = (pc.p.r * std::sin(pc.p.t + i*T)) * LED_WIDTH / 2 + LED_DEPTH/2;
            if (can_show({ x, y, z })){
                SetLed(x, y, z, darken( pc.c, d ));
            }
        }
        Show();
        Wait(50);
    }
}

void brown()
{
    int len = N * 2;
    struct pos_t{ int x, y, z; };
    std::deque<pos_t> poses[3]{
        {{ LED_WIDTH / 2, LED_HEIGHT / 4, LED_DEPTH / 2 }},
        { { LED_WIDTH / 2, LED_HEIGHT / 2, LED_DEPTH / 2 } },
        { { LED_WIDTH / 2, LED_HEIGHT*3 / 4, LED_DEPTH / 2 } }
    };
    
    auto normalize = [](pos_t const & s) -> pos_t {
        return{
            (s.x + LED_WIDTH) % LED_WIDTH,
            (s.y + LED_HEIGHT) % LED_HEIGHT,
            (s.z + LED_DEPTH) % LED_DEPTH
        };
    };
    auto move = [](pos_t const & s) -> pos_t {
        switch (rand() % 12) {
            default:
            case 0: return{ s.x + 1, s.y, s.z };
            case 1: return{ s.x - 1, s.y, s.z };
            case 2:
            case 3:
            case 4:
            case 5:
                return{ s.x, s.y + 1, s.z };
            case 6:
            case 7:
            case 8:
            case 9:
                return{ s.x, s.y - 1, s.z };
            case 10: return{ s.x, s.y, s.z + 1 };
            case 11: return{ s.x, s.y, s.z - 1 };
        }
    };
    auto red = [](int ix)->int {
        int i = ix % 90;
        if (i < 30) {
            return i * 255 / 30;
        } else if (i < 60) {
            return (60 - i) * 255 / 30;
        } else {
            return 0;
        }
    };
    for (int i = 0; i < len; ++i) {
        Clear();
        for (int i = 0; i < 3; ++i) {
            auto & pos = poses[i];
            for (int j = 0; j < 3; ++j) {
                pos.push_back(normalize(move(pos.back())));
            }
            int power = i < len - 20 ? 255 : (len - i) * 255 / 20;
            while (150 < pos.size()) {
                pos.pop_front();
            }
            int ix = 0;
            for (auto it = pos.begin(); it != pos.end(); ++it, ++ix) {
                int r = red(ix) * power / 255;
                int g = red(ix + 30)* power / 255;
                int b = red(ix + 60)* power / 255;
                int rgb = (r << 16) + (g << 8) + b;
                SetLed(it->x, it->y, it->z, rgb);
            }
        }
        Show();
        Wait(50);
    }
}

void cube()
{
    concentric([](int dx, int dy, int dz, double t)->double {
        return max3(std::abs(dx), std::abs(dy), std::abs(dz));
    });
}

void sphere()
{
    concentric([](int dx, int dy, int dz, double t)->double {
        return std::sqrt(dx*dx + dy*dy + dz*dz);
    });
}

void skewed_sphere()
{
    concentric([](int dx0, int dy, int dz0, double t)->double {
        double T = 4;
        double s = std::sin(t*3.14*T);
        double c = std::cos(t*3.14*T);
        double dx = (dx0*c + dz0*s)*0.5;
        double dz = (-dx0*s + dz0*c)*2;
        return std::sqrt(dx*dx + dy*dy + dz*dz);
    });
}

void gala()
{
    struct rtyc_t{ double r, t, y; int c; };
    std::mt19937 rng;
    std::uniform_real_distribution<> tdist(0, 3.1416 * 2);
    std::uniform_real_distribution<> one(-1, 1);
    std::vector<rtyc_t> stars;
    for (int i = 0; i < 2000; ++i){
        if (rng() % 2){
            double r = std::abs(one(rng));
            double y = one(rng);
            double rr = 0.3*std::abs(one(rng)) / sqrt(r*r + y*y + 1e-5);
            stars.push_back({ r * rr, tdist(rng), y * rr, static_cast<int>(rng() % 0x1000000) });
        }
        else {
            double r = 0.3+4*std::abs(one(rng));
            double y = one(rng)*0.1;
            stars.push_back({ r , tdist(rng), y , static_cast<int>(rng() % 0x1000000) });
        }
    }
    const int D = 20;
    auto darken = [D](int x, int d)->int
    {
        int r = ((x & 0xff0000) * d / D) & 0xff0000;
        int g = ((x & 0xff00) * d / D) & 0xff00;
        int b = ((x & 0xff) * d / D) & 0xff;
        return r + g + b;
    };
    const int N = 200;
    for (int i = 0; i < N; ++i){
        Clear();
        int d = i < D ? i : N < i + D ? N - i : D;
        for (auto & star : stars){
            star.t += 0.005 / (std::abs(star.r) + 0.1);
            xyz_t q{
                star.r * std::cos(star.t),
                star.y,
                star.r * std::sin(star.t)};
            double T = 2, U=-4;
            double x = std::cos(T)*q.x - std::sin(T)*q.y;
            double y = std::sin(T)*q.x + std::cos(T)*q.y;
            double z = q.z;
            xyz_t p{
                std::cos(U)*x - std::sin(U)*z,
                y,
                std::sin(U)*x + std::cos(U)*z,
            };
            xyz_t pos{
                p.x * LED_WIDTH / 2 + LED_WIDTH / 2,
                p.y * LED_WIDTH / 2 + LED_HEIGHT / 2,
                p.z * LED_WIDTH / 2 + LED_DEPTH / 2 };
            if (can_show(pos)){
                SetLed(pos.x, pos.y, pos.z, darken(star.c, d));
            }
        }
        Show();
        Wait(50);
    }
}

void balls()
{
    struct p_t { xyz_t p, v; int c; };
    std::vector<p_t> ps;
    std::mt19937 rng;
    for (int i = 1; i < 7; ++i){
        ps.push_back({
            {static_cast<double>(i & 1 ? 0 : LED_WIDTH),
                static_cast<double>(i & 2 ? 0 : LED_HEIGHT),
                static_cast<double>(i & 4 ? 0 : LED_DEPTH)},
            {0,0,0},
            (i & 1 ? 0xff0000 : 0) + (i & 2 ? 0xff00 : 0) + (i & 4 ? 0xff : 0)
        });
    }
    int N = 200;
    for (int ix = 0;; ++ix){
        Clear();
        bool shown = false;
        for (auto & p : ps){
            if (ix<N && ( !can_show(p.p) || p.v.len() == 0)){
                xyz_t dir;
                for (;;){
                    xyz_t dest{ static_cast<double>(rng() % LED_WIDTH), static_cast<double>(rng() % LED_HEIGHT), static_cast<double>(rng() % LED_DEPTH) };
                    dir = dest - p.p;
                    if (dir.len() != 0){
                        break;
                    }
                }
                p.v = dir *(1.0 / dir.len()*2);
            }
            p.p += p.v;
            for (int i = 0; i < 125; ++i){
                xyz_t delta{
                    static_cast<double>(i % 5 - 2),
                    static_cast<double>((i / 5) % 5 - 2),
                    static_cast<double>((i / 25) % 5 - 2) };
                if (delta.len() < 2.5){
                    xyz_t pos = p.p + delta;
                    if (can_show(pos)){
                        shown = true;
                        SetLed(pos.x, pos.y, pos.z, p.c);
                    }
                }
            }
        }
        Show();
        Wait(50);
        if (!shown){
            break;
        }
    }
}

void threed()
{
    char m0[] =
    "00000000"/*0*/
    "01111110"/*1*/
    "01111110"/*2*/
    "00000110"/*3*/
    "00000110"/*4*/
    "00000110"/*5*/
    "00000110"/*6*/
    "00111110"/*7*/
    "00111110"/*8*/
    "00000110"/*9*/
    "00000110"/*a*/
    "00000110"/*b*/
    "00000110"/*c*/
    "01111110"/*d*/
    "01111110"/*e*/
    "00000000"/*f*/;
    char m1[] =
    "00000000"/*0*/
    "01111000"/*1*/
    "01111100"/*2*/
    "01101110"/*3*/
    "01100110"/*4*/
    "01100110"/*5*/
    "01100110"/*6*/
    "01100110"/*7*/
    "01100110"/*8*/
    "01100110"/*9*/
    "01100110"/*a*/
    "01100110"/*b*/
    "01101110"/*c*/
    "01111100"/*d*/
    "01111000"/*e*/
    "00000000"/*f*/;
    std::mt19937 rng;
    auto col_at = [&m0, &m1, &rng](int x, int y, int c)->int {
        char const * m = c == 0 ? m0 : m1;
        return m[x / 2 + (y / 2) * 8] == '0' ? 0 :
        c == 0 ? x * 0x100000 + (16 - x) * 0x1000
        : y * 0x8 + (31 - y) * 0x80000;
    };
    auto col_at2 = [&col_at](xyz_t const & p, int c)->int {
        return col_at(p.x, p.y, c);
    };
    struct pp_t{
        xyz_t p[2];
    };
    std::vector<pp_t> pps;
    while (pps.size()<2000){
        xyz_t a{
            static_cast<double>(rng() % LED_WIDTH),
            static_cast<double>(rng() % LED_HEIGHT),
            static_cast<double>(rng() % LED_DEPTH) };
        xyz_t b{
            static_cast<double>(rng() % LED_WIDTH),
            static_cast<double>(rng() % LED_HEIGHT),
            static_cast<double>(rng() % LED_DEPTH) };
        if (col_at2(a,0) && col_at2(b,1)) {
            pps.push_back({ { a, b } });
        }
    }
    int N0 = 20;
    int N1 = 20;
    auto show = [&col_at2,&pps, N0](double c, double d)->void {
        Clear();
        for (auto & pp : pps) {
            xyz_t p = pp.p[0] * (1-c) + pp.p[1] * c;
            int c0 = col_at2(pp.p[0], 0);
            int c1 = col_at2(pp.p[1], 1);
            auto mean = [&](int mask, double d) {
                int v = ((c0 & mask)*(1 - c) + (c1 & mask)*c)*d;
                return v & mask;
            };
            int col = mean(0xff0000,d) + mean(0xff00,d) + mean(0xff,d);
            if (can_show(p)) {
                SetLed(p.x, p.y, p.z, col);
            }
        }
        Show();
        Wait(50);
    };
    for (int ix = 0; ix < N0; ++ix) {
        show(0, ix*1.0/N0);
    }
    for (int ix = 0; ix < N1; ++ix) {
        show(ix*1.0/N1, 1);
    }
    for (int ix = 0; ix < N0/2; ++ix) {
        show(1, 1);
    }
    for (int ix = 0; ix < N0; ++ix) {
        show(1, (N0 - ix)*1.0 / N0);
    }
}

template< typename t >
t max_(t const & a, t const & b){
    return a < b ? b : a;
}

template< typename t >
t max_(t const & a, t const & b, t const & c){
    return max_(a, max_(b, c));
}

void wave()
{
    const int N0 = 300;
    for (int i = 0; i < N0; ++i){
        Clear();
        double it = 3.14 * 10 / N0;
        double xt = 3.14 * 2;
        double yt = 3.14 * 2 * 1.41;
        double LT = 20.0;
        double light = i < LT ? i / LT : N0 - i<LT ? (N0 - i) / LT : 1;
        for (int iy = 0; iy < LED_HEIGHT; ++iy){
            for (int ix = 0; ix < LED_WIDTH; ++ix){
                double x = xt*(ix-LED_WIDTH/2)/LED_WIDTH*2;
                double y = yt*(iy - LED_HEIGHT / 2) / LED_HEIGHT*2;
                double iz = static_cast<int>(sin(x)*sin(y)*sin(i*it)*LED_DEPTH/2+ LED_DEPTH/2);
                auto col = [&](double t)->int{
                    double dx = (ix - LED_WIDTH / 2);
                    double dy = (iy - LED_HEIGHT / 2);
                    double d = std::pow(dx*dx + dy*dy, 0.7) / 10.0 + t + i*it;
                    double W = sin(3.1416 * 7 / 6);
                    double v = sin(d) < W ? 0 : (sin(d)-W) / (1-W);
                    return 255 & int(v * 255*light);
                };
                const double T = 3.1416 * 2 / 3;
                int c = col(T) * 1 + col(T*2) * 0x100 + col(T*3) * 0x10000;
                for (int dz : { -1, 0, 1 }){
                    if (can_show({ static_cast<double>(ix), static_cast<double>(iy), iz+dz })){
                        SetLed(ix, iy, iz+dz, c);
                    }
                }
            }
        }
        Show();
        Wait(50);
    }
}

int main(int argc, const char* argv[])
{
    static_cast<void>(argc); // unused
    static_cast<void>(argv); // unused
    if (1 < argc){
        SetUrl(argv[1]);
    }
    if(2 < argc){
        SetPort(atoi(argv[2]));
    }
    EnableSimulator(true);
    for (;;) {
        ShowMotioningText1("HelloWorld");
        wave();
        balls();
        gala();
        skewed_sphere();
        threed();
        repbang();
        color_cube ();
        spiral();
        cube();
        fireworks();
        brown();
        sphere();
    }
}
