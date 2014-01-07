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
        if ($_POST["password1" == $_POST["password2"])
        {
            $this->rusted->register($_POST["login"], $_POST["password1"]);
        }
        else
        {
            echo("passwords doesn't match");
        }
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
