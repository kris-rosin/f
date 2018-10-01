<?php 
require_once "config.php";
$username = $_SESSION['username'];
if (empty($username)) {
  header("Location: $root/login.php");
} else {
  header("Location: $root/dashboard.php");
}
?>