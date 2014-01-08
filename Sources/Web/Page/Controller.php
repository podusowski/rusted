<?php

require_once("Rusted.php");

class Controller
{
    public function __construct()
    {
        session_start();
        $this->rusted = new Rusted();
    }

    public function index()
    {
        $this->render("Templates/index.php");
    }

    public function register()
    {
        if ($_POST["password1"] == $_POST["password2"])
        {
            if ($this->rusted->register($_POST["login"], $_POST["password1"]))
            {
                $this->addMessage("your account has been created");
            }
            else
            {
                $this->addMessage("some shit happened");
            }
        }
        else
        {
            $this->addMessage("passwords doesn't match");
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

    private function addMessage($string)
    {
        $_SESSION["messages"][] = $string;
    }

    private function messages()
    {
        $messages = array();

        if (isset($_SESSION["messages"]))
        {
            $messages = $_SESSION["messages"];
            unset($_SESSION["messages"]);
        }
        return $messages;
    }
}

?>
