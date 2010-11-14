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
