#pragma once

#include <cstdint>

#include "units_def.h"


namespace units
{

class health_ratio_tag {};

using health = quantity<std::int32_t, health_ratio_tag>;

constexpr health health_min = units::health( -2000000, units::health::unit_type{} );
constexpr health health_max = units::health( +2000000, units::health::unit_type{} );
constexpr health health_zero = units::health( 0, units::health_ratio_tag{} );

template<typename value_type>
constexpr value_type to_raw_health( const quantity<value_type, health_ratio_tag> &v )
{
    return v.value();
}

template<typename value_type>
constexpr value_type to_unit_health( const quantity<value_type, health_ratio_tag> &v )
{
    return v.value() / 10000.0;
}


template<typename value_type>
constexpr quantity<value_type, health_ratio_tag> from_raw_health(
    const value_type v )
{
    return quantity<value_type, health_ratio_tag>( v, health_ratio_tag{} );
}

template<typename value_type>
constexpr quantity<value_type, health_ratio_tag> from_unit_health(
    const value_type v )
{
    return from_raw_health( v * 10000 );
}

} // namespace units
