<? include('header.inc.php'); ?>

<div id="body">

<p>server status: <?= $this->rusted->online() ? "online" : "offline" ?></p>
<p>users registered: <?= $this->rusted->users_registered(); ?></p>

</div>

<? include('footer.inc.php'); ?>
