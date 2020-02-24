<?PHP
if (!isset($_GET['p'])) {
  header ('Location: '.$PHP_SELF.'?p=news');
  exit;
}
$p = $_GET['p'];

function echo_err($s) {
  echo '<font size=4 color=#DDDDDD>'.$s.'</font>';
}

$menu_html="";

if(!file_exists('.menu')) {
  echo_err('No menu');
  exit;
}

$menu = file('.menu');
foreach ($menu as $node) {
  $node = trim($node);
  if ($node[0] == '#'
      || count($node) == 0)
    continue;
  $n = explode("\t", $node);
  for ($i=0; $i<count($n); $i++)
    $n[$i] =  trim($n[$i]);

  $page_title = $n[0];
  $menu_href = ($n[1] === '->')
    ? $n[2]                     // external link
    : $PHP_SELF.'?p='.$n[1];    // internal link


  $options = array();
  if (count($n) >= 3) {
    $options = explode(" ", $n[2]);
  }

  if (!in_array('hidden', $options)) {
    $style = ($p === $n[1]) ? 'style="color:#aaa;"' : '';
    $menu_html .= '<a class="menu" href="'.$menu_href.'" '.$style.'>';
    $menu_html .= $n[0].'</a><br /><br />'."\n";
  }
}

?><!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
    "http://www.w3.org/TR/html4/loose.dtd">
<HTML>
<HEAD>
  <TITLE>Patchwork13!</TITLE>
  <LINK rel="STYLESHEET" type="text/css" href="main.css">
  <META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">
</HEAD>

<BODY>
<TABLE border="0" width="77%" cellspacing="0" cellpadding="0">
  <TR>
    <TD align="left" valign="middle" style="padding: 2px 5px 17px;">
      <FONT size="7" color="#000000"><B>
        Patchwork13!
      </B></FONT>
    </TD>
    <TD align="right" valign="bottom">
      <FONT size="6" color="#DDDDDD"><B>
        <?PHP echo $page_title; ?>
      </B></FONT>
    </TD>
  </TR>
</TABLE>

<TABLE border="0" width="100%" cellspacing="0" cellpadding="8"
 style="border-top:solid 1px #EEEEEE">
<COL align="left" width="20%">
<COL align="left" width="80%">
<TR>
<TD valign="top" style="border-right:solid 1px #EEEEEE;">
<br>
<?PHP
echo $menu_html;
?>
<br><br><br><br><br>

  <A href="http://sourceforge.net/projects/patchwork13/"><FONT size="2"><B>
    Project page </B></FONT></A> at <br>
  <a href="http://sourceforge.net"><img
   src="http://sourceforge.net/sflogo.php?group_id=133991&amp;type=2"
   width="125" height="37" border="0" alt="SourceForge.net Logo"></a>

<br><br>
  <br>

  <a href="http://validator.w3.org/check?uri=referer"><img border="0"
   src="http://www.w3.org/Icons/valid-html401"
   alt="Valid HTML 4.01!" height="31" width="88"></a>

  <a href="http://jigsaw.w3.org/css-validator/check/referer"><img
   style="border:0;width:88px;height:31px"
   src="http://jigsaw.w3.org/css-validator/images/vcss" 
   alt="Valid CSS!"></a>

</TD>
<TD valign="top" style="padding-left:32px; padding-right:15%;">
<br>
<?PHP
if(!isset($page_title)) {
  echo_err('403 not in menu');
  exit;
}

$page = 'pages/'.$p.'.inc.php';
if(!file_exists($page)) {
  echo_err('404');
  exit;
}
include($page);

?>
</div>
</TD>
</TR>
</TABLE>
</BODY>
</HTML>
