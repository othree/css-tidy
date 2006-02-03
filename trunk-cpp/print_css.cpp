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

void print_css(css_manage &css, string filename)
{
	if(css.css.empty() && css.charset == "" && css.namesp == "" && css.import.empty())
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

	stringstream out;
	
	css.css.sort();

	if(css.charset != "")
	{
		out << raw_template[0] << "@charset " << raw_template[5] << css.charset << raw_template[6] << raw_template[12];
	}
	
	if(css.import.size() > 0)
	{
		for(int i = 0; i < css.import.size(); i ++)
		{
			out << raw_template[0] << "@import " << raw_template[5] << css.import[i] << raw_template[6] << raw_template[12];
		}
	}
	
	if(css.namesp != "")
	{
		out << raw_template[0] << "@namespace " << raw_template[5] << css.namesp << raw_template[6] << raw_template[12];
	}
	
	for(css_struct::iterator i = css.css.begin(); i != css.css.end(); ++i )
	{
		if(i->first != "standard") out << raw_template[0] << i->first << raw_template[1];
		
		if(settings["sort_selectors"]) i->second.sort();
		
		for(sstore::iterator j = i->second.begin(); j != i->second.end(); ++j)
		{
			if(settings["sort_properties"]) j->second.sort();
			
			if(i->first != "standard") out << raw_template[10];
			
			if(css.comments.count(i->first + j->first) > 0)
			{
				out << raw_template[13] << "/*" << css.comments[i->first + j->first] << "*/" << raw_template[14];
			}
			
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
		cout << endl << "Selectors: " << css.selectors << " | Properties: " << css.properties << endl;
		float ratio = round(((css.input_size - (float) output.length())/css.input_size)*100,2);
		float i_b = round(((float) css.input_size)/1024,3);
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
	
	if(css.logs.size() > 0 && !settings["silent"])
	{
		cout << "-----------------------------------\n\n";
		for(map<int, vector<message> >::iterator j = css.logs.begin(); j != css.logs.end(); j++ )
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
