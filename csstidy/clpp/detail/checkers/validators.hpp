// C++ Command line parameters parser.
// http://clp-parser.sourceforge.net/
//
// Copyright (C) Denis Shevchenko, 2010.
// shev.denis @ gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// See http://www.opensource.org/licenses/mit-license.php

#ifndef CLPP_DETAIL_VALIDATORS_HPP
#define CLPP_DETAIL_VALIDATORS_HPP

#include <boost/filesystem/operations.hpp>
#include <boost/asio/ip/address_v4.hpp>
#include <boost/asio/ip/address_v6.hpp>
#include <boost/regex.hpp>

/// \namespace clpp
/// \brief Main namespace of library.
namespace clpp {

/// \namespace clpp::detail
/// \brief Details of realization.
namespace detail {

inline std::string semantic_error_prefix() { return lib_prefix() + "Semantic error: "; }

inline void check_ipv4_validity( const std::string& address, const std::string& parameter_name ) {
    try {
        boost::asio::ip::address_v4 a( boost::asio::ip::address_v4::from_string( address ) );
    } catch ( const std::exception& /* exc */ ) {
        const std::string what_happened = semantic_error_prefix() 
                                          + "parameter '" + parameter_name + "'" 
                                          + " has invalid IPv4 value '" + address + "'!";
        throw std::invalid_argument( what_happened );
    }
}

inline void check_ipv6_validity( const std::string& address, const std::string& parameter_name ) {
    try {
        boost::asio::ip::address_v6 a( boost::asio::ip::address_v6::from_string( address ) );
    } catch ( const std::exception& /* exc */ ) {
        const std::string what_happened = semantic_error_prefix()
                                          + "parameter '" + parameter_name + "'" 
                                          + " has invalid IPv6 value '" + address + "'!";
        throw std::invalid_argument( what_happened );
    }
}

inline void check_ip_validity( const std::string& address, const std::string& parameter_name ) {
    try {
        check_ipv4_validity( address, parameter_name );
    } catch ( const std::exception& /* exc */ ) {
        try {
            check_ipv6_validity( address, parameter_name );
        } catch ( const std::exception& /* exc */ ) {
            const std::string what_happened = semantic_error_prefix()
                                              + "parameter '" + parameter_name + "'" 
                                              + " has invalid value '" + address + "' (not IPv4, not IPv6)!";
            throw std::invalid_argument( what_happened );
        }
    }
}

inline void check_path_existence( const std::string& path, const std::string& parameter_name ) {
	if ( !boost::filesystem::exists( path ) ) {
        const std::string what_happened = semantic_error_prefix()
                                          + "parameter '" + parameter_name + "'" 
                                          + " has invalid path value '" + path + "' (no such path)!";
        throw std::invalid_argument( what_happened );
    } else {}
}

inline void check_email_validity( const std::string& email, const std::string& parameter_name ) {
    const std::string all_acceptable_chars      = "[a-zA-Z0-9_.-]";
    const std::string repeat_one_or_more_times  = "{1,}";
    const std::string only_letters_and_numbers  = "[a-zA-Z0-9]";
    const std::string only_letters              = "[a-zA-Z]";
    const std::string repeat_as_domain          = "{2,6}";

    const std::string regular_expression_for_email_validity = 
            all_acceptable_chars + repeat_one_or_more_times
            + only_letters_and_numbers + repeat_one_or_more_times
            + "@"
            + only_letters_and_numbers + repeat_one_or_more_times
            + all_acceptable_chars + repeat_one_or_more_times
            + "."
            + only_letters + repeat_as_domain
            ;

    const boost::regex e( regular_expression_for_email_validity );
    if ( !regex_match( email, e ) ) {
        const std::string what_happened = semantic_error_prefix()
                                          + "parameter '" + parameter_name + "'"
                                          + " has invalid e-mail value '" + email + "'!";
        throw std::invalid_argument( what_happened );
    } else {}
}

} // namespace detail
} // namespace clpp

#endif // CLPP_DETAIL_VALIDATORS_HPP
