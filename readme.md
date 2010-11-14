# CSSTidy

## Requirements

   * [Python, for SCons](http://www.python.org/)
   * [SCons](http://www.scons.org/)
   * C++ compiler – g++ or MinGW

## Install

    git clone git://github.com/alexanderte/css-tidy.git
    cd css-tidy
    scons
    sudo cp release/csstidy/csstidy /usr/local/bin/

## Usage

    csstidy input_filename [
        --allow_html_in_templates=[false|true] |
        --compress_colors=[true|false] |
        --compress_font_weight=[true|false] |
        --discard_invalid_properties=[false|true] |
        --lowercase_s=[false|true] |
        --preserve_css=[false|true] |
        --remove_backslash=[true|false] |
        --remove_last_semicolon=[false|true] |
        --silent=[false|true] |
        --sort_properties=[false|true] |
        --sort_selectors=[false|true] |
        --timestamp=[false|true] |
        --merge_selectors=[2|1|0] |
        --case_properties=[0|1|2] |
        --optimise_shorthands=[1|2|0] |
        --template=[default|filename|low|high|highest] |
        output_filename ]*

## Templates

A template is a file that consists of strings seperated by `|`. They may contain
any other characters like newlines, tabs and HTML-tags for the visual formatting
(only <span> is allowed). 

1. string before @rule
2. string+bracket after @-rule
3. string before selector
4. string+bracket after selector
5. string before property
6. string after property+before value
7. string after value
8. closing bracket - selector
9. space between blocks {...}
10. closing bracket @-rule
11. indentation in @-rules
12. before comments
13. after comments
14. after last one-line @-rule

### Position of templates

([1] = first string, [2] = second string,...):

    [1]@charset [6]"utf-8"[7];

    [1]@import [6]"my.css"[7];
    [14]
    [1]@media screen[2] {
    [11][3]b[4] {
    [11][5]color:[6]blue[7];

    [11][8]}[9]
    [11]
    [11][3]a[4] {
    [11][5]color:[6]red[7];
    [11][8]}[10]
    }

    [3]a[4] {
    [5]color:[6]red[7];
    [8]}[9]

    [3]b[4] {

    [5]color:[6]blue[7];
    [8]}[9]
