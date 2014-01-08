<!DOCTYPE HTML>
<html>
    <head>
        <title>Rusted</title>
        <link rel="stylesheet" href="style.css" type="text/css" media="screen" />
    </head>
    <body>
        <div id="header">
            <h1>Rusted</h1>
        </div>
        <div id="menu">
            <a href="<?= $this->makeUri("index"); ?>">home</a>
            <a href="<?= $this->makeUri("register"); ?>">register</a>
            <a href="<?= $this->makeUri("download"); ?>">get client</a>
        </div>
