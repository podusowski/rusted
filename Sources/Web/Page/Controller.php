<?php

require_once("Rusted.php");

class Controller
{
    public function __construct()
    {
        $this->rusted = new Rusted();
    }

    public function index()
    {
        $this->render("Templates/index.php");
    }

    public function register()
    {
        $this->rusted->register($_POST["login"], $_POST["password1"]);
        $this->redirect("index");
    }

    private function render($filename)
    {
        include($filename);
    }

    private function redirect($action)
    {
        header("location: ?action=$action");
    }

    private function makeUri($action)
    {
        return "?action=$action";
    }
}

?>
