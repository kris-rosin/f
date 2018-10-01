<?php require_once "../config.php" ?>
<?php require_once "../database.php" ?>
<?php require_once "../privileges.php" ?>
<?php 
$is_success = false;
$is_failed = false;

$database = new Database($DB_HOST, $DB_USER, $DB_PASS);
$database->connect_db();
$database->choose_database_name($DB_NAME);

if (!empty($_REQUEST['action']) && 
    $_REQUEST['action'] == 'update' && 
    (empty($_REQUEST['name']) || empty($_REQUEST['pass']))) {
  $is_failed = true;
  $message = "Make sure 'Admin Name' and 'Password' not empty!";
}

if (!empty($_REQUEST['id']) && 
    !empty($_REQUEST['action']) && 
    $_REQUEST['action'] == 'update' &&
    !empty($_REQUEST['name']) &&
    !empty($_REQUEST['pass']) && 
    !$is_failed) {  
  $id = $_REQUEST['id'];
  $pass = $_REQUEST['pass'];
  
  $sql = "SELECT * FROM sysadmin WHERE id=%d";
  $query = sprintf($sql, 
            $database->escape($id)
          );
  $result = $database->query($query);
  
  // initialisasi
  $is_failed = true;
  $message = "Password tidak sesuai!";
  
  foreach($result as $object) {
    $is_failed = false;
    $name = $_REQUEST['name'];    
    $privileges = json_encode($_REQUEST['privileges']);
    
    $sql = "UPDATE sysadmin SET name='%s',password='%s',privileges='%s' WHERE id=%d;";
    $query = sprintf($sql,
              $database->escape($name),
              $database->escape($pass),
              $database->escape($privileges),
              $database->escape($id)
            );
    $result = $database->execute($query);
    
    $is_failed = false;
    $is_success = true;
    $message = "Admin <strong>$name</strong> is successfully updated!";
    
    if ($id == $_SESSION['id']) {
      $_SESSION['username'] = $name;
      $_SESSION['privileges'] = $privileges;
    }
  }
  
  // Show what I have now!
  $id = $_REQUEST['id'];

  $sql = "SELECT * FROM sysadmin WHERE id=%d";
  $query = sprintf($sql, $database->escape($id));
  $result = $database->query($query);
  
  foreach ($result as $object) {
    $name = $object->name;
    $password = $object->password;
    $privileges = json_decode($object->privileges);
  }  
  for ($i=0; $i<$PRIV_MCOUNT; $i++) {
    $priv_bool[$i] = false;
  }
  for ($i=0; $i<count($privileges); $i++) {
    $priv_bool[$privileges[$i]] = true;
  }  
  
} else if (!empty($_REQUEST['id'])) {
  // Show what I have now!
  $id = $_REQUEST['id'];

  $sql = "SELECT * FROM sysadmin WHERE id=%d";
  $query = sprintf($sql, $database->escape($id));
  $result = $database->query($query);
  
  foreach ($result as $object) {
    $name = $object->name;
    $password = $object->password;
    $privileges = json_decode($object->privileges);
  }
  for ($i=0; $i<$PRIV_MCOUNT; $i++) {
    $priv_bool[$i] = false;
  }
  for ($i=0; $i<count($privileges); $i++) {
    $priv_bool[$privileges[$i]] = true;
  }  

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

    <title>Edit Admin</title>

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
            <h1>Edit Admin</h1>
            <ol class="breadcrumb">
              <li><i class="fa fa-user"></i> Admin</a></li>
              <li class="active">Edit</li>
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

            
        <form action="edit-admin.php?action=update&id=<?php print $id; ?>" method="post" enctype="multipart/form-data">
          <div class="row">
            <div class="col-lg-6">
              <form role="form">
              
                <div class="form-group">
                  <label>Nama User</label>
                  <input class="form-control" name="name" size="50" value="<?php print $name; ?>">
                </div>

                <div class="form-group">
                  <label>Password Baru</label>
                  <input class="form-control" type="password" name="pass" size="50" value="<?php print $password;?>">
                </div>                
                
                <div class="form-group">
                  <label>Privileges</label>
                  <?php for($i=0; $i<count($priv_bool); $i++): ?>
                  <?php 
                    $isChecked = false;
                    //var_dump($priv_bool[$i]);
                    if ($priv_bool[$i]) $isChecked = true;
                  ?>
                  <div class="checkbox">
                    <label>
                      <input type="checkbox" name="privileges[ ]" value="<?php print $i;?>" <?php if($isChecked) print "checked";?>>
                      <?php print $privileges_type[$i]; ?>
                    </label>
                  </div>                  
                  <?php endfor; ?>
                </div>
              
                <input type="submit" class="btn btn-primary" name="update" value="Update">
                
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
