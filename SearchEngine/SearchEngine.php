<html>

<style>
 h2 {text-shadow: 1px 1px 1px #F0F8FF;  
font-family: "AppleGothic", AppleGothic, serif; 
 text-align: center;

}



body{

font-family: "AppleGothic", AppleGothic, serif;
text-align: center;
background: linear-gradient(CornflowerBlue, coral, DarkOrange); }







</style>


<body>

<?php
   // File:  adduser.php
   // Place in your cgi-bin directory and make executable.
   // remove the spaces around html and body, /html and /body, and ?php

if($_POST['username'] == NULL)
{ echo '<h1>No query to process.</h1>';
  echo '<a href="~yumi/public_html/SearchEngine/SearchEngine.html">Go Back?</a>';

}
else if (isset($_POST['username']))
{  
  querySearch();
}

function querySearch()
   { 
     $check = $_POST['username'];

     $speedracer = './hw4.sh ' . $check;
 
    echo '<h1>Query: ' . $_POST['username'] . '</h1><br><br>';
   $stump =  passthru ($speedracer);
      
   }

?>

</body>
</html>


