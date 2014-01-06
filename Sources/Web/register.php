<?php

require_once("Rusted.php");

$rusted = new Rusted();

$rusted->register($_POST["login"], $_POST["password1"]);

//header("location: index.php");

?>
