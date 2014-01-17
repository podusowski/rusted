<?php

class Rusted
{
    function __construct()
    {
        $this->rusted_directory = "/var/rusted";
        $this->pid_file = "/var/rusted/Server.pid";
        $this->db = new PDO("sqlite:" . $this->rusted_directory . "/database.sqlite3");
        $this->db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    }

    function online()
    {
        return file_exists($this->pid_file);
    }

    function users_registered()
    {
        $result = $this->db->query("SELECT count(*) FROM users");

        foreach ($result as $row)
        {
            return $row[0];
        }
    }

    function register($login, $password)
    {
        $passwordHash = sha1($password);

        $this->db->beginTransaction();

        $insert = $this->db->prepare("INSERT INTO users (login, password) VALUES(:login, :password)");
        $insert->execute(array(':login' => $login, ':password' => $passwordHash));

        $insert = $this->db->prepare("
            INSERT INTO objects (type, class, x, y, z, owner, integrity, carbon, helium)
            VALUES(:type, :class, :x, :y, :z, :owner, :integrity, :carbon, :helium)");

        $playerId = $this->db->lastInsertId();

        $insert->execute(array(
            ':type' => "Ship",
            ':class' => 1,
            ":x" => 100,
            ":y" => 100,
            ":z" => 100,
            ":owner" => $playerId,
            ":integrity" => 100,
            ":carbon" => 0,
            ":helium" => 0
        ));

        $this->db->commit();
    }
}

?>
