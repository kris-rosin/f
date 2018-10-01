<?php
$PRIV_MCOUNT = 5;

$privileges_type[0] = 'Delete Users';
$privileges_type[1] = 'Edit Users';
$privileges_type[2] = 'Send Users Message Push Notification';
$privileges_type[3] = 'Add, Edit, Delete Casino Games';
$privileges_type[4] = 'Add, Edit, Delete Admin';

$PRIV_DELETE_USERS = 0;
$PRIV_EDIT_USERS = 1;
$PRIV_SEND_MESSAGE_PUSH_NOTIFICATION = 2;
$PRIV_CASINO_GAMES = 3;
$PRIV_ADMIN = 4;

$PRIVILEGES_JSON = '["0","1","2","3","4"]';

function isUserAllowed($priv_id) {
  //var_dump("passing param priv_id: " . $priv_id);
  //var_dump("session_priv: " . $_SESSION['privileges']);

  if (!empty($_SESSION['username'])) {
    $myPrivileges = json_decode($_SESSION['privileges']); 
  }
  $return = false;
  
  foreach ($myPrivileges as $iter_priv_id) {
    if ($priv_id == $iter_priv_id) {
      $return = true;
      break;
    }
  }
  //var_dump($return);
  return $return;
}
?>