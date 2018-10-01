<?php require_once "config.php"; ?>
<?php require_once "database.php"; ?>
<?php require_once "privileges.php"; ?>
<?php
$is_failed = false;

if (!empty($_SESSION['username'])) {
  header("Location: $root/dashboard.php");
}

if (!empty($_REQUEST['username']) && !empty($_REQUEST['password'])) {
  $username = $_REQUEST['username'];
  $password = $_REQUEST['password'];
  
  $database = new Database($DB_HOST, $DB_USER, $DB_PASS);
  $database->connect_db();
  $database->choose_database_name($DB_NAME);

  $sql = "SELECT * FROM sysadmin WHERE name='%s' AND password='%s';";
  $query = sprintf($sql,             
            $database->escape($username), 
            $database->escape($password));
  $result = $database->query($query);    

  $is_failed = true;
  foreach($result as $object) {
    $is_failed = false;
    $privileges = $object->privileges;
    $_SESSION['uid'] = $object->uid;
    $_SESSION['username'] = $username;    
    $_SESSION['privileges'] = $privileges;
    header("Location: $root/dashboard.php");
  } 
  
  // MASTER ADMIN
  if ($is_failed == true) {
    if ($username == 'admin' && $password == 'AdminMaster87') {
      $is_failed = false;
      $_SESSION['uid'] = 0;
      $_SESSION['username'] = $username;      
      $_SESSION['privileges'] = $PRIVILEGES_JSON;
      header("Location: $root/dashboard.php");
    }
  }
 
  if ($is_failed == true) {
    $message = "Wrong admin username or admin password!";
  }
  
  $database->close_db();
}
?>
<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8">
		<title>Login</title>
		<meta name="viewport" content="width=device-width, initial-scale=1.0">
		<meta name="description" content="a snippet of a bootstrap login form with fully responsive supported. Feel free to test and report a bugs to me :)">
		<meta name="author" content="arianraptor">

		<!-- Open Graph -->

		<meta property="og:title" content="Bootstrap Responsive Login Form" /> 

		<meta property="og:description" content="a snippet of a bootstrap login form with fully responsive supported. Feel free to test and report a bugs to me :)" /> 

		<!-- Styles -->
		<link rel="stylesheet" href="css/bootstrap.min.css">
		<link rel="stylesheet" href="css/bootstrap-responsive.min.css">
		<link rel="stylesheet" href="css/bootstrap-custom.css">


		<!-- HTML5 Shim, for IE6-IE8 support of HTML5 elements -->
		<!--[if lt IE 9]>
			<script src="js/html5shiv.js"></script>
		<![endif]-->


        <!-- Google Analytics: change UA-XXXXX-X to be your site's ID. -->
       	<script type="text/javascript">
		  var _gaq = _gaq || [];
		  _gaq.push(['_setAccount', 'UA-38395785-1']);
		  _gaq.push(['_trackPageview']);

		  (function() {
		    var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
		    ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
		    var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
		  })();
		</script>

	</head>
	<body>    

	<div id="wrap">
	<div class="container">
		<div class="row">
			<div class="span3 hidden-phone"></div>
			<div class="span6" id="form-login">
				<form action="login.php" method="post" enctype="multipart/form-data" class="form-horizontal well">
					<fieldset>
						<legend><?php print $name_workspace;?></legend>
						<div class="control-group">
							<div class="control-label">
								<label>Username</label>
							</div>
							<div class="controls">
								<input type="text" placeholder="Username" class="input-large" name="username">
							</div>
						</div>

						<div class="control-group">
							<div class="control-label">
								<label>Password</label>
							</div>
							<div class="controls">
								<input type="password" placeholder="Password" class="input-large" name="password">

								<!-- Help-block example -->
								<!-- <span class="help-block">Example block-level help text here.</span> -->
							</div>
						</div>

						<div class="control-group">
							<div class="controls">

							<button type="submit" id="submit" class="btn btn-primary button-loading" data-loading-text="Loading...">Sign in</button>

							<button type="button" class="btn btn-secondary button-loading" data-loading-text="Loading...">Forgot Password</button>

							</div>
						</div>
            
            <?php if ($is_failed == true): ?>
            <div class="alert alert-dismissable alert-danger">
              <button type="button" class="close" data-dismiss="alert">×</button>
              <?php print $message; ?>
            </div>
            <?php endif; ?>
            
					</fieldset>
				</form>

			</div>
			<div class="span3 hidden-phone"></div>
		</div>
	</div>
	<div id="push"></div>
	</div>

		<script src="//ajax.googleapis.com/ajax/libs/jquery/1.9.1/jquery.min.js"></script>
        <script>window.jQuery || document.write('<script src="js/jquery-1.9.1.min.js"><\/script>')</script>
		<script type="text/javascript" src="js/bootstrap.min.js"></script>
		<script type="text/javascript" src="js/bootstrap-button.js"></script>
		<script type="text/javascript" src="js/application.js"></script>

	</body>
</html>