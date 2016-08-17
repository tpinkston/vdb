// ============================================================================
// VDB (VDIS Debugger)
// Tony Pinkston (git@github.com:tpinkston/vdb.git)
//
// VDB is free software: you can redistribute it and/or modify it under the 
// terms of the GNU General Public License as published by the Free Software 
// Foundation, either version 3 of the License, or (at your option) any later 
// version.
//
// VDB is distributed in the hope that it will be useful, but WITHOUT ANY 
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS 
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more 
// details (http://www.gnu.org/licenses).
// ============================================================================

#include "vdb_enums.h"
#include "vdb_hexadecimal.h"
#include "vdb_logger.h"
#include "vdb_spatial.h"

#include "vdb_byte_stream.h"
#include "vdb_string.h"

namespace
{
    const float64_t
        PI = 3.1415926535897932384626433,
        HALF_PI = (PI / 2.0),
        NEGATIVE_HALF_PI = (-PI / 2.0),
        DEGREES_PER_RADIAN = 57.2957795130823208768,
        RADIANS_PER_DEGREE = 0.01745329251994329576;

    // Constants based on WGS 84 Ellipsoid.
    const float64_t
        A = 6378137.0,
        A2 = (A * A),
        A_HALF = (A / 2.0),
        B = (0.9999944354799 / 4.0),
        C = (1.0 / 298.257223563),
        D = 0.100225438677758E+01,
        E = -0.393246903633930E-04,
        F = 0.241216653453483E+12,
        G = 0.133733602228679E+14,
        H = 0.984537701867943E+00,
        J = (A * (1.0 - C)),
        J2 = (J * J),
        J2_54 = (54.0 * J2),
        L = (C * (2.0 - C)), // (F)
        L1 = (L - 1.0),
        L2 = (L * L),
        L2_2 = (2.0 * L2),
        L4TH = (0.25 * L),
        L_SQRT = std::sqrt(L),
        L_SQRT_HALF = (std::sqrt(L) / 2.0),
        P = (J2 / A),
        Q = (A2 - J2),
        R = (Q / J2),
        LQ = (L * Q),
        THIRD = (1.0 / 3.0),
        AR1 = std::pow((A + 50005.0), 2.0),
        AR2 = (AR1 / std::pow((J + 50005.0), 2.0)),
        BR1 = std::pow((A - 10005.0), 2.0),
        BR2 = (BR1 / std::pow((J - 10005.0), 2.0));
}

const uint32_t
    vdb::location12_t::LENGTH = 12,
    vdb::location24_t::LENGTH = 24,
    vdb::orientation_t::LENGTH = 12,
    vdb::velocity_t::LENGTH = 12,
    vdb::dead_reckoning_t::LENGTH = 40;

// ----------------------------------------------------------------------------
vdb::location12_t::location12_t() : x(0.0), y(0.0), z(0.0)
{

}

// ----------------------------------------------------------------------------
vdb::location12_t::location12_t(const location12_t &copy) :
    x(copy.x),
    y(copy.y),
    z(copy.z)
{

}

// ----------------------------------------------------------------------------
vdb::location12_t::~location12_t()
{

}

// ----------------------------------------------------------------------------
float32_t vdb::location12_t::get_length(void) const
{
    return std::sqrt((x * x) + (y * y) + (z * z));
}

// ----------------------------------------------------------------------------
void vdb::location12_t::clear(void)
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

// ----------------------------------------------------------------------------
void vdb::location12_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix
           << "(" << float_to_string(x, 1, 3)
           << ", " << float_to_string(y, 1, 3)
           << ", " << float_to_string(z, 1, 3) << ")";
}

// ----------------------------------------------------------------------------
void vdb::location12_t::read(byte_stream &stream)
{
    stream.read(x);
    stream.read(y);
    stream.read(z);
}

// ----------------------------------------------------------------------------
void vdb::location12_t::write(byte_stream &stream) const
{
    stream.write(x);
    stream.write(y);
    stream.write(z);
}

// ----------------------------------------------------------------------------
vdb::location24_t::location24_t() : x(0.0), y(0.0), z(0.0)
{

}

// ----------------------------------------------------------------------------
vdb::location24_t::location24_t(const location24_t &copy) :
    x(copy.x),
    y(copy.y),
    z(copy.z)
{

}

// ----------------------------------------------------------------------------
vdb::location24_t::~location24_t()
{

}

// ----------------------------------------------------------------------------
float64_t vdb::location24_t::get_length(void) const
{

    return std::sqrt((x * x) + (y * y) + (z * z));
}

// ----------------------------------------------------------------------------
void vdb::location24_t::get_geodetic(
    float64_t &latitude,
    float64_t &longitude,
    float64_t &elevation) const
{
    bool
        complete = false;

    if (x == 0.0)
    {
        if (y > 0.0)
        {
            latitude = HALF_PI;
        }
        else if (y < 0.0)
        {
            longitude  = NEGATIVE_HALF_PI;
        }
        else if (z > 0)
        {
            latitude = HALF_PI;
            longitude  = 0.0;
            elevation = z;
            complete = true;
        }
        else if (z < 0.0)
        {
            latitude = NEGATIVE_HALF_PI;
            longitude  = 0.0;
            elevation = z;
            complete = true;
        }
        else
        {
            latitude = 0.0;
            longitude  = 0.0;
            elevation = 0.0;
            complete = true;
        }
    }

    if (not complete)
    {
        const float64_t
            w2 = ((x * x) + (y * y)),
            w = std::sqrt(w2),
            z2 = (z * z);

        const float64_t
            ar2 = (w2 + (AR2 * z2)),
            br2 = (w2 + (BR2 * z2));

        if ((br2 > BR1) && (ar2 < AR1))
        {
            float64_t
                top = z * (D + (E * w2 + F) / (G + w2 * H + z2)),
                top2 = (top * top),
                top2w2 = (top2 + w2),
                top2w2sqrt = std::sqrt(top2w2),
                r = (top2 / top2w2),
                n = (0.25 - (L4TH * r)),
                nb = (n + B),
                a = A / (nb + n / nb);

            latitude = std::atan(top / w);
            longitude  = std::atan2(y, x);

            if (r < 0.50)
            {
                elevation = (top2w2sqrt - a);
            }
            else
            {
                elevation = z / (top / top2w2sqrt) + (L1 * a);
            }
        }
        else
        {
            float64_t
                oz2 = (J2_54 * z2),
                mz2 = (w2 - (L1 * z2) - LQ),
                a = (oz2 / (mz2 * mz2)),
                s0 = (L2 * w2 * a / mz2),
                s1 = (s0 * (s0 + 2.0)),
                s2 = (1.0 + s0 + std::sqrt(s1)),
                s3 = std::pow(s2, THIRD),
                p = a / (3.0 * std::pow((s3 + (1.0 / s3) + 1.0), 2)),
                pT = (1.0 + (L2_2 * p)),
                pTsqrt = std::sqrt(pT),
                r0 = (pTsqrt * (1.0 + pTsqrt)),
                r1 = (1.0 + (1.0 / pTsqrt)),
                r2 = ((-p * (2.0 * (1.0 - L) * z2 / r0 + w2)) / A2),
                r3 = 0.0;

            if ((r1 + r2) > 0.0)
            {
                r3 = A * std::sqrt(.50 * (r1 + r2));
            }

            r3 -= (p * L * w / (1.0 + pTsqrt));

            float64_t
                r3L = (r3 * L),
                r3Lz2 = (std::pow((w - r3L), 2.0) + z2),
                v = std::sqrt(r3Lz2 - (L * z2)),
                zo = (P * z / v);

            latitude = std::atan((z + (R * zo)) / w);
            longitude  = std::atan2(y, x);
            elevation = std::sqrt(r3Lz2) * (1.0 - P / v);
        }

        latitude *= DEGREES_PER_RADIAN;
        longitude *= DEGREES_PER_RADIAN;
    }
}

// ----------------------------------------------------------------------------
void vdb::location24_t::set_geodetic(
    float64_t latitude,
    float64_t longitude,
    float64_t elevation)
{
    float64_t
        latitude_radians = (RADIANS_PER_DEGREE * latitude),
        longitude_radians = (RADIANS_PER_DEGREE * longitude),
        sin_latitude = std::sin(latitude_radians),
        sin_longitude = std::sin(longitude_radians),
        cos_latitude = std::cos(latitude_radians),
        cos_longitude = std::cos(longitude_radians),
        sin_latitude2 = (sin_latitude * sin_latitude),
        a25 = (0.25 - (L4TH * sin_latitude2)),
        ab25 = (a25 + B),
        a = (A / (ab25 + (a25 / ab25))),
        ae = (a + elevation);

    x = (ae * cos_latitude * cos_longitude);
    y = (ae * cos_latitude * sin_longitude);
    z = (((J2 / A2) * a + elevation) * sin_latitude);
}

// ----------------------------------------------------------------------------
std::string vdb::location24_t::geodetic_str(void) const
{
    std::ostringstream
        stream;
    float64_t
        latitude,
        longitude,
        elevation;

    get_geodetic(latitude, longitude, elevation);

    stream << "(" << float_to_string(latitude, 1, 5)
           << " lat, " << float_to_string(longitude, 1, 5)
           << " long, " << float_to_string(elevation, 1, 2)
           << " meters)";

    return stream.str();
}

// ----------------------------------------------------------------------------
void vdb::location24_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix
           << "(" << float_to_string(x, 1, 3)
           << ", " << float_to_string(y, 1, 3)
           << ", " << float_to_string(z, 1, 3) << ")";
}

// ----------------------------------------------------------------------------
void vdb::location24_t::read(byte_stream &stream)
{
    stream.read(x);
    stream.read(y);
    stream.read(z);
}

// ----------------------------------------------------------------------------
void vdb::location24_t::write(byte_stream &stream) const
{
    stream.write(x);
    stream.write(y);
    stream.write(z);
}

// ----------------------------------------------------------------------------
void vdb::location24_t::clear(void)
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

// ----------------------------------------------------------------------------
vdb::orientation_t::orientation_t() : psi(0.0), theta(0.0), phi(0.0)
{

}

// ----------------------------------------------------------------------------
vdb::orientation_t::orientation_t(const orientation_t &copy) :
    psi(copy.psi),
    theta(copy.theta),
    phi(copy.phi)
{

}

// ----------------------------------------------------------------------------
vdb::orientation_t::~orientation_t()
{

}

// ----------------------------------------------------------------------------
void vdb::orientation_t::clear(void)
{
    psi = 0.0;
    theta = 0.0;
    phi = 0.0;
}

// ----------------------------------------------------------------------------
void vdb::orientation_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix
           << "(psi: " << float_to_string(psi, 1, 3)
           << ", theta: " << float_to_string(theta, 1, 3)
           << ", phi: " << float_to_string(phi, 1, 3) << ")";
}

// ----------------------------------------------------------------------------
void vdb::orientation_t::read(byte_stream &stream)
{
    stream.read(psi);
    stream.read(theta);
    stream.read(phi);
}

// ----------------------------------------------------------------------------
void vdb::orientation_t::write(byte_stream &stream) const
{
    stream.write(psi);
    stream.write(theta);
    stream.write(phi);
}

// ----------------------------------------------------------------------------
vdb::velocity_t::velocity_t() : x(0.0), y(0.0), z(0.0)
{

}

// ----------------------------------------------------------------------------
vdb::velocity_t::velocity_t(const velocity_t &copy) :
    x(copy.x),
    y(copy.y),
    z(copy.z)
{

}

// ----------------------------------------------------------------------------
vdb::velocity_t::~velocity_t()
{

}

// ----------------------------------------------------------------------------
float32_t vdb::velocity_t::get_length(void) const {

    return std::sqrt((x * x) + (y * y) + (z * z));
}

// ----------------------------------------------------------------------------
void vdb::velocity_t::clear(void)
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}

// ----------------------------------------------------------------------------
void vdb::velocity_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix
           << "(" << float_to_string(x, 1, 3)
           << ", " << float_to_string(y, 1, 3)
           << ", " << float_to_string(z, 1, 3) << ")";
}

// ----------------------------------------------------------------------------
void vdb::velocity_t::read(byte_stream &stream)
{
    stream.read(x);
    stream.read(y);
    stream.read(z);
}

// ----------------------------------------------------------------------------
void vdb::velocity_t::write(byte_stream &stream) const
{
    stream.write(x);
    stream.write(y);
    stream.write(z);
}

// ----------------------------------------------------------------------------
vdb::dead_reckoning_t::dead_reckoning_t() : algorithm(0)
{
    std::memset(parameters, 0, 15);
}

// ----------------------------------------------------------------------------
vdb::dead_reckoning_t::dead_reckoning_t(
    const dead_reckoning_t &copy
) :
    algorithm(copy.algorithm),
    linear_acceleration(copy.linear_acceleration),
    angular_velocity(copy.angular_velocity)
{
    std::memcpy(parameters, copy.parameters, 15);
}

// ----------------------------------------------------------------------------
vdb::dead_reckoning_t::~dead_reckoning_t()
{

}

// ----------------------------------------------------------------------------
void vdb::dead_reckoning_t::clear(void)
{
    algorithm = 0;

    std::memset(parameters, 0, 15);

    linear_acceleration.clear();
    angular_velocity.clear();
}

// ----------------------------------------------------------------------------
void vdb::dead_reckoning_t::print(
    const std::string &prefix,
    std::ostream &stream) const
{
    stream << prefix << "dead_reckoning.algorithm "
           << (dead_reckoning_e)algorithm << " ["
           << (int)algorithm << "]" << std::endl
           << prefix << "dead_reckoning.linear_acceleration "
           << linear_acceleration << std::endl
           << prefix << "dead_reckoning.angular_velocity "
           << angular_velocity << std::endl
           << prefix << "dead_reckoning.parameters [ ";

    for(unsigned i = 0; i < 15; ++i)
    {
        stream << hexadecimal::str(parameters[i]) << ((i < 14) ? "-" : " ");
    }

    stream << "]" << std::endl;
}

// ----------------------------------------------------------------------------
void vdb::dead_reckoning_t::read(byte_stream &stream)
{
    stream.read(algorithm);

    for(unsigned i = 0; i < 15; ++i)
    {
        stream.read(parameters[i]);
    }

    stream.read(linear_acceleration);
    stream.read(angular_velocity);
}

// ----------------------------------------------------------------------------
void vdb::dead_reckoning_t::write(byte_stream &stream) const
{
    stream.write(algorithm);

    for(unsigned i = 0; i < 15; ++i)
    {
        stream.write(parameters[i]);
    }

    stream.write(linear_acceleration);
    stream.write(angular_velocity);
}
