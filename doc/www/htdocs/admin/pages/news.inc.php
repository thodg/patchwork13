<?PHP

if (isset ($_POST['a'])) {
  $id = $_POST['id'];
  $author = $_POST['author'];
  $title = $_POST['title'];
  $text = $_POST['text'];

  mysql_connect ('mysql4-p', 'p133991rw', '$*3_Ah7') ||
    die ('Could not connect: ' . mysql_error ());
  mysql_select_db ('p133991_pw13') ||
    die ('Could not select database');

  if ($_POST['a'] === 'Ajouter') {
    $query = ("INSERT INTO `news` ( `id` , `author` , `title` , `text` ) ".
	      "VALUES ( $id, '$author', '$title', '$text' )");
    ($result = mysql_query($query)) ||
      die ('Query failed: ' . mysql_error ());
  }
  else if ($_POST['a'] === 'Modifier') {
    $query = ("UPDATE `news` SET `author`='$author' , `title`='$title'".
	      " , `text`='$text' WHERE `id`=$id");
    ($result = mysql_query($query)) ||
      die ('Query failed: ' . mysql_error ());
  }
  else if ($_POST['a'] === 'Supprimer') {
  }
  else
    die ("Unknown action");

  echo `./generate-news`;
  echo '<hr>'."\n";
}

function print_news_form ($row)
{
  echo '<table width="100%"><tr>'."\n";
  echo '<td style="newsdate" align="left">'.$row['date'].'</td>'."\n";
  echo '<td align="right">'."\n";
  echo '<span class="prop">author</span> ';
  echo $row['author'].'<br>'."\n";
  echo '<span class="prop">id</span> ';
  echo $row['id']."\n";
  echo '</td>'."\n";
  echo '</tr></table>'."\n";
  echo '<FORM action="'.$PHP_SELF.'" method="POST">'."\n";
  echo '<input name="id" type="hidden" value="'.$row['id'].'">'."\n";
  echo '<input name="author" type="hidden" value="'.$row['author'].'">'."\n";
  echo '<input name="title" type="text" value="'.$row['title'].'"'."\n";
  echo ' style="width:100%"><br>'."\n";
  echo '<textarea name="text" rows=10 cols=60'."\n";
  echo ' style="width:100%;">'.$row['text'].'</textarea><br>'."\n";
  echo '<table width="100%"><tr>'."\n";
  if ($row['new']) {
    echo '<td align="right" width="100%">'."\n";
    echo '<input name="a" type="submit" value="Ajouter">'."\n";
    echo '</td>'."\n";
  }
  else {
    echo '<td align="left" width="50%">'."\n";
    echo '<input name="a" type="submit" value="Supprimer">'."\n";
    echo '</td>'."\n";
    echo '<td align="right" width="50%">'."\n";
    echo '<input name="a" type="submit" value="Modifier">'."\n";
    echo '</td>'."\n";
  }
  echo '</table>'."\n";
  echo '</FORM>'."\n";
  echo '<br><br><br><br><br><br><br>'."\n";
  echo ''."\n";
}

mysql_connect ('mysql4-p', 'p133991ro', '99#o)u[') ||
  die ('Could not connect: ' . mysql_error ());
mysql_select_db ('p133991_pw13') ||
  die ('Could not select database');

/* get max id */
($result = mysql_query ("SELECT id FROM news ORDER BY id DESC LIMIT 1")) ||
die ('Query failed: ' . mysql_error ());
if ($line = mysql_fetch_array ($result))
  $new_id = $line[0] + 1;
else
  $new_id = 1;

echo '<center style="width:100%">'."\n";
echo '<div style="width:100%">'."\n";
echo "\n";

/* add news */
$new_news = array ('new'    => true,
		   'id'     => $new_id,
		   'author' => $_SERVER["PHP_AUTH_USER"],
		   'title'  => '',
		   'text'   => '',
		   'date'   => '');
print_news_form ($new_news);

echo "<br> <hr> <br><br><br><br><br><br>\n";

/* read news */
($result = mysql_query("SELECT id, date, modified, author, title, text".
		       " FROM news ORDER BY id DESC LIMIT 10")) ||
  die ('Query failed: ' . mysql_error ());
while ($line = mysql_fetch_array ($result, MYSQL_ASSOC))
  print_news_form ($line);

echo '</div>';
echo '</center>';
?>
