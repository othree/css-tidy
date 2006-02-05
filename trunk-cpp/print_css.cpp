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

extern vector<string> raw_template,html_template;

void csstidy::_convert_raw_css()
{
	vector<token> newtokens;
	
	css.sort();
        
    for (css_struct::iterator i = css.begin(); i != css.end(); ++i)
    {
        if (settings["sort_selectors"]) i->second.sort();
        if (i->first != "standard") {
            add_token(AT_START, i->first, true);
        }
        
        for(sstore::iterator j = i->second.begin(); j != i->second.end(); ++j)
        {
            if (settings["sort_properties"]) j->second.sort();
            add_token(SEL_START, j->first, true);
            
            for(umap<string,string>::iterator k = j->second.begin(); k != j->second.end(); ++k)
            {
                add_token(PROPERTY, k->first, true);
                add_token(VALUE, k->second, true);
            }
            
            add_token(SEL_END, j->first, true);
        }
        
        if (i->first != "standard") {
            add_token(AT_END, i->first, true);
        }
    }
}

void csstidy::print_css(string filename)
{
	if(css.empty() && charset == "" && namesp == "" && import.empty())
	{
		if(!settings["silent"]) cout << "Invalid CSS!" << endl;
		return;
	}

	ofstream file_output;
	if(filename != "")
	{
		file_output.open(filename.c_str(),ios::binary);
		if(file_output.bad())
		{
			if(!settings["silent"]) cout << "Error when trying to save the output file!" << endl;
			return;
		}
	}
	
	if(!settings["preserve_css"]) {
		_convert_raw_css();
	}

	stringstream out;
	
	css.sort();

	if(charset != "")
	{
		out << raw_template[0] << "@charset " << raw_template[5] << charset << raw_template[6] << raw_template[12];
	}
	
	if(import.size() > 0)
	{
		for(int i = 0; i < import.size(); i ++)
		{
			out << raw_template[0] << "@import " << raw_template[5] << import[i] << raw_template[6] << raw_template[12];
		}
	}
	
	if(namesp != "")
	{
		out << raw_template[0] << "@namespace " << raw_template[5] << namesp << raw_template[6] << raw_template[12];
	}
	
	for(css_struct::iterator i = css.begin(); i != css.end(); ++i )
	{
		if(i->first != "standard") out << raw_template[0] << i->first << raw_template[1];
		
		if(settings["sort_selectors"]) i->second.sort();
		
		for(sstore::iterator j = i->second.begin(); j != i->second.end(); ++j)
		{
			if(settings["sort_properties"]) j->second.sort();
			
			if(i->first != "standard") out << raw_template[10];
			
			out << ((j->first[0] != '@') ? raw_template[2] : raw_template[0]) << j->first;

			out << ((i->first != "standard") ? raw_template[11] : raw_template[3]);
			
			for(umap<string,string>::iterator k = j->second.begin(); k != j->second.end(); ++k)
			{				
				out <<  raw_template[4];
				if(settings["uppercase_properties"]) out << strtoupper(k->first); else out << k->first;
				out << raw_template[5] << ":" << k->second;
				
				// Remove last ; if necessary
				if(k.islast() && settings["remove_last_;"])
				{
					out << str_replace(";","",raw_template[6]);
				}
				else
				{
					out << raw_template[6];
				}

			}
			
			if (i->first != "standard") out << raw_template[10];
			out << raw_template[7];
			if ( (!j.islast() && i->first != "standard") || i->first == "standard") out << raw_template[8];
			out.flush();
		}
		
		if (i->first != "standard") out << raw_template[9];
	}
	
	string c,output;
	
	while(out.good())
	{
		getline(out,c);
		output += c + "\n";
	}
	
	output = trim(output);
		
	if(!settings["silent"]) {
		cout << endl << "Selectors: " << selectors << " | Properties: " << properties << endl;
		float ratio = round(((input_size - (float) output.length())/input_size)*100,2);
		float i_b = round(((float) input_size)/1024,3);
		float o_b = round(((float) output.length())/1024,3);
		cout << "Input size: " << i_b << "KiB  Output size: " << o_b << "KiB  Compression ratio: " << ratio << "%" << endl;
	}
	
	if(filename == "")
	{
		if(!settings["silent"]) cout << "-----------------------------------\n\n";
		cout << output << "\n";
	}
	else
	{
		file_output << output;
	}	
	
	if(logs.size() > 0 && !settings["silent"])
	{
		cout << "-----------------------------------\n\n";
		for(map<int, vector<message> >::iterator j = logs.begin(); j != logs.end(); j++ )
		{
			for(int i = 0; i < j->second.size(); ++i)
			{
				cout << j->first << ": " << j->second[i].m << "\n" ;
			}
		}
	}

	if(!settings["silent"]) {
		cout << "\n-----------------------------------" << endl << "CSSTidy " << CSSTIDY_VERSION << " by Florian Schmitz 2005" << endl;
	}
	file_output.close();
}
