<?php require_once "../config.php" ?>
<?php require_once "../database.php" ?>
<?php require_once "../privileges.php" ?>
<?php 
$is_success = false;
$is_failed = false;
if (!empty($_REQUEST['name']) &&
    !empty($_REQUEST['pass']) &&
    !empty($_REQUEST['conf_pass']) &&
    !empty($_REQUEST['privileges'])
    ) {
  $name = trim($_REQUEST['name']);
  $pass = $_REQUEST['pass'];
  $conf_pass = $_REQUEST['conf_pass'];
  $privileges = $_REQUEST['privileges'];
  
  if ($pass != $conf_pass) {
    $message = "<strong>Password</strong> and <strong>Confirm Password</strong> are different!";
    $is_failed = true;
  } else {
  
    $json_privileges = json_encode($privileges);
      
    $writer = 'admin';
    
    $database = new Database($DB_HOST, $DB_USER, $DB_PASS);
    $database->connect_db();
    $database->choose_database_name($DB_NAME);

    $sql = "SELECT count(name) as mcount FROM sysadmin WHERE name='%s'";
    $query = sprintf($sql, $database->escape($name));
    $result = $database->query($query);
    $mcount = 0;
    foreach($result as $object) {
      $mcount = $object->mcount;
    }
    
    if ($mcount > 0) {
      $is_failed = true;
      $message = "Admin with name <strong>$name</strong> is not available!";
    } else {
      $sql = "INSERT INTO sysadmin (name, password, privileges) VALUES ('%s', '%s', '%s');";
      $query = sprintf($sql, 
                $database->escape($name), 
                $database->escape($pass), 
                $database->escape($json_privileges));
      $result = $database->execute($query);
      
      $is_success = true;
      $message = "Admin <strong>$name</strong> is successfully created!";
    }
    
    $database->close_db();
  }
}
?>
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>Create Admin</title>

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

        <?php $isUserAllowed = isUserAllowed($PRIV_ADMIN); // function from privileges.php?>
        
        <?php if ($isUserAllowed): ?>
        <div class="row">
          <div class="col-lg-12">
            <h1>Create Admin</h1>
            <ol class="breadcrumb">
              <li><i class="fa fa-user"></i> Admin</a></li>
              <li class="active">Create</li>
            </ol>
          </div>
        </div><!-- /.row -->

        <?php if ($is_success == true): ?>
        <div class="alert alert-dismissable alert-success">
          <button type="button" class="close" data-dismiss="alert">×</button>
          <?php print $message; ?>
        </div>
        <?php endif; ?>
        
        <?php if ($is_failed == true): ?>
        <div class="alert alert-dismissable alert-danger">
          <button type="button" class="close" data-dismiss="alert">×</button>
          <?php print $message;?>
        </div>
        <?php endif; ?>

            
        <form action="create-admin.php" method="post" enctype="multipart/form-data">
          <div class="row">
            <div class="col-lg-6">
              <form role="form">
              
                <div class="form-group">
                  <label>Nama User</label>
                  <input class="form-control" name="name" size="50">
                </div>

                <div class="form-group">
                  <label>Password</label>
                  <input class="form-control" type="password" name="pass" size="50">
                </div>

                <div class="form-group">
                  <label>Confirm Password</label>
                  <input class="form-control" type="password" name="conf_pass" size="50">
                </div>

                <div class="form-group">
                  <label>Privileges</label>
                  <?php for($i=0; $i<count($privileges_type); $i++): ?>
                  <div class="checkbox">
                    <label>
                      <input type="checkbox" name="privileges[ ]" value="<?php print $i;?>">
                      <?php print $privileges_type[$i]; ?>
                    </label>
                  </div>                  
                  <?php endfor; ?>
                </div>
              
                <input type="submit" class="btn btn-primary" name="add" value="Add">                
                
              </form>
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
