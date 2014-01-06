<?php

require_once("Controller.php");

$controller = new Controller();
$action = isset($_GET["action"]) ? $_GET["action"] : "index";
$controller->$action();

?>

