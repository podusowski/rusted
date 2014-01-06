<?php

require_once("Rusted.php");

$rusted = new Rusted();

echo("server status: " . $rusted->online());
echo("users registered: " . $rusted->users_registered());

?>

<form action="register.php" method="post">
    <label>login</label><input type="text" name="login" />
    <label>password</label><input type="password" name="password1" />
    <labeL>repeat password</label><input type="password" name="password2" />
    <input type="submit" value="register" />
</form>
