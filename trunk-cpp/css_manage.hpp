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
 
#ifndef HEADER_CSS_MANAGE
#define HEADER_CSS_MANAGE 

extern map<string,bool> settings;
extern map< string, vector<string> > shorthands;

class css_manage 
{ 
	public: 
		css_struct css; 
		int properties,selectors,input_size,output_size;
		string charset,namesp;
		vector<string> import;
		map<int, vector<message> > logs;
		map<string,string> comments;
	
	private:
		string tokens;
		int line;

	public:
	    css_manage();
	    	
	    // Various function to manage the CSS structure
		void add(const string &media, const string &selector, const string property, const string value, const bool ie = true);
	    void set(string& media, string& selector, string& property, string& value)  ;
		void remove(const string media,const string selector,const string property);
	    void copy(const string media, const string selector, const string media_new, const string selector_new);
	    string get(string media, string selector, string property);
	    void remove(const string media,const string selector);
	
	    // Adds a property-value pair to an existing property-block. ie=false forces deactivation of IE-Hacks
		void css_add_property(const string& media, const string& selector, const string& property, const string& value, const bool ie = true);
		
		// Add a message to the message log
		void log(const string msg, const message_type type, int iline = 0);
		
		// Parse a piece of CSS code
		void parse_css(string css_input);
		string optimise_subvalue(string subvalue, const string property);
		
		// Checks if the chat in istring at i is a token
		bool is_token(string& istring,const int i);
		
		// Parses unicode notations
		string unicode(string& istring,int& i);
		
		/* Merges properties like margin */
		void merge_4value_shorthands(string media, string selector);
		
		/* Dissolves properties like padding:10px 10px 10px to padding-top:10px;padding-bottom:10px;... */
		map<string,string> dissolve_4value_shorthands(string property, string value);
}; 

#endif // HEADER_CSS_MANAGE
