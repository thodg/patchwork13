<?PHP

mysql_connect ('mysql4-p', 'p133991ro', '99#o)u[') ||
die ('Could not connect: ' . mysql_error ());
mysql_select_db ('p133991_pw13') ||
die ('Could not select database');

/* read news */
$rows = "id,UNIX_TIMESTAMP(date),title,text,author";
$result = mysql_query("SELECT $rows FROM news ORDER BY id DESC");
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
echo '<a href="<?= $PHP_SELF ?>?p=newsarchive1">1</a>'."\n";
echo '</font>'."\n";
echo '</div>'."\n";
?>
