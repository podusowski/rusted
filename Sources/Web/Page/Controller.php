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
        if ($_SERVER['REQUEST_METHOD'] == "POST")
        {
            if (strlen($_POST["login"]) < 3)
            {
                $this->addErrorMessage("your login has to be at least 3 character long");
                $this->redirect("register");
            }
            else if ($_POST["password1"] != $_POST["password2"])
            {
                $this->addErrorMessage("passwords doesn't match");
                $this->redirect("register");
            }

            try
            {
                $this->rusted->register($_POST["login"], $_POST["password1"]);
            }
            catch (Exception $e)
            {
                $this->addErrorMessage("some shit happened");
                $this->redirect("register");
            }

            $this->addInfoMessage("your account has been created");
            $this->redirect("index");
        }
        else
        {
            $this->render("Templates/register.php");
        }
    }

    public function download()
    {
        $this->render("Templates/download.php");
    }

    public function gallery()
    {
        $this->render("Templates/gallery.php");
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
        $_SESSION["messages"][] = array('message' => $string);
    }

    private function addErrorMessage($string)
    {
        $_SESSION["messages"][] = array('type' => 'error', 'message' => $string);
    }

    private function addInfoMessage($string)
    {
        $_SESSION["messages"][] = array('type' => 'info', 'message' => $string);
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
