<?php

foreach ($this->messages() as $message)
{
    echo('<div class="message-' . $message["type"] . '">' . $message["message"] . '</div>');
}

?>
