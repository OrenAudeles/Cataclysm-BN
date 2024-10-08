#include "ammo.h"

#include <unordered_map>

#include "debug.h"
#include "item.h"
#include "json.h"
#include "translations.h"
#include "string_id.h"
#include "type_id.h"

static const itype_id itype_UPS( "UPS" );

namespace
{
using ammo_map_t = std::unordered_map<ammotype, ammunition_type>;

ammo_map_t &all_ammunition_types()
{
    static ammo_map_t the_map;
    return the_map;
}
} //namespace

void ammunition_type::load_ammunition_type( const JsonObject &jsobj )
{
    ammunition_type &res = all_ammunition_types()[ ammotype( jsobj.get_string( "id" ) ) ];
    res.name_             = jsobj.get_string( "name" );
    jsobj.read( "default", res.default_ammotype_, true );
}

/** @relates string_id */
template<>
bool string_id<ammunition_type>::is_valid() const
{
    return all_ammunition_types().contains( *this );
}

/** @relates string_id */
template<>
const ammunition_type &string_id<ammunition_type>::obj() const
{
    const auto &the_map = all_ammunition_types();

    const auto it = the_map.find( *this );
    if( it != the_map.end() ) {
        return it->second;
    }

    debugmsg( "Tried to get invalid ammunition: %s", c_str() );
    static const ammunition_type null_ammunition {
        "null"
    };
    return null_ammunition;
}

void ammunition_type::reset()
{
    all_ammunition_types().clear();
}

void ammunition_type::check_consistency()
{
    for( const auto &ammo : all_ammunition_types() ) {
        const auto &id = ammo.first;
        const auto &at = ammo.second.default_ammotype_;

        // TODO: these ammo types should probably not have default ammo at all.
        if( at == itype_UPS || at.str() == "components" || at.str() == "thrown" ) {
            continue;
        }

        if( !at.is_empty() && !at.is_valid() ) {
            debugmsg( "ammo type %s has invalid default ammo %s", id.c_str(), at.c_str() );
        }
    }
}

std::string ammunition_type::name() const
{
    return _( name_ );
}
