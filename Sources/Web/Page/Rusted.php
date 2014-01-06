<?php

class Rusted
{
    function __construct()
    {
        $this->rusted_directory = "/var/rusted";
        $this->pid_file = "/var/rusted/Server.pid";
        $this->db = new PDO("sqlite:" . $this->rusted_directory . "/database.sqlite3");
    }

    function online()
    {
        return file_exists($this->pid_file);
    }

    function users_registered()
    {
        $result = $this->db->query("SELECT count(*) from users");

        foreach ($result as $row)
        {
            return $row[0];
        }
    }

    function register($login, $password)
    {
        $insert = $this->db->prepare("INSERT INTO users (login, password) VALUES(:login, :password)");
        $insert->bindParam(":login", $login);
        $insert->bindParam(":password", $password);
        $insert->execute();
    }
}

?>

