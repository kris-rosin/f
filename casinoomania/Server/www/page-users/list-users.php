<?php require_once "../config.php" ?>
<?php require_once "../database.php" ?>
<?php 
$is_success = false;
$database = new Database($DB_HOST, $DB_USER, $DB_PASS);
$database->connect_db();
$database->choose_database_name($DB_NAME);

if (!empty($_REQUEST['delete_id']) && !empty($_REQUEST['name'])) {
  $id = $_REQUEST['delete_id'];
  $name = $_REQUEST['name'];

  $sql = "DELETE FROM users WHERE id = %d";
  $query = sprintf($sql, $id);
  $result = $database->execute($query);
  if (mysql_affected_rows() > 0) {
    $is_success = true;
    $message = "User <strong>$name</strong> is successfully deleted!";
  }
} else if (!empty($_REQUEST['update_id']) && !empty($_REQUEST['name'])) {
  $id = $_REQUEST['update_id'];
  $name = $_REQUEST['name'];
  
  $sql = "UPDATE master_category SET name='%s' WHERE id=%d;";
  $query = sprintf($sql, $name, $id);
  $result = $database->execute($query);
  if (mysql_affected_rows() > 0) {
    $is_success = true;
    $message = "Update Category sukses!";
  }  
}

// Get Users
$search = $_REQUEST['search_username'];
$sql = "SELECT * FROM users";  
if (!empty($search)) {
	$sql .= " WHERE users.username LIKE '%%%s%%'";
}
$sql .= " ORDER BY my_coins DESC LIMIT 100;";
$sql .= ";";

$query = sprintf($sql, $search);  
$result = $database->query($query);
  
$database->close_db();
?>
<script src="<?php print $root ?>/js/jquery-1.10.2.js"></script>
<script type="text/javascript">
  function confirm_delete(name, id) {
    var r = confirm("Delete username " + name + "?");
    if (r == true) {
      hostAddress = top.location.host.toString();
      url = "<?php print $root; ?>/page-users/list-users.php?delete_id="+id+"&name="+name;
      window.location = url;
    } else {
    }
  }
  
  function edit(id) {
    hostAddress = top.location.host.toString();
    url = "<?php print $root; ?>/page-users/edit_users.php?id="+id;
    window.location = url;
  }
  
  function add_page() {
    hostAddress = top.location.host.toString();
    url = "<?php print $root; ?>/page-master/master_category.php";
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

    <title>Users</title>

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

        <?php $isUserEditAllowed = isUserAllowed($PRIV_EDIT_USERS); // function from privileges.php?>
        <?php $isUserDeleteAllowed = isUserAllowed($PRIV_DELETE_USERS); // function from privileges.php?>        
      
        <div class="row">
          <div class="col-lg-12">
            <h1>Users</h1>
            <ol class="breadcrumb">
              <li><i class="fa fa-group"></i> Users</a></li>
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

        <form action="list-users.php" method="post" enctype="multipart/form-data">
          <div class="row">
            <div class="col-lg-6">
              <form role="form">
              
                <div class="form-group">
                  <label>Search Username</label>
                  <input class="form-control" name="search_username" size="50" value="<?php print $search ?>">
                </div>            
              
                <input type="submit" class="btn btn-primary" name="search" value="search">                
                
              </form>
            </div>
          </div>
        </form>   
                            
        <div class="row">
          <div class="col-lg-12">              
            <div class="table-responsive">
              <table class="table table-bordered table-hover tablesorter">
                <thead>
                  <tr>
                    <th class="header" width="1">Delete </th>
                    <th class="header" width="1">Edit </th>
                    <th class="header headerSortDown">Facebook ID <i class="fa fa-sort"></i></th>
                    <th class="header headerSortDown">Username <i class="fa fa-sort"></i></th>
                    <th class="header headerSortDown">Email Address <i class="fa fa-sort"></i></th>
                    <th class="header headerSortDown">Coins <i class="fa fa-sort"></i></th>                    
                    <!--<th class="header">Writer <i class="fa fa-sort"></i></th>-->
                  </tr>
                </thead>
                <tbody>
                  <?php foreach($result as $object): ?>
                  <tr>
                    <td><button class="btn btn-danger btn-xs <?php if (!$isUserDeleteAllowed) {print 'disabled';} ?>" type="button" onclick="confirm_delete('<?php print $object->username; ?>','<?php print $object->id; ?>');">Delete</button></td>
                    <td><button class="btn btn-primary btn-xs <?php if (!$isUserEditAllowed) {print 'disabled';} ?>" type="button" onclick="edit('<?php print $object->id; ?>')">Edit</button></td>
                    <td><?php print $object->fid;?></a></td>
                    <td><?php print $object->username;?></a></td>
                    <td><?php print $object->email_address;?></a></td>
                    <td><?php print $object->my_coins;?></a></td>                    
                    <!--<td><?php print $object->writer;?></td>-->
                  </tr>
                  <?php endforeach; ?>
                </tbody>
              </table>
            </div>
          </div>          
        </div>
        
<!-- 
        <div class="row">
          <div class="col-lg-4">
            <input type="submit" class="btn btn-primary" value="Add" onclick="add_page();">
          </div>
        </div>
 -->
        
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
