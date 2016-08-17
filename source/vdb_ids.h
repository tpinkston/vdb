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

#ifndef VDB_IDS_H
#define VDB_IDS_H

#include "vdb_object.h"

namespace vdb
{
    class entity_marking_t;

    // ------------------------------------------------------------------------
    class simulation_id_t : public record_t
    {
      public:

        simulation_id_t(void);
        simulation_id_t(uint16_t, uint16_t);
        simulation_id_t(const simulation_id_t &copy);
        virtual ~simulation_id_t(void);

        virtual bool is_none(void) const;
        virtual bool is_all(void) const;

        virtual void clear(void);

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        virtual bool operator==(const simulation_id_t &) const;
        virtual bool operator!=(const simulation_id_t &) const;
        virtual bool operator<(const simulation_id_t &) const;
        virtual bool operator>(const simulation_id_t &) const;

        bool matches(const simulation_id_t &id) const;

        uint16_t
            site,
            application;

        static const uint32_t
            LENGTH;
        static const uint16_t
            NONE = 0,
            ALL = 65535;
    };

    // ------------------------------------------------------------------------
    class id_t : public simulation_id_t
    {
      public:

        id_t(void);
        id_t(uint16_t, uint16_t, uint16_t);
        id_t(const id_t &copy);
        virtual ~id_t(void);

        virtual bool is_none(void) const;
        virtual bool is_all(void) const;

        virtual void clear(void);

        virtual void print_full(std::ostream &) const;

        virtual void print(const std::string &prefix, std::ostream &) const;

        virtual uint32_t length(void) const { return LENGTH; }

        virtual void read(byte_stream &stream);
        virtual void write(byte_stream &stream) const;

        virtual bool operator==(const id_t &) const;
        virtual bool operator!=(const id_t &) const;
        virtual bool operator<(const id_t &) const;
        virtual bool operator>(const id_t &) const;

        bool matches(const id_t &id) const;

        uint16_t
            identity;

        static const uint32_t
            LENGTH;
    };

    // ------------------------------------------------------------------------
    class id_mappings
    {
      public:

        // --------------------------------------------------------------------
        // Entity ID to entity markings

        static const entity_marking_t *get_marking(const id_t &);

        static void set_marking(
            const id_t &id,
            const entity_marking_t &value
        );

      private:

        static std::map<id_t, entity_marking_t>
            entity_markings;
    };
}

#endif
