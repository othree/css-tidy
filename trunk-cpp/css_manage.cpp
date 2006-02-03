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

css_manage::css_manage()
{ 
	properties = 0;
	selectors = 0;
	charset = "";
	namesp = "";
	line = 1;
	tokens = "{};:()@='\"/,\\!$%&*+.<>?[]^`|~";
} 

void css_manage::add(const string& media,const string& selector,const string property,const string value, const bool ie)  
{
	if(css[media].has(selector))
	{
		css_add_property(media,selector,property,value,ie);
	}
	else
	{
		css[media][selector][property] = value;
	}
}

void css_manage::set(string& media, string& selector, string& property, string& value)  
{
	if(css[media].has(selector))
	{
		css[media][selector][property] = value;
	}
	else
	{
		css[media][selector][property] = value;
	}
} 

void css_manage::remove(const string media,const string selector,const string property) 
{
	css[media][selector].erase(property);
}

string css_manage::get(string media, string selector, string property)
{
	return css[media][selector][property];
}

void css_manage::copy(const string media, const string selector, const string media_new, const string selector_new)
{
	for(int k = 0; k < css[media][selector].size(); k++)
	{	
		string property = css[media][selector].at(k);
		string value = css[media][selector][property];
		add(media_new,selector_new,property,value);
	}
}

void css_manage::remove(string media, string selector) 
{ 
	css[media].erase(selector);
}

// Adds a property-value pair to an existing property-block. ie=false forces deactivation of IE-Hacks
void css_manage::css_add_property(const string& media, const string& selector, const string& property, const string& value, const bool ie)
{
	if(settings["save_ie_hacks"] && ie)
	{
		// if property does exist and is a hack, but selector is not
		if(!is_important(value) && css[media][selector].has(property) && is_important(css[media][selector][property]) && is_no_hack(selector))
		{
			log("Converted !important-hack in selector \"" + selector + "\"",Information);
			if(trim(selector).substr(0,4) == "html")
			{
				add(media,"* " + selector,property,value + " !important",false);
			}
			else
			{
				add(media,"* html " + selector,property,value + " !important",false);
			}
		}
		else
		{
			add(media,selector,property,value,false);
		}
	}
	else
	{
		if(css[media][selector].has(property))
		{
			if( (is_important(css[media][selector][property]) && is_important(value)) || !is_important(css[media][selector][property]) )
			{
				remove(media,selector,property);
				css[media][selector][property] = trim(value);
			}
		}
		else
		{
			css[media][selector][property] = trim(value);
		}
	}
}

void css_manage::log(const string msg, const message_type type, int iline)
{
	message new_msg;
	new_msg.m = msg;
	new_msg.t = type;
	if(iline == 0)
	{
		iline = line;
	}
	if(logs.count(line) > 0)
	{
		for(int i = 0; i < logs[line].size(); ++i)
		{
			if(logs[line][i].m == new_msg.m && logs[line][i].t == new_msg.t)
			{
				return;
			}
		}
	}
	logs[line].push_back(new_msg);
}

string css_manage::unicode(string& istring,int& i)
{
	++i;
	string add = "";
	bool replaced = false;
	
	while(i < istring.length() && (ctype_xdigit(istring[i]) || ctype_space(istring[i])) && add.length()< 6)
	{
		add += istring[i];

		if(ctype_space(istring[i]))
		{
			break;
		}
		i++;
	}

	if(hexdec(add) > 47 && hexdec(add) < 58 || hexdec(add) > 64 && hexdec(add) < 91 || hexdec(add) > 96 && hexdec(add) < 123)
	{
		string msg = "Replaced unicode notation: Changed \\" + rtrim(add) + " to ";
		add = static_cast<int>(hexdec(add));
		msg += add;
		log(msg,Information);
		replaced = true;
	}
	else
	{
		add = trim("\\" + add);
	}

	if(ctype_xdigit(istring[i+1]) && ctype_space(istring[i]) && !replaced || !ctype_space(istring[i]))
	{
		i--;
	}
	
	if(add != "\\" || !settings["remove_bslash"] || in_str_array(tokens,istring[i+1]))
	{
		return add;
	}
	if(add == "\\")
	{
		log("Removed unnecessary backslash",Information);
	}
	return "";
}

bool css_manage::is_token(string& istring,const int i)
{
	if(in_str_array(tokens,istring[i]) && !escaped(istring,i))
	{
		return true;
	}
	return false;
}

void css_manage::merge_4value_shorthands(string media, string selector)
{
	for(map< string, vector<string> >::iterator i = shorthands.begin(); i != shorthands.end(); ++i )
	{
		string temp;

		if(css[media][selector].has(i->second[0]) && css[media][selector].has(i->second[1])
		&& css[media][selector].has(i->second[2]) && css[media][selector].has(i->second[3]))
		{
			string important = "";
			for(int j = 0; j < 4; ++j)
			{
				string val = get(media, selector, i->second[j]);
				if(is_important(val))
				{
					important = " !important";
					temp += gvw_important(val)+ " ";
				}
				else
				{
					temp += val + " ";
				}
				remove(media, selector, i->second[j]);
			}
			add(media, selector, i->first, shorthand(trim(temp + important)));		
		}
	}
} 

map<string,string> css_manage::dissolve_4value_shorthands(string property, string value)
{
	map<string, string> ret;
	extern map< string, vector<string> > shorthands;
	
	if(shorthands[property][0] == "0")
	{
		ret[property] = value;
		return ret;
	}
	
	string important = "";
	if(is_important(value))
	{
		value = gvw_important(value);
		important = " !important";
	}
	vector<string> values = explode(" ",value);

	if(values.size() == 4)
	{
		for(int i=0; i < 4; ++i)
		{
			ret[shorthands[property][i]] = values[i] + important;
		}
	}
	else if(values.size() == 3)
	{
		ret[shorthands[property][0]] = values[0] + important;
		ret[shorthands[property][1]] = values[1] + important;
		ret[shorthands[property][3]] = values[1] + important;
		ret[shorthands[property][2]] = values[2] + important;
	}
	else if(values.size() == 2)
	{
		for(int i = 0; i < 4; ++i)
		{
			ret[shorthands[property][i]] = ((i % 2 != 0)) ? values[1] + important : values[0] + important;
		}
	}
	else
	{
		for(int i = 0; i < 4; ++i)
		{
			ret[shorthands[property][i]] = values[0] + important;
		}	
	}
	
	return ret;
}
