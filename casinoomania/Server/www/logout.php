<?php require_once "config.php" ?>
<?php
session_destroy();
header("Location: $root/login.php");
?>