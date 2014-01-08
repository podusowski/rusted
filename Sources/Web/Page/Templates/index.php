<? include('header.inc.php'); ?>

<div id="body">

<h2>Welcome</h2>
<p>
Welcome in Rusted alpha. Rusted is multiplayer online game taking action in outer space.
</p>

<p>server status: <?= $this->rusted->online() ? "online" : "offline" ?></p>
<p>users registered: <?= $this->rusted->users_registered(); ?></p>

<? include('messages.inc.php'); ?>

</div>

<? include('footer.inc.php'); ?>
