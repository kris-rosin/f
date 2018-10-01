<?php require_once "../config.php" ?>
<?php require_once "../database.php" ?>
<?php 
$is_success = false;
$database = new Database($DB_HOST, $DB_USER, $DB_PASS);
$database->connect_db();
$database->choose_database_name($DB_NAME);

if (!empty($_REQUEST['delete_id']) && !empty($_REQUEST['game'])) {
  $id = $_REQUEST['delete_id'];
  $game = $_REQUEST['game'];

  $sql = "DELETE FROM game WHERE id = %d";
  $query = sprintf($sql, $id);
  $result = $database->execute($query);
  if (mysql_affected_rows() > 0) {
    $is_success = true;
    $message = "Game <strong>$game</strong> is successfully deleted!";
  }
} else if (!empty($_REQUEST['update_id']) && !empty($_REQUEST['game'])) {
//   $id = $_REQUEST['update_id'];
//   $name = $_REQUEST['name'];
//   
//   $sql = "UPDATE master_category SET name='%s' WHERE id=%d;";
//   $query = sprintf($sql, $name, $id);
//   $result = $database->execute($query);
//   if (mysql_affected_rows() > 0) {
//     $is_success = true;
//     $message = "Update Category sukses!";
//   }  
}

// Get List Category
$sql = "SELECT * FROM game;";  
$query = sprintf($sql);  
$result = $database->query($query);
  
$database->close_db();
?>
<script src="<?php print $root ?>/js/jquery-1.10.2.js"></script>
<script type="text/javascript">
  function confirm_delete(game, id) {
    var r = confirm("Delete " + game + "?");
    if (r == true) {
      hostAddress = top.location.host.toString();
      url = "<?php print $root; ?>/page-game/list-game.php?delete_id="+id+"&game="+game;
      window.location = url;
    } else {
    }
  }
  
  function edit(id) {
    hostAddress = top.location.host.toString();
    url = "<?php print $root; ?>/page-edit/edit_users.php?id="+id;
    window.location = url;
  }
  
  function add_page() {
    hostAddress = top.location.host.toString();
    url = "<?php print $root; ?>/page-game/add-game.php";
    window.location = url;    
  }
</script>
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>Casino Game</title>

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
      
        <div class="row">
          <div class="col-lg-12">
            <h1>Casino Game</h1>
            <ol class="breadcrumb">
              <li><i class="fa fa-gamepad"></i> Casino Game</a></li>
              <li class="active">List</li>
            </ol>
          </div>
        </div><!-- /.row -->

        <?php if ($is_success == true): ?>
        <div class="alert alert-dismissable alert-success">
          <button type="button" class="close" data-dismiss="alert">×</button>
          <?php print $message; ?>
        </div>
        <?php endif; ?>
                    
        <div class="row">
          <div class="col-lg-6">              
            <div class="table-responsive">
              <table class="table table-bordered table-hover tablesorter">
                <thead>
                  <tr>
                    <th class="header" width="1">Delete </th>
                    <th class="header headerSortDown">ID <i class="fa fa-sort"></i></th>
                    <th class="header headerSortDown">Game <i class="fa fa-sort"></i></th>
                    <!--<th class="header">Writer <i class="fa fa-sort"></i></th>-->
                  </tr>
                </thead>
                <tbody>
                  <?php foreach($result as $object): ?>
                  <tr>
                    <td><button class="btn btn-danger btn-xs <?php if (!$isUserAllowed) {print 'disabled';} ?>" type="button" onclick="confirm_delete('<?php print $object->game; ?>','<?php print $object->id; ?>');">Delete</button></td>
                    <td><a href="#"><?php print $object->id;?></a></td>
                    <td><a href="#"><?php print $object->game;?></a></td>
                    <!--<td><?php print $object->writer;?></td>-->
                  </tr>
                  <?php endforeach; ?>
                </tbody>
              </table>
            </div>
          </div>          
        </div>
        

        <div class="row">
          <div class="col-lg-4">
            <input type="submit" class="btn btn-primary" value="Add Game" onclick="add_page();">
          </div>
        </div>

        
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
