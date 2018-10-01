<?php require_once "../config.php" ?>
<?php require_once "../database.php" ?>
<?php 
$is_success = false;

$database = new Database($DB_HOST, $DB_USER, $DB_PASS);
$database->connect_db();
$database->choose_database_name($DB_NAME);

if (!empty($_REQUEST['id'])) {
  $id = $_REQUEST['id'];
  $writer = 'admin';
  
  if (!empty($_REQUEST['update_id'])) {
    $id = $_REQUEST['update_id'];
    $username = $_REQUEST['username'];
    $mycoins = $_REQUEST['my_coins'];
    $message = $_REQUEST['message'];
    if (empty($message)) $message = '';
  
    $sql = "UPDATE users SET username='%s', my_coins=%d, message='%s' WHERE id=%d;";
    $query = sprintf($sql, $username, $mycoins, $message, $id);
    $result = $database->execute($query);
    if (mysql_affected_rows() > 0) {
      $is_success = true;
      $message = "Update Users Successful!";
    }
  }

  $sql = "SELECT * FROM users WHERE id=%d";
  $query = sprintf($sql, $id);
  $result = $database->query($query);
  
  // Get List Category
	$sql = "SELECT * FROM game;";  
	$query = sprintf($sql);  
	$game_result = $database->query($query);
}
$database->close_db();

?>
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>Edit Users</title>

    <!-- Bootstrap core CSS -->
    <link href="<?php print $root ?>/css/bootstrap.css" rel="stylesheet">

    <!-- Add custom CSS here -->
    <link href="<?php print $root ?>/css/sb-admin.css" rel="stylesheet">
    <link rel="stylesheet" href="<?php print $root ?>/font-awesome/css/font-awesome.min.css">
    <!-- Page Specific CSS -->    
  </head>

  <body>

    <div id="wrapper">

      <?php include "../sidebar.php";?>

      <div id="page-wrapper">

        <?php $isUserAllowed = isUserAllowed($PRIV_EDIT_USERS); // function from privileges.php?>
        
        <?php if ($isUserAllowed): ?>
        <div class="row">
          <div class="col-lg-12">
            <h1>Edit Users</h1>
            <ol class="breadcrumb">
              <li><i class="fa fa-group"></i> Users</a></li>
              <li class="active">Edit</li>
            </ol>
          </div>
        </div><!-- /.row -->

        <?php if ($is_success == true): ?>
        <div class="alert alert-dismissable alert-success">
          <button type="button" class="close" data-dismiss="alert">×</button>
          User <strong><?php print $username; ?></strong> is successfully updated!
        </div>
        <?php endif; ?>
        
            
        <form action="<?php print $root; ?>/page-users/edit_users.php?id=<?php print $id; ?>&update_id=<?php print $id; ?>" method="post" enctype="multipart/form-data">
          <div class="row">
            <div class="col-lg-6">
              <form role="form">
                
                <?php foreach ($result as $object): ?>
                <div class="form-group">
                  <label>Facebook ID</label>
                  <input class="form-control" name="fid" size="50" value="<?php print $object->fid;?>" readonly>
                </div>            

                <div class="form-group">
                  <label>Username</label>
                  <input class="form-control" name="username" size="50" value="<?php print $object->username;?>">
                </div>            

                <div class="form-group">
                  <label>Password</label>
                  <input class="form-control" name="password" size="50" value="<?php print $object->password;?>" readonly>
                </div>            

                <div class="form-group">
                  <label>Email Address</label>
                  <input class="form-control" name="name" size="50" value="<?php print $object->email_address;?>" readonly>
                </div>            

                <div class="form-group">
                  <label>Android Push Notification ID</label>
                  <input class="form-control" name="name" size="50" value="<?php print $object->dev_android;?>" readonly>
                </div>            

                <div class="form-group">
                  <label>Android Push Location</label>
                  <input class="form-control" name="name" size="50" value="<?php print $object->dev_android_location;?>" readonly>
                </div>            
              
                <div class="form-group">
                  <label>iOS Push Notification ID</label>
                  <input class="form-control" name="name" size="50" value="<?php print $object->dev_ios;?>" readonly>
                </div>            

                <div class="form-group">
                  <label>iOS Push Location</label>
                  <input class="form-control" name="name" size="50" value="<?php print $object->dev_ios_location;?>" readonly>
                </div>            

                <div class="form-group">
                  <label>User Coins</label>
                  <input class="form-control" name="my_coins" size="50" value="<?php print $object->my_coins;?>">
                </div>            

                <div class="form-group">
                  <label>Money Spent</label>
                  <input class="form-control" name="money_spent" size="50" value="<?php print $object->money_spent;?>" readonly>
                </div>            

                <div class="form-group">
                  <label>Message</label>
                  <textarea class="form-control" name="message" size="50" value="<?php print $object->message;?>"><?php print $object->message; ?></textarea>
                </div>                  
                <?php endforeach; ?>

                <input type="submit" class="btn btn-primary" name="update" value="Update">
                
              </form>
            </div>
            
            <div class="col-lg-6">
              <div class="list-group">
                <a href="#" class="list-group-item active">Users Bet History</a>
			        	<?php foreach ($game_result as $object): ?>
      			    <a href="<?php print $root; ?>/page-users/user-history.php?uid=<?php print $id; ?>&gid=<?php print $object->id ?>" class="list-group-item"><?php print $object->game?></a>
			          <?php endforeach ?>                
              </div>
            </div>
                        
          </div>
        </form>
                
        <?php else: ?>
        <div class="alert alert-dismissable alert-danger">
          <button type="button" class="close" data-dismiss="alert">×</button>
          User is not authorized to see this page!
        </div>        
        <?php endif; ?>
        
      </div><!-- /#page-wrapper -->

    </div><!-- /#wrapper -->

    <!-- JavaScript -->
    <script src="<?php print $root ?>/js/jquery-1.10.2.js"></script>
    <script src="<?php print $root ?>/js/bootstrap.js"></script>

    <!-- Page Specific Plugins -->    
    <script src="<?php print $root ?>/js/morris/chart-data-morris.js"></script>
    <script src="<?php print $root ?>/js/tablesorter/jquery.tablesorter.js"></script>
    <script src="<?php print $root ?>/js/tablesorter/tables.js"></script>

  </body>
</html>
