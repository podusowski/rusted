<? include('header.inc.php'); ?>

<div id="body">

<h2>Welcome</h2>
<p>
Welcome in Rusted alpha. Rusted is a multiplayer online game taking action in outer space.
</p>

<h2>FAQ</h2>
<h3>When it's going to be ready?</h3>
<p>
Not sure, Rusted is developed by a team which consists of... me :) Also, it's not a part of any company so I'm working on the game only on my free time.
</p>

<h3>Will it be free?</h3>
<p>
This isn't decided yet, for sure, I'll have to pay at least for a server so it'll have to earn money in one way or another, maybe some ads the web site will be enough, will see.
</p>

<h2>Beta server status</h2>
<p>server status: <?= $this->rusted->online() ? "online" : "offline" ?></p>
<p>users registered: <?= $this->rusted->users_registered(); ?></p>

<? include('messages.inc.php'); ?>

</div>

<? include('footer.inc.php'); ?>
