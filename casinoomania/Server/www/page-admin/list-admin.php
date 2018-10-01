<?php require_once "../config.php" ?>
<?php require_once "../database.php" ?>
<?php require_once "../privileges.php" ?>
<?php 
$is_success = false;
$is_failed = false;

$database = new Database($DB_HOST, $DB_USER, $DB_PASS);
$database->connect_db();
$database->choose_database_name($DB_NAME);

if (!empty($_REQUEST['delete_id']) && !empty($_REQUEST['name'])) {
  $delete_id = $_REQUEST['delete_id'];
  $username = $_REQUEST['name'];
  
  $sql = "DELETE FROM sysadmin WHERE id = %d";
  $query = sprintf($sql, $delete_id);
  $result = $database->execute($query);
  if (mysql_affected_rows() > 0) {
    $is_success = true;
    $message = "Admin <strong>$username</strong> is successfully deleted!";
  }
}
$sql = "SELECT * FROM sysadmin;";
$query = sprintf($sql);
$result = $database->query($query);
    
$database->close_db();
?>
<script src="<?php print $root ?>/js/jquery-1.10.2.js"></script>
<script type="text/javascript">
  function confirm_delete(name, id) {
    var r = confirm("Delete " + name + "?");
    if (r == true) {
      hostAddress = top.location.host.toString();
      url = "<?php print $root; ?>/page-admin/list-admin.php?delete_id="+id+"&name="+name;
      window.location = url;
    } else {
    }
  }
  
  function edit(id) {
    hostAddress = top.location.host.toString();
    url = "<?php print $root; ?>/page-admin/edit-admin.php?id="+id;
    window.location = url;
  }

  function add_page() {
    hostAddress = top.location.host.toString();
    url = "<?php print $root; ?>/page-admin/create-admin.php";
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

    <title>Admin</title>

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
        
        <div class="row">
          <div class="col-lg-12">
            <h1>Admin</h1>
            <ol class="breadcrumb">
              <li><i class="fa fa-user"></i> Admin</a></li>
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
        
        <?php if ($is_failed == true): ?>
        <div class="alert alert-dismissable alert-danger">
          <button type="button" class="close" data-dismiss="alert">×</button>
          <?php print $message;?>
        </div>
        <?php endif; ?>
            
        <div class="row">
          <div class="col-lg-6">              
            <div class="table-responsive">
              <table class="table table-bordered table-hover tablesorter">
                <thead>
                  <tr>
                    <th class="header" width="1">Delete </th>
                    <th class="header" width="1">Edit </th>
                    <th class="header headerSortDown">Nama User <i class="fa fa-sort"></i></th>
                    <th class="header">Privileges <i class="fa fa-sort"></i></th>
                    <!--<th class="header">Writer <i class="fa fa-sort"></i></th>-->
                  </tr>
                </thead>
                <tbody>
                  <?php foreach($result as $object): ?>
                  <?php 
                    $privileges = json_decode($object->privileges);                      
                  ?>
                  <tr>
                    <td><button class="btn btn-danger btn-xs <?php if (!$isUserAllowed) {print 'disabled';} ?>" type="button" onclick="confirm_delete('<?php print $object->name; ?>','<?php print $object->id; ?>');">Delete</button></td>
                    <td><button class="btn btn-primary btn-xs <?php if (!$isUserAllowed) {print 'disabled';} ?>" type="button" onclick="edit('<?php print $object->id; ?>')">Edit</button></td>
                    <td><a href="<?php print $root; ?>/page-admin/edit-admin.php?id=<?php print $object->id;?>"><?php print $object->name;?></a></td>
                    <td><ul>
                    <?php for($i=0; $i<count($privileges); $i++):?>
                    <li><?php print $privileges_type[$privileges[$i]]; ?></li>
                    <?php endfor;?>                      
                    </ul></td>
                    <!--<td"><?php print $object->writer;?></td>-->
                  </tr>
                  <?php endforeach; ?>
                </tbody>
              </table>
            </div>
          </div>      
        </div>
        
        <div class="row">
          <div class="col-lg-4">
            <input type="submit" class="btn btn-primary" value="Add" onclick="add_page();">
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
