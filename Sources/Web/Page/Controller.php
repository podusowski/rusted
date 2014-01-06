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
/*
<form action="register.php" method="post">
    <label>login</label><input type="text" name="login" />
    <label>password</label><input type="password" name="password1" />
    <labeL>repeat password</label><input type="password" name="password2" />
    <input type="submit" value="register" />
</form>
*/
    }

    public function register()
    {

    }

    private function render($filename)
    {
        include($filename);
    }
}

?>
