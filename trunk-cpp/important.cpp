/*
 * This file is part of CSSTidy.
 *
 * CSSTidy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * CSSTidy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CSSTidy; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
#include "csspp_globals.hpp"
using namespace std;

extern vector<string> whitespace;

bool is_important(string value)
{
	// Remove whitespaces
	value = strtolower(str_replace(whitespace,"",value));

	if(value.length() > 9 && value.substr(value.length()-10,10) == "!important")
	{
		return true;
	}
	return false;
}


string gvw_important(string value)
{
	if(is_important(value))
	{
		value = trim(value);
		value = value.substr(0,value.length()-9);
		value = trim(value);
		value = value.substr(0,value.length()-1);
		value = trim(value);
	}
	return value;
}

string c_important(string value)
{
	if(is_important(value))
	{
		value = gvw_important(value) + " !important";
	}
	return value;
}
