<?php require_once "../config.php" ?>
<?php require_once "../database.php" ?>
<?php 
$is_success = false;
if (!empty($_REQUEST['name'])) {
  $name = $_REQUEST['name'];
  
  $database = new Database($DB_HOST, $DB_USER, $DB_PASS);
  $database->connect_db();
  $database->choose_database_name($DB_NAME);

  $sql = "INSERT INTO game (game) VALUES ('%s');";
  $query = sprintf($sql, 
            $database->escape($name));
  $result = $database->execute($query);
  
  $is_success = true;
  
  $database->close_db();
}
?>
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>Add Casino Game</title>

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

        <?php $isUserAllowed = isUserAllowed($PRIV_CASINO_GAMES); // function from privileges.php?>
  
        <?php if ($isUserAllowed): ?>
        <div class="row">
          <div class="col-lg-12">
            <h1>Add Casino Game</h1>
            <ol class="breadcrumb">
              <li><i class="fa fa-gamepad"></i> Casino Game</a></li>
              <li class="active">Add </li>
            </ol>
          </div>
        </div><!-- /.row -->

        <?php if ($is_success == true): ?>
        <div class="alert alert-dismissable alert-success">
          <button type="button" class="close" data-dismiss="alert">×</button>
          Game <strong><?php print $name; ?></strong> is successfully added!
        </div>
        <?php endif; ?>
        
            
        <form action="add-game.php" method="post" enctype="multipart/form-data">
          <div class="row">
            <div class="col-lg-6">
              <form role="form">
              
                <div class="form-group">
                  <label>Game</label>
                  <input class="form-control" name="name" size="50">
                  <p class="help-block">Add New Game</p>
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
