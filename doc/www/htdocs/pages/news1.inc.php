<?PHP

if ($_REQUEST['previous'] == 1) {
  echo '<div align="left">'."\n";
  echo '<font size="5">Archives</font>'."\n";
  echo '</div>'."\n";
  echo '<div align="right">'."\n";
  echo '<font size="2">'."\n";
  echo 'Back to <a href="'.$PHP_SELF.'?p='.$_GET['p'].'">news</a>'."\n";
  echo '</font>'."\n";
  echo '</div>'."\n";
  echo '<br>'."\n";
  require ((dirname ($_SERVER['PATH_TRANSLATED'])).'/pages/oldnews.inc.php');
}
else {
  mysql_connect ('mysql4-p', 'p133991ro', '99#o)u[') ||
    die ('Could not connect: ' . mysql_error ());
  mysql_select_db ('p133991_pw13') ||
    die ('Could not select database');
  
  /* read news */
  $rows = "id,UNIX_TIMESTAMP(date),title,text,author";
  $result = mysql_query("SELECT $rows FROM news ORDER BY id DESC LIMIT 10");
  if (!$result)
    die ('Query failed: ' . mysql_error ());
  
  while ($L = mysql_fetch_array ($result)) {
    echo '<H3>'.date("l jS, F Y", $L[1]).' &nbsp; &nbsp; &nbsp; '."\n";
    echo $L[2].'</H3>'."\n";
    echo nl2br ($L[3]).'<br>'."\n";
    echo '<div align="right">'."\n";
    echo '<font size="-1">'.$L[4].'</font><br>'."\n";
    echo '</div>'."\n";
    echo '<br><br>'."\n";
  }

  echo '<div align="left">'."\n";
  echo '<font size="2">'."\n";
  echo 'Previous news :'."\n";
  echo '<a href="'.$_SERVER['REQUEST_URI'].'&previous=1">1</a>'."\n";
  echo '</font>'."\n";
  echo '</div>'."\n";
}
?>
