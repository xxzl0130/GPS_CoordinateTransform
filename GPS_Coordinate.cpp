#include "Coordinate.h"
#define _USE_MATH_DEFINES
#include <math.h>

// 一些常量
static constexpr double pi = M_PI;
static constexpr double xPi = M_PI * 3000 / 180.0;
static constexpr double ee = 0.00669342162296594323;
static constexpr double a = 6378245.0;
constexpr static double MCBAND[] = {12890594.86, 8362377.87, 5591021.0, 3481989.83, 1678043.12, 0.0};
constexpr static double LLBAND[] = {75.0, 60.0, 45.0, 30.0, 15.0, 0.0};
constexpr static double MC2LL[][10] = {
    {1.410526172116255e-8, 0.00000898305509648872, -1.9939833816331, 200.9824383106796, -187.2403703815547,
     91.6087516669843, -23.38765649603339, 2.57121317296198, -0.03801003308653, 17337981.2},
    {-7.435856389565537e-9, 0.000008983055097726239, -0.78625201886289, 96.32687599759846, -1.85204757529826,
     -59.36935905485877, 47.40033549296737, -16.50741931063887, 2.28786674699375, 10260144.86},
    {-3.030883460898826e-8, 0.00000898305509983578, 0.30071316287616, 59.74293618442277, 7.357984074871,
     -25.38371002664745, 13.45380521110908, -3.29883767235584, 0.32710905363475, 6856817.37},
    {-1.981981304930552e-8, 0.000008983055099779535, 0.03278182852591, 40.31678527705744, 0.65659298677277,
     -4.44255534477492, 0.85341911805263, 0.12923347998204, -0.04625736007561, 4482777.06},
    {3.09191371068437e-9, 0.000008983055096812155, 0.00006995724062, 23.10934304144901, -0.00023663490511,
     -0.6321817810242, -0.00663494467273, 0.03430082397953, -0.00466043876332, 2555164.4},
    {2.890871144776878e-9, 0.000008983055095805407, -3.068298e-8, 7.47137025468032, -0.00000353937994,
     -0.02145144861037, -0.00001234426596, 0.00010322952773, -0.00000323890364, 826088.5}};
constexpr static double LL2MC[][10] = {
    {-0.0015702102444, 111320.7020616939, 1704480524535203.0, -10338987376042340.0, 26112667856603880.0,
     -35149669176653700.0, 26595700718403920.0, -10725012454188240.0, 1800819912950474.0, 82.5},
    {0.0008277824516172526, 111320.7020463578, 647795574.6671607, -4082003173.641316, 10774905663.51142,
     -15171875531.51559, 12053065338.62167, -5124939663.577472, 913311935.9512032, 67.5},
    {0.00337398766765, 111320.7020202162, 4481351.045890365, -23393751.19931662, 79682215.47186455,
     -115964993.2797253, 97236711.15602145, -43661946.33752821, 8477230.501135234, 52.5},
    {0.00220636496208, 111320.7020209128, 51751.86112841131, 3796837.749470245, 992013.7397791013,
     -1221952.21711287, 1340652.697009075, -620943.6990984312, 144416.9293806241, 37.5},
    {-0.0003441963504368392, 111320.7020576856, 278.2353980772752, 2485758.690035394, 6070.750963243378,
     54821.18345352118, 9540.606633304236, -2710.55326746645, 1405.483844121726, 22.5},
    {-0.0003218135878613132, 111320.7020701615, 0.00369383431289, 823725.6402795718, 0.46104986909093,
     2351.343141331292, 1.58060784298199, 8.77738589078284, 0.37238884252424, 7.45}};

static double transformLat(GPS_Coordinate point);
static double transformLng(GPS_Coordinate point);
static bool isOutOfChina(GPS_Coordinate point);
static GPS_Coordinate converter(const GPS_Coordinate &pos, const double p[10]);
static double getLoop(double x, double low, double high);
static double getRange(double x, double low, double high);

#define deg2rad(x) ((x) / 180.0 * pi)
#define rad2deg(x) ((x) / pi * 180.0)

GPS_Coordinate::~GPS_Coordinate()
{
}

double transformLat(GPS_Coordinate point)
{
    const auto lat = point.latitude - 35;
    const auto lng = point.longitude - 105;
    double ret = -100.0 + 2.0 * lat + 3.0 * lng + 0.2 * lng * lng + 0.1 * lat * lng + 0.2 * sqrt(fabs(lat));
    ret += (20.0 * sin(6.0 * lat * pi) + 20.0 * sin(2.0 * lat * pi)) * 2.0 / 3.0;
    ret += (20.0 * sin(lng * pi) + 40.0 * sin(lng / 3.0 * pi)) * 2.0 / 3.0;
    ret += (160.0 * sin(lng / 12.0 * pi) + 320 * sin(lng * pi / 30.0)) * 2.0 / 3.0;
    return ret;
}

double transformLng(GPS_Coordinate point)
{
    const auto lat = point.latitude - 35;
    const auto lng = point.longitude - 105;
    double ret = 300.0 + lat + 2.0 * lng + 0.1 * lat * lat + 0.1 * lat * lng + 0.1 * sqrt(fabs(lat));
    ret += (20.0 * sin(6.0 * lat * pi) + 20.0 * sin(2.0 * lat * pi)) * 2.0 / 3.0;
    ret += (20.0 * sin(lat * pi) + 40.0 * sin(lat / 3.0 * pi)) * 2.0 / 3.0;
    ret += (150.0 * sin(lat / 12.0 * pi) + 300.0 * sin(lat / 30.0 * pi)) * 2.0 / 3.0;
    return ret;
}

bool isOutOfChina(GPS_Coordinate point)
{
    return !(point.longitude > 73.66 && point.longitude < 135.05 && point.latitude > 3.86 && point.latitude < 53.55);
}

GPS_Coordinate converter(const GPS_Coordinate &pos, const double p[10])
{
    auto xTemp = p[0] + p[1] * abs(pos.longitude);
    auto cC = abs(pos.latitude) / p[9];
    auto yTemp = p[2];
    for (auto i = 1; i <= 6; ++i)
    {
        yTemp += p[2 + i] * pow(cC, i);
    }
    xTemp *= (pos.longitude < 0 ? -1 : 1);
    yTemp *= (pos.latitude < 0 ? -1 : 1);
    return GPS_Coordinate(xTemp, yTemp);
}

double getLoop(double x, double low, double high)
{
    auto dif = high - low;
    while (x > high)
    {
        x -= dif;
    }
    while (x < low)
    {
        x += dif;
    }
    return x;
}

double getRange(double x, double low, double high)
{
    return x > high ? high : (x < low ? low : x);
}

GPS_Coordinate GPS_Coordinate::GCJ02toBD09(GPS_Coordinate src)
{
    const auto &x = src.longitude;
    const auto &y = src.latitude;
    const auto z = sqrt(x * x + y * y) + 0.00002 * sin(y * xPi);
    const auto theta = atan2(y, x) + 0.000003 * cos(x * xPi);
    return GPS_Coordinate(z * cos(theta) + 0.0065, z * sin(theta) + 0.006);
}

GPS_Coordinate GPS_Coordinate::BD09toGCJ02(GPS_Coordinate src)
{
    const auto x = src.longitude - 0.0065;
    const auto y = src.latitude - 0.006;
    const auto z = sqrt(x * x + y * y) - 0.00002 * sin(y * xPi);
    const auto theta = atan2(y, x) - 0.000003 * cos(x * xPi);
    return GPS_Coordinate(z * cos(theta), z * sin(theta));
}

GPS_Coordinate GPS_Coordinate::WGS84toGCJ02(GPS_Coordinate src)
{
    if (isOutOfChina(src))
    {
        return src;
    }
    auto lat = transformLat(src);
    auto lng = transformLng(src);
    auto radLat = deg2rad(src.latitude);
    auto magic = sin(radLat);
    magic = 1 - ee * magic * magic;
    double sqrtmagic = sqrt(magic);
    lat = (lat * 180.0) / ((a * (1 - ee)) / (magic * sqrtmagic) * pi);
    lng = (lng * 180.0) / (a / sqrtmagic * cos(radLat) * pi);
    return src + GPS_Coordinate(lng, lat);
}

GPS_Coordinate GPS_Coordinate::GCJ02toWGS84(GPS_Coordinate src)
{
    if (isOutOfChina(src))
    {
        return src;
    }
    auto lat = transformLat(src - GPS_Coordinate(35.0, 105.0));
    auto lng = transformLng(src - GPS_Coordinate(35.0, 105.0));
    auto radLat = deg2rad(src.latitude);
    auto magic = sin(radLat);
    magic = 1 - ee * magic * magic;
    double sqrtmagic = sqrt(magic);
    lat = (lat * 180.0) / ((a * (1 - ee)) / (magic * sqrtmagic) * pi);
    lng = (lng * 180.0) / (a / sqrtmagic * cos(radLat) * pi);
    return GPS_Coordinate(lng, lat);
}

GPS_Coordinate GPS_Coordinate::BD09toWGS84(GPS_Coordinate src)
{
    return GCJ02toWGS84(BD09toGCJ02(src));
}

GPS_Coordinate GPS_Coordinate::WGS84toBD09(GPS_Coordinate src)
{
    return GCJ02toBD09(WGS84toGCJ02(src));
}

GPS_Coordinate GPS_Coordinate::BD09toMC(GPS_Coordinate src)
{
    if (isOutOfChina(src))
        return GPS_Coordinate();
    auto lng = getLoop(src.longitude, -180.0, 180.0);
    auto lat = getRange(src.latitude, -74, 74);
    double const *p = nullptr;
    for (auto i = 0; i < sizeof(LLBAND) / sizeof(LLBAND[0]); ++i)
    {
        if (lat >= LLBAND[i])
        {
            p = LL2MC[i];
            break;
        }
    }
    if (p == nullptr)
    {
        for (auto i = sizeof(LLBAND) / sizeof(LLBAND[0]) - 1; i >= 0; --i)
        {
            if (lat <= -LLBAND[i])
            {
                p = LL2MC[i];
                break;
            }
        }
    }
    return converter(src, p);
}

GPS_Coordinate GPS_Coordinate::MCtoBD09(GPS_Coordinate src)
{
    auto x = abs(src.longitude);
    auto y = abs(src.latitude);
    for (auto i = 0; i < sizeof(MCBAND) / sizeof(MCBAND[0]); ++i)
    {
        if (y >= MCBAND[i])
        {
            return converter(src, MC2LL[i]);
        }
    }
    return GPS_Coordinate();
}

const GPS_Coordinate GPS_Coordinate::operator+(GPS_Coordinate rhs) const
{
    return GPS_Coordinate(this->longitude + rhs.longitude, this->latitude + rhs.latitude);
}

GPS_Coordinate &GPS_Coordinate::operator+=(GPS_Coordinate rhs)
{
    this->longitude += rhs.longitude;
    this->latitude += rhs.latitude;
    return *this;
}

const GPS_Coordinate GPS_Coordinate::operator-(GPS_Coordinate rhs) const
{
    return GPS_Coordinate(this->longitude - rhs.longitude, this->latitude - rhs.latitude);
}

GPS_Coordinate &GPS_Coordinate::operator-=(GPS_Coordinate rhs)
{
    this->longitude -= rhs.longitude;
    this->latitude -= rhs.latitude;
    return *this;
}

const GPS_Coordinate GPS_Coordinate::operator-() const
{
    return GPS_Coordinate(-this->longitude, -this->latitude);
}
