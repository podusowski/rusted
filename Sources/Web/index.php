<?php

require_once("Rusted.php");

$rusted = new Rusted();

echo("server status: " . $rusted->online());
echo("users registered: " . $rusted->users_registered());

?>

<form>
    <input type="text" name="login" />
    <input type="password" name="password1" />
    <input type="password" name="password2" />
    <input type="submit" value="register" />
</form>
