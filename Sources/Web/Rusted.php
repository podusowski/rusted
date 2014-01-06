<?php

class Rusted
{
    __construct()
    {
        $this->rusted_directory = "/var/rusted";
        $this->pid_file = "/var/rusted/Server.pid";
    }

    bool online()
    {
        return file_exists($this->pid_file);
    }
}

?>
