<? include('header.inc.php'); ?>

<div id="body">

<p>server status: <?= $this->rusted->online() ? "online" : "offline" ?></p>
<p>users registered: <?= $this->rusted->users_registered(); ?></p>

<? include('messages.inc.php'); ?>

<form action="<?= $this->makeUri("register"); ?>" method="post">
    <label>login</label><input type="text" name="login" />
    <label>password</label><input type="password" name="password1" />
    <labeL>repeat password</label><input type="password" name="password2" />
    <input type="submit" value="register" />
</form>

</div>

<? include('footer.inc.php'); ?>
