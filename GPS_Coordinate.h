#pragma once
#include <algorithm>

/**
 * \brief GPS经纬度坐标类，成员变量只有经纬度，以便与其他二进制格式兼容。
 * 提供了加减运算，提供了若干坐标系之间转换的方法，包括静态方法和动态方法。
 * 不保存坐标系类型，需要用户自行记录，也因此各转换函数不会做合法性检查。
 * 参考：
 * https://github.com/DingSoung/CoordinateTransformation
 * https://github.com/wandergis/coordtransform
 */
class GPS_Coordinate
{
public:
    GPS_Coordinate():longitude(0.0), latitude(0.0)
    {
    }

    explicit GPS_Coordinate(double lng, double lat):
        longitude(lng), latitude(lat)
    {
    }

    template<typename T>
    explicit GPS_Coordinate(std::pair<T, T> point) :
        longitude(point.first), latitude(point.second)
    {
    }

    ~GPS_Coordinate();

    static GPS_Coordinate GCJ02toBD09(GPS_Coordinate src);

    const GPS_Coordinate GCJ02toBD09() const
    {
        return GCJ02toBD09(*this);
    }

    static GPS_Coordinate BD09toGCJ02(GPS_Coordinate src);

    const GPS_Coordinate BD09toGCJ02() const
    {
        return BD09toGCJ02(*this);
    }

    static GPS_Coordinate WGS84toGCJ02(GPS_Coordinate src);

    const GPS_Coordinate WGS84toGCJ02() const
    {
        return WGS84toGCJ02(*this);
    }

    static GPS_Coordinate GCJ02toWGS84(GPS_Coordinate src);

    const GPS_Coordinate GCJ02toWGS84() const
    {
        return GCJ02toWGS84(*this);
    }

    static GPS_Coordinate BD09toWGS84(GPS_Coordinate src);

    const GPS_Coordinate BD09toWGS84() const
    {
        return BD09toWGS84(*this);
    }

    static GPS_Coordinate WGS84toBD09(GPS_Coordinate src);

    const GPS_Coordinate WGS84toBD09() const
    {
        return WGS84toBD09(*this);
    }

    static GPS_Coordinate BD09toMC(GPS_Coordinate src);

    const GPS_Coordinate BD09toMC() const
    {
        return BD09toMC(*this);
    }

    static GPS_Coordinate MCtoBD09(GPS_Coordinate src);

    const GPS_Coordinate MCtoBD09() const
    {
        return MCtoBD09(*this);
    }

    const GPS_Coordinate operator +(GPS_Coordinate rhs) const;
    GPS_Coordinate& operator +=(GPS_Coordinate rhs);
    const GPS_Coordinate operator -(GPS_Coordinate rhs) const;
    GPS_Coordinate& operator -=(GPS_Coordinate rhs);
    const GPS_Coordinate operator -() const;

    // 经度
    double longitude;
    // 纬度
    double latitude;
};

