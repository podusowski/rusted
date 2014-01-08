<? include('header.inc.php'); ?>

<div id="body">

<? include('messages.inc.php'); ?>

<form action="<?= $this->makeUri("register"); ?>" method="post">
    <label>
        <span>login</span>
        <input type="text" name="login" autocomplete="off" />
    </label>

    <label>
        <span>password</span>
        <input type="password" name="password1" autocomplete="off" />
    </label>

    <labeL>
        <span>repeat password</span>
        <input type="password" name="password2" autocomplete="off" />
    </label>

    <input type="submit" value="register" />
</form>

</div>

<? include('footer.inc.php'); ?>
