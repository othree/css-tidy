<?php
header('Content-Type:text/html; charset=utf-8');
require('class.csstidy.php');
require('lang.inc.php');


if (isset($_REQUEST['css_text']) && get_magic_quotes_gpc()) {
 	$_REQUEST['css_text'] = stripslashes($_REQUEST['css_text']);
}

function rmdirr($dirname,$oc=0)
{
	// Sanity check
	if (!file_exists($dirname)) {
	  return false;
	}
	// Simple delete for a file
	if (is_file($dirname) && (time()-fileatime($dirname))>3600) {
	   return unlink($dirname);
	}
	// Loop through the folder
	if(is_dir($dirname))
	{
	$dir = dir($dirname);
	while (false !== $entry = $dir->read()) {
	   // Skip pointers
	   if ($entry == '.' || $entry == '..') {
		   continue;
	   }
	   // Recurse
	   rmdirr("$dirname/$entry",$oc);
	}
	$dir->close();
	}
	// Clean up
	if ($oc==1)
	{
		return rmdir($dirname);
	}
}

function options($array_options,$string_selected,$array_values = false)
{
	$return = '';
	foreach($array_options as $key => $option)
	{
		if($option === $string_selected)
		{
			$return .= '<option';
			if($array_values !== false) $return .= ' value="'.$array_values[$key].'"';
			$return .= ' selected="selected">'.$option.'</option>';
		}
		else
		{
			$return .= '<option';
			if($array_values !== false) $return .= ' value="'.$array_values[$key].'" ';
			$return .= '>'.$option.'</option>';
		}
	}
	return $return;
}

$css = new csstidy();
if(isset($_REQUEST['custom']) && !empty($_REQUEST['custom']))
{
    setcookie ('custom_template', $_REQUEST['custom'], time()+360000);
}
rmdirr('temp');

if(isset($_REQUEST['case_properties'])) $css->set_cfg('case_properties',$_REQUEST['case_properties']);
if(isset($_REQUEST['lowercase'])) $css->set_cfg('lowercase_s',true);
if(!isset($_REQUEST['compress_c']) && isset($_REQUEST['post'])) $css->set_cfg('compress_colors',false);
if(!isset($_REQUEST['compress_fw']) && isset($_REQUEST['post'])) $css->set_cfg('compress_font-weight',false);
if(isset($_REQUEST['merge_selectors'])) $css->set_cfg('merge_selectors', $_REQUEST['merge_selectors']);
if(!isset($_REQUEST['optimise_shorthands']) && isset($_REQUEST['post'])) $css->set_cfg('optimise_shorthands',false);
if(!isset($_REQUEST['rbs']) && isset($_REQUEST['post'])) $css->set_cfg('remove_bslash',false);
if(!isset($_REQUEST['preserve_css']) && isset($_REQUEST['post'])) $css->set_cfg('preserve_css',false);
if(isset($_REQUEST['sort_sel'])) $css->set_cfg('sort_selectors',true);
if(isset($_REQUEST['sort_de'])) $css->set_cfg('sort_properties',true);
if(isset($_REQUEST['remove_last_sem'])) $css->set_cfg('remove_last_;',true);
if(isset($_REQUEST['discard'])) $css->set_cfg('discard_invalid_properties',true);
if(isset($_REQUEST['css_level'])) $css->set_cfg('css_level',$_REQUEST['css_level']);
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
  <head>
    <title>
      <?php echo $lang[$l][0]; echo $css->version; ?>)
    </title>
    <meta http-equiv="Content-Type"
    content="application/xhtml+xml; charset=utf-8" />
    <link rel="stylesheet" href="cssparse.css" type="text/css" />
  </head>
  <body>
    <div><h1 style="display:inline">
      <?php echo $lang[$l][1]; ?>
    </h1>
    <?php echo $lang[$l][2]; ?> <a
      href="http://csstidy.sourceforge.net/">csstidy</a> <?php echo $css->version; ?>)
    </div><p>
    <?php echo $lang[$l][39]; ?>: <a hreflang="en" href="?lang=en">English</a> <a hreflang="de" href="?lang=de">Deutsch</a> <a hreflang="fr" href="?lang=fr">French</a></p>
    <p><?php echo $lang[$l][4]; ?>
      <?php echo $lang[$l][6]; ?>
    </p>

    <form method="post" action="">
      <div>
        <fieldset id="field_input">
          <legend><?php echo $lang[$l][8]; ?></legend> <label for="css_text"
          class="block"><?php echo $lang[$l][9]; ?></label><textarea id="css_text" name="css_text" rows="20" cols="35"><?php if(isset($_REQUEST['css_text'])) echo htmlspecialchars($_REQUEST['css_text']); ?></textarea>
            <label for="url"><?php echo $lang[$l][10]; ?></label> <input type="text"
          name="url" id="url" <?php if(isset($_REQUEST['url']) &&
          !empty($_REQUEST['url'])) echo 'value="'.$_REQUEST['url'].'"'; ?>
          size="35" /><br />
          <input type="submit" value="<?php echo $lang[$l][35]; ?>" id="submit" />
        </fieldset>
        <div id="rightcol">
          <fieldset id="code_layout">
            <legend><?php echo $lang[$l][11]; ?></legend> <label for="template"
            class="block"><?php echo $lang[$l][12]; ?></label> <select
            id="template" name="template" style="margin-bottom:1em;">
              <?php if(isset($_REQUEST['template']) &&
              is_numeric($_REQUEST['template'])) $num = $_REQUEST['template']; else
              $num = 1; ?>
              <option value="3" <?php if($num==3) echo 'selected="selected"';
              ?>>
                <?php echo $lang[$l][13]; ?>
              </option>
              <option value="2" <?php if($num==2) echo 'selected="selected"';
              ?>>
              <?php echo $lang[$l][14]; ?>
              </option>
              <option value="1" <?php if($num==1) echo 'selected="selected"';
              ?>>
              <?php echo $lang[$l][15]; ?>
              </option>
              <option value="0" <?php if($num==0) echo 'selected="selected"';
              ?>>
              <?php echo $lang[$l][16]; ?>                
              </option>
              <option value="4" <?php if($num==4) echo 'selected="selected"';
              ?>>
              <?php echo $lang[$l][17]; ?> 
              </option>
            </select><br />
            <label for="custom" class="block">
            <?php echo $lang[$l][18]; ?> </label> <textarea id="custom"
            name="custom" cols="33" rows="4"><?php
               if(isset($_REQUEST['custom']) && !empty($_REQUEST['custom'])) echo
              htmlspecialchars($_REQUEST['custom']);
               elseif(isset($_COOKIE['custom_template']) &&
              !empty($_COOKIE['custom_template'])) echo
              htmlspecialchars($_COOKIE['custom_template']);
               ?></textarea>
          </fieldset>
          <fieldset id="options">
         <legend><?php echo $lang[$l][19]; ?></legend>
         
            <input type="checkbox" name="preserve_css" id="preserve_css"
                   <?php if($css->get_cfg('preserve_css')) echo 'checked="checked"'; ?> />
            <label for="preserve_css" title="<?php echo $lang[$l][52]; ?>" class="help"><?php echo $lang[$l][51]; ?></label><br />
         
         
            <input type="checkbox" name="sort_sel" id="sort_sel"
                   <?php if($css->get_cfg('sort_selectors')) echo 'checked="checked"'; ?> />
            <label for="sort_sel" title="<?php echo $lang[$l][41]; ?>" class="help"><?php echo $lang[$l][20]; ?></label><br />
            
            
            <input type="checkbox" name="sort_de" id="sort_de"
                   <?php if($css->get_cfg('sort_properties')) echo 'checked="checked"'; ?> />
            <label for="sort_de"><?php echo $lang[$l][21]; ?></label><br />
            
            
            <label for="merge_selectors"><?php echo $lang[$l][22]; ?></label>
            <div id="merge_selectors">
            <input type="radio" title="<?php echo $lang[$l][47]; ?>" name="merge_selectors" id="ms0" value="0"
                   <?php if($css->get_cfg('merge_selectors') == 0) echo 'checked="checked"'; ?>/><label for="ms0">0</label>
            <input type="radio" title="<?php echo $lang[$l][48]; ?>" name="merge_selectors" id="ms1" value="1"
                   <?php if($css->get_cfg('merge_selectors') == 1) echo 'checked="checked"'; ?>/><label for="ms1">1</label>
            <input type="radio" title="<?php echo $lang[$l][49]; ?>" name="merge_selectors" id="ms2" value="2"
                   <?php if($css->get_cfg('merge_selectors') == 2) echo 'checked="checked"'; ?>/><label for="ms2">2</label>
                   <a href="http://csstidy.sourceforge.net/merge_selectors.php" style="cursor:help;">(?)</a>
            </div>
             
             
            <input type="checkbox" name="optimise_shorthands" id="optimise_shorthands"
                   <?php if($css->get_cfg('optimise_shorthands')) echo 'checked="checked"'; ?> />
            <label for="optimise_shorthands"><?php echo $lang[$l][23]; ?></label><br />
            
            
            <input type="checkbox" name="compress_c" id="compress_c"
                   <?php if($css->get_cfg('compress_colors')) echo 'checked="checked"';?> />
            <label for="compress_c"><?php echo $lang[$l][24]; ?></label><br />
            
            
            <input type="checkbox" name="compress_fw" id="compress_fw"
                   <?php if($css->get_cfg('compress_font-weight')) echo 'checked="checked"';?> />
            <label for="compress_fw"><?php echo $lang[$l][45]; ?></label><br />
            
            
            <input type="checkbox" name="lowercase" id="lowercase" value="lowercase"
                   <?php if($css->get_cfg('lowercase_s')) echo 'checked="checked"'; ?> />
            <label title="<?php echo $lang[$l][30]; ?>" class="help" for="lowercase"><?php echo $lang[$l][25]; ?></label><br />
            
            
            <?php echo $lang[$l][26]; ?><br />
            <input type="radio" name="case_properties" id="none" value="0"
                   <?php if($css->get_cfg('case_properties') == 0) echo 'checked="checked"'; ?> />
            <label for="none"><?php echo $lang[$l][53]; ?></label>
            <input type="radio" name="case_properties" id="lower_yes" value="1"
                   <?php if($css->get_cfg('case_properties') == 1) echo 'checked="checked"'; ?> />
            <label for="lower_yes"><?php echo $lang[$l][27]; ?></label>
            <input type="radio" name="case_properties" id="upper_yes" value="2"
                   <?php if($css->get_cfg('case_properties') == 2) echo 'checked="checked"'; ?> />
            <label for="upper_yes"><?php echo $lang[$l][29]; ?></label><br />      
            
            <input type="checkbox" name="rbs" id="rbs"
                   <?php if($css->get_cfg('remove_bslash')) echo 'checked="checked"'; ?> />
            <label for="rbs"><?php echo $lang[$l][31]; ?></label><br />
             
            
            <input type="checkbox" id="remove_last_sem" name="remove_last_sem"
                   <?php if($css->get_cfg('remove_last_;')) echo 'checked="checked"'; ?> />
   			<label for="remove_last_sem"><?php echo $lang[$l][42]; ?></label><br />
            
           
            <input type="checkbox" id="discard" name="discard"
                   <?php if($css->get_cfg('discard_invalid_properties')) echo 'checked="checked"'; ?> />
            <label for="discard"><?php echo $lang[$l][43]; ?></label>
            <select name="css_level"><?php echo options(array('CSS2.1','CSS2.0','CSS1.0'),$css->get_cfg('css_level')); ?></select><br />
            
            <input type="checkbox" name="file_output" id="file_output" value="file_output"
                   <?php if(isset($_REQUEST['file_output'])) echo 'checked="checked"'; ?> />
            <label class="help" title="<?php echo $lang[$l][34]; ?>" for="file_output">
				<strong><?php echo $lang[$l][33]; ?></strong>
			</label><br />

          </fieldset>
        <input type="hidden" name="post" />
        </div>
      </div>
    </form>
    <?php

    $file_ok = false;
    $result = false;

    $url = (isset($_REQUEST['url']) && !empty($_REQUEST['url'])) ? $_REQUEST['url'] : false;

	if(isset($_REQUEST['template']))
	{
		switch($_REQUEST['template'])
		{
			case 4:
			if(isset($_REQUEST['custom']) && !empty($_REQUEST['custom']))
			{
				$css->load_template($_REQUEST['custom'],false);
			}
			break;
			
			case 3:
			$css->load_template('highest_compression');
			break;
		
			case 2:
			$css->load_template('high_compression');
			break;
		
			case 0:
			$css->load_template('low_compression');
			break;
		}
	}
      
    if($url)
    {
    	if(substr($_REQUEST['url'],0,7) != 'http://')
		{
			$_REQUEST['url'] = 'http://'.$_REQUEST['url'];
		}
        $result = $css->parse_from_url($_REQUEST['url'],0);
    }
    elseif(isset($_REQUEST['css_text']) && strlen($_REQUEST['css_text'])>5)
    {
        $result = $css->parse($_REQUEST['css_text']);
    }

    if($result)
    {     
        $ratio = $css->print->get_ratio();
        $diff = $css->print->get_diff();
        if(isset($_REQUEST['file_output']))
        {
            $filename = md5(mt_rand().mt_rand());
            $handle = fopen('temp/'.$filename.'.css','w');
            if($handle) {
               
                if(fwrite($handle,$css->print->plain()))
                {
                    $file_ok = true;
                }
            }
            fclose($handle);
        }
        if($ratio>0) $ratio = '<span style="color:green;">'.$ratio.'%</span>
    ('.$diff.' Bytes)'; else $ratio = '<span
    style="color:red;">'.$ratio.'%</span> ('.$diff.' Bytes)';
        if(count($css->log) > 0): ?>
        <fieldset id="messages"><legend>Messages</legend>
			<div><dl><?php
			foreach($css->log as $line => $array)
			{
				echo '<dt>'.$line.'</dt>';
				for($i = 0; $i < count($array); $i++)
				{
					echo '<dd class="'.$array[$i]['t'].'">'.$array[$i]['m'].'</dd>';
				}
			}
			?></dl></div>
        </fieldset>
        <?php endif;    
        echo '<fieldset><legend>'.$lang[$l][37].': '.$css->print->size('input').'KB, '.$lang[$l][38].':'.$css->print->size('output').'KB, '.$lang[$l][36].': '.$ratio;
        if($file_ok)
        {
            echo ' - <a href="temp/'.$filename.'.css">Download</a>';
        }
        echo '</legend>';
        echo '<pre><code>';
        echo $css->print->formatted();
        echo '</code></pre>';
        echo '</fieldset>';
     }
     elseif(isset($_REQUEST['css_text']) || isset($_REQUEST['url'])) echo '<p
    class="important">'.$lang[$l][28].'</p>';
     ?>
    <p style="text-align:center;font-size:0.8em;clear:both;">
      For bugs and suggestions feel free to <a
      href="http://csstidy.sourceforge.net/contact.php">contact me</a>.
    </p>
  </body>
</html>