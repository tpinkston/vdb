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

#ifndef VDB_SPATIAL_H
#define VDB_SPATIAL_H

#include "vdb_object.h"

namespace vdb
{
    // ------------------------------------------------------------------------
    class location12_t : public record_t
    {
      public:

        location12_t(void);
        location12_t(const location12_t &copy);
        virtual ~location12_t(void);

        float32_t get_length(void) const;

        virtual void clear(void);

        virtual void print(
            const std::string &prefix,
            std::ostream &ost
        ) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        float32_t x, y, z;

        static const uint32_t
            LENGTH;
    };

    // ------------------------------------------------------------------------
    class location24_t : public record_t
    {
      public:

        location24_t(void);
        location24_t(const location24_t &copy);
        virtual ~location24_t(void);

        float64_t get_length(void) const;

        void get_geodetic(
            float64_t &latitude,
            float64_t &longitude,
            float64_t &elevation
        ) const;

        void set_geodetic(
            float64_t latitude,
            float64_t longitude,
            float64_t elevation
        );

        std::string geodetic_str(void) const;

        virtual void clear(void);

        virtual void print(
            const std::string &prefix,
            std::ostream &ost
        ) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        float64_t x, y, z;

        static const uint32_t
            LENGTH;
    };

    // ------------------------------------------------------------------------
    class orientation_t : public record_t
    {
      public:

        orientation_t(void);
        orientation_t(const orientation_t &copy);
        virtual ~orientation_t(void);

        virtual void clear(void);

        virtual void print(
            const std::string &prefix,
            std::ostream &ost
        ) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        float32_t psi, theta, phi;

        static const uint32_t
            LENGTH;
    };

    // ------------------------------------------------------------------------
    class velocity_t : public record_t
    {
      public:

        velocity_t(void);
        velocity_t(const velocity_t &copy);
        virtual ~velocity_t(void);

        float32_t get_length(void) const;

        virtual void clear(void);

        virtual void print(
            const std::string &prefix,
            std::ostream &ost
        ) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        float32_t x, y, z;

        static const uint32_t
            LENGTH;
    };

    // ------------------------------------------------------------------------
    class dead_reckoning_t : public record_t
    {
      public:

        dead_reckoning_t(void);
        dead_reckoning_t(const dead_reckoning_t &copy);
        virtual ~dead_reckoning_t(void);

        float32_t get_length(void) const;

        virtual void clear(void);

        virtual void print(
            const std::string &prefix,
            std::ostream &ost
        ) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        uint8_t
            algorithm;
        uint8_t
            parameters[15];
        velocity_t
            linear_acceleration,
            angular_velocity;

        static const uint32_t
            LENGTH;
    };
}

#endif
